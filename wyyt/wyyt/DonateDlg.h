#pragma once


// CDonateDlg 对话框

class CDonateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDonateDlg)

public:
	CDonateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDonateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DONATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
