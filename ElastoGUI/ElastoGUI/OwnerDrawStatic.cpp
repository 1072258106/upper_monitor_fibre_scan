// OwnerDrawStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ElastoGUI.h"
#include "OwnerDrawStatic.h"


// COwnerDrawStatic

IMPLEMENT_DYNAMIC(COwnerDrawStatic, CStatic)

COwnerDrawStatic::COwnerDrawStatic():m_lpCBDrawHandler(NULL)
{

}

COwnerDrawStatic::~COwnerDrawStatic()
{
}


BEGIN_MESSAGE_MAP(COwnerDrawStatic, CStatic)
END_MESSAGE_MAP()



// COwnerDrawStatic ��Ϣ�������
DrawHandler  COwnerDrawStatic::RegisterDrawItem(DrawHandler handler)
{
	DrawHandler old_handler = m_lpCBDrawHandler;
	m_lpCBDrawHandler = handler;
	return old_handler;
}


void COwnerDrawStatic::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	CStatic::PreSubclassWindow();

	ModifyStyle(NULL,SS_OWNERDRAW);	
}


void COwnerDrawStatic::DrawItem(LPDRAWITEMSTRUCT  lpStruct/*lpDrawItemStruct*/)
{
	// TODO:  ������Ĵ����Ի���ָ����
	if (m_lpCBDrawHandler)
	{
		CWnd * pParent = GetParent();
		m_lpCBDrawHandler(pParent, lpStruct);
	}
	else
	{
		CStatic::DrawItem(lpStruct);
	}
}
