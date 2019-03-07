#pragma once

#include "afxcmn.h"
#include "XHtmlTree/XHtmlTree.h"
#include "XTipComboBox/XTipComboBox.h"
#include "Inc.h"
#include "DutyDlg.h"
#include "afxwin.h"
#include <shlwapi.h>


// CConfigDlg dialog

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CTreeItem *m_pTreeListHead;
public:
	CImageList m_TreeImgList;
	CXHtmlTree m_PathTree;
	CXTipComboBox m_TreeSelect;
	virtual BOOL OnInitDialog();
	BOOL GetRegProfile(CProfileInfo &m_Profile);
/*
	CTreeItem *GetDefaultTreeList();
	BOOL FillParentChildItems(CTreeItem *in_pParent, TCHAR *szString[], size_t tLen);
	void FreeTreeList(CTreeItem *in_pParent = NULL);
	void FillTreeListWithItems(CTreeItem *in_Root, HTREEITEM in_Parent = NULL);
*/
	CTreeItem *GetTreeListByCtrl(HTREEITEM hParent);
	CTreeItem *GetTreeListByFile(LPCTSTR lpszFileName = NULL);
	void FreeTreeList(CTreeItem *in_pParent = NULL);
	afx_msg void OnBnClickedSaveList();
	afx_msg void OnBnClickedLoadList();
	afx_msg void OnBnClickedResDefault();
	afx_msg void OnBnClickedDelAll();
	afx_msg void OnBnClickedApplyBtn();
	CButton m_DelRecycleBtn;
	CButton m_EndCloseBtn;
	CButton m_MinTrayBtn;
	afx_msg void OnBnClickedDelRecycleCheck();
	afx_msg void OnBnClickedEndCloseCheck();
	afx_msg void OnBnClickedMinTrayCheck();
	afx_msg void OnBnClickedSetDefault();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSmingBtn();
	afx_msg void OnBnClickedMinzeBtn();
	CButton m_AssRMenuBtn;
	BOOL RegSetAssociateRMenu(BOOL bAssociate);
};
