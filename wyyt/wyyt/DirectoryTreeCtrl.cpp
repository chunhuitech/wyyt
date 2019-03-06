//this file is part of eMule
//Copyright (C)2002-2008 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
#include "wyyt.h"
#include "DirectoryTreeCtrl.h"
#include "otherfunctions.h"
#include "Preferences.h"
#include "TitleMenu.h"
#include "UserMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////
// written by robert rostek - tecxx@rrs.at //
/////////////////////////////////////////////

struct STreeItem
{
	CString strPath;
};


// CDirectoryTreeCtrl

IMPLEMENT_DYNAMIC(CDirectoryTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CDirectoryTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnTvnGetdispinfo)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteItem)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CDirectoryTreeCtrl::CDirectoryTreeCtrl()
{
	m_bSelectSubDirs = false;
	m_nDataSourceType = EDST_SYS;

	//m_pRootAllClassInfos = NULL;
	//m_pRootNotesClassInfos = NULL;
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		m_ArrayRootClassInfos[i] = NULL;
	}

	m_nMainClassID = ERROR_CLASSID;
}

CDirectoryTreeCtrl::~CDirectoryTreeCtrl()
{
	// don't destroy the system's image list
	m_image.Detach();

	//delete m_pRootAllClassInfos;
	//delete m_pRootNotesClassInfos;
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		if ( m_ArrayRootClassInfos[i] )
		{
			delete m_ArrayRootClassInfos[i];
		}		
	}
}

void CDirectoryTreeCtrl::OnDestroy()
{
	// If a treeview control is created with TVS_CHECKBOXES, the application has to 
	// delete the image list which was implicitly created by the control.
	CImageList *piml = GetImageList(TVSIL_STATE);
	if (piml)
		piml->DeleteImageList();

	CTreeCtrl::OnDestroy();
}

void CDirectoryTreeCtrl::DeleteChildItems(CClassInfoItem* pParent)
{
	while(!pParent->liSubDirectories.IsEmpty())
	{
		CClassInfoItem* pToDelete = pParent->liSubDirectories.RemoveHead();

		DeleteItem(pToDelete->m_htItem);

		DeleteChildItems(pToDelete);
		delete pToDelete;
	}
}

