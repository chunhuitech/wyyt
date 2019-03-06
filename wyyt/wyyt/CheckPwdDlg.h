#pragma once


// CCheckPwdDlg 对话框

class CCheckPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckPwdDlg)

public:
	CCheckPwdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckPwdDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN_PRI };
	void Localize(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
