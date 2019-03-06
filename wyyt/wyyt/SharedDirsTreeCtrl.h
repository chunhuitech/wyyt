//this file is part of eMule
//Copyright (C)2002-2008 Merkur ( devs@emule-project.net / http://www.emule-project.net )
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

#pragma once
#include "map_inc.h"
#include "TitleMenu.h"
#include "wyytExtendDataManage.h"
#include "DataInfos.h"
#include "ResPublic.h"
//
//enum ESpecialDirectoryItems{
//	SDI_NO = 0,
//	SDI_ALL,				// "All Shared Files" node
//	SDI_INCOMING,			// "Incoming Files" node
//	SDI_TEMP,				// "Incomplete Files" node
//	SDI_DIRECTORY,			// "Shared Directories" node
//	SDI_CATINCOMING,		// Category subnode in the "Incoming Files" node
//	SDI_UNSHAREDDIRECTORY,
//	SDI_FILESYSTEMPARENT	// "All Directories" (the file system)
//};

class CSharedFilesCtrl;
class CKnownFile;
class CShareableFile;

//**********************************************************************************
// CDirectoryItem
//
//class CDirectoryItem{
//public:
//	CDirectoryItem(CString strFullPath, HTREEITEM htItem = TVI_ROOT, ESpecialDirectoryItems eItemType = SDI_NO, int m_nCatFilter = -1);
//	~CDirectoryItem();
//	CDirectoryItem*		CloneContent() { return new CDirectoryItem(m_strFullPath, 0, m_eItemType, m_nCatFilter); }
//	HTREEITEM			FindItem(CDirectoryItem* pContentToFind) const;
//
//	CString		m_strFullPath;
//	HTREEITEM	m_htItem;
//	int			m_nCatFilter;
//	CAtlList<CDirectoryItem*> liSubDirectories;
//	ESpecialDirectoryItems m_eItemType;
//};

//**********************************************************************************
// CSharedDirsTreeCtrl

class CSharedDirsTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CSharedDirsTreeCtrl)

public:
	CSharedDirsTreeCtrl();
	virtual ~CSharedDirsTreeCtrl();
	
	void			Initalize(CSharedFilesCtrl* pSharedFilesCtrl);
	void			SetAllIcons();

	CDirectoryItem* GetSelectedFilter() const;
	CClassInfoItem* GetSelectedClass() const;
	bool			IsCreatingTree() const		{return m_bCreatingTree;};
	void			Localize();
	void			EditSharedDirectories(const CDirectoryItem* pDir, bool bAdd, bool bSubDirectories);

	void			AddNodeClassInfo(CClassInfoItem* pClass, bool bAdd, bool bSubClass);
	void			ModNodeClassInfo(HTREEITEM hSelect,CClassInfoItem* pClass);
	void			DelNodeClassInfo(HTREEITEM hSelect,CClassInfoItem* pClass);
	void			SetNodeClassInfoType(CClassInfoItem* pClass);
	void			CancleNodeClassInfoType(CClassInfoItem* pClass);

	void			Reload(bool bFore = false);
	void			OnVolumesChanged();
	void			FileSystemTreeUpdateBoldState(const CDirectoryItem* pDir = NULL);
	bool			ShowFileSystemDirectory(const CString& strDir);
	bool			ShowSharedDirectory(const CString& strDir);
	void			ShowAllSharedFiles();

