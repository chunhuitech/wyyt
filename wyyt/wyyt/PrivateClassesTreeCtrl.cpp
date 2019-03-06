#include "stdafx.h"
#include "wyyt.h"
#include "PrivateClassesTreeCtrl.h"
#include "preferences.h"
#include "otherfunctions.h"
#include "PrivateSharedRecordsCtrl.h"
#include "MenuCmds.h"
#include "wyytdlg.h"
#include "AddModClassInfoDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**********************************************************************************
// CPrivateClassesTreeCtrl


IMPLEMENT_DYNAMIC(CPrivateClassesTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CPrivateClassesTreeCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONUP()
	ON_WM_SYSCOLORCHANGE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnTvnGetdispinfo)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBeginDrag)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CPrivateClassesTreeCtrl::CPrivateClassesTreeCtrl()
{
	//m_pRootAllClassInfos = NULL;
	//m_pRootNotesClassInfos = NULL;
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		m_ArrayRootClassInfos[i] = NULL;
	}

	m_pRootDirectoryItem = NULL;
	m_bCreatingTree = false;
	m_pSharedFilesCtrl = NULL;
	m_pRootUnsharedDirectries = NULL;
	m_pDraggingItem = NULL;
	m_bFileSystemRootDirty = false;
}

CPrivateClassesTreeCtrl::~CPrivateClassesTreeCtrl()
{
	//delete m_pRootAllClassInfos;
	//delete m_pRootNotesClassInfos;
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		if ( m_ArrayRootClassInfos[i] )
		{
			delete m_ArrayRootClassInfos[i];
		}		
	}

	delete m_pRootDirectoryItem;
	delete m_pRootUnsharedDirectries;
}

void CPrivateClassesTreeCtrl::Initalize(CPrivateSharedRecordsCtrl* pSharedFilesCtrl){
	m_pSharedFilesCtrl = pSharedFilesCtrl;
	
	// Win98: Explicitly set to Unicode to receive Unicode notifications.
	//SendMessage(CCM_SETUNICODEFORMAT, TRUE);

//	m_bUseIcons = true;/*(thePrefs.GetWindowsVersion() == _WINVER_2K_ || thePrefs.GetWindowsVersion() == _WINVER_XP_ || thePrefs.GetWindowsVersion() == _WINVER_ME_);*/
	SetAllIcons();
/*	InitalizeStandardItems();// X: [CI] - [Code Improvement] will be Localize later
	FilterTreeReloadTree();
	CreateMenues();*/
}

void CPrivateClassesTreeCtrl::OnSysColorChange()
{
	CTreeCtrl::OnSysColorChange();
	SetAllIcons();
	CreateMenues();
}

void CPrivateClassesTreeCtrl::SetAllIcons()
{
	// This treeview control contains an image list which contains our own icons and a
	// couple of icons which are copied from the Windows System image list. To properly
	// support an update of the control and the image list, we need to 'replace' our own
	// images so that we are able to keep the already stored images from the Windows System
	// image list.
	CImageList *pCurImageList = GetImageList(TVSIL_NORMAL);
	if (pCurImageList != NULL && pCurImageList->GetImageCount() >= 7)
	{
		pCurImageList->Replace(0, CTempIconLoader(_T("NOTES")));			// 0: 有子类的节点
		pCurImageList->Replace(1, CTempIconLoader(_T("NOTE")));		// 1: 无子类的节点	
		pCurImageList->Replace(2, CTempIconLoader(_T("NODE_SELECTED")));	
		pCurImageList->Replace(3, CTempIconLoader(_T("NODE_RECOMM")));	// : Overlay
	}
	else
	{
		CImageList iml;
		iml.Create(16, 16, theApp.m_iDfltImageListColorFlags | ILC_MASK, 0, 1);
		iml.Add(CTempIconLoader(_T("NOTES")));							// 0: 有子类的节点
		iml.Add(CTempIconLoader(_T("NOTE")));							// 1: 无子类的节点
		iml.Add(CTempIconLoader(_T("NODE_SELECTED")));	
		iml.SetOverlayImage(iml.Add(CTempIconLoader(_T("NODE_RECOMM"))), 1);	//  Overlay 从１开始索引
		SetImageList(&iml, TVSIL_NORMAL);
#ifdef REPLACE_ATLMAP
		unordered_map<int, int>().swap(m_mapSystemIcons);
#else
		m_mapSystemIcons.RemoveAll();
#endif
		m_imlTree.DeleteImageList();
		m_imlTree.Attach(iml.Detach());
	}
	
	COLORREF crBk = GetSysColor(COLOR_WINDOW);
	COLORREF crFg = GetSysColor(COLOR_WINDOWTEXT);
	//theApp.LoadSkinColorAlt(_T("SharedDirsTvBk"), _T("SharedFilesLvBk"), crBk);
	//theApp.LoadSkinColorAlt(_T("SharedDirsTvFg"), _T("SharedFilesLvFg"), crFg);
	SetBkColor(crBk);
	SetTextColor(crFg);
}

void CPrivateClassesTreeCtrl::Localize()
{
	InitalizeStandardItems();
	FilterTreeReloadTree();
	CreateMenues();
}

void CPrivateClassesTreeCtrl::InitalizeStandardItems()
{
	// add standard items
	DeleteAllItems();
	//创建大根类
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		if ( m_ArrayRootClassInfos[i] )
		{
			delete m_ArrayRootClassInfos[i];
		}		
	}
	int nRootClassIndex = 0;
	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end() && nRootClassIndex < ROOT_CLASS_COUNTS; it++,nRootClassIndex++ )
	{
		if ( it->ParentID == ROOT_CLASS_PARENT_ID)
		{
			m_ArrayRootClassInfos[nRootClassIndex] = new CClassInfoItem((LPCTSTR)it->cname.c_str(), TVI_ROOT, (ESpecialClassInfoItems)it->ID, it->ID, it->ParentID,it->sHaseNode,it->sHaseRecomm);
			
			if ( wyytExtendDataManagePri::Instance().IfHaseSubClass(it->ID) )
			{
				m_ArrayRootClassInfos[nRootClassIndex]->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE 
#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
					| TVIF_CHILDREN
#endif
					, (LPCTSTR)it->cname.c_str(), 0, 0, 0, 0, (LPARAM)m_ArrayRootClassInfos[nRootClassIndex], TVI_ROOT, TVI_LAST);
			}			
			else
			{
				m_ArrayRootClassInfos[nRootClassIndex]->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
					//| TVIF_CHILDREN
#endif
					, (LPCTSTR)it->cname.c_str(), 0, 0, 0, 0, (LPARAM)m_ArrayRootClassInfos[nRootClassIndex], TVI_ROOT, TVI_LAST);
			}
			Expand(m_ArrayRootClassInfos[nRootClassIndex]->m_htItem, TVE_EXPAND);
		}
	}
	//////////公告大类节点创建////////////////
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
	
	
	///////////////////////////////
	//delete m_pRootDirectoryItem;
	//delete m_pRootUnsharedDirectries;

	//FetchSharedDirsList();

	/*
	m_pRootDirectoryItem = new CDirectoryItem(_T(""), TVI_ROOT);
	CDirectoryItem* pAll = new CDirectoryItem(_T(""), 0, SDI_ALL);
	pAll->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE, GetResString(IDS_ALLSHAREDFILES), 0, 0, TVIS_EXPANDED, TVIS_EXPANDED, (LPARAM)pAll, TVI_ROOT, TVI_LAST);
	m_pRootDirectoryItem->liSubDirectories.AddTail(pAll);
	
	CDirectoryItem* pIncoming = new CDirectoryItem(_T(""), pAll->m_htItem, SDI_INCOMING);
	pIncoming->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE, GetResString(IDS_INCOMING_FILES), 2, 2, 0, 0, (LPARAM)pIncoming, pAll->m_htItem, TVI_LAST);
	m_pRootDirectoryItem->liSubDirectories.AddTail(pIncoming);
	
	CDirectoryItem* pTemp = new CDirectoryItem(_T(""), pAll->m_htItem, SDI_TEMP);
	pTemp->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE, GetResString(IDS_INCOMPLETE_FILES), 1, 1, 0, 0, (LPARAM)pTemp, pAll->m_htItem, TVI_LAST);
	m_pRootDirectoryItem->liSubDirectories.AddTail(pTemp);

	CDirectoryItem* pDir = new CDirectoryItem(_T(""), pAll->m_htItem, SDI_DIRECTORY);
	pDir->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE, GetResString(IDS_SHARED_DIRECTORIES), 5, 5, TVIS_EXPANDED, TVIS_EXPANDED, (LPARAM)pDir, pAll->m_htItem, TVI_LAST);
	m_pRootDirectoryItem->liSubDirectories.AddTail(pDir);

	m_pRootUnsharedDirectries = new CDirectoryItem(_T(""), TVI_ROOT, SDI_FILESYSTEMPARENT);
	m_pRootUnsharedDirectries->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE
#ifndef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
		| TVIF_CHILDREN
#endif
		, GetResString(IDS_ALLDIRECTORIES), 4, 4, 0, 0, (LPARAM)m_pRootUnsharedDirectries, TVI_ROOT, TVI_LAST);
#ifdef _WIN64// X: [BF] - [Bug Fix] Can't Expand FileSystem in x64 Release
	DeleteChildItems(m_pRootUnsharedDirectries);
	CDirectoryItem* pUnused = new CDirectoryItem(_T(""), TVI_ROOT, SDI_NO);
	pUnused->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM, _T(""), 0, 0, 0, 0, (LPARAM)pUnused, m_pRootUnsharedDirectries->m_htItem, TVI_LAST);
	m_pRootUnsharedDirectries->liSubDirectories.AddTail(pUnused);
#endif
	*/
}

