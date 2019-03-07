// ProjCleanerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyListBox.h"
#include "IconListBox/IconListBox.h"
#include "XHtmlTree/XHtmlTree.h"
#include "ExtStatusControlBar/ExtStatusControlBar.h"
#include "ConfigDlg.h"
#include "MyListCtrl.h"
#include "PictureEx/PictureExt.h"
#include "PictureEx/PictureEx.h"


// CProjCleanerDlg dialog
class CProjCleanerDlg : public CDialog
{
// Construction
public:
	CProjCleanerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PROJCLEANER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//config info ;
	BOOL GetRegProfile(CProfileInfo &m_Profile);
	// end
	BOOL m_bDeling;
	BOOL m_bShowAll;
	// progress ctrl;
	DWORD dwPos;
	CProgressCtrl m_ProgCtrl;
	// progress ctrl end
	CImageList m_PathImgList;
	CMyListCtrl m_LogList;
	CMyListBox m_PathList;
	//CExtStatusControlBar m_StatBar;
	CStatusBarCtrl m_StatBar;
	afx_msg void OnBnClickedAddPath();
	afx_msg void OnBnClickedDelPath();
	afx_msg void OnBnClickedSetInfo();
	afx_msg void OnBnClickedScanDel();
	afx_msg void OnBnClickedLogBtn();
	BOOL EnableWindow(UINT uID, BOOL bEnable = TRUE);
	void ExtendDiaog(BOOL bShow = TRUE);
	void ShowLogMsg(CString strPath, BOOL bFolder, BOOL bSucceed = TRUE);
	//for the tree list;
	CTreeItem *m_pTreeList;
	CTreeItem *GetTreeList();
	void FreeTreeList(CTreeItem *in_pParent = NULL);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_SavePathBtn;
	afx_msg void OnBnClickedSavePathList();
	// tray icon
	BOOL m_CurrIsTrayIcon;
	LRESULT OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam);
	BOOL TrayMyIcon(BOOL bAdd = TRUE);
public:
	virtual void OnCancel();
	afx_msg void OnMShow();
	afx_msg void OnMExit();
	//for the top picture;
	//CPictureExt m_TopPicture;
};
