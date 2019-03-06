// GroupResManageWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "GroupResManageWnd.h"
#include "afxdialogex.h"


// CGroupResManageWnd 对话框

IMPLEMENT_DYNAMIC(CGroupResManageWnd, CDialog)

CGroupResManageWnd::CGroupResManageWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupResManageWnd::IDD, pParent)
{

}

CGroupResManageWnd::~CGroupResManageWnd()
{
}

void CGroupResManageWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGroupResManageWnd, CDialog)
END_MESSAGE_MAP()


// CGroupResManageWnd 消息处理程序
