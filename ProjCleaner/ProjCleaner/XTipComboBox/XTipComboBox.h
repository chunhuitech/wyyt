// XTipComboBox.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.  You are free to use it 
// in any way you like.
//
// This software is provided "as is" with no expressed or implied warranty.  
// I accept no liability for any damage or loss of business that this software 
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XTIPCOMBOBOX_H
#define XTIPCOMBOBOX_H

#include "XTipListBox.h"
#include "../XHtmlTree/XHtmlTree.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CXTipComboBox window

class CXTipComboBox : public CComboBox
{
// Construction
public:
	CXTipComboBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTipComboBox)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CXTipListBox	m_listbox;
	HWND			m_hWndToolTip;
	TOOLINFO		m_ToolInfo;
	CWnd *m_pParentWnd;
	int m_CurSel;

	// Generated message map functions
protected:
	//{{AFX_MSG(CXTipComboBox)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void SetParentWnd(CWnd *pWnd);
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnDropdown();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XTIPCOMBOBOX_H