bool CPrivateClassesTreeCtrl::FilterTreeIsSubDirectory(CString strDir, CString strRoot, const CAtlList<CString>& liDirs){
	POSITION pos = liDirs.GetHeadPosition();
	strRoot.MakeLower();
	strDir.MakeLower();
	if (strDir.Right(1) != _T('\\')){
		strDir += _T('\\');
	}
	if (strRoot.Right(1) != _T('\\')){
		strRoot += _T('\\');
	}
	while (pos){
		CString strCurrent = liDirs.GetNext(pos);
		strCurrent.MakeLower();
		if (strCurrent.Right(1) != _T('\\')){
			strCurrent += _T('\\');
		}
		if (strRoot.Find(strCurrent, 0) != 0 && strDir.Find(strCurrent, 0) == 0 && strCurrent != strRoot && strCurrent != strDir)
			return true;
	}
	return false;
}



void CPrivateClassesTreeCtrl::FilterTreeAddSubDirectories(CDirectoryItem* pDirectory, const CAtlList<CString>& liDirs, 
													  int nLevel, bool &rbShowWarning, bool bParentAccessible){
	// just some sanity check against too deep shared dirs
	// shouldnt be needed, but never trust the filesystem or a recursive function ;)
	if (nLevel > 14){
		ASSERT( false );
		return;
	}
	POSITION pos = liDirs.GetHeadPosition();
	CString strDirectoryPath = pDirectory->m_strFullPath;
	strDirectoryPath.MakeLower();
	while (pos){
		CString strCurrent = liDirs.GetNext(pos);
		CString strCurrentLow = strCurrent;
		strCurrentLow.MakeLower();
		if ( (strDirectoryPath.IsEmpty() || strCurrentLow.Find(strDirectoryPath + _T('\\'), 0) == 0) && strCurrentLow != strDirectoryPath){
			if (!FilterTreeIsSubDirectory(strCurrentLow, strDirectoryPath, liDirs)){
				bool bAccessible = !bParentAccessible ? false : (_taccess(strCurrent, 00) == 0);
				CString strName = GetFolderLabel(strCurrent, nLevel == 0, bAccessible);
				CDirectoryItem* pNewItem = new CDirectoryItem(strCurrent);
				pNewItem->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE, strName, 5, 5, 0, 0, (LPARAM)pNewItem, pDirectory->m_htItem, TVI_SORT);
				if (!bAccessible) {
					SetItemState(pNewItem->m_htItem, INDEXTOOVERLAYMASK(2), TVIS_OVERLAYMASK);
					rbShowWarning = true;
				}
				pDirectory->liSubDirectories.AddTail(pNewItem);
				FilterTreeAddSubDirectories(pNewItem, liDirs, nLevel+1, rbShowWarning, bAccessible);
			}
		}
	}
}

