#pragma once
#include "afxwin.h"


// CPPgGeneral �Ի���

class CPPgGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgGeneral)

public:
	CPPgGeneral();
	virtual ~CPPgGeneral();

// �Ի�������
	enum { IDD = IDD_DIALOG_PPG_GENERAL };

	void Localize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	bool m_bModified;
	virtual BOOL OnApply();
public:
	CComboBox m_language;
	void LoadSettings(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeLangs();
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); }
	afx_msg void OnBnClickedButtonVerinfo();
	afx_msg void OnBnClickedButtonUpsysdata();
};