protected:
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	void			CreateMenues();
	void			ShowFileDialog(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage = 0);
	void			DeleteChildItems(CDirectoryItem* pParent);
	void			DeleteChildItems(CClassInfoItem* pParent);
	void			AddSharedDirectory(CString strDir, bool bSubDirectories);
	void			RemoveSharedDirectory(CString strDir, bool bSubDirectories);
	void			RemoveAllSharedDirectories();
	int				AddSystemIcon(HICON hIcon, int nSystemListPos);
	void			FetchSharedDirsList();

	DECLARE_MESSAGE_MAP()
	afx_msg void	OnSysColorChange();
	afx_msg void	OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg	void	OnRButtonDown(UINT nFlags, CPoint point );
	afx_msg	void	OnLButtonUp(UINT nFlags, CPoint point );
	afx_msg void	OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnTvnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnCancelMode();

	CTitleMenu			m_ClassInfoOperatorMenu; //分类信息操作上下文菜单
	CTitleMenu			m_SharedFilesMenu;
	CTitleMenu			m_ShareDirsMenu;
	CMenu				m_PrioMenu;
	CDirectoryItem*		m_pRootDirectoryItem;
	CDirectoryItem*		m_pRootUnsharedDirectries;
	CDirectoryItem*		m_pDraggingItem;
	CSharedFilesCtrl*	m_pSharedFilesCtrl;
	CAtlList<CString>			m_strliSharedDirs; //记录是否共享
	CAtlList<int64>			m_lstAllSelectIDs; //记录已经选中的ID标记集合
	set<int64/*,less<int64>*/> m_setAllBoldIDs; //记录已经改变字体的ID标记集合

	//CClassInfoItem*		m_pRootAllClassInfos;
	//CClassInfoItem*		m_pRootNotesClassInfos;
	CClassInfoItem*		m_ArrayRootClassInfos[ROOT_CLASS_COUNTS];

	CAtlList<CString>			m_strliCatIncomingDirs;
	CImageList			m_imlTree;
	bool				m_bFileSystemRootDirty;

private:
	void	InitalizeStandardItems();
	
	void	FileSystemTreeCreateTree();
	void	FileSystemTreeAddChildItem(CDirectoryItem* pRoot, CString strText, bool bTopLevel);
	bool	FileSystemTreeHasSubdirectories(CString strDir);
	bool	FileSystemTreeHasSharedSubdirectory(CString strDir, bool bOrFiles);
	void	FileSystemTreeAddSubdirectories(CDirectoryItem* pRoot);
	bool	FileSystemTreeIsShared(CString strDir);

	void	FileSystemTreeUpdateShareState(const CDirectoryItem* pDir = NULL);
	void	FileSystemTreeSetShareState(const CDirectoryItem* pDir, bool bSubDirectories);
	void	FilterTreeAddSharedDirectory(CDirectoryItem* pDir, bool bRefresh);
	void	FilterTreeAddSubDirectories(CDirectoryItem* pDirectory, const CAtlList<CString>& liDirs, int nLevel, bool &rbShowWarning, bool bParentAccessible);
	bool	FilterTreeIsSubDirectory(CString strDir, CString strRoot, const CAtlList<CString>& liDirs);
	void	FilterTreeReloadTree();

	//只加载指定父节点下的所有直接关联的子节点
	//注：只有在点开相应节点时才加载相应节点下的数据，否则只加载根节点
	void	AddClassInfoChildItem(CClassInfoItem* pRoot, int64 nParentID, bool bTopLevel);
	void	AddTreeClassInfoChildItem(CClassInfoItem* pRoot, bool bTopLevel, stSysClassInfoItem Cii);
	void	AddCompleteClassInfo(CClassInfoItem* pRoot, bool bTopLevel, bool bSubClass);
	void	ModCompleteClassInfo(HTREEITEM hSelect,CClassInfoItem* pRoot);
	void	DelCompleteClassInfo(HTREEITEM hSelect,CClassInfoItem* pRoot);
	void	SetCompleteClassInfoType(CClassInfoItem* pRoot);
	void	CancleCompleteClassInfoType(CClassInfoItem* pRoot);
	void SelectSubClassTree(HTREEITEM hItem, BOOL bShare);
	void MarkChilds(HTREEITEM hChild,bool mark);
	// check status of an item has changed
	void CheckChanged(HTREEITEM hItem, bool bChecked);
	// return the ID of an item 
	int64 GetClassID(HTREEITEM hItem);

	// select list access
	bool IsSelected(int64 nCID);
	void AddSelected(int64 nCID);
	void DelSelected(int64 nCID);

	// when selecting a directory, make all parent directories bold
	void UpdateParentItems(HTREEITEM hChild);
	// returns true if a subClass of nCID is selected
	bool HasSelectedSubclass(int64 nParentCID/*, HTREEITEM hParent = NULL*/);
	bool HasBoldSubclass(int64 nParentCID);

	bool			m_bCreatingTree;
//	bool			m_bUseIcons;
#ifdef REPLACE_ATLMAP
	unordered_map<int, int> m_mapSystemIcons;
#else
	CAtlMap<int, int> m_mapSystemIcons;
#endif
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


