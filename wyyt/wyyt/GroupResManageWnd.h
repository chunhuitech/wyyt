#pragma once


// CGroupResManageWnd 对话框

class CGroupResManageWnd : public CDialog
{
	DECLARE_DYNAMIC(CGroupResManageWnd)

public:
	CGroupResManageWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupResManageWnd();

// 对话框数据
	enum { IDD = IDD_DIALOG_GROUP_RES_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
