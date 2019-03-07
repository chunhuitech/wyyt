#pragma once


// CDutyDlg dialog

class CDutyDlg : public CDialog
{
	DECLARE_DYNAMIC(CDutyDlg)

public:
	CDutyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDutyDlg();

// Dialog Data
	enum { IDD = IDD_DUTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Str;
	CString m_StrTitle;
	virtual BOOL OnInitDialog();
	void SetShowString(CString strText, CString strTitle);
};
