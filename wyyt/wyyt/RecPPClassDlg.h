#pragma once


// CRecPPClassDlg �Ի���

class CRecPPClassDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRecPPClassDlg)

public:
	CRecPPClassDlg();
	virtual ~CRecPPClassDlg();

// �Ի�������
	enum { IDD = IDD_DLG_REC_PP_CLASS };

	void Localize(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	bool m_bModified;
	virtual BOOL OnApply();
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); }
};
