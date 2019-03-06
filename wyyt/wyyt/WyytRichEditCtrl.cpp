///////////////////////////////////////////////////////
//sonystone 2002.4.3
///////////////////////////////////////////////////////
// WyytRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WyytRichEditCtrl.h"
#include "OtherFunctions.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_RICH_UNDO                    101
#define ID_RICH_CUT                     102
#define ID_RICH_COPY                    103
#define ID_RICH_PASTE                   104
#define ID_RICH_CLEAR                   105
#define ID_RICH_SELECTALL               106
#define ID_RICH_SETFONT                 107

/////////////////////////////////////////////////////////////////////////////
// CWyytRichEditCtrl

CWyytRichEditCtrl::CWyytRichEditCtrl()
{
}

CWyytRichEditCtrl::~CWyytRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CWyytRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CWyytRichEditCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_RICH_COPY, OnCopy)
	ON_COMMAND(ID_RICH_CUT, OnCut)
	ON_COMMAND(ID_RICH_PASTE, OnPaste)
	ON_COMMAND(ID_RICH_SELECTALL, OnSelectall)
	ON_COMMAND(ID_RICH_UNDO, OnUndo)
	ON_COMMAND(ID_RICH_CLEAR, OnClear)
	ON_COMMAND(ID_RICH_SETFONT, OnSelectfont)
	ON_NOTIFY_REFLECT_EX(EN_LINK, OnEnLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWyytRichEditCtrl message handlers

void CWyytRichEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//设置为焦点
	SetFocus();
	//创建一个弹出式菜单
	CMenu popmenu;
	popmenu.CreatePopupMenu();
	//添加菜单项目
	popmenu.AppendMenu(0, ID_RICH_UNDO, GetResString(IDS_RICH_UNDO));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_CUT, GetResString(IDS_RICH_CUT));
	popmenu.AppendMenu(0, ID_RICH_COPY, GetResString(IDS_RICH_COPY));
	popmenu.AppendMenu(0, ID_RICH_PASTE, GetResString(IDS_RICH_PASTE));
	popmenu.AppendMenu(0, ID_RICH_CLEAR, GetResString(IDS_RICH_CLEAR));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SELECTALL, GetResString(IDS_RICH_SELECTALL));
	popmenu.AppendMenu(0, MF_SEPARATOR);
	popmenu.AppendMenu(0, ID_RICH_SETFONT, GetResString(IDS_RICH_SETFONT));

	//初始化菜单项
	UINT nUndo=(CanUndo() ? 0 : MF_GRAYED );
	popmenu.EnableMenuItem(ID_RICH_UNDO, MF_BYCOMMAND|nUndo);

	UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED) ;
	popmenu.EnableMenuItem(ID_RICH_CUT, MF_BYCOMMAND|nSel);
	popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND|nSel);
	popmenu.EnableMenuItem(ID_RICH_CLEAR, MF_BYCOMMAND|nSel);
	
	UINT nPaste=(CanPaste() ? 0 : MF_GRAYED) ;
	popmenu.EnableMenuItem(ID_RICH_PASTE, MF_BYCOMMAND|nPaste);

	//显示菜单
	CPoint pt;
	GetCursorPos(&pt);
	popmenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	popmenu.DestroyMenu();
	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CWyytRichEditCtrl::OnSelectfont() 
{
	// TODO: Add your command handler code here
	CHARFORMAT cf;
	LOGFONT lf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	memset(&lf, 0, sizeof(LOGFONT));
	//判断是否选择了内容
	BOOL m_bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;
	if (m_bSelect)
	{
		GetSelectionCharFormat(cf);
	}
	else
	{
		GetDefaultCharFormat(cf);
	}
	//得到相关字体属性
	BOOL bIsBold = cf.dwEffects & CFE_BOLD;
	BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;
	BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;
	BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;
	//设置属性
	lf.lfCharSet = cf.bCharSet;
	lf.lfHeight = cf.yHeight/15;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	lf.lfItalic = bIsItalic;
	lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);
	lf.lfUnderline = bIsUnderline;
	lf.lfStrikeOut = bIsStrickout;
	//sprintf(lf.lfFaceName, cf.szFaceName);
	_snwprintf(lf.lfFaceName,32, cf.szFaceName);
	
	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = cf.crTextColor;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCharFormat(cf);//获得所选字体的属性
		if (m_bSelect) 
			SetSelectionCharFormat(cf);	//为选定的内容设定所选字体
		else
			SetWordCharFormat(cf);	//为将要输入的内容设定字体
	}
	


}
BOOL CWyytRichEditCtrl::OnEnLink(NMHDR *pNMHDR, LRESULT *pResult)
{
	BOOL bMsgHandled = FALSE;
	*pResult = 0;
	ENLINK* pEnLink = reinterpret_cast<ENLINK *>(pNMHDR);
	if (pEnLink && pEnLink->msg == WM_LBUTTONDOWN)
	{
		CString strUrl;
		GetTextRange(pEnLink->chrg.cpMin, pEnLink->chrg.cpMax, strUrl);

		ShellExecute(NULL, NULL, strUrl, NULL, NULL, SW_SHOWDEFAULT);
		*pResult = 1;
		bMsgHandled = TRUE; // do not route this message to any parent
	}
	return bMsgHandled;
}
