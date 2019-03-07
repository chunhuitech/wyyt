#pragma once

#include "XHtmlTree/XHtmlTree.h"


// CMyTreeCtrl

class CMyTreeCtrl : public CXHtmlTree
{
	DECLARE_DYNAMIC(CMyTreeCtrl)

public:
	CMyTreeCtrl();
	virtual ~CMyTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCmAddFolder();
	afx_msg void OnCmAddExt();
	afx_msg void OnCmDelItem();
	afx_msg void OnCmDelItems();
	afx_msg void OnCmRename();
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};


