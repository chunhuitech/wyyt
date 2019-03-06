// PPgSecurity.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "PPgSecurity.h"
//#include "afxdialogex.h"
#include "Preferences.h"
#include "OtherFunctions.h"
#include "CheckPwdDlg.h"
#include "GlobalFunction.h"
// CPPgSecurity 对话框

IMPLEMENT_DYNAMIC(CPPgSecurity, CPropertyPage)

CPPgSecurity::CPPgSecurity()
	: CPropertyPage(CPPgSecurity::IDD)
{

}

CPPgSecurity::~CPPgSecurity()
{
}

void CPPgSecurity::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPPgSecurity, CDialog)
	ON_BN_CLICKED(IDC_CHECK_PRI_PWD_ENABLE, &CPPgSecurity::OnBnClickedCheckPriPwdEnable)
	ON_EN_CHANGE(IDC_EDIT_USERNAME, &CPPgSecurity::OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_PWD, &CPPgSecurity::OnSettingsChange)
END_MESSAGE_MAP()


// CPPgSecurity 消息处理程序
void CPPgSecurity::LoadSettings(void)
{
	if (!thePrefs.IsPriPwdCheckSupported())
	{
		CheckDlgButton(IDC_CHECK_PRI_PWD_ENABLE,1);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(TRUE);
		//GetDlgItem(IDC_CHECK_PRI_PWD_ENABLE)->EnableWindow(FALSE);
		
	}
	else
	{
		CheckDlgButton(IDC_CHECK_PRI_PWD_ENABLE,0);
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
	}

	SetDlgItemText(IDC_EDIT_USERNAME,thePrefs.m_strUserName);
	SetDlgItemText(IDC_EDIT_PWD,thePrefs.m_strUserPwd);
}


BOOL CPPgSecurity::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitWindowStyles(this);

	LoadSettings();
	Localize();

	
	m_bModified = false; 

	return TRUE;  
}


BOOL CPPgSecurity::OnApply()
{
	if(m_bModified) 
	{
		CString strUserName;
		GetDlgItemText(IDC_EDIT_USERNAME, strUserName);		
		CString cstrUserPwd;
		GetDlgItemText(IDC_EDIT_PWD, cstrUserPwd);

		if ( strUserName.IsEmpty() || cstrUserPwd.IsEmpty() )
		{
			MessageBox(GetResString(IDS_PROP_USERPWD_EMPTY));
			return FALSE;
		}
		thePrefs.m_bPriPwdCheckSupported = IsDlgButtonChecked(IDC_CHECK_PRI_PWD_ENABLE) == 0;//不支持为真
		if (!thePrefs.m_strUserPwd.IsEmpty())
		{
			CCheckPwdDlg dlgCheck;
			if(dlgCheck.DoModal()  == IDCANCEL)
			{
				return FALSE;
			}
		}

		if ( thePrefs.m_strUserPwd  != cstrUserPwd )//密码已改变
		{
			string strUserPwd = CGlobalFunction::WideCharToMultiByte(cstrUserPwd.GetBuffer(0));
			string strClipherUserPwd =StringEncrypt(strUserPwd.c_str(),strUserPwd.length()).c_str();
			wstring wstrUserPwd = CGlobalFunction::MultiByteToWideChar(strClipherUserPwd.c_str());
			thePrefs.m_strUserPwd = wstrUserPwd.c_str();
		}
		
		thePrefs.m_strUserName = strUserName;

		LoadSettings();
		SetModified(FALSE);
		m_bModified = false; 
	}
	return CPropertyPage::OnApply();
}


void CPPgSecurity::Localize(void)
{
	if (m_hWnd)
	{
		SetWindowText(GetResString(IDS_SECURITY));
		SetDlgItemText(IDC_CHECK_PRI_PWD_ENABLE,GetResString(IDS_PRI_PWD_ENABLE));
		SetDlgItemText(IDC_STATIC_USERNAME,GetResString(IDS_PRI_USERNAME));
		SetDlgItemText(IDC_STATIC_PWD,GetResString(IDS_PRI_PWD));
	}
}


void CPPgSecurity::OnBnClickedCheckPriPwdEnable()
{
	if (IsDlgButtonChecked(IDC_CHECK_PRI_PWD_ENABLE) == 0)
	{
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
		//CheckDlgButton(IDC_ONLYOBFUSCATED, 0);
	}
	else
	{
		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PWD)->EnableWindow(TRUE);
	}
	OnSettingsChange();
}
