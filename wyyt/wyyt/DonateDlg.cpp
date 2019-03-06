// DonateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "DonateDlg.h"
#include "afxdialogex.h"


// CDonateDlg 对话框

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


// CDonateDlg 消息处理程序


void CDonateDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


BOOL CDonateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString BankInfo = L"\r\n\r\n\r\n银行转账：中国银行 深圳艺园路支行 \r\n\r\n\r\n户        名：何成进 \r\n\r\n\r\n账        户：6013822000618365301";
	SetDlgItemText(IDC_EDIT_BANK,BankInfo);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
