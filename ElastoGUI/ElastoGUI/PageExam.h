#pragma once

#include "pagebase.h"
#include "afxwin.h"
#include "FontText.h"
#include "OwnerDrawStatic.h"
#include "opencv/cv.h"
#include "CElasto.h"
//#include "LedEx.h"
#include "TestTime.h"

const BYTE  CyTriggerByte = 0x55;

const char  DataDir[] = "data\\";
const char  DemoMModeImageFileName[] = "M_Demo.bmp"; // for demo 
const char  DemoDisplacementImageFileName[] = "disp_demo.bmp";
const char  DemoStrainImageFileName[] = "strain_demo.bmp";
const char  DemoRFTxtFileName[] = "rf_demo.dat";
const char  DemoRFBinFileName[] = "rf_demo.raw";

const char  DisplacementImageFileName[] = "displace.bmp"; // ��ʱ�ļ��������ڲ���������ʾ����Ƶ��м��ļ�
const char  StrainImageFileName[] = "strain.bmp";// ��ʱ�ļ��������ڲ���������ʾ����Ƶ��м��ļ�
const char  MModeImageFileName[]  = "mmode.bmp";

class CPageExam;
class CElastoGUIDlg;

typedef struct SMonitorParam
{
	CPageExam     *pPage;
	CElastoGUIDlg *pMainDlg;

	SMonitorParam():pPage(0), pMainDlg(0)
	{

	}
} SMonitorParam, * PSMonitorParam;

//�ṹ�壺���ڴ��ݸ��̵߳Ĳ���
typedef struct SBulkParam
{
	CPageBase    *pPageImg;
	CWnd         *pMainDlg;
	int           nBufSize;     // һ�ν������ݻ��������ȣ���λ���ֽ�
	BYTE         *pBulkBuf;     // �����߳̽������ݴ������ȵ���nBufSize�Ļ������������ڸñ�����
	int           nTimeout;     // ���յ��ӳ٣�ms

	SBulkParam()
	{
		pPageImg   = NULL;
		pMainDlg   = NULL;
		nBufSize   = 0;
		pBulkBuf   = NULL;
		nTimeout   = 0;
	}

	~SBulkParam()
	{
		if (pBulkBuf)
		{
			delete [] pBulkBuf;
		}
	}
} SBulkParam, *PSBulkParam;


// �ṹ�壺 ��ʾӰ��Ĳ���
typedef struct SImgParam
{
	int   nDyn;    // default:50
	int   nGain;   // 
	int   nDCode;  // decimation: default, 4
	int   nGrayScale; // 255
} SImgParam, *PSImgParam;

// �������ͣ� Data Type Code
enum
{
	DT_ENVELOPE, // �����
	DT_RAW       // ԭʼRF
};

// ���ݸ�ʽ�� Data Format Code
enum
{
	DF_BIN, // ������
	DF_TXT  // �ļ�
};

class CPageExam : public CPageBase
{
	DECLARE_DYNAMIC(CPageExam)

public:
	CPageExam(CWnd *pParent = NULL);
	virtual ~CPageExam(void);

	void CbOnRefreshDevice();//�ص��ӿڣ��豸ˢ��

	static void  DrawMModeImage       (CWnd *, LPDRAWITEMSTRUCT);
	static void  DrawMModeRuler       (CWnd *, LPDRAWITEMSTRUCT);
	static void  DrawDisplacementImage(CWnd *, LPDRAWITEMSTRUCT);
	static void  DrawStrainImage      (CWnd *, LPDRAWITEMSTRUCT);

	static void  HandleEpEvent(EProcessEvent, CvMat*, void*);

	//////////////////////////////////////////////////////////////////////////
	// �Զ����Ƹ�ʽ�������ݣ��ļ��ĺ�׺�� .bin, �ļ�������datasĿ¼�£�ͬʱ����ʱ����Ϣ
	// pmat,  ����Դ
	// prefix, �ļ���ǰ׺��  RF���ݣ���rf���� ��������ݣ���env��
	//////////////////////////////////////////////////////////////////////////
	static void  SaveRawDataB(const CvMat *pmat, const char *path);// bin��ʽ�� д�����ļ�

