#include "StdAfx.h"
#include "StrainDevice.h"
#include <io.h>

CStrainDevice * CStrainDevice::s_ptrStrainDevice = NULL;

CStrainDevice::CStrainDevice(void)
{
	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	m_OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ASSERT(m_OverlappedRead.hEvent != NULL);

	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));
	m_OverlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ASSERT(m_OverlappedWrite.hEvent != NULL);

	m_nHighLimit = 0;
	m_nZero = 0;
	m_nLowLimit  = 0;
}


CStrainDevice::~CStrainDevice(void)
{
}

CStrainDevice * CStrainDevice::Instance()
{
	if (s_ptrStrainDevice == NULL)
	{
		s_ptrStrainDevice = new CStrainDevice();
	}
	return s_ptrStrainDevice;
}

bool CStrainDevice::Init(void *pParam)
{
	bool ok = InitComm();

	m_nZero      = GetPrivateProfileInt("StrainDevice",  "Zero",   -100,  ConfigFilePath);
	m_nHighLimit = GetPrivateProfileInt("StrainDevice", "High",  1000,  ConfigFilePath);
	m_nLowLimit  = GetPrivateProfileInt("StrainDevice", "Low",  200,  ConfigFilePath);

	if (ok)
	{
		ok = RequestAddr();
	}
	return ok;
}

void CStrainDevice::Release()
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		::PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
	::CloseHandle(m_OverlappedRead.hEvent);
	::CloseHandle(m_OverlappedWrite.hEvent);

	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hCom);
	}
}

bool  CStrainDevice::InitComm(const char *config_file)
{
	if (config_file)
	{
		m_strIniFile = config_file;
	}
	if (_access(m_strIniFile.GetString(), 0) != 0)
	{
		ASSERT(false);
		fprintf(stderr, "%s is non-exist!\n", m_strIniFile.GetString());
		return FALSE;
	}
	char   com[10];
	DWORD  baud = 0;
	DWORD  dataSize = 0;
	DWORD  parity = 0;
	DWORD  stopBit = 0;

	ReadConfig(com, baud, dataSize, parity, stopBit, config_file);
	BOOL  bOk = OpenComm(com);

	if (bOk)
	{
		SetupComm(baud, dataSize, parity, stopBit);
	}
	//_beginthreadex(NULL, 0, KeyboardThread, this, CREATE_SUSPENDED, NULL);
	//m_hReceiveThread = CreateThread(NULL, 0, LPCMonitorThread, this, CREATE_SUSPENDED, &m_dwThreadID);
	return bOk == TRUE;
}

void CStrainDevice::ReadConfig(LPTSTR lpCom, DWORD &baud, DWORD &dataSize, DWORD &parity, DWORD &stopBit, LPCTSTR cfg_file)
{
	GetPrivateProfileString("COM", "Port", "", lpCom, 8, cfg_file);
	baud     = GetPrivateProfileInt("COM", "Baud", 0,  cfg_file);
	dataSize = GetPrivateProfileInt("COM", "DataSize", 0, cfg_file);
	parity   = GetPrivateProfileInt("COM", "Parity", 0, cfg_file);
	stopBit  = GetPrivateProfileInt("COM", "StopBit", 0, cfg_file);
}

BOOL CStrainDevice::OpenComm(LPCTSTR com)
{
	//ȫ�ֱ��������ھ��
	m_hCom = CreateFile(com, //COM�˿�
		GENERIC_READ | GENERIC_WRITE, //�������д
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�첽��ʽ
		NULL);
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		//::AfxMessageBox("�����豸#2-��COMʧ��!");
		return FALSE;
	}
	return TRUE;
}