void CPrivateClassesTreeCtrl::DelCompleteClassInfo(HTREEITEM hSelect,CClassInfoItem* pRoot)
{
	//只能删除子节点和不含有子节点的父节点
	if ( wyytExtendDataManagePri::Instance().IfHaseSubClass(pRoot->m_ID) )
	{
		AfxMessageBox (GetResString(IDS_DEL_CLASSINFO_PROMPT),MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	//不能删除系统相关的根目录
	if (  wyytExtendDataManagePri::Instance().IsSysClassID(pRoot->m_ID) )
	{
		AfxMessageBox (GetResString(IDS_DEL_SYSCLASSINFO_PROMPT),MB_OK|MB_ICONEXCLAMATION);
		return;

	}

	//添加到树控件中
	DeleteItem(hSelect);
	//更新到内存中
	wyytExtendDataManagePri::Instance().DelClassInfo(pRoot->m_ID);
	//添加的数据库中

}

void CPrivateClassesTreeCtrl::SetCompleteClassInfoType(CClassInfoItem* pRoot)
{
	//添加到树控件中
	SetItemState(pRoot->m_htItem, true ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
	//更新到内存中
	wyytExtendDataManagePri::Instance().SetClassInfoRecomm(pRoot->m_ID);
	//添加的数据库中
}

void CPrivateClassesTreeCtrl::CancleCompleteClassInfoType(CClassInfoItem* pRoot)
{
	//添加到树控件中
	SetItemState(pRoot->m_htItem, false ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
	//更新到内存中
	wyytExtendDataManagePri::Instance().CancleClassInfoType(pRoot->m_ID);
	//添加的数据库中
}


void CPrivateClassesTreeCtrl::ModCompleteClassInfo(HTREEITEM hSelect,CClassInfoItem* pRoot)
{
	CAddModClassInfoDlg ciDlg;
	ciDlg.SetLabels(GetResString(IDS_TITLE_MODCLASSINFO),pRoot->m_strName);
	if (ciDlg.DoModal() == IDOK)
	{
		CString csNewClassName;
		ciDlg.GetResult(csNewClassName);

		//添加到树控件中
		//AddTreeClassInfoChildItem(pRoot, bTopLevel, stCII);
		SetItemText(hSelect, csNewClassName);
		//更新到内存中//添加的数据库中
		wyytExtendDataManagePri::Instance().ModClassInfo(pRoot->m_ID,csNewClassName.GetBufferSetLength(csNewClassName.GetLength()));
		
	}
}
//bSubClass=true 表示添加子类  false为添加父类
void CPrivateClassesTreeCtrl::AddCompleteClassInfo(CClassInfoItem* pRoot, bool bTopLevel, bool bSubClass)
{
	if ( !bSubClass && !pRoot->m_sHaseNode || bSubClass && !pRoot->m_sHaseNode )//添加父类必须要在父节点中添加 即
	{
		AfxMessageBox (GetResString(IDS_ADD_CLASSINFO_PROMPT),MB_OK|MB_ICONEXCLAMATION);
		return;

	}

	CAddModClassInfoDlg ciDlg;
	ciDlg.SetLabels(GetResString(IDS_TITLE_ADDCLASSINFO));
	if (ciDlg.DoModal() == IDOK)
	{
		CString csNewClassName;
		ciDlg.GetResult(csNewClassName);
		stSysClassInfoItem  stCII;
		stCII.ID = wyytExtendDataManagePri::Instance().GetSysClassMaxID();
		stCII.ParentID = pRoot->m_ID;
		if ( !bSubClass )
		{
			stCII.sHaseNode = HSN_YES; //是否有子节点 1 有 0 没有
		}
		else
		{
			stCII.sHaseNode = HSN_NO; //是否有子节点 1 有 0 没有
		}
		stCII.sHaseSel = HS_NO; //是否被选中  1　选中 0 没有选中
		stCII.cname = csNewClassName.GetBufferSetLength(csNewClassName.GetLength());
		//添加到树控件中
		AddTreeClassInfoChildItem(pRoot, bTopLevel, stCII);
		//添加到内存及数据库中
		wyytExtendDataManagePri::Instance().InsertNewClassInfo(stCII);
	}
}

void CPrivateClassesTreeCtrl::AddTreeClassInfoChildItem(CClassInfoItem* pRoot, bool bTopLevel, stSysClassInfoItem Cii)
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

	if ( wyytExtendDataManagePri::Instance().IfHaseSubClass(Cii.ID) )//Cii.sHaseNode
		itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
	else
		itInsert.item.cChildren = 0;

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
	}
}
void CPrivateClassesTreeCtrl::AddClassInfoChildItem(CClassInfoItem* pRoot, int64 nParentID, bool bTopLevel)
{
	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
	{
		if ( it->ParentID == nParentID)
		{
			AddTreeClassInfoChildItem(pRoot, bTopLevel, *it);
			//TVINSERTSTRUCT itInsert;
			//memset(&itInsert, 0, sizeof(itInsert));
	
			//itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			//itInsert.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;
	
			////if ( it->sHaseNode ) //共享标记
			////	itInsert.item.state = TVIS_BOLD;
			////else
			////	itInsert.item.state = 0;

			//if ( it->sHaseNode )
			//	itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
			//else
			//	itInsert.item.cChildren = 0;

			//CClassInfoItem* pti = new CClassInfoItem((LPCTSTR)it->name.c_str(), 0, SCII_ALL,it->ID, it->ParentID,it->sHaseNode);

			//itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)it->name.c_str());
			//itInsert.hInsertAfter = !bTopLevel ? TVI_SORT : TVI_LAST;
			//itInsert.hParent = pRoot->m_htItem;
			//itInsert.item.mask |= TVIF_PARAM;
			//itInsert.item.lParam = (LPARAM)pti;
	

			////if ( it->sHaseNode )//共享标记
			////{
			////	itInsert.item.stateMask |= TVIS_OVERLAYMASK;
			////	itInsert.item.state |= INDEXTOOVERLAYMASK(1);
			////}

			//if ( it->sHaseNode ) //图标索引
			//	itInsert.item.iImage = TII_NODES;
			//else
			//	itInsert.item.iImage = TII_NODE;
			//
			//
			//
			//pti->m_htItem = InsertItem(&itInsert);
			//pRoot->liSubDirectories.AddTail(pti);

			//if ( IsSelected(it->ID) )
			//{
			//	SetCheck(pti->m_htItem,true);  
			//}
		}
	}
}
void CPrivateClassesTreeCtrl::FilterTreeReloadTree()
{
	m_bCreatingTree = true;

	//AddClassInfoChildItem(m_pRootAllClassInfos, SCII_NOTES_ID, true);
	//AddClassInfoChildItem(m_pRootAllClassInfos, SCII_ALL_ID, true);

	/////////////////////////////////////////////////////
	//// store current selection
	//CDirectoryItem* pOldSelectedItem = NULL;
	//if (GetSelectedFilter() != NULL){
	//	pOldSelectedItem = GetSelectedFilter()->CloneContent();
	//}


	//// create the tree substructure of directories we want to show
	//POSITION pos = m_pRootDirectoryItem->liSubDirectories.GetHeadPosition();
	//while (pos != NULL){
	//	CDirectoryItem* pCurrent = m_pRootDirectoryItem->liSubDirectories.GetNext(pos);
	//	// clear old items
	//	DeleteChildItems(pCurrent);

	//	switch( pCurrent->m_eItemType ){

	//		case SDI_ALL:
	//			break;
	//		////case SDI_INCOMING:{
	//		////	CString strMainIncDir = thePrefs.GetMuleDirectory(WYYT_CONFIGDIR);
	//		////	if (strMainIncDir.Right(1) == _T('\\')){
	//		////		strMainIncDir = strMainIncDir.Left(strMainIncDir.GetLength()-1);
	//		////	}
	//		////	bool bShowWarning = false;
	//		////	if (thePrefs.GetCatCount() > 1){
	//		////		m_strliCatIncomingDirs.RemoveAll();
	//		////		for (size_t i = 0; i < thePrefs.GetCatCount(); i++){
	//		////			Category_Struct* pCatStruct = thePrefs.GetCategory(i);
	//		////			if (pCatStruct != NULL){
	//		////				CString strCatIncomingPath = pCatStruct->strIncomingPath;
	//		////				if (strCatIncomingPath.Right(1) == _T('\\')){
	//		////					strCatIncomingPath = strCatIncomingPath.Left(strCatIncomingPath.GetLength()-1);
	//		////				}
	//		////				if (!strCatIncomingPath.IsEmpty() && strCatIncomingPath.CompareNoCase(strMainIncDir) != 0
	//		////					&& m_strliCatIncomingDirs.Find(strCatIncomingPath) == NULL)
	//		////				{
	//		////					m_strliCatIncomingDirs.AddTail(strCatIncomingPath);
	//		////					bool bAccessible = _taccess(strCatIncomingPath, 00) == 0;
	//		////					CString strName = GetFolderLabel(strCatIncomingPath, true, bAccessible);
	//		////					CDirectoryItem* pCatInc = new CDirectoryItem(strCatIncomingPath, 0, SDI_CATINCOMING);
	//		////					pCatInc->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE, strName, 5, 5, 0, 0, (LPARAM)pCatInc, pCurrent->m_htItem, TVI_SORT);
	//		////					if (!bAccessible) {
	//		////						SetItemState(pCatInc->m_htItem, INDEXTOOVERLAYMASK(2), TVIS_OVERLAYMASK);
	//		////						bShowWarning = true;
	//		////					}
	//		////					pCurrent->liSubDirectories.AddTail(pCatInc);
	//		////				}
	//		////			}
	//		////		}
	//		////	}
	//		////	SetItemState(pCurrent->m_htItem, bShowWarning ? INDEXTOOVERLAYMASK(2) : 0, TVIS_OVERLAYMASK);
	//		////	break;
	//		////}
	//		////case SDI_TEMP:
	//		////	if (thePrefs.GetCatCount() > 1){
	//		////		for (size_t i = 0; i < thePrefs.GetCatCount(); i++){
	//		////			Category_Struct* pCatStruct = thePrefs.GetCategory(i);
	//		////			if (pCatStruct != NULL){
	//		////				//temp dir
	//		////				CDirectoryItem* pCatTemp = new CDirectoryItem(_T(""), 0, SDI_TEMP, i);
	//		////				pCatTemp->m_htItem = InsertItem(TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE, pCatStruct->strTitle, 3, 3, 0, 0, (LPARAM)pCatTemp, pCurrent->m_htItem, TVI_LAST);
	//		////				pCurrent->liSubDirectories.AddTail(pCatTemp);

	//		////			}
	//		////		}
	//		////	}
	//		////	break;
	//		case SDI_DIRECTORY: {
	//			// add subdirectories
	//			bool bShowWarning = false;
	//			FilterTreeAddSubDirectories(pCurrent, m_strliSharedDirs, 0, bShowWarning, true);
	//			SetItemState(pCurrent->m_htItem, bShowWarning ? INDEXTOOVERLAYMASK(2) : 0, TVIS_OVERLAYMASK);
	//			break;
	//		}
	//		default:
	//			ASSERT( false );
	//			break;
	//	}
	//}

	//// restore selection
	//HTREEITEM htOldSection;
	//if (pOldSelectedItem != NULL && (htOldSection = m_pRootDirectoryItem->FindItem(pOldSelectedItem)) != NULL){
	//	Select(htOldSection, TVGN_CARET);
	//	EnsureVisible(htOldSection);
	//}
	//else if( GetSelectedItem() == NULL && !m_pRootDirectoryItem->liSubDirectories.IsEmpty()){
	//	Select(m_pRootDirectoryItem->liSubDirectories.GetHead()->m_htItem, TVGN_CARET);
	//}
	//delete pOldSelectedItem;
	m_bCreatingTree = false;
}

CDirectoryItem* CPrivateClassesTreeCtrl::GetSelectedFilter() const{
	if (GetSelectedItem() != NULL)
		return (CDirectoryItem*)GetItemData(GetSelectedItem());
	else
		return NULL;
}
CClassInfoItem* CPrivateClassesTreeCtrl::GetSelectedClass() const{
	if (GetSelectedItem() != NULL)
		return (CClassInfoItem*)GetItemData(GetSelectedItem());
	else
		return NULL;
}

void CPrivateClassesTreeCtrl::CreateMenues()
{
	if (m_ClassInfoOperatorMenu) 
		VERIFY( m_ClassInfoOperatorMenu.DestroyMenu() );
	m_ClassInfoOperatorMenu.CreatePopupMenu();
	m_ClassInfoOperatorMenu.AddMenuTitle(GetResString(IDS_CLASSINFOOPERATOR), true);
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_ADD_PARENTCLASS, GetResString(IDS_ADD_PARENTCLASS), _T("ADDPARENT"));
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_ADD_SONCLASS, GetResString(IDS_ADD_SONCLASS), _T("ADDSON"));
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING | MF_SEPARATOR);
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_MOD_CLASSINFO, GetResString(IDS_MOD_CLASSINFO), _T("MODCLASSINFO"));
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING | MF_SEPARATOR);
	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_DEL_CLASSINFO, GetResString(IDS_DEL_CLASSINFO), _T("DELCLASSINFO"));
//	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING | MF_SEPARATOR);
//	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_FLAG_CLASSINFO_RECOMMEND, GetResString(IDS_FLAG_CLASSINFO_RECOMMEND));
//	m_ClassInfoOperatorMenu.AppendMenu(MF_STRING, MP_FLAG_CLASSINFO_RECOMMEND_CANCLE, GetResString(IDS_FLAG_CLASSINFO_RECOMMEND_CANCLE));
	
	
}

void CPrivateClassesTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (point.x != -1 || point.y != -1)
	{
		RECT rcClient;
		GetClientRect(&rcClient);
		ClientToScreen(&rcClient);
		if (!PtInRect(&rcClient,point)) 
		{
			Default();
			return;
		}
	}
	CClassInfoItem* pSelectedClass = GetSelectedClass();
	if ( pSelectedClass != NULL/* && pSelectedClass->m_eItemType == SCII_ALL*/ )
	{
		m_ClassInfoOperatorMenu.EnableMenuItem(MP_ADD_PARENTCLASS,  MF_ENABLED );
		m_ClassInfoOperatorMenu.EnableMenuItem(MP_ADD_SONCLASS,  MF_ENABLED );
		m_ClassInfoOperatorMenu.EnableMenuItem(MP_MOD_CLASSINFO,  MF_ENABLED );
		m_ClassInfoOperatorMenu.EnableMenuItem(MP_DEL_CLASSINFO,  MF_ENABLED );
		
		GetPopupMenuPos(*this, point);
		m_ClassInfoOperatorMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);
	}
	

	//CDirectoryItem* pSelectedDir = GetSelectedFilter();

	//if (pSelectedDir != NULL && //pSelectedDir->m_eItemType != SDI_UNSHAREDDIRECTORY && pSelectedDir->m_eItemType != SDI_FILESYSTEMPARENT
	//	(	pSelectedDir->m_eItemType == SDI_INCOMING ||// X: [UIC] - [UIChange] no context menu for SDI_ALL and SDI_DIRECTORY WiZaRd
	//		pSelectedDir->m_eItemType == SDI_TEMP ||
	//		pSelectedDir->m_eItemType == SDI_CATINCOMING ||
	//		pSelectedDir->m_eItemType == SDI_NO
	//	)){
	//	size_t iSelectedItems = m_pSharedFilesCtrl->GetItemCount();
	//	int iCompleteFileSelected = -1;
	//	UINT uPrioMenuItem = 0;
	//	bool bFirstItem = true;
	//	for (size_t i = 0; i < iSelectedItems; i++)
	//	{
	//		const CShareableFile* shfile = (CShareableFile*)m_pSharedFilesCtrl->GetItemData(i);
	//		if (!IsKindOfCKnownFile(shfile)/*!shfile->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//			continue;
	//		const CKnownFile* pFile = (CKnownFile*)shfile;

	//		int iCurCompleteFile = pFile->IsPartFile() ? 0 : 1;
	//		if (bFirstItem)
	//			iCompleteFileSelected = iCurCompleteFile;
	//		else if (iCompleteFileSelected != iCurCompleteFile)
	//			iCompleteFileSelected = -1;

	//		UINT uCurPrioMenuItem = 0;
	//		if (pFile->IsAutoUpPriority())
	//			uCurPrioMenuItem = MP_PRIOAUTO;
	//		else if (pFile->GetUpPriority() == PR_VERYLOW)
	//			uCurPrioMenuItem = MP_PRIOVERYLOW;
	//		else if (pFile->GetUpPriority() == PR_LOW)
	//			uCurPrioMenuItem = MP_PRIOLOW;
	//		else if (pFile->GetUpPriority() == PR_NORMAL)
	//			uCurPrioMenuItem = MP_PRIONORMAL;
	//		else if (pFile->GetUpPriority() == PR_HIGH)
	//			uCurPrioMenuItem = MP_PRIOHIGH;
	//		else if (pFile->GetUpPriority() == PR_VERYHIGH)
	//			uCurPrioMenuItem = MP_PRIOVERYHIGH;
	//		else
	//			ASSERT(0);

	//		if (bFirstItem)
	//			uPrioMenuItem = uCurPrioMenuItem;
	//		else if (uPrioMenuItem != uCurPrioMenuItem)
	//			uPrioMenuItem = 0;

	//		bFirstItem = false;
	//	}

	//	// just avoid that users get bad ideas by showing the comment/delete-option for the "all" selections
	//	// as the same comment for all files/all incimplete files/ etc is probably not too usefull
	//	// - even if it can be done in other ways if the user really wants to do it
	//	bool bWideRangeSelection = (pSelectedDir->m_nCatFilter == -1 && pSelectedDir->m_eItemType != SDI_NO);

	//	m_SharedFilesMenu.EnableMenuItem((UINT)m_PrioMenu.m_hMenu, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	//	m_PrioMenu.CheckMenuRadioItem(MP_PRIOVERYLOW, MP_PRIOAUTO, uPrioMenuItem, 0);

	//	m_SharedFilesMenu.EnableMenuItem(MP_OPENFOLDER, (pSelectedDir != NULL ) ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(MP_REMOVE, (iCompleteFileSelected > 0 && !bWideRangeSelection) ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(MP_CMT, (iSelectedItems > 0 && !bWideRangeSelection) ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(MP_DETAIL, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(thePrefs.GetShowCopyEd2kLinkCmd() ? MP_GETED2KLINK : MP_SHOWED2KLINK, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(MP_UNSHAREDIR, (pSelectedDir->m_eItemType == SDI_NO && !pSelectedDir->m_strFullPath.IsEmpty() && FileSystemTreeIsShared(pSelectedDir->m_strFullPath)) ? MF_ENABLED : MF_GRAYED);
	//	m_SharedFilesMenu.EnableMenuItem(MP_UNSHAREDIRSUB, (pSelectedDir->m_eItemType == SDI_DIRECTORY && ItemHasChildren(pSelectedDir->m_htItem) 
	//		|| (pSelectedDir->m_eItemType == SDI_NO && !pSelectedDir->m_strFullPath.IsEmpty() && (FileSystemTreeIsShared(pSelectedDir->m_strFullPath) 
	//		|| FileSystemTreeHasSharedSubdirectory(pSelectedDir->m_strFullPath, false)))) ? MF_ENABLED : MF_GRAYED);

	//	GetPopupMenuPos(*this, point);
	//	m_SharedFilesMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);
	//}
	//else if(pSelectedDir != NULL && pSelectedDir->m_eItemType == SDI_UNSHAREDDIRECTORY){
	//	m_ShareDirsMenu.EnableMenuItem(MP_UNSHAREDIR, FileSystemTreeIsShared(pSelectedDir->m_strFullPath) ? MF_ENABLED : MF_GRAYED);
	//	m_ShareDirsMenu.EnableMenuItem(MP_UNSHAREDIRSUB, (FileSystemTreeIsShared(pSelectedDir->m_strFullPath) || FileSystemTreeHasSharedSubdirectory(pSelectedDir->m_strFullPath, false)) ? MF_ENABLED : MF_GRAYED);
	//	m_ShareDirsMenu.EnableMenuItem(MP_SHAREDIR,
	//		!FileSystemTreeIsShared(pSelectedDir->m_strFullPath)
	//		// SLUGFILLER: SafeHash remove - removed installation dir unsharing
	//		/*
	//		&& thePrefs.IsShareableDirectory(pSelectedDir->m_strFullPath)
	//		*/
	//		? MF_ENABLED : MF_GRAYED);
	//	m_ShareDirsMenu.EnableMenuItem(MP_SHAREDIRSUB, 
	//		FileSystemTreeHasSubdirectories(pSelectedDir->m_strFullPath)
	//		// SLUGFILLER: SafeHash remove - removed installation dir unsharing
	//		/*
	//		&& thePrefs.IsShareableDirectory(pSelectedDir->m_strFullPath)
	//		*/
	//		? MF_ENABLED : MF_GRAYED);

	//	GetPopupMenuPos(*this, point);
	//	m_ShareDirsMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);
	//}
}

void CPrivateClassesTreeCtrl::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	UINT uHitFlags;
	HTREEITEM hItem = HitTest(point, &uHitFlags);
	if (hItem != NULL && (uHitFlags & TVHT_ONITEM))
	{
		Select(hItem, TVGN_CARET);
		SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
	}
	return;
}