	//////////////////////////////////////////////////////////////////////////
	// ���ı���ʽ��������, �ļ���׺: .dat, �ļ�������datasĿ¼�£�ͬʱ����ʱ����Ϣ
	// pmat������Դ
	// bSigned���Ƿ�����ţ�Ĭ���ǲ����ġ���ΪӲ��Ĭ���ϴ����ǰ��������ݣ�>0
	// prefix�� �ļ���ǰ׺��RF���ݣ���rf���� ��������ݣ���env��
	//////////////////////////////////////////////////////////////////////////
	static void  SaveRawDataT(const CvMat *pmat, const char *path, bool bSigned = false);// �ı���ʽ

	// 
	enum { IDD = IDD_PAGE_EXAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

public:
	FontText m_oTxtVelocity;
	FontText m_oTxtElastic;
	afx_msg void OnBnClickedBtnStart();
	COwnerDrawStatic m_oMModeRuler;
	COwnerDrawStatic m_oMModeImg;
	COwnerDrawStatic m_oDisplacementImg;
	COwnerDrawStatic m_oStrainImg;

	CFont *m_Font;//��������

	void  RemoveMonitorThread();

	// ϵͳ���������������Ϊ�����ȣ���λ��mm
	static float  Pts2Length(int points);

	// ϵͳ����������껻��Ϊ������ȣ���λ��mm
	static float  Pts2Depth(int points);

private:

	void ClearMModeImage(void);

	void ClearMModeRuler(void);
	void DrawMModeRuler(HDC hdc);

	void DrawDemoMModeImage(void);
	void DrawMModeImage(const char *filename = "mmode.bmp");
	void DrawMModeImage(HDC hdc);

	void ClearDisplacementImage(void);

	void DrawDisplacementImage(const char *filename = "disp_demo.bmp");

	void ClearStrainImage(void);

	void DrawStrainImage(const char *filename = "strain_demo.bmp");

	// ����ѹ��
	void Log10(CvMat *pmatLog, const CvMat *pmatSrc, const SImgParam &imgParam, int type = CV_16U);
	
	// ��ʾ����ģ��
	void ShowModulus();

	// �����ʱӰ���ļ���displacement.bmp, strain.bmp
	void ClearTmpImgFile();

	// ����������ʾ���ս��
	void DisplayResult(double src);

private:
	CWinThread  *m_pBulkThread; // �������߳�
	CWinThread  *m_pMonitorThread;

	float  m_fVelocity; //���в��ٶ�, m/s
	float  m_fModulus;  //����ģ��,  kPa

	int    m_nTimerInitID; // ��ʼ����Ҫʹ�õ��Ķ�ʱ����ID
	int    m_nTimerReadStrain; // ��ȡӦ��Ķ�ʱ��

	SBulkParam     m_oBulkParam; // ���������̵߳Ĳ���
	SMonitorParam  m_oMonitorParam;

	//CvMat  *m_pmatBuffer; // ������յ����ݣ�size��һ֡�����ݳ���, 8UC1
	CvMat  *m_pmatCurr;   // ��ǰ���ڴ��������֡���������ԭʼ����, 16UC1, ÿ�д���һ��ɨ���ߡ�
	CvMat  *m_pmatLog;    // ��ǰ����֡��������ѹ���������,������Ϊ�Ҷ�����, 8UC1
	CvMat  *m_pmatDisplacement; // �������Լ����Ժ������λ����, 32FC1
	CvMat  *m_pmatStrain;       // �������Լ����Ժ������Ӧ����, 32FC1

	SImgParam m_oImgParam; // Ӱ�����,

	//CLedEx m_oLed;

	CString  m_strCurrSavePath;// ��ǰ�������ݵ�·��

	CTestTime  m_oTestTime;// ����Ϊ�˲�������ʱ��

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg LRESULT OnGetData(WPARAM wParam, LPARAM lParam);
	BOOL m_bSaveDat;
	BOOL m_bSaveBmp;
	BOOL m_bSaveValue;
	BOOL m_bWorkDemo;
	int m_nDataType;
	afx_msg void OnBnClickedCheckDemo();
	afx_msg void OnBnClickedRadioDtEnvelope();
	afx_msg void OnBnClickedRadioDtRaw();
	afx_msg void OnBnClickedCheckSaveDat();
	afx_msg void OnBnClickedCheckSaveBmp();
	int m_nDataFormat;
	afx_msg void OnBnClickedRadioBin();
	afx_msg void OnBnClickedRadioTxt();
	afx_msg LRESULT  OnRevDevTrigger(WPARAM wParam, LPARAM lParama);
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedBtnZero();
	afx_msg void OnBnClickedBtnCls();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};

