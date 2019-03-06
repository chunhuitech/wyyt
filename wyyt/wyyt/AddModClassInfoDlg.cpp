// AddModClassInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "AddModClassInfoDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"


// CAddModClassInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CAddModClassInfoDlg, CDialog)

CAddModClassInfoDlg::CAddModClassInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddModClassInfoDlg::IDD, pParent)
{
	m_icMain = NULL;
}

CAddModClassInfoDlg::~CAddModClassInfoDlg()
{
	if (m_icMain)
		VERIFY( DestroyIcon(m_icMain) );
}

void CAddModClassInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddModClassInfoDlg, CDialog)
END_MESSAGE_MAP()


// CAddModClassInfoDlg ��Ϣ�������
void CAddModClassInfoDlg::SetLabels(CString title, CString csOldClassName)
{
	m_csTitle = title;
	m_csTxtClassName = csOldClassName;
	
}
void CAddModClassInfoDlg::GetResult(CString& csNewClassName)
{
	csNewClassName = m_csTxtClassName;
}
BOOL CAddModClassInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitWindowStyles(this);
	SetIcon( m_icMain = theApp.LoadIcon(_T("RENAME")),FALSE);

	SetDlgItemText(IDC_STATIC_NEWCLASSNAME,GetResString(IDS_STATIC_CLASSINFO_NEWNAME));
	SetWindowText(m_csTitle);

	SetDlgItemText(IDC_EDIT_NEWCLASSNAME,m_csTxtClassName);

	SetDlgItemText(IDOK, GetResString(IDS_TREEOPTIONS_OK) );
	SetDlgItemText(IDCANCEL,GetResString(IDS_CANCEL));


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CAddModClassInfoDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	GetDlgItemText(IDC_EDIT_NEWCLASSNAME, m_csTxtClassName);
	m_csTxtClassName.Trim();
	CDialog::OnOK();
}
