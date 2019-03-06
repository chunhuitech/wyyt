#pragma once


// CRecPPClassDlg 对话框

class CRecPPClassDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CRecPPClassDlg)

public:
	CRecPPClassDlg();
	virtual ~CRecPPClassDlg();

// 对话框数据
	enum { IDD = IDD_DLG_REC_PP_CLASS };

	void Localize(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	bool m_bModified;
	virtual BOOL OnApply();
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()					{ m_bModified = true;	SetModified(); }
};