BOOL CPrivateClassesTreeCtrl::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{

	
	CClassInfoItem* pSelectedClass = GetSelectedClass();
	HTREEITEM hSelect = GetSelectedItem();
	
	if (pSelectedClass != NULL)
	{
		switch (wParam)
		{
			case MP_ADD_PARENTCLASS:
				{
					//MessageBox(L"添加父类");
					AddNodeClassInfo(pSelectedClass, true, false);
				}
				break;
			case MP_ADD_SONCLASS:
				{
					//MessageBox(L"添加子类");
					AddNodeClassInfo(pSelectedClass, true, true);
				}
				break;
			case MP_MOD_CLASSINFO:
				{
					//MessageBox(L"修改类信息");
					ModNodeClassInfo(hSelect,pSelectedClass);
					

				}
				break;
			case MP_DEL_CLASSINFO:
				{
					//MessageBox(L"删除类信息");
					DelNodeClassInfo(hSelect,pSelectedClass);
				}
				break;
			case MP_FLAG_CLASSINFO_RECOMMEND:
				{
					//MessageBox(L"设置推荐");
					SetNodeClassInfoType(pSelectedClass);
				}
				break;
			case MP_FLAG_CLASSINFO_RECOMMEND_CANCLE:
				{
					//MessageBox(L"取消推荐");
					CancleNodeClassInfoType(pSelectedClass);					
				}
				break;
				
				
				/*	case MP_SHAREDIR:
				EditSharedDirectories(pSelectedDir, true, false);
				break;
				case MP_SHAREDIRSUB:
				EditSharedDirectories(pSelectedDir, true, true);
				break;
				case MP_UNSHAREDIR:
				EditSharedDirectories(pSelectedDir, false, false);
				break;
				case MP_UNSHAREDIRSUB:
				EditSharedDirectories(pSelectedDir, false, true);
				break;*/
		}
	}
	

//	CAtlList<CShareableFile*> selectedList;
//	size_t iSelectedItems = m_pSharedFilesCtrl->GetItemCount();
//	for (size_t i = 0; i < iSelectedItems; i++)
//	{
//		selectedList.AddTail((CShareableFile*)m_pSharedFilesCtrl->GetItemData(i));
//	}
//	CDirectoryItem* pSelectedDir = GetSelectedFilter();
//
//	// folder based
//	if (pSelectedDir != NULL){
//		switch (wParam){
//			case MP_OPENFOLDER:
//				if (pSelectedDir && !pSelectedDir->m_strFullPath.IsEmpty() /*&& pSelectedDir->m_eItemType == SDI_NO*/){
//					ShellExecute(NULL, _T("open"), pSelectedDir->m_strFullPath, NULL, NULL, SW_SHOW);
//				}
//				else if (pSelectedDir && pSelectedDir->m_eItemType == SDI_INCOMING)
//					ShellExecute(NULL, _T("open"), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR), NULL, NULL, SW_SHOW);
//				else if (pSelectedDir && pSelectedDir->m_eItemType == SDI_TEMP)
//					ShellExecute(NULL, _T("open"), thePrefs.GetTempDir(), NULL, NULL, SW_SHOW);
//				break;
//			case MP_SHAREDIR:
//				EditSharedDirectories(pSelectedDir, true, false);
//				break;
//			case MP_SHAREDIRSUB:
//				EditSharedDirectories(pSelectedDir, true, true);
//				break;
//			case MP_UNSHAREDIR:
//				EditSharedDirectories(pSelectedDir, false, false);
//				break;
//			case MP_UNSHAREDIRSUB:
//				EditSharedDirectories(pSelectedDir, false, true);
//				break;
//		}
//	}
//
//	// file based
//	if (selectedList.GetCount() > 0 && pSelectedDir != NULL)
//	{
//		CShareableFile* file = NULL;
//		if (selectedList.GetCount() == 1)
//			file = selectedList.GetHead();
//
//		CKnownFile* pKnownFile = NULL;
//		if (file != NULL && IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
//			pKnownFile = (CKnownFile*)file;
//
//		switch (wParam){
//			case MP_GETED2KLINK:{
//				CString str;
//				POSITION pos = selectedList.GetHeadPosition();
//				while (pos != NULL)
//				{
//					CShareableFile* file = selectedList.GetNext(pos);
//					if (IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/){
//						if (!str.IsEmpty())
//							str += _T("\r\n");
//						str += ((CKnownFile*)file)->GetED2kLink();
//					}
//				}
//				theApp.CopyTextToClipboard(str);
//				break;
//			}
//			// file operations
//			case MP_REMOVE:
//			case MPG_DELETE:
//			{
//				if (IDNO == AfxMessageBox(GetResString(IDS_CONFIRM_FILEDELETE),MB_ICONWARNING | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_YESNO))
//					return TRUE;
//
//				m_pSharedFilesCtrl->SetRedraw(FALSE);
//				bool bRemovedItems = false;
//				while (!selectedList.IsEmpty())
//				{
//					CShareableFile* myfile = selectedList.RemoveHead();
//					if (!myfile || myfile->IsPartFile())
//						continue;
//					
//					bool delsucc = ShellDeleteFile(myfile->GetFilePath());
//					if (delsucc){
//						if (IsKindOfCKnownFile(myfile)/*myfile->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
//							theApp.sharedfiles->RemoveFile((CKnownFile*)myfile, true);
//						bRemovedItems = true;
//						if (IsCPartFile(myfile)/*myfile->IsKindOf(RUNTIME_CLASS(CPartFile))*/)
//							theApp.emuledlg->transferwnd->GetDownloadList()->ClearCompleted(static_cast<CPartFile*>(myfile));
//					}
//					else{
//						CString strError;
//						strError.Format( GetResString(IDS_ERR_DELFILE) + _T("\r\n\r\n%s"), myfile->GetFilePath(), GetErrorMessage(GetLastError()));
//						AfxMessageBox(strError);
//					}
//				}
//				m_pSharedFilesCtrl->SetRedraw(TRUE);
//				if (bRemovedItems) {
//					m_pSharedFilesCtrl->AutoSelectItem();
//					// Depending on <no-idea> this does not always cause a
//					// LVN_ITEMACTIVATE message sent. So, explicitly redraw
//					// the item.
//					theApp.emuledlg->sharedfileswnd->ShowSelectedFilesDetails();
//					theApp.emuledlg->sharedfileswnd->OnSingleFileShareStatusChanged(); // might have been a single shared file
//				}
//				break; 
//			}
//			case MP_CMT:
//				ShowFileDialog(selectedList, IDD_COMMENT);
//                break; 
//			case MP_DETAIL:
//			case MPG_ALTENTER:
//				ShowFileDialog(selectedList);
//				break;
//			case MP_SHOWED2KLINK:
//				ShowFileDialog(selectedList, IDD_ED2KLINK);
//				break;
//			case MP_PRIOVERYLOW:
//			case MP_PRIOLOW:
//			case MP_PRIONORMAL:
//			case MP_PRIOHIGH:
//			case MP_PRIOVERYHIGH:
//			case MP_PRIOAUTO:
//			{
//					POSITION pos = selectedList.GetHeadPosition();
//					while (pos != NULL)
//					{
//						if (!IsKindOfCKnownFile(selectedList.GetAt(pos))/*!selectedList.GetAt(pos)->IsKindOf(RUNTIME_CLASS(CKnownFile))*/){
//							selectedList.GetNext(pos);
//							continue;
//						}
//						CKnownFile* file = (CKnownFile*)selectedList.GetNext(pos);
//						switch (wParam) {
//							case MP_PRIOVERYLOW:
//								file->SetAutoUpPriority(false);
//								file->SetUpPriority(PR_VERYLOW);
//								m_pSharedFilesCtrl->UpdateFile(file);
//								break;
//							case MP_PRIOLOW:
//								file->SetAutoUpPriority(false);
//								file->SetUpPriority(PR_LOW);
//								m_pSharedFilesCtrl->UpdateFile(file);
//								break;
//							case MP_PRIONORMAL:
//								file->SetAutoUpPriority(false);
//								file->SetUpPriority(PR_NORMAL);
//								m_pSharedFilesCtrl->UpdateFile(file);
//								break;
//							case MP_PRIOHIGH:
//								file->SetAutoUpPriority(false);
//								file->SetUpPriority(PR_HIGH);
//								m_pSharedFilesCtrl->UpdateFile(file);
//								break;
//							case MP_PRIOVERYHIGH:
//								file->SetAutoUpPriority(false);
//								file->SetUpPriority(PR_VERYHIGH);
//								m_pSharedFilesCtrl->UpdateFile(file);
//								break;	
//							case MP_PRIOAUTO:
//								file->SetAutoUpPriority(true);
//								//Xman advanced upload-priority
//								if (thePrefs.UseAdvancedAutoPtio())
//#ifdef _DEBUG
//									file->CalculateAndSetUploadPriority2(); 
//#else
//									file->CalculateAndSetUploadPriority(); 
//#endif
//								else
//									file->UpdateAutoUpPriority();
//								//Xman end
//								m_pSharedFilesCtrl->UpdateFile(file); 
//								break;
//						}
//					}
//					break;
//				}
//		}
//	}
	return TRUE;
}

void CPrivateClassesTreeCtrl::ShowFileDialog(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage)
{
	m_pSharedFilesCtrl->ShowFileDialog(aFiles, uPshInvokePage);
}

void CPrivateClassesTreeCtrl::FileSystemTreeCreateTree()
{
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
			FileSystemTreeAddChildItem(m_pRootUnsharedDirectries, drive, true); // e.g. "C:"

			// Point to the next drive
			pos += _tcslen(pos) + 1;
		}
	}
}

void CPrivateClassesTreeCtrl::FileSystemTreeAddChildItem(CDirectoryItem* pRoot, CString strText, bool bTopLevel)
{
	CString strPath = pRoot->m_strFullPath;
	if (strPath.Right(1) != _T('\\') && !strPath.IsEmpty())
		strPath += _T('\\');
	CString strDir = strPath + strText;
	TVINSERTSTRUCT itInsert;
	memset(&itInsert, 0, sizeof(itInsert));
	
//	if(m_bUseIcons)		
//	{
		itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		itInsert.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;
/*	}
	else{
		itInsert.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_TEXT | TVIF_STATE;
		itInsert.item.stateMask = TVIS_BOLD;
	}*/

	
	if (FileSystemTreeHasSharedSubdirectory(strDir, true) || FileSystemTreeIsShared(strDir))
		itInsert.item.state = TVIS_BOLD;
	else
		itInsert.item.state = 0;

	if (FileSystemTreeHasSubdirectories(strDir))
		itInsert.item.cChildren = I_CHILDRENCALLBACK;		// used to display the + symbol next to each item
	else
		itInsert.item.cChildren = 0;

	if (strDir.Right(1) == _T('\\')){
		strDir = strDir.Left(strPath.GetLength()-1);
	}
	CDirectoryItem* pti = new CDirectoryItem(strDir, 0, SDI_UNSHAREDDIRECTORY);

	itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
	itInsert.hInsertAfter = !bTopLevel ? TVI_SORT : TVI_LAST;
	itInsert.hParent = pRoot->m_htItem;
	itInsert.item.mask |= TVIF_PARAM;
	itInsert.item.lParam = (LPARAM)pti;
	
//	if(m_bUseIcons)		
//	{
		if (FileSystemTreeIsShared(strDir)){
			itInsert.item.stateMask |= TVIS_OVERLAYMASK;
			itInsert.item.state |= INDEXTOOVERLAYMASK(1);
		}

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
			itInsert.itemex.iImage = AddSystemIcon(shFinfo.hIcon, shFinfo.iIcon);
			DestroyIcon(shFinfo.hIcon);
			if (bTopLevel && shFinfo.szDisplayName[0] != _T('\0'))
			{
				strText = shFinfo.szDisplayName;
				itInsert.item.pszText = const_cast<LPTSTR>((LPCTSTR)strText);
			}
		}

		if(!SHGetFileInfo(strTemp, 0, &shFinfo, sizeof(shFinfo), SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON))
		{
			TRACE(_T("Error Gettting SystemFileInfo!"));
			itInsert.itemex.iImage = 0;
		}
		else{
			itInsert.itemex.iSelectedImage = AddSystemIcon(shFinfo.hIcon, shFinfo.iIcon);
			DestroyIcon(shFinfo.hIcon);
		}
