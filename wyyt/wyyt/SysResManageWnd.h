#pragma once
#include "ResizableLib\ResizableDialog.h"
#include "afxcmn.h"
#include "FilterDlg.h"
//#include "DirectoryTreeCtrl.h"
#include "SharedDirsTreeCtrl.h"

#include "SplitterControl.h"
#include "ListViewWalkerPropertySheet.h"
#include "SharedFilesCtrl.h"
#include "afxwin.h"

#include "EditDelayed.h"

#include "EditDelayed2.h"
#include "ResPublic.h"

// CSysResManageWnd 对话框

//CSharedFilesWnd
class CSysResManageWnd : public CResizableDialog/*, public CFilterDlg*/
{
	DECLARE_DYNAMIC(CSysResManageWnd)

public:
	CSysResManageWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysResManageWnd();

	virtual void UpdateFilterLabel();
// 对话框数据
	enum { IDD = IDD_DIALOG_SYS_RES_MANAGE };

public:
	void Localize();
	void LocalizeAll();
	void DoResize(int delta);
	void ShowSelectedFilesDetails(bool bForce = false); 
	void ShowDetailsPanel(bool bShow);
	void SetToolTipsDelay(DWORD dwDelay);
	//加载数据函数
	void Reload(bool bForceTreeReload = false);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnChangeFilter(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	CSharedDirsTreeCtrl m_ctlClassInfoTree; //CSharedDirsTreeCtrl  CDirectoryTreeCtrl
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CSharedFilesCtrl m_ctlRecordsList;
	CEdit 	m_ctlFilter;
	CSplitterControl m_wndSplitter;
	bool			m_bDetailsVisible;
private:
	CSharedFileDetailsModelessSheet	m_dlgDetails;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTvnSelchangedSrmClassinfotree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedSflist(NMHDR *pNMHDR, LRESULT *pResult);	
public:
	
	afx_msg void OnClose();
	
	CEditDelayed m_ctlFilter_Record;

	CTestDlg*	m_TestDlg; 
};
