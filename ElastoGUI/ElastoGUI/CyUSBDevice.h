#pragma once

//������ʼ����ָʾ���Ĵ��䷽��
const BYTE CYUSB_IN  = 0x00;
const BYTE CYUSB_OUT = 0xff;

const BYTE CYUSB_START = 0x02;// ��������ʼһ�β���
const BYTE CYUSB_SET_DT_ENVELOPE = 0x03; // �����������ͣ������
const BYTE CYUSB_SET_DT_RAW      = 0x04; // ������������:ԭʼ

enum FWDWN_RETURN_CODE
{
	FRC_SUCCESS       = 0x00, //�����ɹ�
	FRC_FAILED        = 0x01, //����ʧ��
	FRC_INVALID_PARAM = 0x02, //��Ч�������в���
	FRC_UNKOWN_DEVICE = 0x03, //δ֪���豸
	FRC_BL_UNRUNNMING = 0x04, //BootLoaderδ����
	FRC_PROGER_FAILED = 0x05, //������̼�����ʧ��
	FRC_FW_UNFUND     = 0x06  //�̼�������
};

//////////////////////////////////////////////////////////////////////////
// ��װCyUSBDevice�࣬ ���õ���������
// Ŀ�������й����Ľӿڶ��б���-ʵ���̰߳�ȫ
// Լ���� ϵͳֻ����һ��FX3���豸�� ����Ĭ��ʹ��0���豸
//////////////////////////////////////////////////////////////////////////
class CUSB30Device
{
public:
	~CUSB30Device(void);
    static CUSB30Device *Instance(); // ����ֻ��һ��ʵ������ʹ�þ�̬�����ӿ�����

	void Init(void *pParam = NULL); // �ṩһ���ⲿ����������Ϣ�ĳ�ʼ���ӿ�

	void Release(); // ��������ǰ�ͷ���Դ

	bool InitFX3Device(CString &info); // ��ʼ���豸

	bool Refresh(CString &info); //����ˢ���豸

	bool IsAvailale(); //�豸�Ƿ����

	bool IsUSB30(); //�Ƿ�3.0�豸

	bool IsUSB20();

	bool IsUSB11();

	bool BulkEndPointInXfer(BYTE *pData, LONG &toRead); // bulk�˵������

	// ָʾUSB�豸���ݴ��䷽��
	BOOL InitBulkEndPoint(BYTE chInit); // ����bulk�˵�-������д

	BOOL Open(int ndx = 0); //���豸

	void SetBulkEndPtTimeOut(int timeout); // ���ò��׿˶ϵ�-������ʱ

	// ��USB�豸���ƶ˵�д���ݣ�Ŀǰһ��ֻ��дһ������
	int  CtrlEndPointWrite(BYTE bData);

	// ��USB�豸���ƶ˵�����ݣ�Ŀǰһ��ֻ�ܶ�һ������
	int  CtrlEndPointRead(BYTE &bData);

protected:

private:
	CUSB30Device(void);//��ֹ�ⲿֱ��ʵ����

	static CUSB30Device *s_ptrCUSB30Device; // ����ֻ����һ��ʵ������

	CSemaphore           m_oSemAccessCyCtrl;// �ź���������CyUSBDevice�Ĳ���
	CCyUSBDevice        *m_pUSBDevice;
	int                  m_nDevIndex;

	void OnCbOpenDev(CString &info); // ���豸ʱ��һЩ������

	int  InitFirmware(); // ��ʼ���豸�Ĺ̼�

	int  DownloadImpl(LPCTSTR pszFwFile, FX3_FWDWNLOAD_MEDIA_TYPE nFmt);

	int  DownloadFirmware(LPCTSTR pszFwFile, FX3_FWDWNLOAD_MEDIA_TYPE nFmt);
};