void CDirectoryTreeCtrl::AddClassInfoChildItem(CClassInfoItem* pRoot, int64 nParentID, bool bTopLevel)
{
	if ( EMT_SYS == m_emt )
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( it->ParentID == nParentID)
			{
				AddTreeClassInfoChildItem(pRoot, bTopLevel, *it);
			}
		}
	}
	else
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( it->ParentID == nParentID)
			{
				AddTreeClassInfoChildItem(pRoot, bTopLevel, *it);
			}
		}
	}
}
//
//bool CDirectoryTreeCtrl::IsSelected(int64 nCID)
//{
//	for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
//	{
//		int64 nID = m_lstAllSelectIDs.GetNext(pos);
//		if ( nID == nCID )
//			return true;
//	}
//	return false;
//}
//
//void CDirectoryTreeCtrl::AddSelected(int64 nCID)
//{
//	if ( IsSelected(nCID) )
//		return;
//
//	m_lstAllSelectIDs.AddTail(nCID);
//}
//
//void CDirectoryTreeCtrl::DelSelected(int64 nCID)
//{
//	for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
//	{
//		POSITION pos2 = pos;
//		int64 nID = m_lstAllSelectIDs.GetNext(pos);
//		if (nCID == nID)
//			m_lstAllSelectIDs.RemoveAt(pos2);
//	}
//}
//传入的是当前选中项的父项nParentCID 
//判断其父项nParentCID 下所有的子项是否有选中的，有则返回真
//即递归遍历其所有子类，子类中有任一个在选中列表中，则返回真,同时将相应父类设置为选中，才能递归向上设置为选中和字段颜色的变化
bool CDirectoryTreeCtrl::HasSelectedSubclass(int64 nParentCID/*, HTREEITEM hParent*/)
{
	//HTREEITEM hParent_Parent = GetParentItem(hParent);
	//遍历查找直接子类
	if ( EMT_SYS == m_emt )
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( nParentCID == it->ParentID )
			{
				for (vector<int64>::iterator itSel = m_vecSelected.begin(); itSel != m_vecSelected.end(); itSel++ )
				{
					int64 nID = *itSel;
					if ( nID == it->ID )
						return true;
				}
				if ( HasSelectedSubclass(it->ID/*,hParent_Parent*/) )
					return true; //加上这句，否则只有直接父节点变颜色
			}
		}
	}
	else
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( nParentCID == it->ParentID )
			{
				for (vector<int64>::iterator itSel = m_vecSelected.begin(); itSel != m_vecSelected.end(); itSel++ )
				{
					int64 nID = *itSel;
					if ( nID == it->ID )
						return true;
				}
				if ( HasSelectedSubclass(it->ID/*,hParent_Parent*/) )
					return true; //加上这句，否则只有直接父节点变颜色
			}
		}
	}

	return false;
	
}
//bool CDirectoryTreeCtrl::HasSelectedSubclass(int64 nParentCID)
//{
//	//遍历查找直接子类
//	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end(); it++ )
//	{
//		if ( nParentCID == it->ParentID )
//		{
//			if ( IfSelectedSubclass(it->ID/*,hParent_Parent*/) )
//				return true; //加上这句，否则只有直接父节点变颜色
//		}
//	}
//
//	return false;
//}
void CDirectoryTreeCtrl::AddTreeClassInfoChildItem(CClassInfoItem* pRoot, bool bTopLevel, stSysClassInfoItem Cii)
{
	TVINSERTSTRUCT itInsert;
	memset(&itInsert, 0, sizeof(itInsert));

	//TVIF_CHILDREN cChildren 成员有效 
	//TVIF_HANDLE  hItem 成员有效. 
	//TVIF_TEXT pszText和cchTextMax成员有效 hItem 这个函数引用的项。 
	//TVIF_STATE state和stateMask成员有效. 
	//TVIF_IMAGE iImage成员有效. 
	//TVIF_SELECTEDIMAGE iSelectedImage成员有效. 
	itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	//TVIS_STATEIMAGEMASK  要设置和取回一个项的状态图像索引，设置TVIS_STATEIMAGEMASK位
	itInsert.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;

	//if ( it->sHaseNode ) //共享标记
	//	itInsert.item.state = TVIS_BOLD;
	//else
	//	itInsert.item.state = 0;

	if ( Cii.ID == m_nMainClassID )//主分类标志
	{
		//要设置或取回一个项的覆盖图像的索引，设置TVIS_OVERLAYMASK位
		itInsert.item.stateMask |= TVIS_OVERLAYMASK;
		/////////////////
		//state 位标记和图像列表索引的设置  0至7位包含了项的状态 8至11位指定了以1为基准的覆盖图像索引(隔离这些位，使用TVIS_OVERLAYMASK掩码,设置覆盖图像索引，使用INDEXTOOVERLAYMASK宏)
		itInsert.item.state |= INDEXTOOVERLAYMASK(1);
	}
	if ( EMT_SYS == m_emt )
	{
		if (  wyytExtendDataManage::Instance().IfHaseSubClass(Cii.ID) )
			itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
		else
			itInsert.item.cChildren = 0;
	}
	else
	{
		if (  wyytExtendDataManagePri::Instance().IfHaseSubClass(Cii.ID) )
			itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
		else
			itInsert.item.cChildren = 0;
	}
	

	CClassInfoItem* pti = new CClassInfoItem((LPCTSTR)Cii.cname.c_str(), 0, SCII_ALL_OTHER,Cii.ID, Cii.ParentID,Cii.sHaseNode,Cii.sHaseRecomm);

	itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)Cii.cname.c_str());
	itInsert.hInsertAfter = !bTopLevel ? TVI_SORT : TVI_LAST;
	itInsert.hParent = pRoot->m_htItem;
	itInsert.item.mask |= TVIF_PARAM;//lParam成员有效. 
	itInsert.item.lParam = (LPARAM)pti;


	if ( Cii.sHaseRecomm )//推荐标记
	{
		//要设置或取回一个项的覆盖图像的索引，设置TVIS_OVERLAYMASK位
		itInsert.item.stateMask |= TVIS_OVERLAYMASK;
		/////////////////
		//state 位标记和图像列表索引的设置  0至7位包含了项的状态 8至11位指定了以1为基准的覆盖图像索引(隔离这些位，使用TVIS_OVERLAYMASK掩码,设置覆盖图像索引，使用INDEXTOOVERLAYMASK宏)
		itInsert.item.state |= INDEXTOOVERLAYMASK(1);
	}

	if ( Cii.sHaseNode ) //图标索引
		itInsert.item.iImage = TII_NODES;
	else
		itInsert.item.iImage = TII_NODE;

	//设置选中的图标
	itInsert.item.iSelectedImage = TII_NODE_SELECTED;


	pti->m_htItem = InsertItem(&itInsert);
	pRoot->liSubDirectories.AddTail(pti);

	if ( IsSelected(Cii.ID) )
	{
		SetCheck(pti->m_htItem,true);  
		//UpdateParentItems(pti->m_htItem);
	}

	UpDataItemStatus_CurNode(pti);
}

bool CDirectoryTreeCtrl::IsSelected(int64 nCID)
{
	for (vector<int64>::iterator it = m_vecSelected.begin(); it != m_vecSelected.end(); it++ )
	{
		int64 nID = *it;
		if ( nID == nCID )
			return true;
	}
	return false;
}

void CDirectoryTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWaitCursor curWait;
	SetRedraw(FALSE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CClassInfoItem* pExpanded = (CClassInfoItem*)pNMTreeView->itemNew.lParam;
	if (pExpanded != NULL)
	{
		//if ( pExpanded->m_eItemType == SCII_ALL )
		//{
		// remove all subitems
		DeleteChildItems(pExpanded);
		// fetch all subdirectories and add them to the node
		AddClassInfoChildItem(pExpanded,pExpanded->m_ID,pExpanded->m_sHaseNode);
		//UpDataItemStatus_DirectChildrenNode(pExpanded);
		//}
	}
	else
		ASSERT( false );

	SetRedraw(TRUE);
	Invalidate();
	*pResult = 0;
	/*
	CWaitCursor curWait;
	SetRedraw(FALSE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	// remove all subitems
	HTREEITEM hRemove = GetChildItem(hItem);
	while(hRemove)
	{
		DeleteItem(hRemove);
		hRemove = GetChildItem(hItem);
	}

	// get the directory
	CString strDir = GetFullPath(hItem);

	// fetch all subdirectories and add them to the node
	AddSubdirectories(hItem, strDir);

	SetRedraw(TRUE);
	Invalidate();
	*pResult = 0;
	*/
}

void CDirectoryTreeCtrl::ShareSubDirTree(HTREEITEM hItem, BOOL bRecurse)
{
	CWaitCursor curWait;
	SetRedraw(FALSE);

	HTREEITEM hItemVisibleItem = GetFirstVisibleItem();
	CheckChanged(hItem, !GetCheck(hItem));
	if (bRecurse)
	{
		Expand(hItem, TVE_TOGGLE);
		HTREEITEM hChild = GetChildItem(hItem);
		while (hChild != NULL)
		{
			MarkChilds(hChild, !GetCheck(hItem));
			hChild = GetNextSiblingItem(hChild);
		}
		Expand(hItem, TVE_TOGGLE);
	}
	if (hItemVisibleItem)
		SelectSetFirstVisible(hItemVisibleItem);

	SetRedraw(TRUE);
	Invalidate();
}

void CDirectoryTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//VQB adjustments to provide for sharing or unsharing of subdirectories when control key is Down 
	UINT uHitFlags; 
	HTREEITEM hItem = HitTest(point, &uHitFlags); 
	if (hItem && (uHitFlags & TVHT_ONITEMSTATEICON))
		ShareSubDirTree(hItem, nFlags & MK_CONTROL);
	CTreeCtrl::OnLButtonDown(nFlags, point); 
}

void CDirectoryTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		HTREEITEM hItem = GetSelectedItem();
		if (hItem)
		{
			ShareSubDirTree(hItem, GetKeyState(VK_CONTROL) & 0x8000);

			// if Ctrl+Space is passed to the tree control, it just beeps and does not check/uncheck the item!
			SetCheck(hItem, !GetCheck(hItem));
			return;
		}
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDirectoryTreeCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// If we let any keystrokes which are handled by us -- but not by the tree
	// control -- pass to the control, the user will hear a system event
	// sound (Standard Error!)
	BOOL bCallDefault = TRUE;

	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (nChar == VK_SPACE)
			bCallDefault = FALSE;
	}

	if (bCallDefault)
		CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CDirectoryTreeCtrl::MarkChilds(HTREEITEM hChild,bool mark) { 
	CheckChanged(hChild, mark); 
	SetCheck(hChild,mark);  
	Expand(hChild, TVE_TOGGLE); // VQB - make sure tree has entries 
	HTREEITEM hChild2; 
	hChild2 = GetChildItem(hChild); 
	while( hChild2 != NULL) 
	{ 
		MarkChilds(hChild2,mark); 
		hChild2 = GetNextSiblingItem( hChild2 ); 
	} 
	Expand(hChild, TVE_TOGGLE); // VQB - restore tree to initial disposition 
}

void CDirectoryTreeCtrl::OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	pTVDispInfo->item.cChildren = 1;
	*pResult = 0;
}
void  CDirectoryTreeCtrl::UpDataItemStatus_DirectChildrenNode(CClassInfoItem* pCurClsInfo)
{
	if ( pCurClsInfo->m_htItem )
	{
		if ( HasSelectedSubclass( pCurClsInfo->m_ID ) )
		{
			SetItemState(pCurClsInfo->m_htItem, TVIS_BOLD, TVIS_BOLD);
		}
		else
		{
			SetItemState(pCurClsInfo->m_htItem, 0, TVIS_BOLD);
		}
	}
}
void  CDirectoryTreeCtrl::UpDataItemStatus_CurNode(CClassInfoItem* pCurClsInfo)
{
	if ( pCurClsInfo->m_htItem )
	{
		if ( IfSelectedSubclass( pCurClsInfo->m_ID ) )
		{
			SetItemState(pCurClsInfo->m_htItem, TVIS_BOLD, TVIS_BOLD);
		}
		else
		{
			SetItemState(pCurClsInfo->m_htItem, 0, TVIS_BOLD);
		}
	}
}