//	}

	pti->m_htItem = InsertItem(&itInsert);
	pRoot->liSubDirectories.AddTail(pti);
}

bool CPrivateClassesTreeCtrl::FileSystemTreeHasSubdirectories(CString strDir)
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

bool CPrivateClassesTreeCtrl::FileSystemTreeHasSharedSubdirectory(CString strDir, bool bOrFiles)
{
	strDir.MakeLower();
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	for (POSITION pos = m_strliSharedDirs.GetHeadPosition(); pos != NULL; )
	{
		CString strCurrent = m_strliSharedDirs.GetNext(pos);
		strCurrent.MakeLower();
		if (strCurrent.Find(strDir) == 0 && strDir != strCurrent)
			return true;
	}
	return bOrFiles/* && theApp.sharedfiles->ContainsSingleSharedFiles(strDir)*/;
}

void CPrivateClassesTreeCtrl::FileSystemTreeAddSubdirectories(CDirectoryItem* pRoot)
{
	CString strDir = pRoot->m_strFullPath;
	if (strDir.Right(1) != _T('\\'))
		strDir += _T('\\');
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDir+_T("*.*"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsSystem() || !finder.IsDirectory())
			continue;
		
		CString strFilename = finder.GetFileName();
		ASSERT(strFilename.ReverseFind(_T('\\')) == -1);// X: [CI] - [Code Improvement]
		/*if (strFilename.ReverseFind(_T('\\')) != -1)
			strFilename = strFilename.Mid(strFilename.ReverseFind(_T('\\')) + 1);*/
		FileSystemTreeAddChildItem(pRoot, strFilename, false);
	}
	finder.Close();
}

int	CPrivateClassesTreeCtrl::AddSystemIcon(HICON hIcon, int nSystemListPos){
#ifdef REPLACE_ATLMAP
	unordered_map<int, int>::const_iterator it = m_mapSystemIcons.find(nSystemListPos);
	if (it != m_mapSystemIcons.end())
		return it->second;
	int nPos = GetImageList(TVSIL_NORMAL)->Add(hIcon);
	m_mapSystemIcons[nSystemListPos] = nPos;
#else
	int nPos = 0;
	if (!m_mapSystemIcons.Lookup(nSystemListPos, nPos)){
		nPos = GetImageList(TVSIL_NORMAL)->Add(hIcon);
		m_mapSystemIcons.SetAt(nSystemListPos, nPos);
	}
#endif
	return nPos;
}

void CPrivateClassesTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
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
	CDirectoryItem* pExpanded = (CDirectoryItem*)pNMTreeView->itemNew.lParam;
	if (pExpanded != NULL){
	if (pExpanded->m_eItemType == SDI_UNSHAREDDIRECTORY && !pExpanded->m_strFullPath.IsEmpty()){
	// remove all subitems
	DeleteChildItems(pExpanded);
	// fetch all subdirectories and add them to the node
	FileSystemTreeAddSubdirectories(pExpanded);
	}
	else if(pExpanded->m_eItemType == SDI_FILESYSTEMPARENT){
	DeleteChildItems(pExpanded);
	FileSystemTreeCreateTree();
	}
	}
	else
	ASSERT( false );

	SetRedraw(TRUE);
	Invalidate();
	*pResult = 0;
	*/
}

void CPrivateClassesTreeCtrl::DeleteChildItems(CClassInfoItem* pParent)
{
	while(!pParent->liSubDirectories.IsEmpty())
	{
		CClassInfoItem* pToDelete = pParent->liSubDirectories.RemoveHead();
		DeleteItem(pToDelete->m_htItem);
		DeleteChildItems(pToDelete);
		delete pToDelete;
	}
}

void CPrivateClassesTreeCtrl::DeleteChildItems(CDirectoryItem* pParent){
	while(!pParent->liSubDirectories.IsEmpty())
	{
		CDirectoryItem* pToDelete = pParent->liSubDirectories.RemoveHead();
		DeleteItem(pToDelete->m_htItem);
		DeleteChildItems(pToDelete);
		delete pToDelete;
	}
}

bool CPrivateClassesTreeCtrl::FileSystemTreeIsShared(CString strDir)
{
	for (POSITION pos = m_strliSharedDirs.GetHeadPosition(); pos != NULL; )
	{
		//if (CompareDirectories(m_strliSharedDirs.GetNext(pos), strDir) == 0)
		//	return true;
	}
	return false;
}

void CPrivateClassesTreeCtrl::OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	pTVDispInfo->item.cChildren = 1;
	*pResult = 0;
}

void CPrivateClassesTreeCtrl::AddSharedDirectory(CString strDir, bool bSubDirectories){
	if (!FileSystemTreeIsShared(strDir)
		// SLUGFILLER: SafeHash remove - removed installation dir unsharing
		/*
		&& thePrefs.IsShareableDirectory(strDir)
		*/
		){
		m_strliSharedDirs.AddTail(strDir);
	}
	if (bSubDirectories){
		if (strDir.Right(1) != _T('\\'))
			strDir += _T('\\');
		CFileFind finder;
		BOOL bWorking = finder.FindFile(strDir+_T("*.*"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots() || finder.IsSystem() || !finder.IsDirectory())
				continue;
			AddSharedDirectory(strDir + finder.GetFileName(), true);
		}
		finder.Close();
	}
}

void CPrivateClassesTreeCtrl::RemoveSharedDirectory(CString strDir, bool bSubDirectories){
	if (strDir.Right(1) == _T('\\')){
		strDir = strDir.Left(strDir.GetLength()-1);
	}
	strDir.MakeLower();
	for (POSITION pos1 = m_strliSharedDirs.GetHeadPosition(), pos2;( pos2 = pos1 ) != NULL;){// X: [CI] - [Code Improvement]
		CString str = m_strliSharedDirs.GetNext(pos1);
		str.MakeLower();
		if (str.CompareNoCase(strDir) == 0)
			m_strliSharedDirs.RemoveAt(pos2);
		else if (bSubDirectories && str.Find(strDir) == 0)
			m_strliSharedDirs.RemoveAt(pos2);
	}
}

void CPrivateClassesTreeCtrl::RemoveAllSharedDirectories(){
	POSITION pos1, pos2;
	for (pos1 = m_strliSharedDirs.GetHeadPosition();( pos2 = pos1 ) != NULL;)
	{
		m_strliSharedDirs.GetNext(pos1);
		CString str = m_strliSharedDirs.GetAt(pos2);
		str.MakeLower();
		m_strliSharedDirs.RemoveAt(pos2);
	}
}

void CPrivateClassesTreeCtrl::FileSystemTreeUpdateBoldState(const CDirectoryItem* pDir){
	if (pDir == NULL)
		pDir = m_pRootUnsharedDirectries;
	SetItemState(pDir->m_htItem, ((FileSystemTreeHasSharedSubdirectory(pDir->m_strFullPath, true) || FileSystemTreeIsShared(pDir->m_strFullPath)) ? TVIS_BOLD : 0), TVIS_BOLD);
	POSITION pos = pDir->liSubDirectories.GetHeadPosition();
	while (pos != NULL){
		FileSystemTreeUpdateBoldState(pDir->liSubDirectories.GetNext(pos));
	}
}

