// EditDelayed.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "EditDelayed2.h"


// CEditDelayed2

IMPLEMENT_DYNAMIC(CEditDelayed2, CEdit)

CEditDelayed2::CEditDelayed2()
{

}

CEditDelayed2::~CEditDelayed2()
{
}


BEGIN_MESSAGE_MAP(CEditDelayed2, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CEditDelayed2::OnEnChange)
	ON_CONTROL_REFLECT(EN_SETFOCUS, &CEditDelayed2::OnEnSetfocus)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CEditDelayed2 ��Ϣ�������


void CEditDelayed2::DoDelayedEvalute(bool bForce)
{
	int m_nCurrentColumnIdx = 5;
	CString m_strLastEvaluatedContent = L"abcd";
	GetParent()->SendMessage(UM_DELAYED_EVALUATE, (WPARAM)m_nCurrentColumnIdx, (LPARAM)(LPCTSTR)m_strLastEvaluatedContent);
}

void CEditDelayed2::OnEnChange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DoDelayedEvalute();
}


void CEditDelayed2::OnEnSetfocus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int xxx= 0 ;
}


void CEditDelayed2::OnKillFocus(CWnd* pNewWnd)
{

	DoDelayedEvalute();

	CEdit::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
}