bool CDirectoryTreeCtrl::IfSelectedSubclass(int64 nCurCID)
{
	if ( IsSelected(nCurCID) )
	{
		return true;
	}
	//遍历查找直接子类
	if ( EMT_SYS == m_emt )
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( nCurCID == it->ParentID )
			{
				if ( IfSelectedSubclass(it->ID) )
				{
					return true;
				}
			}
		}
	}
	else
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
		{
			if ( nCurCID == it->ParentID )
			{
				if ( IfSelectedSubclass(it->ID) )
				{
					return true;
				}
			}
		}
	}
	

	return false;
	
}
void CDirectoryTreeCtrl::Init_ClassInfos(void)
{
	// Win98: Explicitly set to Unicode to receive Unicode notifications.
	//SendMessage(CCM_SETUNICODEFORMAT, TRUE);

	ShowWindow(SW_HIDE);
	DeleteAllItems();

	CImageList iml;
	iml.Create(16, 16, theApp.m_iDfltImageListColorFlags | ILC_MASK, 0, 1);
	iml.Add(CTempIconLoader(_T("NOTES")));							// 0: 有子类的节点
	iml.Add(CTempIconLoader(_T("NOTE")));							// 1: 无子类的节点
	iml.Add(CTempIconLoader(_T("NODE_SELECTED")));	
	iml.SetOverlayImage(iml.Add(CTempIconLoader(_T("NODE_RECOMM"))), 1);	//  Overlay 从１开始索引
	SetImageList(&iml, TVSIL_NORMAL);
	m_image.DeleteImageList();
	m_image.Attach(iml.Detach());

	//创建大根类
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		if ( m_ArrayRootClassInfos[i] )
		{
			delete m_ArrayRootClassInfos[i];
		}		
	}
	int nRootClassIndex = 0;
	if ( EMT_SYS == m_emt)
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end() && nRootClassIndex < ROOT_CLASS_COUNTS; it++,nRootClassIndex++ )
		{
			if ( it->ParentID == ROOT_CLASS_PARENT_ID)
			{
				m_ArrayRootClassInfos[nRootClassIndex] = new CClassInfoItem((LPCTSTR)it->cname.c_str(), TVI_ROOT, (ESpecialClassInfoItems)it->ID, it->ID, it->ParentID,it->sHaseNode,it->sHaseRecomm);
				m_ArrayRootClassInfos[nRootClassIndex]->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
					| TVIF_CHILDREN
#endif
					, (LPCTSTR)it->cname.c_str(), 0, 0, 0, 0, (LPARAM)m_ArrayRootClassInfos[nRootClassIndex], TVI_ROOT, TVI_LAST);

				UpDataItemStatus_CurNode(m_ArrayRootClassInfos[nRootClassIndex]);
			}
		}
	}
	else
	{
		for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end() && nRootClassIndex < ROOT_CLASS_COUNTS; it++,nRootClassIndex++ )
		{
			if ( it->ParentID == ROOT_CLASS_PARENT_ID)
			{
				m_ArrayRootClassInfos[nRootClassIndex] = new CClassInfoItem((LPCTSTR)it->cname.c_str(), TVI_ROOT, (ESpecialClassInfoItems)it->ID, it->ID, it->ParentID,it->sHaseNode,it->sHaseRecomm);
				m_ArrayRootClassInfos[nRootClassIndex]->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
					| TVIF_CHILDREN
#endif
					, (LPCTSTR)it->cname.c_str(), 0, 0, 0, 0, (LPARAM)m_ArrayRootClassInfos[nRootClassIndex], TVI_ROOT, TVI_LAST);

				UpDataItemStatus_CurNode(m_ArrayRootClassInfos[nRootClassIndex]);
			}
		}
	}
	

	ShowWindow(SW_SHOW);
}
void CDirectoryTreeCtrl::InitDataSoruce( EModuleType emt )
{
	m_emt = emt;
}
void CDirectoryTreeCtrl::Init(void)
{
	// Win98: Explicitly set to Unicode to receive Unicode notifications.
	//SendMessage(CCM_SETUNICODEFORMAT, TRUE);

	ShowWindow(SW_HIDE);
	DeleteAllItems();

	// START: added by FoRcHa /////////////
	// maybe causes problems on 98 & nt4
//	if (thePrefs.GetWindowsVersion() != _WINVER_95_ && thePrefs.GetWindowsVersion() != _WINVER_NT4_)
//	{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	// Get the system image list using a "path" which is available on all systems. [patch by bluecow]
	hImgList = (HIMAGELIST)SHGetFileInfo(_T("."), 0, &shFinfo, sizeof(shFinfo),
										 SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	if(!hImgList)
	{
		TRACE(_T("Cannot retrieve the Handle of SystemImageList!"));
		//return;
	}

	m_image.m_hImageList = hImgList;
	SetImageList(&m_image, TVSIL_NORMAL);
//	}
	////////////////////////////////


	TCHAR drivebuffer[500];
	DWORD dwRet = GetLogicalDriveStrings(_countof(drivebuffer) - 1, drivebuffer);
	if (dwRet > 0 && dwRet < _countof(drivebuffer))
	{
		drivebuffer[_countof(drivebuffer) - 1] = _T('\0');

		const TCHAR* pos = drivebuffer;
		while(*pos != _T('\0')){

			// Copy drive name
			TCHAR drive[4];
			_tcsncpy(drive, pos, _countof(drive) - 1);
			drive[_countof(drive) - 1] = _T('\0');

			drive[2] = _T('\0');
			AddChildItem(NULL, drive); // e.g. "C:"

			// Point to the next drive
			pos += _tcslen(pos) + 1;
		}
	}
	ShowWindow(SW_SHOW);
}

HTREEITEM CDirectoryTreeCtrl::AddChildItem(HTREEITEM hRoot, CString strText)
{
	CString strPath = GetFullPath(hRoot);
	if (hRoot != NULL && strPath.Right(1) != _T('\\'))
		strPath += _T('\\');
	CString strDir = strPath + strText;
	TVINSERTSTRUCT itInsert = {0};
	
	// START: changed by FoRcHa /////
//	if (thePrefs.GetWindowsVersion() != _WINVER_95_ && thePrefs.GetWindowsVersion() != _WINVER_NT4_)
//	{
	itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT |
						 TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	itInsert.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;
/*	}
	else
	{
		itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE;
		itInsert.item.stateMask = TVIS_BOLD;
	}*/
	// END: changed by FoRcHa ///////
	
	if (HasSharedSubdirectory(strDir))
		itInsert.item.state = TVIS_BOLD;
	else
		itInsert.item.state = 0;
	if (HasSubdirectories(strDir))
		itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
	else
		itInsert.item.cChildren = 0;

	itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
	itInsert.hInsertAfter = hRoot ? TVI_SORT : TVI_LAST;
	itInsert.hParent = hRoot;
	
	// START: added by FoRcHa ////////////////
//	if (wWinVer != _WINVER_95_ && wWinVer != _WINVER_NT4_)
//	{
	CString strTemp = strDir;
	if(strTemp.Right(1) != _T('\\'))
		strTemp += _T('\\');
	
	UINT nType = GetDriveType(strTemp);
	if(DRIVE_REMOVABLE <= nType && nType <= DRIVE_RAMDISK)
		itInsert.item.iImage = nType;

	SHFILEINFO shFinfo;
	shFinfo.szDisplayName[0] = _T('\0');
	if(!SHGetFileInfo(strTemp, 0, &shFinfo,	sizeof(shFinfo),
					SHGFI_ICON | SHGFI_SMALLICON | SHGFI_DISPLAYNAME))
	{
		TRACE(_T("Error Gettting SystemFileInfo!"));
		itInsert.itemex.iImage = 0; // :(
	}
	else
	{
		itInsert.itemex.iImage = shFinfo.iIcon;
		DestroyIcon(shFinfo.hIcon);
		if (hRoot == NULL && shFinfo.szDisplayName[0] != _T('\0'))
		{
			STreeItem* pti = new STreeItem;
			pti->strPath = strText;
			strText = shFinfo.szDisplayName;
			itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
			itInsert.item.mask |= TVIF_PARAM;
			itInsert.item.lParam = (LPARAM)pti;
		}
	}

	if(!SHGetFileInfo(strTemp, 0, &shFinfo, sizeof(shFinfo),
						SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON))
	{
		TRACE(_T("Error Gettting SystemFileInfo!"));
		itInsert.itemex.iImage = 0;
	}
	else
	{
		itInsert.itemex.iSelectedImage = shFinfo.iIcon;
		DestroyIcon(shFinfo.hIcon);
	}
//	}
	// END: added by FoRcHa //////////////

	HTREEITEM hItem = InsertItem(&itInsert);
	if (IsShared(strDir))
	{
		SetCheck(hItem);
	}

	return hItem;
}

CString CDirectoryTreeCtrl::GetFullPath(HTREEITEM hItem)
{
	CString strDir;
	HTREEITEM hSearchItem = hItem;
	while(hSearchItem != NULL)
	{
		CString strSearchItemDir;
		STreeItem* pti = (STreeItem*)GetItemData(hSearchItem);
		if (pti)
			strSearchItemDir = pti->strPath;
		else
			strSearchItemDir = GetItemText(hSearchItem);
		strDir = strSearchItemDir + _T('\\') + strDir;
		hSearchItem = GetParentItem(hSearchItem);
	}
	return strDir;
}

void CDirectoryTreeCtrl::AddSubdirectories(HTREEITEM hRoot, CString strDir)
{
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDir+_T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsSystem())
			continue;
		if (!finder.IsDirectory())
			continue;
		
		CString strFilename = finder.GetFileName();
		ASSERT(strFilename.ReverseFind(_T('\\')) == -1);// X: [CI] - [Code Improvement]
		/*if (strFilename.ReverseFind(_T('\\')) != -1)
			strFilename = strFilename.Mid(strFilename.ReverseFind(_T('\\')) + 1);*/
		AddChildItem(hRoot, strFilename);
	}
	finder.Close();
}

