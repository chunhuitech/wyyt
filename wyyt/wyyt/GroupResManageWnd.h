#pragma once


// CGroupResManageWnd �Ի���

class CGroupResManageWnd : public CDialog
{
	DECLARE_DYNAMIC(CGroupResManageWnd)

public:
	CGroupResManageWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupResManageWnd();

// �Ի�������
	enum { IDD = IDD_DIALOG_GROUP_RES_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
