#pragma once
#include "afxwin.h"


// CPPgDirSet 对话框

class CPPgDirSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgDirSet)

public:
	CPPgDirSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPPgDirSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_PPG_DIRS };
	void Localize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	bool m_bModified;
	virtual BOOL OnApply();
public:
	void LoadSettings(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); }
	afx_msg void OnBnClickedButtonSysattachsel();
	afx_msg void OnBnClickedButtonPriattachsel();
};
