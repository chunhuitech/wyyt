// CheckPwdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "CheckPwdDlg.h"
#include "afxdialogex.h"
#include "Preferences.h"
#include "GlobalFunction.h"
#include "OtherFunctions.h"
// CCheckPwdDlg �Ի���

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


// CCheckPwdDlg ��Ϣ�������


BOOL CCheckPwdDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
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
			MessageBox(L"�������");
			return FALSE;
		}
		
	}
	return CDialog::OnCommand(wParam, lParam);
}


BOOL CCheckPwdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitWindowStyles(this);

	//LoadSettings();
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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