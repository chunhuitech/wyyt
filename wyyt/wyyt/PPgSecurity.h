#pragma once


// CPPgSecurity �Ի���

class CPPgSecurity : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgSecurity)

public:
	CPPgSecurity();   // ��׼���캯��
	virtual ~CPPgSecurity();

// �Ի�������
	enum { IDD = IDD_PPG_SECURITY };

	void Localize(void);
protected:
	bool m_bModified;
	void LoadSettings(void);
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); } 
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckPriPwdEnable();
};
