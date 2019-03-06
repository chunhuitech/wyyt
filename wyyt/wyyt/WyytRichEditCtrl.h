///////////////////////////////////////////////////////
//sonystone 2002.4.3
///////////////////////////////////////////////////////
#if !defined(AFX_WyytRichEditCtrl_H__FCCA69DA_B946_41E2_9D28_FDF410CADD1B__INCLUDED_)
#define AFX_WyytRichEditCtrl_H__FCCA69DA_B946_41E2_9D28_FDF410CADD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WyytRichEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWyytRichEditCtrl window

class CWyytRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CWyytRichEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWyytRichEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWyytRichEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWyytRichEditCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCopy() { Copy(); }
	afx_msg void OnCut() { Cut(); }
	afx_msg void OnPaste() { Paste(); }
	afx_msg void OnSelectall() { SetSel(0, -1); }
	afx_msg void OnUndo() { Undo(); }
	afx_msg void OnClear() { Clear(); }
	afx_msg void OnSelectfont();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WyytRichEditCtrl_H__FCCA69DA_B946_41E2_9D28_FDF410CADD1B__INCLUDED_)
