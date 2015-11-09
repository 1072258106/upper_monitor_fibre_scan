#include "StdAfx.h"
#include "CyUSBDevice.h"

const char * FXFirmwareFilePath = "Fx3Firmware.img";//�̼�ӳ���ļ�

CUSB30Device * CUSB30Device::s_ptrCUSB30Device = NULL;

CUSB30Device::CUSB30Device(void)
{
	m_pUSBDevice = new CCyUSBDevice();
	//CCyFX3Device *pFX3Device = new CCyFX3Device();
	//m_pUSBDevice = (CCyUSBDevice *)pFX3Device;
	//m_pUSBDevice = (CCyUSBDevice *)(new CCyFX3Device());

}


CUSB30Device::~CUSB30Device(void)
{
	if (m_pUSBDevice)
	{
		delete m_pUSBDevice;
	}
}

CUSB30Device * CUSB30Device::Instance()
{
	if (s_ptrCUSB30Device == NULL)
	{
		s_ptrCUSB30Device = new CUSB30Device();
	}
	return s_ptrCUSB30Device;
}

void CUSB30Device::Init(void *pParam /* = NULL */)
{
	//InitFX3Device();
}

void CUSB30Device::Release()
{

}

bool CUSB30Device::InitFX3Device(CString &info)
{
	return Refresh(info);// ˢ��FX3�豸
}


//�̼����غ���
int  CUSB30Device::DownloadImpl(LPCTSTR pszFwFile, FX3_FWDWNLOAD_MEDIA_TYPE nFmt)
{
	CCyFX3Device * pFX3Dev=(CCyFX3Device*)m_pUSBDevice;

	//CCyFX3Device *pFX3Dev = new CCyFX3Device();
	int ret = FRC_SUCCESS;

	//����������Ƿ��������в���ʼ�������ڽ���
	if (pFX3Dev->IsBootLoaderRunning())
	{
		if (SUCCESS == pFX3Dev->DownloadFw((char*)pszFwFile,nFmt))
		{
			ret = FRC_SUCCESS;
		}
		else
		{
			ret = FRC_FAILED;
		}
	}
	else
	{
		ret = FRC_BL_UNRUNNMING;
	}

	//exit:
	//delete pFX3Dev;
	return ret;
}

////////////////////////////////////////////////
//�豸���й̼����ء�
//nFmtΪ�������͡�
//
int  CUSB30Device::DownloadFirmware(LPCTSTR pszFwFile, FX3_FWDWNLOAD_MEDIA_TYPE nFmt)
{
	if (nFmt==RAM)
	{
		return DownloadImpl(pszFwFile, nFmt);
	}
	else//I2CEEPROM��SPIFLASH
	{
		if (FRC_SUCCESS == DownloadImpl(_T("CyBootProgrammer.IMG"), RAM))
		{
			return DownloadImpl(pszFwFile, nFmt);
		}
		else
		{
			return FRC_PROGER_FAILED;
		}
	}
}

int  CUSB30Device::InitFirmware()
{
	int ret = FRC_FAILED;

	{   // �����ʶ��Ϊ2.0�豸,��Ҫ�������ع̼�
		if (m_pUSBDevice->Open(0))
		{
			ret = DownloadFirmware(FXFirmwareFilePath, RAM);
			//if (ret == FRC_SUCCESS)
			{
				::Sleep(200);
				//Refresh();
			}
		}
	}
	return ret;
}


BOOL CUSB30Device::Open(int ndx)
{
	m_oSemAccessCyCtrl.Lock();
	bool ok = m_pUSBDevice->Open(ndx);
	m_oSemAccessCyCtrl.Unlock();
	return ok;
}

void CUSB30Device::SetBulkEndPtTimeOut(int timeout)
{
	m_oSemAccessCyCtrl.Lock();
	m_pUSBDevice->BulkInEndPt->TimeOut = timeout;
	m_oSemAccessCyCtrl.Unlock();
}

BOOL CUSB30Device::InitBulkEndPoint(BYTE chInit)
{
	if (!m_pUSBDevice->IsOpen())  return FALSE;

	CCyUSBDevice * pUSBDev = m_pUSBDevice;

	CCyControlEndPoint * pCtrlEp=pUSBDev->ControlEndPt;
	pCtrlEp->Target   = TGT_INTFC;
	pCtrlEp->ReqType  = REQ_CLASS;	
	pCtrlEp->Value    = 0x0000;
	pCtrlEp->Index    = 0x0000;
	pCtrlEp->Direction= DIR_TO_DEVICE;
	pCtrlEp->ReqCode  = 0x01;

	BYTE chBuf = chInit;
	long nBufLen = 1;

	OVERLAPPED oCtrlOvLap;
	oCtrlOvLap.hEvent = CreateEvent(NULL, FALSE, FALSE, _T("CYUSB_INIT"));

	BYTE * pContext=pCtrlEp->BeginDataXfer(&chBuf, nBufLen, &oCtrlOvLap);
	if (pCtrlEp->WaitForXfer(&oCtrlOvLap, 1500))
	{
		if(pCtrlEp->FinishDataXfer(&chBuf, nBufLen, &oCtrlOvLap, pContext))
		{
			//�˳�ʱ�ر��¼��ں˶���
			CloseHandle(oCtrlOvLap.hEvent);
			return TRUE;
		}
	}

	//�˳�ʱ�ر��¼��ں˶���
	CloseHandle(oCtrlOvLap.hEvent);

	return FALSE;
}


