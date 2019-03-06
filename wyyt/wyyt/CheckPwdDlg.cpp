// CheckPwdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "CheckPwdDlg.h"
#include "afxdialogex.h"
#include "Preferences.h"
#include "GlobalFunction.h"
#include "OtherFunctions.h"
// CCheckPwdDlg 对话框

IMPLEMENT_DYNAMIC(CCheckPwdDlg, CDialog)

CCheckPwdDlg::CCheckPwdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckPwdDlg::IDD, pParent)
{

}

CCheckPwdDlg::~CCheckPwdDlg()
{
}

void CCheckPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckPwdDlg, CDialog)
END_MESSAGE_MAP()


// CCheckPwdDlg 消息处理程序


BOOL CCheckPwdDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (wParam == IDOK )
	{
		CString strUserName;
		GetDlgItemText(IDC_EDIT_USERNAME_CHECK, strUserName);
		CString cstrUserPwd;
		GetDlgItemText(IDC_EDIT_USERPWD_CHECK, cstrUserPwd);
		string strUserPwd = CGlobalFunction::WideCharToMultiByte(thePrefs.m_strUserPwd.GetBuffer(0));		
		string strPlainUserPwd = StringDecrypt(strUserPwd.c_str());
		wstring wstrUserPwd = CGlobalFunction::MultiByteToWideChar(strPlainUserPwd.c_str());
		CString cwstrUserPwd = wstrUserPwd.c_str();		
		if ( thePrefs.m_strUserName == strUserName && cwstrUserPwd == cstrUserPwd )
		{
			return CDialog::OnCommand(wParam, lParam);
		}
		else
		{
			MessageBox(L"密码错误！");
			return FALSE;
		}
		
	}
	return CDialog::OnCommand(wParam, lParam);
}


BOOL CCheckPwdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitWindowStyles(this);

	//LoadSettings();
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCheckPwdDlg::Localize(void)
{
	if (m_hWnd)
	{
		SetWindowText(GetResString(IDS_PASSCHECK));
		SetDlgItemText(IDC_STATIC_SHOW_USERNAME,GetResString(IDS_PRI_USERNAME));
		SetDlgItemText(IDC_STATIC_SHOW_USERPWD,GetResString(IDS_PRI_PWD));
	}
}