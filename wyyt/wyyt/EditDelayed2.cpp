// EditDelayed.cpp : 实现文件
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



// CEditDelayed2 消息处理程序


void CEditDelayed2::DoDelayedEvalute(bool bForce)
{
	int m_nCurrentColumnIdx = 5;
	CString m_strLastEvaluatedContent = L"abcd";
	GetParent()->SendMessage(UM_DELAYED_EVALUATE, (WPARAM)m_nCurrentColumnIdx, (LPARAM)(LPCTSTR)m_strLastEvaluatedContent);
}

void CEditDelayed2::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	DoDelayedEvalute();
}


void CEditDelayed2::OnEnSetfocus()
{
	// TODO: 在此添加控件通知处理程序代码
	int xxx= 0 ;
}


void CEditDelayed2::OnKillFocus(CWnd* pNewWnd)
{

	DoDelayedEvalute();

	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
}
