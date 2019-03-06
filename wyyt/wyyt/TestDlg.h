#pragma once
#include "afxwin.h"
#include "ResizableLib\ResizableDialog.h"
#include "EditDelayed2.h"
// CTestDlg �Ի���

class CTestDlg  : public CResizableDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnChangeFilter(WPARAM wParam, LPARAM lParam);
public:
	CEditDelayed2 m_TestEdit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
};
