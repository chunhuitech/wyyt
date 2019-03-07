// MyTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "MyTreeCtrl.h"


// CMyTreeCtrl

IMPLEMENT_DYNAMIC(CMyTreeCtrl, CTreeCtrl)

CMyTreeCtrl::CMyTreeCtrl()
{

}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyTreeCtrl::OnNMRclick)
	ON_COMMAND(ID_CM_ADD_FOLDER, &CMyTreeCtrl::OnCmAddFolder)
	ON_COMMAND(ID_CM_ADD_EXT, &CMyTreeCtrl::OnCmAddExt)
	ON_COMMAND(ID_CM_DEL_ITEM, &CMyTreeCtrl::OnCmDelItem)
	ON_COMMAND(ID_CM_DEL_ITEMS, &CMyTreeCtrl::OnCmDelItems)
	ON_COMMAND(ID_CM_RENAME, &CMyTreeCtrl::OnCmRename)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CMyTreeCtrl::OnTvnEndlabeledit)
END_MESSAGE_MAP()



// CMyTreeCtrl message handlers



void CMyTreeCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNM_TREEVIEW pNMTree = (LPNM_TREEVIEW)pNMHDR;
	CPoint pt;
	UINT uFlags = 0;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	HTREEITEM hItem = HitTest(pt, &uFlags);
	if ((hItem != NULL)/* && (TVHT_ONITEM & uFlags)*/) {
		Select(hItem, TVGN_CARET);
	}
	///////////////////////////////////////////////////////////
	CMenu mMenu, *pMenu = NULL;
	mMenu.LoadMenu(IDR_TREE_MENU);
	pMenu = mMenu.GetSubMenu(0);
	ClientToScreen(&pt);
	SetForegroundWindow();
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CMyTreeCtrl::OnCmAddFolder()
{
	HTREEITEM hSelect = GetSelectedItem();
	hSelect = (hSelect == NULL) ? TVI_ROOT : hSelect;
	HTREEITEM hInsert = InsertItem(_T(""), 0, 1, hSelect);
	if ( hInsert ) {
		Select(hInsert, TVGN_CARET);
		EditLabel(hInsert);
	}
}

void CMyTreeCtrl::OnCmAddExt()
{
	HTREEITEM hSelect = GetSelectedItem();
	hSelect = (hSelect == NULL) ? TVI_ROOT : hSelect;
	HTREEITEM hInsert = InsertItem(_T("*."), 2, 2, hSelect);
	if ( hInsert ) {
		Select(hInsert, TVGN_CARET);
		EditLabel(hInsert);
	}
}

void CMyTreeCtrl::OnCmDelItem()
{
	HTREEITEM hSelect = GetSelectedItem();
	if ( hSelect ) {
		DeleteItem(hSelect);
	}
}

void CMyTreeCtrl::OnCmDelItems()
{
	DeleteAllItems();
}

void CMyTreeCtrl::OnCmRename()
{
	HTREEITEM hSelect = GetSelectedItem();
	if ( hSelect ) {
		EditLabel(hSelect);
	}
}

void CMyTreeCtrl::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	if( pTVDispInfo->item.pszText != NULL ) {
		SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	}
	*pResult = 0;
}
