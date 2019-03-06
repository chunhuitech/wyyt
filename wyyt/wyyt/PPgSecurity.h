#pragma once


// CPPgSecurity 对话框

class CPPgSecurity : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgSecurity)

public:
	CPPgSecurity();   // 标准构造函数
	virtual ~CPPgSecurity();

// 对话框数据
	enum { IDD = IDD_PPG_SECURITY };

	void Localize(void);
protected:
	bool m_bModified;
	void LoadSettings(void);
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); } 
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckPriPwdEnable();
};