void CStrainDevice::SetupComm(int baud, int dataSize, int parity, int stopBit)
{
	::SetupComm(m_hCom, 1024, 1024); //���뻺����������������Ĵ�С����1024

	COMMTIMEOUTS TimeOuts;

	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout         = 100;
	TimeOuts.ReadTotalTimeoutMultiplier  = 500;
	TimeOuts.ReadTotalTimeoutConstant    = 2000;
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant   = 2000;
	::SetCommTimeouts(m_hCom,&TimeOuts); //���ó�ʱ

	DCB dcb;
	::GetCommState(m_hCom, &dcb);
	dcb.BaudRate = baud; //������
	dcb.ByteSize = dataSize; //ÿ���ֽ���xλ
	dcb.Parity = parity; //��żУ��λ
	dcb.StopBits = stopBit; //ֹͣλ
	::SetCommState(m_hCom, &dcb);

	::PurgeComm(m_hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
}

DWORD  CStrainDevice::ReadComm(BYTE *lpInBuffer, DWORD dwToRead)
{
	DWORD dwBytesRead = dwToRead;
	BOOL bReadStatus;
	DWORD dwErrorFlags;
	COMSTAT ComStat;

	::ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	if (!ComStat.cbInQue)
		return 0;
	dwBytesRead = MIN(dwBytesRead, (DWORD)ComStat.cbInQue);
	bReadStatus = ::ReadFile(m_hCom, lpInBuffer, dwToRead, &dwBytesRead, &m_OverlappedRead);
	if (!bReadStatus) //���ReadFile��������FALSE
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hCom,	&m_OverlappedRead, &dwBytesRead, TRUE);
			// GetOverlappedResult���������һ��������ΪTRUE��
			//������һֱ�ȴ���ֱ����������ɻ����ڴ�������ء�

			return dwBytesRead;
		}
		return 0;
	}
	return dwBytesRead;
}

DWORD CStrainDevice::WriteComm(BYTE *buffer, DWORD dwToWrite)
{
	DWORD dwBytesWritten;
	//DWORD dwErrorFlags;
	//COMSTAT ComStat;
	BOOL bWriteStat;

	bWriteStat = ::WriteFile(m_hCom, buffer, dwToWrite, &dwBytesWritten, &m_OverlappedWrite);
	if (!bWriteStat)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			//::WaitForSingleObject(m_osWrite.hEvent, 1000);
			::GetOverlappedResult(m_hCom, &m_OverlappedWrite, &dwBytesWritten, TRUE);
			return dwBytesWritten;
		}
		return 0;
	}
	return dwBytesWritten;
}

bool  CStrainDevice::RequestAddr(BYTE addr /* = 0 */)
{
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BYTE cmd[2];
	cmd[0] = 'K';
	cmd[1] = 0;
	WriteComm(cmd, 2);
	::Sleep(100);

	BYTE to_read[2];
	ZeroMemory(to_read, 2);
	ReadComm(to_read, 2);

	return memcmp(cmd, to_read, 2) == 0;
}

bool  CStrainDevice::RequestDataB(long &ad)
{
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BYTE cmd = 'M';
	WriteComm(&cmd, 1);
	::Sleep(20);

	BYTE bytesRead[5];
	ZeroMemory(bytesRead, 2);
	DWORD nCount = ReadComm(bytesRead, 5);
	bool ok = (nCount == 5);

	if (ok)
	{
		BYTE *ptr = (BYTE*)&ad;
		ptr[0] = bytesRead[3];
		ptr[1] = bytesRead[2];
		ptr[2] = bytesRead[1];
		ptr[3] = bytesRead[0];
	}
	return ok;
}


bool  CStrainDevice::GetZero(long *zero /* = 0 */)
{
	long  ad;

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	bool ok = RequestDataB(ad);
	if (ok)
	{
		m_nZero = ad;
		if (zero) *zero = ad;
	}
	return ok;
}

bool  CStrainDevice::IsAvaliable(long ad)
{
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	ad -= m_nZero;//Ҫ��ȥ��ֵ

	return (ad > m_nLowLimit && ad < m_nHighLimit);
}