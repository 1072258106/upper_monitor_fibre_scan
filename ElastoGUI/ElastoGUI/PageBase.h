#pragma once


// CPageBase �Ի���

class CPageBase : public CDialog
{
	DECLARE_DYNAMIC(CPageBase)

public:
	CPageBase(UINT nDlgID, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageBase();

private:
	LOGFONT m_lfBaseFont;
	TCHAR * m_pszTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void SetTitle(LPTSTR pszTitle);
};