int  CUSB30Device::CtrlEndPointWrite(BYTE bData)
{
	m_oSemAccessCyCtrl.Lock();

	int error = -1;
	if (m_pUSBDevice->IsOpen())
	{
		CCyUSBDevice * pUSBDev = m_pUSBDevice;
		CCyControlEndPoint * pCtrlEp=pUSBDev->ControlEndPt;
		pCtrlEp->Target   = TGT_INTFC;
		pCtrlEp->ReqType  = REQ_CLASS;	
		pCtrlEp->Value    = 0x0000;
		pCtrlEp->Index    = 0x0000;
		//pCtrlEp->Direction= DIR_TO_DEVICE;
		pCtrlEp->ReqCode  = 0x01;

		BYTE chBuf = bData;
		LONG nBufLen = 1;

		bool ok = pCtrlEp->Write(&chBuf, nBufLen);

		error = ok ? 1 : 0;
	}

	m_oSemAccessCyCtrl.Unlock();
	return error;
}

int  CUSB30Device::CtrlEndPointRead(BYTE &bData)
{
	m_oSemAccessCyCtrl.Lock();
	int error = -1;
	if (m_pUSBDevice->IsOpen())
	{
		CCyUSBDevice * pUSBDev = m_pUSBDevice;
		CCyControlEndPoint * pCtrlEp=pUSBDev->ControlEndPt;
		pCtrlEp->Target   = TGT_INTFC;
		pCtrlEp->ReqType  = REQ_CLASS;	
		pCtrlEp->Value    = 0x0000;
		pCtrlEp->Index    = 0x0000;
		//pCtrlEp->Direction= DIR_TO_DEVICE;
		pCtrlEp->ReqCode  = 0x81;

		BYTE chBuf = 0;
		LONG nBufLen = 1;

		bool ok = pCtrlEp->Read(&chBuf, nBufLen);

		if (ok)
		{
			bData = chBuf;
		}
		error = ok ? 1 : 0;
	}

	m_oSemAccessCyCtrl.Unlock();
	return error;
}

bool  CUSB30Device::Refresh(CString &info)
{
	CCyUSBDevice * pUSBDevice = m_pUSBDevice;
	bool  ok = false;

	m_oSemAccessCyCtrl.Lock();

	//ȷ����������һ��USB����
	if (pUSBDevice->DeviceCount())
	{
		/*
		//��Ѱ���������ӵ���������������ӵ������б�
		for (int i = 0; i < pUSBDevice->DeviceCount(); i++)
		{
			pUSBDevice->Open(i);
		}		
		pUSBDevice->Close();
		::Sleep(100);
		*/
		//pUSBDevice->Open(0);
		if (IsUSB20())
		{
			InitFirmware();
		}
		OnCbOpenDev(info);
		ok = true;
	}
	else
	{
		//MessageBox(_T("δ��⵽USB�豸��\r\n\r\n�п���������û������ϣ���ೢ�Լ��Σ�"),_T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
		//WriteInfoTxt(_T("δ��⵽USB�豸�� �п���������û������ϣ���ೢ�Լ��Σ�"), INFO_WARNING);
		info = _T("δ��⵽USB�豸�� �п���������û������ϣ���ೢ�Լ��Σ�");
	}

	m_oSemAccessCyCtrl.Unlock();
	return ok;
}


void CUSB30Device::OnCbOpenDev(CString &info)
{
	CCyUSBDevice *pUSBDevice = m_pUSBDevice;

	CString csBuf;
	pUSBDevice->Open(0);
	info = pUSBDevice->FriendlyName;
	csBuf.Format(_T("; �豸PID��0x%04X; "), pUSBDevice->ProductID);
	info += csBuf;

	csBuf.Empty();
	csBuf.Format(_T("�豸VID��0x%04X; "), pUSBDevice->VendorID);

	info += csBuf;
	csBuf.Empty();

	if (pUSBDevice->bSuperSpeed)  
	{
		csBuf = _T("�豸���ͣ�USB3.0�����豸");
	}
	else if (pUSBDevice->bHighSpeed) 
	{
		csBuf = _T("�豸���ͣ�USB2.0�����豸");
	}
	else 
	{
		csBuf = _T("�豸���ͣ�USB1.1ȫ���豸");
	}
	info += csBuf;
	//WriteInfoTxt(info);

	//pUSBDevice->Close();
}


bool  CUSB30Device::IsAvailale()
{
	m_oSemAccessCyCtrl.Lock();

	bool ok = m_pUSBDevice->DeviceCount() && IsUSB30();

	m_oSemAccessCyCtrl.Unlock();
	return ok;
}

bool  CUSB30Device::IsUSB30()
{
	return m_pUSBDevice->bSuperSpeed;
}

bool  CUSB30Device::IsUSB20()
{
	return m_pUSBDevice->bHighSpeed;
}

bool  CUSB30Device::IsUSB11()
{
	return !m_pUSBDevice->bHighSpeed && !m_pUSBDevice->bSuperSpeed;
}

bool  CUSB30Device::BulkEndPointInXfer(BYTE *pData, LONG &toRead)
{
	m_oSemAccessCyCtrl.Lock();

	CCyBulkEndPoint *pEndPt = m_pUSBDevice->BulkInEndPt;
    bool ok = false;
	if (pEndPt)
	{
		ok = pEndPt->XferData(pData, toRead);
	}
	m_oSemAccessCyCtrl.Unlock();
	return ok;

}