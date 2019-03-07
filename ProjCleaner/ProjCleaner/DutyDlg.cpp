// DutyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "DutyDlg.h"


// CDutyDlg dialog

IMPLEMENT_DYNAMIC(CDutyDlg, CDialog)

CDutyDlg::CDutyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDutyDlg::IDD, pParent)
{
}

CDutyDlg::~CDutyDlg()
{
}

void CDutyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDutyDlg, CDialog)
END_MESSAGE_MAP()


// CDutyDlg message handlers

BOOL CDutyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_StrTitle);
	SetDlgItemText(IDC_EDIT_DUTY, m_Str);

	return TRUE;
}

void CDutyDlg::SetShowString(CString strText, CString strTitle)
{
	m_Str = strText;
	m_StrTitle = strTitle;
}
