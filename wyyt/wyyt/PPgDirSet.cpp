// PPgDirSet.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "PPgDirSet.h"
#include "afxdialogex.h"
#include "Preferences.h"
#include "wyytdlg.h"

// CPPgDirSet 对话框

IMPLEMENT_DYNAMIC(CPPgDirSet, CPropertyPage)

CPPgDirSet::CPPgDirSet(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CPPgDirSet::IDD)
{

}

CPPgDirSet::~CPPgDirSet()
{
}

void CPPgDirSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPPgDirSet, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SYSATTACHSEL, &CPPgDirSet::OnBnClickedButtonSysattachsel)
	ON_EN_CHANGE(IDC_EDIT_SYSATTACHPATH, &CPPgDirSet::OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_PRIATTACHPATH, &CPPgDirSet::OnSettingsChange)
	ON_BN_CLICKED(IDC_BUTTON_PRIATTACHSEL, &CPPgDirSet::OnBnClickedButtonPriattachsel)
END_MESSAGE_MAP()


// CPPgDirSet 消息处理程序
void CPPgDirSet::Localize(void)
{
	if(m_hWnd)
	{
		SetWindowText(GetResString(IDS_PW_DIRSET));
		//SetDlgItemText(IDC_LANG_FRM,GetResString(IDS_PW_LANG));
		
	}
}

BOOL CPPgDirSet::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitWindowStyles(this);
	((CEdit*)GetDlgItem(IDC_EDIT_SYSATTACHPATH))->SetLimitText(MAX_PATH);
	LoadSettings();
	Localize();
	m_bModified = false; 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPPgDirSet::LoadSettings(void)
{
	SetDlgItemText(IDC_STATIC_SYSATTACHPATH,GetResString(IDS_SELECT_SYSATTACHDIR));
	SetDlgItemText(IDC_STATIC_PRIATTACHPATH,GetResString(IDS_SELECT_PRIATTACHDIR));
	SetDlgItemText(IDC_EDIT_SYSATTACHPATH,thePrefs.m_strSysAttachPath);
	SetDlgItemText(IDC_EDIT_PRIATTACHPATH,thePrefs.m_strPriAttachPath);
}


BOOL CPPgDirSet::OnApply()
{
	if(m_bModified)
	{
		

		CString strSysAttachDir;
		GetDlgItemText(IDC_EDIT_SYSATTACHPATH, strSysAttachDir);
		MakeFoldername(strSysAttachDir);
		if (strSysAttachDir.IsEmpty()){
			strSysAttachDir = thePrefs.GetDefaultDirectory(WYYT_ATTACHMENTS_SYS_DIR, true); // will create the directory here if it doesnt exists
			SetDlgItemText(IDC_EDIT_SYSATTACHPATH, strSysAttachDir);
		}
		thePrefs.m_strSysAttachPath = strSysAttachDir;
		MakeFoldername(thePrefs.m_strSysAttachPath);


		CString strPriAttachDir;
		GetDlgItemText(IDC_EDIT_PRIATTACHPATH, strPriAttachDir);
		MakeFoldername(strPriAttachDir);
		if (strPriAttachDir.IsEmpty()){
			strPriAttachDir = thePrefs.GetDefaultDirectory(WYYT_ATTACHMENTS_PRIVATE_DIR, true); // will create the directory here if it doesnt exists
			SetDlgItemText(IDC_EDIT_PRIATTACHPATH, strPriAttachDir);
		}
		thePrefs.m_strPriAttachPath = strPriAttachDir;
		MakeFoldername(thePrefs.m_strPriAttachPath);

		LoadSettings();

		SetModified(FALSE);
		m_bModified = false;
	}
	return CPropertyPage::OnApply();
}


void CPPgDirSet::OnBnClickedButtonSysattachsel()
{
	TCHAR buffer[MAX_PATH] = {0};
	GetDlgItemText(IDC_EDIT_SYSATTACHPATH, buffer, _countof(buffer));
	if(SelectDir(GetSafeHwnd(),buffer,GetResString(IDS_SELECT_SYSATTACHDIR)))
		SetDlgItemText(IDC_EDIT_SYSATTACHPATH,buffer);
}




void CPPgDirSet::OnBnClickedButtonPriattachsel()
{
	TCHAR buffer[MAX_PATH] = {0};
	GetDlgItemText(IDC_EDIT_PRIATTACHPATH, buffer, _countof(buffer));
	if(SelectDir(GetSafeHwnd(),buffer,GetResString(IDS_SELECT_PRIATTACHDIR)))
		SetDlgItemText(IDC_EDIT_PRIATTACHPATH,buffer);
}
