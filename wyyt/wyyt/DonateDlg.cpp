// DonateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "DonateDlg.h"
#include "afxdialogex.h"


// CDonateDlg �Ի���

IMPLEMENT_DYNAMIC(CDonateDlg, CDialog)

CDonateDlg::CDonateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDonateDlg::IDD, pParent)
{

}

CDonateDlg::~CDonateDlg()
{
}

void CDonateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDonateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDonateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDonateDlg ��Ϣ�������


void CDonateDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


BOOL CDonateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString BankInfo = L"\r\n\r\n\r\n����ת�ˣ��й����� ������԰·֧�� \r\n\r\n\r\n��        �����γɽ� \r\n\r\n\r\n��        ����6013822000618365301";
	SetDlgItemText(IDC_EDIT_BANK,BankInfo);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
