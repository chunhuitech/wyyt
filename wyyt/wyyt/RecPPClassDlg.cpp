// RecPPClassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecPPClassDlg.h"
#include "afxdialogex.h"
#include "Preferences.h"
#include "wyytdlg.h"

// CRecPPClassDlg 对话框

IMPLEMENT_DYNAMIC(CRecPPClassDlg, CPropertyPage)

CRecPPClassDlg::CRecPPClassDlg()
	: CPropertyPage(CRecPPClassDlg::IDD)
{

}

CRecPPClassDlg::~CRecPPClassDlg()
{
}

void CRecPPClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecPPClassDlg, CPropertyPage)
END_MESSAGE_MAP()


// CRecPPClassDlg 消息处理程序
void CRecPPClassDlg::Localize(void)
{
	if(m_hWnd)
	{
		//SetWindowText(GetResString(IDS_PW_GENERAL));
		//SetDlgItemText(IDC_LANG_FRM,GetResString(IDS_PW_LANG));
	}
}

BOOL CRecPPClassDlg::OnApply()
{
	if(m_bModified)
	{
		SetModified(FALSE);
		m_bModified = false;
	}
	return CPropertyPage::OnApply();
}


BOOL CRecPPClassDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitWindowStyles(this);

//	LoadSettings();
	Localize();
	m_bModified = false; 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}