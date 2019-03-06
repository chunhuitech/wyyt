#pragma once


// CSysResSearchDlg 对话框

class CSysResSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysResSearchDlg)

public:
	CSysResSearchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysResSearchDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SYSRES_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