void CPrivateClassesTreeCtrl::FileSystemTreeUpdateShareState(const CDirectoryItem* pDir){
	if (pDir == NULL)
		pDir = m_pRootUnsharedDirectries;
	SetItemState(pDir->m_htItem, FileSystemTreeIsShared(pDir->m_strFullPath) ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
	POSITION pos = pDir->liSubDirectories.GetHeadPosition();
	while (pos != NULL){
		FileSystemTreeUpdateShareState(pDir->liSubDirectories.GetNext(pos));
	}
}

void CPrivateClassesTreeCtrl::FileSystemTreeSetShareState(const CDirectoryItem* pDir, bool bSubDirectories){
	if (/*m_bUseIcons && */pDir->m_htItem != NULL)
		SetItemState(pDir->m_htItem, FileSystemTreeIsShared(pDir->m_strFullPath) ? INDEXTOOVERLAYMASK(1) : 0, TVIS_OVERLAYMASK);
	if (bSubDirectories){
		POSITION pos = pDir->liSubDirectories.GetHeadPosition();
		while (pos != NULL){
			FileSystemTreeSetShareState(pDir->liSubDirectories.GetNext(pos), true);
		}
	}
}

void CPrivateClassesTreeCtrl::DelNodeClassInfo(HTREEITEM hSelect,CClassInfoItem* pClass)
{
	//ASSERT( pClass->m_eItemType == SCII_ALL  );
	CWaitCursor curWait;
	DelCompleteClassInfo(hSelect,pClass);

}
void CPrivateClassesTreeCtrl::SetNodeClassInfoType(CClassInfoItem* pClass)
{
	//ASSERT( pClass->m_eItemType == SCII_ALL  );
	CWaitCursor curWait;
	SetCompleteClassInfoType(pClass);

}
void CPrivateClassesTreeCtrl::CancleNodeClassInfoType(CClassInfoItem* pClass)
{
	//ASSERT( pClass->m_eItemType == SCII_ALL  );
	CWaitCursor curWait;
	CancleCompleteClassInfoType(pClass);

}
void CPrivateClassesTreeCtrl::ModNodeClassInfo(HTREEITEM hSelect,CClassInfoItem* pClass)
{
	//ASSERT( pClass->m_eItemType == SCII_ALL  );
	CWaitCursor curWait;
	ModCompleteClassInfo(hSelect,pClass);
	
}
void CPrivateClassesTreeCtrl::AddNodeClassInfo(CClassInfoItem* pClass, bool bAdd, bool bSubClass)
{
	//ASSERT( pClass->m_eItemType == SCII_ALL  );
	CWaitCursor curWait;
	if ( bAdd )
	{
		AddCompleteClassInfo(pClass,true,bSubClass);
	}
	
}

void CPrivateClassesTreeCtrl::EditSharedDirectories(const CDirectoryItem* pDir, bool bAdd, bool bSubDirectories){
	ASSERT( pDir->m_eItemType == SDI_UNSHAREDDIRECTORY || pDir->m_eItemType == SDI_NO || (pDir->m_eItemType == SDI_DIRECTORY && !bAdd && pDir->m_strFullPath.IsEmpty()) );

	CWaitCursor curWait;
	if (bAdd)
		AddSharedDirectory(pDir->m_strFullPath, bSubDirectories);
	else if (pDir->m_eItemType == SDI_DIRECTORY)
		RemoveAllSharedDirectories();
	else
		RemoveSharedDirectory(pDir->m_strFullPath, bSubDirectories);

	if (pDir->m_eItemType == SDI_NO || pDir->m_eItemType == SDI_DIRECTORY) {
		// An 'Unshare' was invoked from within the virtual "Shared Directories" folder, thus we do not have
		// the tree view item handle of the item within the "All Directories" tree -> need to update the
		// entire tree in case the tree view item is currently visible.
		FileSystemTreeUpdateShareState();
	}
	else {
		// A 'Share' or 'Unshare' was invoked for a certain tree view item within the "All Directories" tree,
		// thus we know the tree view item handle which needs to be updated for showing the new share state.
		FileSystemTreeSetShareState(pDir, bSubDirectories);
	}
	FileSystemTreeUpdateBoldState();
	FilterTreeReloadTree();

	// sync with the preferences list
	//thePrefs.shareddir_list.RemoveAll();
	POSITION pos = m_strliSharedDirs.GetHeadPosition();
	// copy list
	while (pos){
		CString strPath = m_strliSharedDirs.GetNext(pos);
		if (strPath.Right(1) != _T('\\')){
			strPath += _T('\\');
		}
		//thePrefs.shareddir_list.AddTail(strPath);
	}

	//  update the sharedfiles list
	//theApp.wyytdlg->sharedfileswnd->Reload();
	if (GetSelectedFilter() != NULL && GetSelectedFilter()->m_eItemType == SDI_UNSHAREDDIRECTORY)
		m_pSharedFilesCtrl->UpdateWindow(); // if in filesystem view, update the list to reflect the changes in the checkboxes
	if(!thePrefs.prefReadonly) // X: [ROP] - [ReadOnlyPreference]
		thePrefs.Save();
}

//void CPrivateClassesTreeCtrl::Reload(bool bForce){
//	bool bChanged = false;
//	
//	if (bChanged || bForce)
//	{
//		//wyytExtendDataManagePri::Instance().Reload();
//		InitalizeStandardItems();
//	
//	}
//}

void CPrivateClassesTreeCtrl::FetchSharedDirsList()
{
	//m_strliSharedDirs.RemoveAll();
	//POSITION pos = thePrefs.shareddir_list.GetHeadPosition();
	//// copy list
	//while (pos){
	//	CString strPath = thePrefs.shareddir_list.GetNext(pos);
	//	if (strPath.Right(1) == _T('\\')){
	//		strPath = strPath.Left(strPath.GetLength()-1);
	//	}
	//	m_strliSharedDirs.AddTail(strPath);
	//}
}

void CPrivateClassesTreeCtrl::OnTvnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW)pNMHDR;
	*pResult = 0;

	CDirectoryItem* pToDrag = (CDirectoryItem*)lpnmtv->itemNew.lParam;
	if (pToDrag == NULL || pToDrag->m_eItemType != SDI_UNSHAREDDIRECTORY || FileSystemTreeIsShared(pToDrag->m_strFullPath))
		return;

	ASSERT( m_pDraggingItem == NULL );
	delete m_pDraggingItem;
	m_pDraggingItem = pToDrag->CloneContent(); // to be safe we store a copy, as items can be deleted when collapsing the tree etc

	CImageList* piml = NULL;
	POINT ptOffset;
	RECT rcItem;
	if ((piml = CreateDragImage(lpnmtv->itemNew.hItem)) == NULL)
		return;

	/* get the bounding rectangle of the item being dragged (rel to top-left of control) */
	if (GetItemRect(lpnmtv->itemNew.hItem, &rcItem, TRUE))
	{
		CPoint ptDragBegin;
		int nX, nY;
		/* get offset into image that the mouse is at */
		/* item rect doesn't include the image */
		ptDragBegin = lpnmtv->ptDrag;
		ImageList_GetIconSize(piml->GetSafeHandle(), &nX, &nY);
		ptOffset.x = (ptDragBegin.x - rcItem.left) + (nX - (rcItem.right - rcItem.left));
		ptOffset.y = (ptDragBegin.y - rcItem.top) + (nY - (rcItem.bottom - rcItem.top));
		/* convert the item rect to screen co-ords, for use later */
		MapWindowPoints(NULL, &rcItem);
	}
	else
	{
		GetWindowRect(&rcItem);
		ptOffset.x = ptOffset.y = 8;
	}

	if (piml->BeginDrag(0, ptOffset))
	{
		CPoint ptDragEnter = lpnmtv->ptDrag;
		ClientToScreen(&ptDragEnter);
		piml->DragEnter(NULL, ptDragEnter);
	}
	delete piml;

	/* set the focus here, so we get a WM_CANCELMODE if needed */
	SetFocus();

	/* redraw item being dragged, otherwise it remains (looking) selected */
	InvalidateRect(&rcItem, TRUE);
	UpdateWindow();

	/* Hide the mouse cursor, and direct mouse input to this window */
	SetCapture(); 
}

void CPrivateClassesTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pDraggingItem != NULL)
	{
		CPoint pt;

		/* drag the item to the current position */
		pt = point;
		ClientToScreen(&pt);

		CImageList::DragMove(pt);
		CImageList::DragShowNolock(FALSE);
		if (CWnd::WindowFromPoint(pt) != this)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		else
		{
			TVHITTESTINFO tvhti;
			tvhti.pt = pt;
			ScreenToClient(&tvhti.pt);
			HTREEITEM hItemSel = HitTest(&tvhti);
			CDirectoryItem* pDragTarget;
			if (hItemSel != NULL && (pDragTarget = (CDirectoryItem*)GetItemData(hItemSel)) != NULL){
				//only allow dragging to shared folders
				if (pDragTarget->m_eItemType == SDI_DIRECTORY || pDragTarget->m_eItemType == SDI_NO){
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
					SelectDropTarget(pDragTarget->m_htItem);
				}
				else
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
			}
			else{
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
			}
		}

		CImageList::DragShowNolock(TRUE);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}
void CPrivateClassesTreeCtrl::OnCancelMode() 
{
	if (m_pDraggingItem != NULL){
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		ShowCursor(TRUE);
		SelectDropTarget(NULL);

		delete m_pDraggingItem;
		m_pDraggingItem = NULL;
		RedrawWindow();
	}
	CTreeCtrl::OnCancelMode();
}

void CPrivateClassesTreeCtrl::OnVolumesChanged()
{
	m_bFileSystemRootDirty = true;
}

bool CPrivateClassesTreeCtrl::ShowFileSystemDirectory(const CString& strDir)
{
	// expand directories untill we find our target directory and select it
	CDirectoryItem* pCurrentItem = m_pRootUnsharedDirectries;
	bool bContinue = true;
	while (bContinue)
	{
		bContinue = false;
		Expand(pCurrentItem->m_htItem, TVE_EXPAND);
		POSITION pos = pCurrentItem->liSubDirectories.GetHeadPosition();
		while (pos != NULL){
			CDirectoryItem* pTemp = pCurrentItem->liSubDirectories.GetNext(pos);
			if (strDir.CompareNoCase(pTemp->m_strFullPath + '\\') == 0)
			{
				Select(pTemp->m_htItem, TVGN_CARET);
				EnsureVisible(pTemp->m_htItem);
				return true;
			}
			else if (strDir.Find(pTemp->m_strFullPath + '\\') == 0)
			{
				pCurrentItem = pTemp;
				bContinue = true;
				break;
			}
		}
	}
	return false;
}