bool CDirectoryTreeCtrl::HasSubdirectories(CString strDir)
{
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	// Never try to enumerate the files of a drive and thus physically access the drive, just
	// for the information whether the drive has sub directories in the root folder. Depending
	// on the physical drive type (floppy disk, CD-ROM drive, etc.) this creates an annoying
	// physical access to that drive - which is to be avoided in each case. Even Windows
	// Explorer shows all drives by default with a '+' sign (which means that the user has
	// to explicitly open the drive to really get the content) - and that approach will be fine
	// for eMule as well.
	// Since the restriction for drives 'A:' and 'B:' was removed, this gets more important now.
	if (PathIsRoot(strDir))
		return true;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDir+_T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsSystem())
			continue;
		if (!finder.IsDirectory())
			continue;
		finder.Close();
		return true;
	}
	finder.Close();
	return false;
}

void CDirectoryTreeCtrl::GetSharedDirectories(CAtlList<CString>* list)
{
	for (POSITION pos = m_lstShared.GetHeadPosition(); pos != NULL; )
		list->AddTail(m_lstShared.GetNext(pos));
}

//void CDirectoryTreeCtrl::InitalizeStandardItems_SYS()
//{
//	// add standard items
//	DeleteAllItems();
//	//////////公告大类节点创建////////////////
//	delete m_pRootNotesClassInfos;
//	m_pRootNotesClassInfos = new CClassInfoItem(GetResString(IDS_SYS_NOTESCLASSINFOS), TVI_ROOT, SCII_NOTES, SCII_NOTES_ID, SCII_NO_ID);
//	m_pRootNotesClassInfos->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
//#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
//		| TVIF_CHILDREN
//#endif
//		, GetResString(IDS_SYS_NOTESCLASSINFOS), 0, 0, 0, 0, (LPARAM)m_pRootNotesClassInfos, TVI_ROOT, TVI_LAST);
//	//////////所有资源大类节点创建////////////////
//	delete m_pRootAllClassInfos;
//	m_pRootAllClassInfos = new CClassInfoItem(GetResString(IDS_ALLCLASSINFOS), TVI_ROOT, SCII_ALL, SCII_ALL_ID, SCII_NO_ID/*, NHL_YES*/);
//	m_pRootAllClassInfos->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
//#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
//		| TVIF_CHILDREN
//#endif
//		, GetResString(IDS_ALLCLASSINFOS), 0, 0, 0, 0, (LPARAM)m_pRootAllClassInfos, TVI_ROOT, TVI_LAST);
//	
//}
//void CDirectoryTreeCtrl::SetClassInfos(int nDataSourceType)
//{
//	m_nDataSourceType = nDataSourceType;
//	switch( m_nDataSourceType )
//	{
//	case EDST_SYS:
//		{
//			InitalizeStandardItems_SYS(); 
//		}
//		break;
//	case EDST_GROUP:
//		{
//
//		}
//		break;
//	case EDST_PRIVATE:
//		{
//
//		}
//		break;
//	default:
//		break;
//	}
//}
void CDirectoryTreeCtrl::SetSelectedClasses(vector<int64>* vecSel,int64 nMainClassID)
{
	m_vecSelected.clear();
	for (vector<int64>::iterator it = vecSel->begin(); it != vecSel->end(); it++ )
	{
		if ( ERROR_CLASSID != (*it))
		{
			m_vecSelected.push_back(*it);
		}		
	}
	m_nMainClassID = nMainClassID;
	Init_ClassInfos();
}

