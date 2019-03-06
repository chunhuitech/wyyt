#pragma once


// CAddModClassInfoDlg 对话框

class CAddModClassInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddModClassInfoDlg)

public:
	CAddModClassInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddModClassInfoDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ADDMOD_CLASSINFO };

	void	SetLabels(CString title, CString csOldClassName = NULL );
	void	GetResult(CString& csNewClassName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	
	CString m_csTitle;
	//CString m_csLabClassName;
	CString m_csTxtClassName;
	HICON	m_icMain;
	//bool	m_cancel;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