bool CPrivateClassesTreeCtrl::ShowSharedDirectory(const CString& strDir)
{
	// expand directories untill we find our target directory and select it
	POSITION pos = m_pRootDirectoryItem->liSubDirectories.GetHeadPosition();
	while (pos != NULL)
	{
		CDirectoryItem* pTemp = m_pRootDirectoryItem->liSubDirectories.GetNext(pos);
		if (pTemp->m_eItemType == SDI_DIRECTORY)
		{
			Expand(pTemp->m_htItem, TVE_EXPAND);
			if (strDir.IsEmpty()) // we want the parent item only
			{
				Select(pTemp->m_htItem, TVGN_CARET);
				EnsureVisible(pTemp->m_htItem);
				return true;
			}
			else // search for the fitting sub dir
			{
				POSITION pos2 = pTemp->liSubDirectories.GetHeadPosition();
				while (pos2 != NULL)
				{
					CDirectoryItem* pTemp2 = pTemp->liSubDirectories.GetNext(pos2);
					if (strDir.CompareNoCase(pTemp2->m_strFullPath + _T('\\')) == 0)
					{
						Select(pTemp2->m_htItem, TVGN_CARET);
						EnsureVisible(pTemp2->m_htItem);
						return true;							
					}
				}
				return false;
			}
		}
	}
	return false;
}

void CPrivateClassesTreeCtrl::ShowAllSharedFiles()
{
	Select(GetRootItem(), TVGN_CARET);
	EnsureVisible(GetRootItem());
}

bool CPrivateClassesTreeCtrl::IsSelected(int64 nCID)
{
	for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
	{
		int64 nID = m_lstAllSelectIDs.GetNext(pos);
		if ( nID == nCID )
			return true;
	}
	return false;
}

void CPrivateClassesTreeCtrl::AddSelected(int64 nCID)
{
	if ( IsSelected(nCID) )
		return;

	m_lstAllSelectIDs.AddTail(nCID);
}

void CPrivateClassesTreeCtrl::DelSelected(int64 nCID)
{
	for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
	{
		POSITION pos2 = pos;
		int64 nID = m_lstAllSelectIDs.GetNext(pos);
		if (nCID == nID)
			m_lstAllSelectIDs.RemoveAt(pos2);
	}
}
int64 CPrivateClassesTreeCtrl::GetClassID(HTREEITEM hItem)
{
	int64 nClassID;
	CClassInfoItem* pti = (CClassInfoItem*)GetItemData(hItem);
	if (pti)
		nClassID = pti->m_ID;
	else
		nClassID = ERROR_CLASS_ID;
	return nClassID;

	//HTREEITEM hSearchItem = hItem;
	//while(hSearchItem != NULL)
	//{
	//	int64 nSearchItemClassID;
	//	CClassInfoItem* pti = (CClassInfoItem*)GetItemData(hSearchItem);
	//	if (pti)
	//		nSearchItemClassID = pti->m_ID;
	//	else
	//		nSearchItemClassID = ERROR_CLASS_ID;
	//	nClassID = nSearchItemClassID;
	//	hSearchItem = GetParentItem(hSearchItem);
	//}
	//return nClassID;
}
//从子类往上开始遍历设置
//传入的是选中的当前项
void CPrivateClassesTreeCtrl::UpdateParentItems(HTREEITEM hChild)
{
	HTREEITEM hSearch = GetParentItem(hChild); //hSearch 指向当前选中项的父项
	while(hSearch != NULL)
	{
		if ( HasSelectedSubclass(GetClassID(hSearch)) )
		{
			SetItemState(hSearch, TVIS_BOLD, TVIS_BOLD);
			//m_setAllBoldIDs.insert(GetClassID(hSearch));
		}
		else
		{
			SetItemState(hSearch, 0, TVIS_BOLD);
			//m_setAllBoldIDs.erase(GetClassID(hSearch));
		}
		hSearch = GetParentItem(hSearch); //依次向上　验证相关父项
	}
}

//传入的是当前选中项的父项nParentCID 
//判断其父项nParentCID 下所有的子项是否有选中的，有则返回真
//即递归遍历其所有子类，子类中有任一个在选中列表中，则返回真,同时将相应父类设置为选中，才能递归向上设置为选中和字段颜色的变化
bool CPrivateClassesTreeCtrl::HasSelectedSubclass(int64 nParentCID/*, HTREEITEM hParent*/)
{
	if ( nParentCID == 4 )
	{
		int xxx = 0;
	}
	//HTREEITEM hParent_Parent = GetParentItem(hParent);
	//遍历查找直接子类
	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
	{
		if ( nParentCID == it->ParentID )
		{
			for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
			{
				int64 nID = m_lstAllSelectIDs.GetNext(pos);
				if ( nID == it->ID )
				{
					//设置父项为选中
					/*if ( hParent )
					{
					SetCheck(hParent,true);
					}*/
					return true;
				}
			}
			if ( HasSelectedSubclass(it->ID/*,hParent_Parent*/) )
				return true; //加上这句，否则只有直接父节点变颜色
		}
	}

	return false;
	
	//如果父项nCID在小于或等于已选的列表中存在则返回为真 这种算法不太正确
	//for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
	//{
	//	int64 nID = m_lstAllSelectIDs.GetNext(pos);
	//	if ( nID >= nParentCID )
	//		return true;
	//}
	//return false;
}
//加上这个会死循环
bool CPrivateClassesTreeCtrl::HasBoldSubclass(int64 nParentCID)
{
	//遍历查找直接子类
	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
	{
		if ( nParentCID == it->ParentID )
		{
			set<int64>::iterator it_setBold = m_setAllBoldIDs.begin();
			//for(it_setBold = m_setAllBoldIDs.begin();it_setBold < m_setAllBoldIDs.end(); it_setBold++)  //如果使用这句会出错，因为set容器对<没有重载
			while(it_setBold != m_setAllBoldIDs.end() )
			{
				if ( *it_setBold == it->ID )
				{
					return true;
				}
			}

			HasBoldSubclass(it->ID);
			//break;
		}
	}

	return false;
}

//
//bool CPrivateClassesTreeCtrl::HasSelectedSubclass_Recursion(int64 nParentCID, int nDeep)
//{
//	//遍历查找直接子类
//	for ( vector<stClassInfoItem>::iterator it = wyytExtendDataManagePri::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManagePri::Instance().m_vecClassInfos.end(); it++ )
//	{
//		if ( nParentCID == it->ParentID )
//		{
//			for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
//			{
//				int64 nID = m_lstAllSelectIDs.GetNext(pos);
//				if ( nID == it->ID )
//					return true;
//			}
//			HasSelectedSubclass(it->ID, nDeep + 1);
//			//break;
//		}
//	
//}
void CPrivateClassesTreeCtrl::CheckChanged(HTREEITEM hItem, bool bChecked)
{
	int64 nClassID = GetClassID(hItem);
	if (bChecked)
		AddSelected(nClassID);
	else
		DelSelected(nClassID);

	UpdateParentItems(hItem);

	//这里转换下，一开始没想好
	vector<int64> vecNowSelClassIDs;
	for (POSITION pos = m_lstAllSelectIDs.GetHeadPosition(); pos != NULL; )
	{
		int64 nID = m_lstAllSelectIDs.GetNext(pos);
		vecNowSelClassIDs.push_back(nID);
	}
	m_pSharedFilesCtrl->SetClassFilter(vecNowSelClassIDs);
	//GetParent()->SendMessage(WM_COMMAND, UM_ITEMSTATECHANGED, (LPARAM)m_hWnd);
}

void CPrivateClassesTreeCtrl::MarkChilds(HTREEITEM hChild,bool mark) { 
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

void CPrivateClassesTreeCtrl::SelectSubClassTree(HTREEITEM hItem, BOOL bRecurse)
{
	CWaitCursor curWait;
	SetRedraw(FALSE);

	HTREEITEM hItemVisibleItem = GetFirstVisibleItem();
	CheckChanged(hItem, !GetCheck(hItem));
	if (bRecurse)
	{
		Expand(hItem, TVE_TOGGLE);//如果是按ctl键全部选中的话，这里会把下面的子目录打开，再选中相应的项
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

void CPrivateClassesTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		HTREEITEM hItem = GetSelectedItem();
		if (hItem)
		{
			SelectSubClassTree(hItem, GetKeyState(VK_CONTROL) & 0x8000);

			// if Ctrl+Space is passed to the tree control, it just beeps and does not check/uncheck the item!
			SetCheck(hItem, !GetCheck(hItem));
			return;
		}
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	
}


void CPrivateClassesTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT uHitFlags; 
	HTREEITEM hItem = HitTest(point, &uHitFlags); 
	if (hItem && (uHitFlags & TVHT_ONITEMSTATEICON))
		SelectSubClassTree(hItem, nFlags & MK_CONTROL);
	CTreeCtrl::OnLButtonDown(nFlags, point); 
}


void CPrivateClassesTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