bool CDirectoryTreeCtrl::GetSelectedClasses(vector<int64>* vecSel,int64& nMainClassID)
{
	vector<int64>::iterator it = find( m_vecSelected.begin(), m_vecSelected.end(), m_nMainClassID);
	if ( it ==  m_vecSelected.end() )
	{
		MessageBox(GetResString(IDS_PROP_INFO_SEL_MAINMUST));//必须选择主分类
		return false;
	}
	if ( m_vecSelected.size() <= SUBCLASS_MAXCOUNT )
	{
		(*vecSel).clear();
		for (vector<int64>::iterator it = m_vecSelected.begin(); it != m_vecSelected.end(); it++ )
		{
			(*vecSel).push_back(*it);
		}
		nMainClassID = m_nMainClassID;
		return true;
	}
	else
	{

		CString csTemp;
		csTemp.Format(GetResString(IDS_PROP_INFO_SEL_MAXCOUNTS),SUBCLASS_MAXCOUNT);
		MessageBox(csTemp);
		return false;
	}
	
}
CClassInfoItem* CDirectoryTreeCtrl::GetSelectedClass() const{
	if (GetSelectedItem() != NULL)
		return (CClassInfoItem*)GetItemData(GetSelectedItem());
	else
		return NULL;
}
void CDirectoryTreeCtrl::SetSharedDirectories(CAtlList<CString>* list)
{
	m_lstShared.RemoveAll();

	for (POSITION pos = list->GetHeadPosition(); pos != NULL; )
	{
		CString str = list->GetNext(pos);
		if (str.Left(2)==_T("\\\\")) continue;
		if (str.Right(1) != _T('\\'))
			str += _T('\\');
		m_lstShared.AddTail(str);
	}
	Init();
}

