// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "TestDlg.h"
#include "afxdialogex.h"


// CTestDlg �Ի���

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


// CTestDlg ��Ϣ�������

LRESULT CTestDlg::OnChangeFilter(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor curWait; // this may take a while

	int xxx = 0;

	return 0;
}

BOOL CTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CResizableDialog::PreTranslateMessage(pMsg);
}


LRESULT CTestDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CTestDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
