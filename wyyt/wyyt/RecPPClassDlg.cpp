// RecPPClassDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecPPClassDlg.h"
#include "afxdialogex.h"
#include "Preferences.h"
#include "wyytdlg.h"

// CRecPPClassDlg �Ի���

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


// CRecPPClassDlg ��Ϣ�������
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
	// �쳣: OCX ����ҳӦ���� FALSE
}