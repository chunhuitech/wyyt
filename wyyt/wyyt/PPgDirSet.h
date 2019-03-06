#pragma once
#include "afxwin.h"


// CPPgDirSet �Ի���

class CPPgDirSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgDirSet)

public:
	CPPgDirSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPPgDirSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_PPG_DIRS };
	void Localize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