bool CDirectoryTreeCtrl::HasSharedSubdirectory(CString strDir)
{
	strDir.MakeLower();
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	for (POSITION pos = m_lstShared.GetHeadPosition(); pos != NULL; )
	{
		CString str = m_lstShared.GetNext(pos);
		str.MakeLower();
		if (str.Find(strDir) == 0 && strDir != str)//strDir.GetLength() != str.GetLength())
			return true;
	}
	return false;
}

int64 CDirectoryTreeCtrl::GetClassID(HTREEITEM hItem)
{
	int64 nClassID;
	CClassInfoItem* pti = (CClassInfoItem*)GetItemData(hItem);
	if (pti)
		nClassID = pti->m_ID;
	else
		nClassID = ERROR_CLASS_ID;
	return nClassID;

}
void CDirectoryTreeCtrl::CheckChanged(HTREEITEM hItem, bool bChecked)
{
	int64 nClassID = GetClassID(hItem);
	//不能选择根节点
	if ( EMT_SYS == m_emt )
	{
		//if (  wyytExtendDataManage::Instance().IsSysClassID(nClassID) )
		//{
		//	AfxMessageBox (GetResString(IDS_SEL_SYSCLASSINFO_PROMPT),MB_OK|MB_ICONEXCLAMATION);
		//	return;
		//}
	}
	else
	{
		if (  wyytExtendDataManagePri::Instance().IsSysClassID(nClassID) )
		{
			AfxMessageBox (GetResString(IDS_SEL_SYSCLASSINFO_PROMPT),MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
	
	if (bChecked)
		AddSelected(nClassID);
	else
		DelSelected(nClassID);

	UpdateParentItems(hItem);
	GetParent()->SendMessage(WM_COMMAND, UM_ITEMSTATECHANGED, (LPARAM)m_hWnd);
	/*
	CString strDir = GetFullPath(hItem);
	if (bChecked)
		AddShare(strDir);
	else
		DelShare(strDir);

	UpdateParentItems(hItem);
	GetParent()->SendMessage(WM_COMMAND, UM_ITEMSTATECHANGED, (LPARAM)m_hWnd);
	*/
}

bool CDirectoryTreeCtrl::IsShared(CString strDir)
{
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	for (POSITION pos = m_lstShared.GetHeadPosition(); pos != NULL; )
	{
		CString str = m_lstShared.GetNext(pos);
		if (str.Right(1) != _T('\\'))
			str += _T('\\');
		if (str.CompareNoCase(strDir) == 0)
			return true;
	}
	return false;
}
void CDirectoryTreeCtrl::AddSelected(int64 nID)
{
	vector<int64>::iterator it = find( m_vecSelected.begin(), m_vecSelected.end(), nID);
	if ( it ==  m_vecSelected.end() )
		m_vecSelected.push_back(nID);
}

void CDirectoryTreeCtrl::DelSelected(int64 nID)
{
	for (vector<int64>::iterator it = m_vecSelected.begin(); it != m_vecSelected.end(); )
	{
		if ( nID == (*it) )
		{
			it = m_vecSelected.erase(it);
		}
		else
		{
			it++; //这里要加
		}
		if(it == m_vecSelected.end()) //要控制迭代器不能超过整个容器 
		{  
			break; 
		}  
	}
}

void CDirectoryTreeCtrl::AddShare(CString strDir)
{
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	
	if (IsShared(strDir) || !strDir.CompareNoCase(thePrefs.GetMuleDirectory(WYYT_CONFIGDIR)))
		return;

	m_lstShared.AddTail(strDir);
}

void CDirectoryTreeCtrl::DelShare(CString strDir)
{
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	for (POSITION pos = m_lstShared.GetHeadPosition(); pos != NULL; )
	{
		POSITION pos2 = pos;
		CString str = m_lstShared.GetNext(pos);
		if (str.CompareNoCase(strDir) == 0)
			m_lstShared.RemoveAt(pos2);
	}
}

void CDirectoryTreeCtrl::UpdateParentItems(HTREEITEM hChild)
{
	HTREEITEM hSearch = GetParentItem(hChild);
	while(hSearch != NULL)
	{
		//if (HasSharedSubdirectory(GetFullPath(hSearch)))
		if ( HasSelectedSubclass(GetClassID(hSearch)) )
			SetItemState(hSearch, TVIS_BOLD, TVIS_BOLD);
		else
			SetItemState(hSearch, 0, TVIS_BOLD);
		hSearch = GetParentItem(hSearch);
	}
}

void CDirectoryTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (point.x != -1 || point.y != -1) {
		RECT rcClient;
		GetClientRect(&rcClient);
		ClientToScreen(&rcClient);
		if (!PtInRect(&rcClient,point)) {
			Default();
			return;
		}
	}

	CPoint ptMenu(-1, -1);
	if (point.x != -1 && point.y != -1)
	{
		ptMenu = point;
		ScreenToClient(&point);
	}
	else
	{
		HTREEITEM hSel = GetNextItem(TVI_ROOT, TVGN_CARET);
		if (hSel)
		{
			CRect rcItem;
			if (GetItemRect(hSel, &rcItem, TRUE))
			{
				ptMenu.x = rcItem.left;
				ptMenu.y = rcItem.top;
				ClientToScreen(&ptMenu);
			}
		}
		else
		{
			ptMenu.SetPoint(0, 0);
			ClientToScreen(&ptMenu);
		}
	}

	HTREEITEM hItem = HitTest(point);

	// create the menu
	

	if (hItem)
	{
		int64 nClassID = GetClassID(hItem);
		if (IsSelected(nClassID))
		{			
			CTitleMenu SharedMenu;
			SharedMenu.CreatePopupMenu();
			SharedMenu.AddMenuTitle(GetResString(IDS_CLASSINFOSEL));

			
			if (m_nMainClassID == nClassID )
			{
				//SharedMenu.AppendMenu(MF_SEPARATOR);
				SharedMenu.AppendMenu(MF_STRING, MP_RECORD_MAINCLASS_CANCLE, GetResString(IDS_FLAG_RECORD_MAINCLASS_CANCLE));
			}
			else
			{
				SharedMenu.AppendMenu(MF_STRING, MP_RECORD_MAINCLASS, GetResString(IDS_FLAG_RECORD_MAINCLASS));
			}

			SharedMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);
			VERIFY( SharedMenu.DestroyMenu() );
		}
		
	}

	//bool bMenuIsEmpty = true;

	//// add all shared directories
	//int iCnt = 0;
	//for (POSITION pos = m_lstShared.GetHeadPosition(); pos != NULL; iCnt++)
	//{
	//	CString strDisplayPath(m_lstShared.GetNext(pos));
	//	PathRemoveBackslash(strDisplayPath.GetBuffer(strDisplayPath.GetLength()));
	//	strDisplayPath.ReleaseBuffer();
	//	SharedMenu.AppendMenu(MF_STRING,MP_SHAREDFOLDERS_FIRST+iCnt, GetResString(IDS_VIEW1) + strDisplayPath);
	//	bMenuIsEmpty = false;
	//}

	//// add right clicked folder, if any
	//if (hItem)
	//{
	//	m_strLastRightClicked = GetFullPath(hItem);
	//	if (!IsShared(m_strLastRightClicked))
	//	{
	//		CString strDisplayPath(m_strLastRightClicked);
	//		PathRemoveBackslash(strDisplayPath.GetBuffer(strDisplayPath.GetLength()));
	//		strDisplayPath.ReleaseBuffer();
	//		if (!bMenuIsEmpty)
	//			SharedMenu.AppendMenu(MF_SEPARATOR);
	//		SharedMenu.AppendMenu(MF_STRING, MP_SHAREDFOLDERS_FIRST-1, GetResString(IDS_VIEW1) + strDisplayPath + GetResString(IDS_VIEW2));
	//		bMenuIsEmpty = false;
	//	}
	//}

	// display menu
	
	
}

void CDirectoryTreeCtrl::OnRButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
	// catch WM_RBUTTONDOWN and do not route it the default way.. otherwise we won't get a WM_CONTEXTMENU.
	//CTreeCtrl::OnRButtonDown(nFlags, point);
}

