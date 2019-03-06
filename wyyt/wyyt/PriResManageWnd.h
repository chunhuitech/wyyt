#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ResizableLib\ResizableDialog.h"
#include "PrivateClassesTreeCtrl.h"
#include "PrivateSharedRecordsCtrl.h"
#include "EditDelayed.h"
#include "SplitterControl.h"
#include "ListViewWalkerPropertySheet.h"
#include "ResPublic.h"
//#include "../interface/DB/DbDefine.h"

// CPriResManageWnd 对话框

class CPriResManageWnd : public CResizableDialog
{
	DECLARE_DYNAMIC(CPriResManageWnd)

public:
	CPriResManageWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPriResManageWnd();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRI_RES_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnChangeFilter(WPARAM wParam, LPARAM lParam);
public:
	CEditDelayed m_ctlFilter_Record;
	CPrivateClassesTreeCtrl m_ctlClassInfoTree;
	CPrivateSharedRecordsCtrl m_ctlRecordsList;
	virtual BOOL OnInitDialog();
	void ShowDetailsPanel(bool bShow);
	void ShowSelectedFilesDetails(bool bForce = false); 
	void Localize();
	void LocalizeAll();
	void DoResize(int delta);
	void SetToolTipsDelay(DWORD dwDelay);
public:
	CSplitterControl m_wndSplitter;
	bool			m_bDetailsVisible;
private:
	CSharedFileDetailsModelessSheet	m_dlgDetails;
public:
	CEdit m_ctlFilter;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLvnItemchangedPrmRecordlist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
