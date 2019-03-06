#pragma once
#include "afxwin.h"
#include "ResizableLib\ResizableDialog.h"
#include "EditDelayed2.h"
// CTestDlg 对话框

class CTestDlg  : public CResizableDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnChangeFilter(WPARAM wParam, LPARAM lParam);
public:
	CEditDelayed2 m_TestEdit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
};
