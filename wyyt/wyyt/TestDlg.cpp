// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "TestDlg.h"
#include "afxdialogex.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEST, m_TestEdit);
}


BEGIN_MESSAGE_MAP(CTestDlg, CResizableDialog)
	ON_MESSAGE(UM_DELAYED_EVALUATE, OnChangeFilter)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

LRESULT CTestDlg::OnChangeFilter(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor curWait; // this may take a while

	int xxx = 0;

	return 0;
}

BOOL CTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CResizableDialog::PreTranslateMessage(pMsg);
}


LRESULT CTestDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CTestDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
