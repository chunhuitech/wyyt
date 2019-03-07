#pragma once
#include "IconListBox/IconListBox.h"

// CMyListBox

class CMyListBox : public CIconListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMAddPath();
	afx_msg void OnMDelPath();
	afx_msg void OnMDelPaths();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


