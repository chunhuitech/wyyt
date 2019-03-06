#pragma once
#include "UserMsgs.h"

//#define  UM_DELAYED_EVALUATE (WM_USER + 1000)
// CEditDelayed2

class CEditDelayed2 : public CEdit
{
	DECLARE_DYNAMIC(CEditDelayed2)

public:
	CEditDelayed2();
	virtual ~CEditDelayed2();

protected:
	void	DoDelayedEvalute(bool bForce = false);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
	afx_msg void OnEnSetfocus();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