BOOL CDirectoryTreeCtrl::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	CClassInfoItem* pSelectedClass = GetSelectedClass();
	HTREEITEM hSelect = GetSelectedItem();
	
	if (pSelectedClass != NULL)
	{
		switch (wParam)
		{
		case MP_RECORD_MAINCLASS:
			{
				if ( m_nMainClassID != pSelectedClass->m_ID &&  m_nMainClassID != ERROR_CLASSID )
				{
					MessageBox(GetResString(IDS_PROP_INFO_FIRST_CANCEL_MAINCLASS));
				}
				else if (  m_nMainClassID == ERROR_CLASSID )
				{
					m_nMainClassID = pSelectedClass->m_ID;
					SetItemState(/*pSelectedClass->m_htItem*/hSelect, true ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
					MessageBox(GetResString(IDS_PROP_INFO_SUCCESS));
				}
				
			}
			break;
		case MP_RECORD_MAINCLASS_CANCLE:
			{
				m_nMainClassID = ERROR_CLASSID;
				SetItemState(/*pSelectedClass->m_htItem*/hSelect, false ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
				MessageBox(GetResString(IDS_PROP_INFO_SUCCESS));
			}
			break;
		}
	}
	//if (wParam < MP_SHAREDFOLDERS_FIRST)
	//{
	//	ShellExecute(NULL, _T("open"), m_strLastRightClicked, NULL, NULL, SW_SHOW);
	//}
	//else
	//{
	//	POSITION pos = m_lstShared.FindIndex(wParam - MP_SHAREDFOLDERS_FIRST);
	//	if (pos)
	//		ShellExecute(NULL, _T("open"), m_lstShared.GetAt(pos), NULL, NULL, SW_SHOW);
	//}

	return TRUE;
}

void CDirectoryTreeCtrl::OnTvnDeleteItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->itemOld.lParam)
	{
		//delete (STreeItem*)pNMTreeView->itemOld.lParam; //这里去掉删除，统一由创建一起管理删除
	}
	*pResult = 0;
}
