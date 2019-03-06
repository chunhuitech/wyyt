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
#include "wyytdlg.h"
#include "SharedFilesCtrl.h"
#include "OtherFunctions.h"
//#include "FileInfoDialog.h"
//#include "MetaDataDlg.h"
//#include "ED2kLinkDlg.h"
#include "ArchivePreviewDlg.h"
#include "AttachsDlg.h"
//#include "CommentDialog.h"
#include "HighColorTab.hpp"
#include "ListViewWalkerPropertySheet.h"
#include "UserMsgs.h"
#include "ResizableLib/ResizableSheet.h"
//#include "KnownFile.h"
//#include "MapKey.h"
//#include "SharedFileList.h"
#include "MemDC.h"
//#include "PartFile.h"
#include "MenuCmds.h"
//#include "IrcWnd.h"
//#include "SharedFilesWnd.h"
#include "RecordInfoSheetDlg.h"
#include "RecordDetails.h"

//#include "InputBox.h"
#include "WebServices.h"
//#include "TransferDlg.h"
//#include "ClientList.h"
//#include "UpDownClient.h"
//#include "Collection.h"
//#include "CollectionCreateDialog.h"
//#include "CollectionViewDialog.h"
//#include "SearchParams.h"
//#include "SearchDlg.h"
//#include "SearchResultsWnd.h"
#include "ToolTipCtrlX.h"
//#include "MassRename.h" //Xman Mass Rename (Morph)
//#include "Log.h" //Xman Mass Rename (Morph)
//#include "kademlia/kademlia/kademlia.h"
//#include "kademlia/kademlia/UDPFirewallTester.h"
//#include "MediaInfo.h"
//#include "Log.h"
//#include "KnownFileList.h"
#include "VisualStylesXP.h"
#include "ModInfo.h"
//#include "DropDownButton.h"
#include "ResPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool NeedArchiveInfoPage(const CSimpleArray<void*>* paItems);
void UpdateFileDetailsPages(CListViewPropertySheet *pSheet,
							CResizablePage *pArchiveInfo, CResizablePage *pAttachInfo);


////////////////////////////////////////////////////////////////////////////////
//// CSharedFileDetailsSheet
//
//class CSharedFileDetailsSheet : public CListViewWalkerPropertySheet
//{
//	DECLARE_DYNAMIC(CSharedFileDetailsSheet)
//
//public:
//	CSharedFileDetailsSheet(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage = 0, CListCtrlItemWalk* pListCtrl = NULL);
//	virtual ~CSharedFileDetailsSheet();
//
//protected:
//	
//	//CMetaDataDlg		m_wndMetaData;
//	//CED2kLinkDlg		m_wndFileLink;
//	//CCommentDialog		m_wndFileComments;
//	CArchivePreviewDlg	m_wndArchiveInfo;
//	CAttachsDlg		m_wndAttachsInfo;
//
//	UINT m_uPshInvokePage;
//	static LPCTSTR m_pPshStartPage;
//
//	void UpdateTitle();
//
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
//	virtual BOOL OnInitDialog();
//
//	DECLARE_MESSAGE_MAP()
//	afx_msg void OnDestroy();
//	afx_msg LRESULT OnDataChanged(WPARAM, LPARAM);
//};
//
//LPCTSTR CSharedFileDetailsSheet::m_pPshStartPage;
//
//IMPLEMENT_DYNAMIC(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)
//
//BEGIN_MESSAGE_MAP(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)
//	ON_WM_DESTROY()
//	ON_MESSAGE(UM_DATA_CHANGED, OnDataChanged)
//END_MESSAGE_MAP()
//
//CSharedFileDetailsSheet::CSharedFileDetailsSheet(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage, CListCtrlItemWalk* pListCtrl)
//	: CListViewWalkerPropertySheet(pListCtrl)
//{
//	m_uPshInvokePage = uPshInvokePage;
//	POSITION pos = aFiles.GetHeadPosition();
//	while (pos)
//		m_aItems.Add(aFiles.GetNext(pos));
//	m_psh.dwFlags &= ~PSH_HASHELP;
//
//	//m_wndFileComments.m_psp.dwFlags &= ~PSP_HASHELP;
//	//m_wndFileComments.m_psp.dwFlags |= PSP_USEICONID;
//	//m_wndFileComments.m_psp.pszIcon = _T("FileComments");
//	//m_wndFileComments.SetFiles(&m_aItems);
//	//AddPage(&m_wndFileComments);
//
//	m_wndArchiveInfo.m_psp.dwFlags &= ~PSP_HASHELP;
//	m_wndArchiveInfo.m_psp.dwFlags |= PSP_USEICONID;
//	m_wndArchiveInfo.m_psp.pszIcon = _T("ARCHIVE_PREVIEW");
//	m_wndArchiveInfo.SetFiles(&m_aItems);
//	AddPage(&m_wndArchiveInfo);
//	m_wndAttachsInfo.m_psp.dwFlags &= ~PSP_HASHELP;
//	m_wndAttachsInfo.m_psp.dwFlags |= PSP_USEICONID;
//	m_wndAttachsInfo.m_psp.pszIcon = _T("ATTACH_PREVIEW");
//	m_wndAttachsInfo.SetFiles(&m_aItems);
//	AddPage(&m_wndAttachsInfo);
//	/*if (NeedArchiveInfoPage(&m_aItems))
//	AddPage(&m_wndArchiveInfo);
//	else
//	{
//	AddPage(&m_wndMediaInfo);
//	}*/
//
//	//m_wndMetaData.m_psp.dwFlags &= ~PSP_HASHELP;
//	//m_wndMetaData.m_psp.dwFlags |= PSP_USEICONID;
//	//m_wndMetaData.m_psp.pszIcon = _T("METADATA");
//	//if (m_aItems.GetSize() == 1 && thePrefs.IsExtControlsEnabled()) {
//	//	m_wndMetaData.SetFiles(&m_aItems);
//	//	AddPage(&m_wndMetaData);
//	//}
//
//	//m_wndFileLink.m_psp.dwFlags &= ~PSP_HASHELP;
//	//m_wndFileLink.m_psp.dwFlags |= PSP_USEICONID;
//	//m_wndFileLink.m_psp.pszIcon = _T("ED2KLINK");
//	//m_wndFileLink.SetFiles(&m_aItems);
//	//AddPage(&m_wndFileLink);
//
//	LPCTSTR pPshStartPage = m_pPshStartPage;
//	if (m_uPshInvokePage != 0)
//		pPshStartPage = MAKEINTRESOURCE(m_uPshInvokePage);
//	for (int i = 0; i < m_pages.GetSize(); i++)
//	{
//		CPropertyPage* pPage = GetPage(i);
//		if (pPage->m_psp.pszTemplate == pPshStartPage)
//		{
//			m_psh.nStartPage = i;
//			break;
//		}
//	}
//}
//
//CSharedFileDetailsSheet::~CSharedFileDetailsSheet()
//{
//}
//
//void CSharedFileDetailsSheet::OnDestroy()
//{
//	if (m_uPshInvokePage == 0)
//		m_pPshStartPage = GetPage(GetActiveIndex())->m_psp.pszTemplate;
//	CListViewWalkerPropertySheet::OnDestroy();
//}
//
//BOOL CSharedFileDetailsSheet::OnInitDialog()
//{
//	EnableStackedTabs(FALSE);
//	BOOL bResult = CListViewWalkerPropertySheet::OnInitDialog();
//	HighColorTab::UpdateImageList(*this);
//	InitWindowStyles(this);
//	EnableSaveRestore(_T("SharedFileDetailsSheet"), !thePrefs.prefReadonly); // call this after(!) OnInitDialog // X: [ROP] - [ReadOnlyPreference]
//	UpdateTitle();
//	return bResult;
//}
//
//LRESULT CSharedFileDetailsSheet::OnDataChanged(WPARAM, LPARAM)
//{
//	UpdateTitle();
//	//UpdateFileDetailsPages(this, &m_wndArchiveInfo, &m_wndAttachsInfo);
//	return 1;
//}
//
//void CSharedFileDetailsSheet::UpdateTitle()
//{
//	CString text = GetResString(IDS_DETAILS);
//	if(m_aItems.GetSize() == 1)
//		//text.AppendFormat(_T(": %s"), STATIC_DOWNCAST(CAbstractFile, m_aItems[0])->GetFileName());
//		///text.AppendFormat(_T(": %s"), ((CAbstractFile*)m_aItems[0])->GetFileName());
//	SetWindowText(text);
//}
//
//BOOL CSharedFileDetailsSheet::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	if (wParam == ID_APPLY_NOW)
//	{
//		CSharedFilesCtrl* pSharedFilesCtrl = DYNAMIC_DOWNCAST(CSharedFilesCtrl, m_pListCtrl->GetListCtrl());
//		if (pSharedFilesCtrl)
//		{
//			for (int i = 0; i < m_aItems.GetSize(); i++) 
//			{
//				// so, and why does this not(!) work while the sheet is open ??
////>>> WiZaRd::Optimization
//				//pSharedFilesCtrl->UpdateFile(DYNAMIC_DOWNCAST(CKnownFile, m_aItems[i]));
//				//theApp.sharedfiles->UpdateFile(DYNAMIC_DOWNCAST(CKnownFile, m_aItems[i])); 
////<<< WiZaRd::Optimization				
////				theApp.sharedfiles->UpdateFile((CKnownFile*)m_aItems[i]); 
//			}
//		}
//	}
//
//	return CListViewWalkerPropertySheet::OnCommand(wParam, lParam);
//}

//////////////////////////////////////////////////////////////////////////////
// CSharedFilesCtrl
static const UINT colStrID[]={
	 IDS_DL_RECORD_TITLE
	,IDS_DL_RECORD_SIZE
	,IDS_RECORD_SUMMARY
};

IMPLEMENT_DYNAMIC(CSharedFilesCtrl, CMuleListCtrl)

BEGIN_MESSAGE_MAP(CSharedFilesCtrl, CMuleListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnClick)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_GETINFOTIP, OnLvnGetInfoTip)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNmDblClk)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnNMClick)
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CSharedFilesCtrl::CSharedFilesCtrl()
	: CListCtrlItemWalk(this),m_pDirectoryFilter(NULL),m_pClassInfoFilter(NULL)
{
	memset(&m_aSortBySecondValue, 0, sizeof(m_aSortBySecondValue));
	SetGeneralPurposeFind(true);
	m_pToolTip = new CToolTipCtrlX;
	SetSkinKey(L"SharedFilesLv");
	m_pHighlightedItem = NULL;
}

CSharedFilesCtrl::~CSharedFilesCtrl()
{
	//while (!liTempShareableFilesInDir.IsEmpty())	// delete shareble files
	//	delete liTempShareableFilesInDir.RemoveHead();

	// Tux: Fix: memleak fix [WiZaRd] [start]
	//if (m_PrioMenu) VERIFY( m_PrioMenu.DestroyMenu() );
	//if (m_CollectionsMenu) VERIFY( m_CollectionsMenu.DestroyMenu() );
	//if (m_SharedFilesMenu) VERIFY( m_SharedFilesMenu.DestroyMenu() );
	// Tux: Fix: memleak fix [WiZaRd] [end]
	if (m_RecordsMenu) VERIFY( m_RecordsMenu.DestroyMenu() );
	
	delete m_pToolTip;
	m_ctlListHeader.Detach();
}

void CSharedFilesCtrl::Init()
{
	//设置配置文件preferences.ini中关于列表控件相关属性(排序、列宽等)的前缀名称
	SetPrefsKey(_T("SysRecordCtrl"));
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	ASSERT( (GetStyle() & LVS_SINGLESEL) == 0 ); //不是　　同时只能选中列表中一项

	//int64 ID;
	//wstring Titlel;
	//wstring KeyValues;
	//int HaseAttach;
	//int AttachSpaceSize;
	//int HaseRecomm;
	//wstring Content;
	//wstring InputTime;

	InsertColumn(0, GetResString(IDS_DL_RECORD_TITLE),		LVCFMT_CENTER,  DFLT_RECORD_TITLE_COL_WIDTH);//标题
	InsertColumn(1, GetResString(IDS_DL_RECORD_SID),		LVCFMT_CENTER,  DFLT_RECORD_SID_COL_WIDTH);//SID
	InsertColumn(2, GetResString(IDS_RECORD_SUMMARY),				LVCFMT_CENTER,  DFLT_RECORD_SUMMARY_COL_WIDTH);//摘要
	InsertColumn(3, GetResString(IDS_DL_RECORD_SIZE),			LVCFMT_CENTER, DFLT_RECORD_SIZE_COL_WIDTH);//附件大小
	InsertColumn(4, GetResString(IDS_DL_RECORD_MODTIME),		LVCFMT_CENTER,  DFLT_RECORD_MODTIME_COL_WIDTH);//ModTime
	InsertColumn(5, GetResString(IDS_DL_RECORD_INPUTTIME),		LVCFMT_CENTER,  DFLT_RECORD_INPUTTIME_COL_WIDTH);//InputTime
	

	//SetAllIcons();
	CreateMenues();
	//LoadSettings();

	//m_aSortBySecondValue[0] = true; // Requests:			Sort by 2nd value by default
	//m_aSortBySecondValue[1] = true; // Accepted Requests:	Sort by 2nd value by default
	//m_aSortBySecondValue[2] = true; // Transferred Data:	Sort by 2nd value by default
	//m_aSortBySecondValue[3] = false; // Shared ED2K|Kad:	Sort by 1st value by default
	//if (GetSortItem() >= 5 && GetSortItem() <= 7)
	//	m_aSortBySecondValue[GetSortItem() - 5] = GetSortSecondValue();
	//else if (GetSortItem() == 11)
	//	m_aSortBySecondValue[3] = GetSortSecondValue();
	SetSortArrow();
	SortItems(SortProc, GetSortItem() + (GetSortAscending() ? 0 : 20) + (GetSortSecondValue() ? 100 : 0));

	//CToolTipCtrl* tooltip = GetToolTips();
	//if (tooltip){
	//	m_pToolTip->SetFileIconToolTip(true);
	//	m_pToolTip->SubclassWindow(*tooltip);
	//	tooltip->ModifyStyle(0, TTS_NOPREFIX);
	//	tooltip->SetDelayTime(TTDT_AUTOPOP, 20000);
	//	tooltip->SetDelayTime(TTDT_INITIAL, thePrefs.GetToolTipDelay()*1000);
	//}
	//IgnoredColums = 0x900;	//8: shared parts, 11:shared ed2k/kad
	//m_ctlListHeader.Attach(GetHeaderCtrl()->Detach());

	//m_ShareDropTarget.SetParent(this);
	//VERIFY( m_ShareDropTarget.Register(this) );
}

void CSharedFilesCtrl::OnSysColorChange()
{
	CMuleListCtrl::OnSysColorChange();
	SetAllIcons();
	CreateMenues();
}

void CSharedFilesCtrl::SetAllIcons()
{
	ApplyImageList(NULL);//设置排序箭头
	m_ImageList.DeleteImageList();
	m_ImageList.Create(16, 16, theApp.m_iDfltImageListColorFlags | ILC_MASK, 0, 1);
	//m_ImageList.Add(CTempIconLoader(_T("EMPTY")));
	//这一块是设置什么图标??--其它界面用到此控件显示时设置的相应图标!!
	m_ImageList.Add(CTempIconLoader(_T("FileSharedServer")));
	m_ImageList.Add(CTempIconLoader(_T("FileSharedKad")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_NotRated")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Fake")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Poor")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Fair")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Good")));
	m_ImageList.Add(CTempIconLoader(_T("Rating_Excellent")));
	m_ImageList.Add(CTempIconLoader(_T("Collection_Search"))); // rating for comments are searched on kad
	m_ImageList.SetOverlayImage(m_ImageList.Add(CTempIconLoader(_T("FileCommentsOvl"))), 1);
	// Apply the image list also to the listview control, even if we use our own 'DrawItem'.
	// This is needed to give the listview control a chance to initialize the row height.
	ASSERT( (GetStyle() & LVS_SHAREIMAGELISTS) != 0 );
	VERIFY( ApplyImageList(m_ImageList) == NULL );
}

void CSharedFilesCtrl::Localize()
{
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	HDITEM hdi;
	hdi.mask = HDI_TEXT;
	CString strRes;
	for (int icol=0;icol<pHeaderCtrl->GetItemCount();++icol) {
		strRes=GetResString(colStrID[icol]);
		hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
		pHeaderCtrl->SetItem(icol, &hdi);
	}
/*
	strRes = GetResString(IDS_DL_FILENAME);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(0, &hdi);

	strRes = GetResString(IDS_DL_SIZE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(1, &hdi);

	strRes = GetResString(IDS_TYPE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(2, &hdi);

	strRes = GetResString(IDS_PRIORITY);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(3, &hdi);

	strRes = GetResString(IDS_FILEID);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(4, &hdi);

	strRes = GetResString(IDS_SF_REQUESTS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(5, &hdi);

	strRes = GetResString(IDS_SF_ACCEPTS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(6, &hdi);

	strRes = GetResString(IDS_SF_TRANSFERRED);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(7, &hdi);

	strRes = GetResString(IDS_SHARED_STATUS);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(8, &hdi);

	strRes = GetResString(IDS_FOLDER);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(9, &hdi);

	strRes = GetResString(IDS_COMPLSOURCES);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(10, &hdi);

	strRes = GetResString(IDS_SHAREDTITLE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(11, &hdi);

	strRes = GetResString(IDS_ARTIST);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(12, &hdi);

	strRes = GetResString(IDS_ALBUM);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(13, &hdi);

	strRes = GetResString(IDS_TITLE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(14, &hdi);

	strRes = GetResString(IDS_LENGTH);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(15, &hdi);

	strRes = GetResString(IDS_BITRATE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(16, &hdi);

	strRes = GetResString(IDS_CODEC);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(17, &hdi);

	//Xman see OnUploadqueue
	strRes = GetResString(IDS_ONQUEUE);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(18, &hdi);
	//Xman end

	//Xman advanced upload-priority
	strRes = GetResString(IDS_SHAREFACTOR);
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	pHeaderCtrl->SetItem(19, &hdi);
	//Xman end
*/
	CreateMenues();

	Invalidate();// X: [CI] - [Code Improvement]
	/*size_t iItems = GetItemCount();
	for (size_t i = 0; i < iItems; i++)
		Update(i);*/
}

void CSharedFilesCtrl::AddRecord(const CRecordItem* record)
{
	if (!CwyytDlg::IsRunning())
		return;
	//对不同的大类节点过滤处理
	//if (m_pDirectoryFilter != NULL){
	//	ASSERT( IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/ || CheckBoxesEnabled()/*m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/ );
	//	switch(m_pDirectoryFilter->m_eItemType){
	//		case SDI_ALL:
	//			// No filter
	//			break;
	//		case SDI_FILESYSTEMPARENT:
	//			return;
	//.........
	//	}
	//}
	//具体实现添加
	//if (CheckBoxesEnabled()/*m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/ && IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//{
	//	for (POSITION pos = liTempShareableFilesInDir.GetHeadPosition(); pos != NULL; )
	//	{
	//		CShareableFile* pFile = liTempShareableFilesInDir.GetNext(pos);
	//		if (pFile->GetFilePath().CompareNoCase(file->GetFilePath()) == 0)
	//		{
	//			int iOldFile = FindFile(pFile);
	//			if (iOldFile != (-1))
	//			{
	//				SetItemData(iOldFile, (LPARAM)file);
	//				Update(iOldFile);
	//				//theApp.emuledlg->sharedfileswnd->ShowFilesCount();
	//				return;
	//			}
	//		}
	//	}
	//}

	//SetItemData(iOldFile, (LPARAM)record);
	//Update(iOldFile);
	InsertItem(LVIF_TEXT | LVIF_PARAM, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, 0, (LPARAM)record);
}
void CSharedFilesCtrl::AddFile(const CShareableFile* file)
{
	if (!CwyytDlg::IsRunning())
		return;
	// check filter conditions if we should show this file right now
	//if (m_pDirectoryFilter != NULL){
	//	ASSERT( IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/ || CheckBoxesEnabled()/*m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/ );
	//	switch(m_pDirectoryFilter->m_eItemType){
	//		case SDI_ALL:
	//			// No filter
	//			break;
	//		case SDI_FILESYSTEMPARENT:
	//			return;

	//		case SDI_UNSHAREDDIRECTORY:
	//			// Items from the whole filesystem tree
	//			if (file->IsPartFile())
	//				return;
	//		case SDI_NO:
	//			// some shared directory
	//		case SDI_CATINCOMING:
	//			// Categories with special incoming dirs
	//			if (CompareDirectories(file->GetSharedDirectory(), m_pDirectoryFilter->m_strFullPath) != 0)
	//				return;
	//			break;

	//		case SDI_TEMP:
	//			// only tempfiles
	//			if (!file->IsPartFile()||(m_pDirectoryFilter->m_nCatFilter != -1 && (UINT)m_pDirectoryFilter->m_nCatFilter != ((CPartFile*)file)->GetCategory()))
	//				return;
	//			break;

	//		case SDI_DIRECTORY:
	//			// any userselected shared dir but not incoming or temp
	//			if (file->IsPartFile()||CompareDirectories(file->GetSharedDirectory(), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR)) == 0)
	//				return;
	//			break;

	//		case SDI_INCOMING:
	//			// Main incoming directory
	//			if (CompareDirectories(file->GetSharedDirectory(), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR)) != 0)
	//				return;
	//			// Hmm should we show all incoming files dirs or only those from the main incoming dir here?
	//			// hard choice, will only show the main for now
	//			break;

	//	}
	//}
	//if ((filter>0 && (filter-1) != GetED2KFileTypeID(file->GetFileName())) || IsFilteredItem(file))
	//	return;
	//if (FindFile(file) != -1)
	//{
	//	// in the filesystem view the shared status might have changed so we need to update the item to redraw the checkbox
	//	if (CheckBoxesEnabled()/*m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/)
	//		UpdateFile(file);
	//	return;
	//}
	//
	//// if we are in the filesystem view, this might be a CKnownFile which has to replace a CShareableFile
	//// (in case we start sharing this file), so make sure to replace the old one instead of adding a new
	//if (CheckBoxesEnabled()/*m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/ && IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//{
	//	for (POSITION pos = liTempShareableFilesInDir.GetHeadPosition(); pos != NULL; )
	//	{
	//		CShareableFile* pFile = liTempShareableFilesInDir.GetNext(pos);
	//		if (pFile->GetFilePath().CompareNoCase(file->GetFilePath()) == 0)
	//		{
	//			int iOldFile = FindFile(pFile);
	//			if (iOldFile != (-1))
	//			{
	//				SetItemData(iOldFile, (LPARAM)file);
	//				Update(iOldFile);
	//				//theApp.emuledlg->sharedfileswnd->ShowFilesCount();
	//				return;
	//			}
	//		}
	//	}
	//}

	//InsertItem(LVIF_TEXT | LVIF_PARAM, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, 0, (LPARAM)file);
}

void CSharedFilesCtrl::RemoveFile(const CShareableFile* file, bool bDeletedFromDisk)
{
	int iItem = FindFile(file);
	//if (iItem != -1)
	//{
	//	if (!bDeletedFromDisk && CheckBoxesEnabled()/*m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY*/)
	//		// in the file system view we usally dont need to remove a file, if it becomes unshared it will
	//		// still be visible as its still in the file system and the knownfile object doesn't gets deleted neither
	//		// so to avoid having to reload the whole list we just update it instead of removing and refinding
	//		UpdateFile(file);
	//	else
	//		DeleteItem(iItem);
	//	theApp.emuledlg->sharedfileswnd->ShowFilesCount();
	//}
}

void CSharedFilesCtrl::UpdateFile(const CShareableFile* file, bool bUpdateFileSummary/*, bool force*/) //Xman advanced upload-priority
{
	if (!file || !CwyytDlg::IsRunning())
		return;

	////MORPH START - SiRoB, Don't Refresh item if not needed
	//if( theApp.emuledlg->activewnd != theApp.emuledlg->sharedfileswnd/* && !force*/) //Xman advanced upload-priority 
	//	return;
	////MORPH END   - SiRoB, Don't Refresh item if not needed

	//int iItem = FindFile(file);
	//if (iItem != -1)
	//{
	//	Update(iItem);
	//	if (bUpdateFileSummary && GetItemState(iItem, LVIS_SELECTED) && IsWindowVisible()) //Xman [MoNKi: -Downloaded History-]
	//		theApp.emuledlg->sharedfileswnd->ShowSelectedFilesDetails();
	//}
}
void CSharedFilesCtrl::UpdateRecord(const CRecordItem* record, bool bUpdateFileSummary/*, bool force=false*/)
{
	if (!record || !CwyytDlg::IsRunning())
		return;
}
int CSharedFilesCtrl::FindRecord(const CRecordItem* pRecord)
{
	LVFINDINFO find;
	find.flags = LVFI_PARAM;
	find.lParam = (LPARAM)pRecord;
	return FindItem(&find);
}
int CSharedFilesCtrl::FindFile(const CShareableFile* pFile)
{
	LVFINDINFO find;
	find.flags = LVFI_PARAM;
	find.lParam = (LPARAM)pFile;
	return FindItem(&find);
}
void CSharedFilesCtrl::ReloadRecordList()
{
	DeleteAllItems();
	AddRecordDatasByClassIDs();
	//AddRecordDatas();
}
void CSharedFilesCtrl::ReloadRecordList(vector<int64> nVecSelClassIDs)
{
	DeleteAllItems();
	AddRecordDatas_PreDeal(nVecSelClassIDs);
}
void CSharedFilesCtrl::ReloadFileList()
{
	DeleteAllItems();
//	theApp.wyytdlg->sharedfileswnd->ShowSelectedFilesDetails();
//	
//#ifdef REPLACE_ATLMAP
//	for (CKnownFilesMap::const_iterator it = theApp.sharedfiles->m_Files_map.begin(); it != theApp.sharedfiles->m_Files_map.end(); ++it)
//		AddFile(it->second);
//#else
//	CCKey bufKey;
//	CKnownFile* cur_file;
//	for (POSITION pos = theApp.sharedfiles->m_Files_map.GetStartPosition(); pos != 0; ){
//		theApp.sharedfiles->m_Files_map.GetNextAssoc(pos, bufKey, cur_file);
//		AddFile(cur_file);
//	}
//#endif
//
//	if(m_pDirectoryFilter != NULL){
//		switch(m_pDirectoryFilter->m_eItemType){
//			case SDI_INCOMING:
//				AddShareableFiles(thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR));
//				break;
//			case SDI_UNSHAREDDIRECTORY:
//			case SDI_NO:
//			case SDI_CATINCOMING:
//				if (!m_pDirectoryFilter->m_strFullPath.IsEmpty())
//					AddShareableFiles(m_pDirectoryFilter->m_strFullPath);
//				else {
//					while (!liTempShareableFilesInDir.IsEmpty())	// cleanup temp filelist
//						delete liTempShareableFilesInDir.RemoveHead();
//				}
//				break;
//		}
//	}
//	theApp.wyytdlg->sharedfileswnd->ShowFilesCount();
}

void CSharedFilesCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect cur_rec(lpDrawItemStruct->rcItem);
	/*const*/ CRecordItem* record = (CRecordItem*)lpDrawItemStruct->itemData;
	//CKnownFile* pKnownFile = NULL;
	//if (IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//	pKnownFile = (CKnownFile*)file;
	CMemoryDC dc(CDC::FromHandle(lpDrawItemStruct->hDC), &lpDrawItemStruct->rcItem);
	InitItemMemDC(dc, lpDrawItemStruct->rcItem, /*(pKnownFile && pKnownFile->GetUpPriority()==PR_POWER)*/false ? RGB(255, 210, 210) : ((lpDrawItemStruct->itemID % 2)?m_crEvenLine:m_crWindow), lpDrawItemStruct->itemState);

	RECT rcClient;// X: [DDHC] - [Don't Draw Hidden Column]
	GetClientRect(&rcClient);

	CHeaderCtrl *pHeaderCtrl = GetHeaderCtrl();
	int iCount = pHeaderCtrl->GetItemCount();
	cur_rec.right = cur_rec.left - sm_iSubItemInset;
	cur_rec.left += sm_iSubItemInset;
	for(int iCurrent = 0; iCurrent < iCount; iCurrent++)
	{
		int iColumn = pHeaderCtrl->OrderToIndex(iCurrent);
		if(IsColumnHidden(iColumn)) continue;
		UINT uDrawTextAlignment;
		int iColumnWidth = GetColumnWidth(iColumn, uDrawTextAlignment);
		cur_rec.right += iColumnWidth;
		if(iColumnWidth > 2*sm_iSubItemInset && cur_rec.right>0)
		{// X: [DDHC] - [Don't Draw Hidden Column]
			TCHAR szItem[1024];
			GetItemDisplayText(record, iColumn, szItem, _countof(szItem));
			switch(iColumn)
			{
				case 0:
					{
					int iCheckboxDrawWidth = 0;
					if (CheckBoxesEnabled())
					{
						int iState = (record == m_pHighlightedItem) ? CBS_UNCHECKEDHOT : CBS_UNCHECKEDNORMAL;
						int iNoStyleState = (record == m_pHighlightedItem) ? DFCS_PUSHED : 0;
						// no interacting with shell linked files or default shared directories
						////if (theApp.sharedfiles->ShouldBeShared(file->GetSharedDirectory(), file->GetFilePath(), false)  && IsKindOfCKnownFile(file))
						////{
						////	if(file->IsShellLinked()){
						////		iState = CBS_CHECKEDDISABLED;
						////		iNoStyleState = DFCS_CHECKED | DFCS_INACTIVE;
						////	}
						////	else if((theApp.sharedfiles->ShouldBeShared(file->GetSharedDirectory(), _T("")/*file->GetFilePath()*/, true)))
						////	{
						////		if(IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/){ // X: [QOH] - [QueryOnHashing]
						////			iState = CBS_CHECKEDDISABLED;
						////			iNoStyleState = DFCS_CHECKED | DFCS_INACTIVE;
						////		}
						////	}
						////	else{
						////		iState = (file == m_pHighlightedItem) ? CBS_CHECKEDHOT : CBS_CHECKEDNORMAL;
						////		iNoStyleState = (file == m_pHighlightedItem) ? (DFCS_PUSHED | DFCS_CHECKED) : DFCS_CHECKED;
						////	}
						////}
						// SLUGFILLER: SafeHash remove - removed installation dir unsharing
						/*
						else if (!thePrefs.IsShareableDirectory(file->GetPath())){
							iState = CBS_DISABLED;
							iNoStyleState = DFCS_INACTIVE;
						}
						*/

						HTHEME hTheme = (g_xpStyle.IsThemeActive() && g_xpStyle.IsAppThemed()) ? g_xpStyle.OpenThemeData(NULL, L"BUTTON") : NULL;

						RECT recCheckBox;
						recCheckBox.left = cur_rec.left;
						recCheckBox.right = recCheckBox.left + 16;
						recCheckBox.top = cur_rec.top;
						if(cur_rec.Height() > 16)
							recCheckBox.top += (cur_rec.Height() - 16) / 2;
						recCheckBox.bottom = recCheckBox.top + 16;
						if (hTheme != NULL)
							g_xpStyle.DrawThemeBackground(hTheme, dc.GetSafeHdc(), BP_CHECKBOX, iState, &recCheckBox, NULL);
						else
							dc.DrawFrameControl(&recCheckBox, DFC_BUTTON, DFCS_BUTTONCHECK | iNoStyleState | DFCS_FLAT);
						iCheckboxDrawWidth = 16 + sm_i2IconOffset;
						cur_rec.left += iCheckboxDrawWidth;
					}

					int iIconDrawWidth = 16;
					int iIconPosY = (cur_rec.Height() > 19) ? ((cur_rec.Height() - 16) / 2) : 1;
					////int iImage = theApp.GetFileTypeSystemImageIdx(szItemfile->GetFileName());
					////if (theApp.GetSystemImageList() != NULL)
					////	ImageList_Draw(theApp.GetSystemImageList(), iImage, dc.GetSafeHdc(), cur_rec.left, cur_rec.top + iIconPosY, ILD_TRANSPARENT);
					////if (!file->GetFileComment().IsEmpty() || file->GetFileRating())
					////{
					////	POINT point = {cur_rec.left, cur_rec.top + iIconPosY};
					////	m_ImageList.Draw(dc, 0, point, ILD_NORMAL | INDEXTOOVERLAYMASK(1));
					////}
					cur_rec.left += iIconDrawWidth;

					////if (thePrefs.ShowRatingIndicator() && (file->HasComment() || file->HasRating() || file->IsKadCommentSearchRunning()))
					////{
					////	cur_rec.left += sm_i2IconOffset;
					////	POINT point2 = {cur_rec.left, cur_rec.top + iIconPosY};
					////	m_ImageList.Draw(dc, file->UserRating(true)+2, point2, ILD_NORMAL);
					////	cur_rec.left += 16;
					////	iIconDrawWidth += 16 + sm_i2IconOffset;
					////}

					cur_rec.left += sm_iLabelOffset;
					dc.DrawText(szItem, -1, &cur_rec, MLC_DT_TEXT);
					cur_rec.left -= iIconDrawWidth + iCheckboxDrawWidth + sm_iLabelOffset;
					break;
				}
				case 8:
					{

					}
					break;				
				default:
					if(szItem[0] != 0)
						dc.DrawText(szItem, -1, &cur_rec, MLC_DT_TEXT | uDrawTextAlignment);
					break;
			}
		}
		cur_rec.left += iColumnWidth;
		if(cur_rec.left>=rcClient.right)// X: [DDHC] - [Don't Draw Hidden Column]
			break;
	}
}

void CSharedFilesCtrl::GetItemDisplayText(const CRecordItem* record, int iSubItem, LPTSTR pszText, int cchTextMax) const
{
	if (pszText == NULL || cchTextMax <= 0) {
		ASSERT(0);
		return;
	}
	pszText[0] = _T('\0');
	switch (iSubItem)
	{
	case 0:
		_tcsncpy(pszText, record->Titlel.c_str(), cchTextMax);
		break;
	case 1:
		_tcsncpy(pszText, record->SID.c_str(), cchTextMax);
		break;
	case 2:
		_tcsncpy(pszText, record->KeyValues.c_str(), cchTextMax);
		break;
		
	case 3:
		_tcsncpy(pszText, CastItoXBytes((uint64)record->AttachSpaceSize, false, false), cchTextMax);
		break;
	case 4:
		_tcsncpy(pszText, record->ModTime.c_str(), cchTextMax);
		break;
	case 5:
		_tcsncpy(pszText, record->InputTime.c_str(), cchTextMax);
		break;

		////case 9:
		////	_tcsncpy(pszText, reinterpret_cast<const CShareableFile*>(file)->GetPath(), cchTextMax);
		////	pszText[cchTextMax - 1] = _T('\0');
		////	PathRemoveBackslash(pszText);
		////	break;
		default:
		////if (IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
		////{
		////	CKnownFile* pKnownFile = (CKnownFile*)file;
		////	switch (iSubItem)
		////	{			
		////		case 3:
		////			_tcsncpy(pszText, pKnownFile->GetUpPriorityDisplayString(), cchTextMax);
		////			break;
		////	
		////		case 4:
		////			md4str(pKnownFile->GetFileHash(), pszText);
		////			break;
		////	
		////		case 5:
		////			_sntprintf(pszText, cchTextMax, _T("%u (%u)"), pKnownFile->statistic.GetRequests(), pKnownFile->statistic.GetAllTimeRequests());
		////			break;
		////	
		////		case 6:
		////			_sntprintf(pszText, cchTextMax, _T("%u (%u)"), pKnownFile->statistic.GetAccepts(), pKnownFile->statistic.GetAllTimeAccepts());
		////			break;
		////	
		////		case 7:
		////			_sntprintf(pszText, cchTextMax, _T("%s (%s)"), CastItoXBytes(pKnownFile->statistic.GetTransferred(), false, false), CastItoXBytes(pKnownFile->statistic.GetAllTimeTransferred(), false, false));
		////			break;
		////	
		////		case 8:
		////			//_sntprintf(pszText, cchTextMax, _T("%s: %u"), GetResString(IDS_SHARED_STATUS), pKnownFile->GetPartCount());
		////			break;
		////	
		////		case 10:
		////			//Xman show virtual sources (morph) + virtualUploadsources
		////			if(pKnownFile->IsPartFile()==false || thePrefs.UseAdvancedAutoPtio()==false)
		////			{
		////				if (pKnownFile->m_nCompleteSourcesCountLo == pKnownFile->m_nCompleteSourcesCountHi)
		////					_sntprintf(pszText, cchTextMax, _T("%u (%u)"), pKnownFile->m_nCompleteSourcesCountLo, pKnownFile->m_nVirtualCompleteSourcesCount);
		////				else if (pKnownFile->m_nCompleteSourcesCountLo == 0)
		////					_sntprintf(pszText, cchTextMax, _T("< %u (%u)"), pKnownFile->m_nCompleteSourcesCountHi, pKnownFile->m_nVirtualCompleteSourcesCount);
		////				else
		////					_sntprintf(pszText, cchTextMax, _T("%u - %u (%u)"), pKnownFile->m_nCompleteSourcesCountLo, pKnownFile->m_nCompleteSourcesCountHi, pKnownFile->m_nVirtualCompleteSourcesCount);
		////			}
		////			else
		////			{
		////				//Xman advanced upload-priority
		////				if (pKnownFile->m_nCompleteSourcesCountLo == pKnownFile->m_nCompleteSourcesCountHi)
		////					_sntprintf(pszText, cchTextMax, _T("%u (%u/%u)"), pKnownFile->m_nCompleteSourcesCountLo, pKnownFile->m_nVirtualCompleteSourcesCount, pKnownFile->m_nVirtualUploadSources);
		////				else if (pKnownFile->m_nCompleteSourcesCountLo == 0)
		////					_sntprintf(pszText, cchTextMax, _T("< %u (%u/%u)"), pKnownFile->m_nCompleteSourcesCountHi, pKnownFile->m_nVirtualCompleteSourcesCount, pKnownFile->m_nVirtualUploadSources);
		////				else
		////					_sntprintf(pszText, cchTextMax, _T("%u - %u (%u/%u)"), pKnownFile->m_nCompleteSourcesCountLo, pKnownFile->m_nCompleteSourcesCountHi, pKnownFile->m_nVirtualCompleteSourcesCount, pKnownFile->m_nVirtualUploadSources);
		////				//Xman end
		////			}
		////			//Xman end
		////			break;
		////	
		////		case 11:
		////			//_sntprintf(pszText, cchTextMax, _T("%s|%s"), GetResString(pKnownFile->GetPublishedED2K() ? IDS_YES : IDS_NO), GetResString(IsSharedInKad(pKnownFile) ? IDS_YES : IDS_NO));
		////			break;
		////	
		////		case 12:
		////			_tcsncpy(pszText, pKnownFile->GetStrTagValue(FT_MEDIA_ARTIST), cchTextMax);
		////			break;
		////	
		////		case 13:
		////			_tcsncpy(pszText, pKnownFile->GetStrTagValue(FT_MEDIA_ALBUM), cchTextMax);
		////			break;
		////	
		////		case 14:
		////			_tcsncpy(pszText, pKnownFile->GetStrTagValue(FT_MEDIA_TITLE), cchTextMax);
		////			break;
		////	
		////		case 15:{
		////			uint_ptr nMediaLength = pKnownFile->GetIntTagValue(FT_MEDIA_LENGTH);
		////			if (nMediaLength){
		////				CString buffer;
		////				SecToTimeLength(nMediaLength, buffer);
		////				_tcsncpy(pszText, buffer, cchTextMax);
		////			}
		////			break;
		////		}
		////	
		////		case 16:{
		////			uint32 nBitrate = pKnownFile->GetIntTagValue(FT_MEDIA_BITRATE);
		////			if (nBitrate)
		////				_sntprintf(pszText, cchTextMax, _T("%u %s"), nBitrate, GetResString(IDS_KBITSSEC));
		////			break;
		////		}
		////	
		////		case 17:
		////			_tcsncpy(pszText, GetCodecDisplayName(pKnownFile->GetStrTagValue(FT_MEDIA_CODEC)), cchTextMax);
		////			break;

		////		//Xman see OnUploadqueue
		////		case 18:
		////			_ultot_s(pKnownFile->GetOnUploadqueue(), pszText, cchTextMax, 10);// X: [CI] - [Code Improvement]
		////			break;
		////		//Xman end

		////		//Xman advanced upload-priority
		////		case 19:
		////			if(thePrefs.UseAdvancedAutoPtio())
		////				_sntprintf(pszText, cchTextMax, _T("%.0f%% / %.0f%%"), pKnownFile->CalculateUploadPriorityPercent(), pKnownFile->statistic.GetAllTimeTransferred()/(float)pKnownFile->GetFileSize()*100);
		////			else
		////				_sntprintf(pszText, cchTextMax, _T("%.0f%%"), pKnownFile->statistic.GetAllTimeTransferred()/(float)pKnownFile->GetFileSize()*100);
		////			break;
		////		//Xman end
		////	}
		////}
		break;
	}

	pszText[cchTextMax - 1] = _T('\0');
}

void CSharedFilesCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
#ifdef _PUBLISH_
	return;
#endif // _PUBLISH_

	if (point.x != -1 || point.y != -1) {// X: [BF] - [Bug Fix]
		RECT rcClient;
		GetClientRect(&rcClient);
		ClientToScreen(&rcClient);
		if (!PtInRect(&rcClient,point)) {
			Default();
			return;
		}
	}
	// get merged settings
	bool bFirstItem = true;  //获取选中的那一行
	size_t iSelectedItems = GetSelectedCount();
	const CRecordItem* pSingleSelRecord = NULL;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		const CRecordItem* pRecord = (CRecordItem*)GetItemData(GetNextSelectedItem(pos));
		if (bFirstItem)
			pSingleSelRecord = pRecord;
		else
			pSingleSelRecord = NULL;

		bFirstItem = false;
	}
	m_RecordsMenu.EnableMenuItem(MP_ADD_RECORD,  MF_ENABLED );
	m_RecordsMenu.EnableMenuItem(MP_MOD_RECORD, iSelectedItems == 1 ?  MF_ENABLED : MF_GRAYED);
	m_RecordsMenu.EnableMenuItem(MP_DEL_RECORD, iSelectedItems > 0 ?  MF_ENABLED : MF_GRAYED);
	m_RecordsMenu.EnableMenuItem(MP_SHOW_RECORD, iSelectedItems > 0 ?  MF_ENABLED : MF_GRAYED);
	GetPopupMenuPos(*this, point);
	m_RecordsMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);

	// get merged settings
	//bool bFirstItem = true;
	//bool bContainsShareableFiles = false;
	//bool bContainsOnlyShareableFile = true;
	//bool bContainsUnshareableFile = false;
	//size_t iSelectedItems = GetSelectedCount();
	//sint_ptr iCompleteFileSelected = -1;
	//UINT uPrioMenuItem = 0;
	//const CShareableFile* pSingleSelFile = NULL;
	//POSITION pos = GetFirstSelectedItemPosition();
	//while (pos)
	//{
	//	const CShareableFile* pFile = (CShareableFile*)GetItemData(GetNextSelectedItem(pos));
	//	if (bFirstItem)
	//		pSingleSelFile = pFile;
	//	else
	//		pSingleSelFile = NULL;

	//	////sint_ptr iCurCompleteFile = pFile->IsPartFile() ? 0 : 1;
	//	////if (bFirstItem)
	//	////	iCompleteFileSelected = iCurCompleteFile;
	//	////else if (iCompleteFileSelected != iCurCompleteFile)
	//	////	iCompleteFileSelected = -1;

	//	////bContainsUnshareableFile = !pFile->IsShellLinked() && !pFile->IsPartFile() && (bContainsUnshareableFile || (theApp.sharedfiles->ShouldBeShared(pFile->GetSharedDirectory(), pFile->GetFilePath(), false)
	//	////	&& !theApp.sharedfiles->ShouldBeShared(pFile->GetSharedDirectory(), _T("")/*pFile->GetFilePath()*/, true)));

	//	////if (IsKindOfCKnownFile(pFile)/*pFile->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//	////{
	//	////	bContainsOnlyShareableFile = false;

	//	////	UINT uCurPrioMenuItem = 0;
	//	////	if (((CKnownFile*)pFile)->IsAutoUpPriority())
	//	////		uCurPrioMenuItem = MP_PRIOAUTO;
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority() == PR_VERYLOW)
	//	////		uCurPrioMenuItem = MP_PRIOVERYLOW;
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority() == PR_LOW)
	//	////		uCurPrioMenuItem = MP_PRIOLOW;
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority() == PR_NORMAL)
	//	////		uCurPrioMenuItem = MP_PRIONORMAL;
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority() == PR_HIGH)
	//	////		uCurPrioMenuItem = MP_PRIOHIGH;
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority() == PR_VERYHIGH)
	//	////		uCurPrioMenuItem = MP_PRIOVERYHIGH;
	//	////	//Xman PowerRelease
	//	////	else if (((CKnownFile*)pFile)->GetUpPriority()==PR_POWER)
	//	////		uCurPrioMenuItem = MP_PRIOPOWER;
	//	////	//Xman end
	//	////	else
	//	////		ASSERT(0);

	//	////	if (bFirstItem)
	//	////		uPrioMenuItem = uCurPrioMenuItem;
	//	////	else if (uPrioMenuItem != uCurPrioMenuItem)
	//	////		uPrioMenuItem = 0;
	//	////}
	//	////else
	//	////	bContainsShareableFiles = true;

	//	bFirstItem = false;
	//}

	//m_SharedFilesMenu.EnableMenuItem((UINT)m_PrioMenu.m_hMenu, (!bContainsOnlyShareableFile && iSelectedItems > 0) ? MF_ENABLED : MF_GRAYED);
	////m_PrioMenu.CheckMenuRadioItem(MP_PRIOVERYLOW, MP_PRIOAUTO, uPrioMenuItem, 0);
	//m_PrioMenu.CheckMenuRadioItem(MP_PRIOVERYLOW, MP_PRIOPOWER, uPrioMenuItem, 0); //Xman PowerRelease 

	//bool bSingleCompleteFileSelected = (iSelectedItems == 1 && (iCompleteFileSelected == 1 || bContainsOnlyShareableFile));
	//m_SharedFilesMenu.EnableMenuItem(MP_SAVE_AS, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);
	//UINT uInsertedMenuItem = 0;
	//static const TCHAR _szSkinPkgSuffix1[] = _T(".") EMULSKIN_BASEEXT _T(".zip");
	//static const TCHAR _szSkinPkgSuffix2[] = _T(".") EMULSKIN_BASEEXT _T(".rar");
	//if (bSingleCompleteFileSelected 
	//	&& pSingleSelFile 
	//	&& (   pSingleSelFile->GetFilePath().Right(ARRSIZE(_szSkinPkgSuffix1)-1).CompareNoCase(_szSkinPkgSuffix1) == 0
	//		|| pSingleSelFile->GetFilePath().Right(ARRSIZE(_szSkinPkgSuffix2)-1).CompareNoCase(_szSkinPkgSuffix2) == 0))
	//{
	//	MENUITEMINFO mii = {0};
	//	mii.cbSize = sizeof mii;
	//	mii.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID;
	//	mii.fType = MFT_STRING;
	//	mii.fState = MFS_ENABLED;
	//	mii.wID = MP_INSTALL_SKIN;
	//	////CString strBuff(GetResString(IDS_INSTALL_SKIN));
	//	////mii.dwTypeData = const_cast<LPTSTR>((LPCTSTR)strBuff);
	//	if (::InsertMenuItem(m_SharedFilesMenu, MP_SHOW_ALTER_VERSION, FALSE, &mii))
	//		uInsertedMenuItem = mii.wID;
	//}
	////Xman PowerRelease
	//m_SharedFilesMenu.EnableMenuItem(MP_PRIOPOWER, (!bContainsShareableFiles && iCompleteFileSelected >0) ? MF_ENABLED : MF_GRAYED);
	////Xman end
	//m_SharedFilesMenu.EnableMenuItem(MP_SHOW_ALTER_VERSION, bSingleCompleteFileSelected ? MF_ENABLED : MF_GRAYED);
	//m_SharedFilesMenu.EnableMenuItem(MP_SHOW_COMMENT, (!bContainsShareableFiles && bSingleCompleteFileSelected) ? MF_ENABLED : MF_GRAYED);
	//m_SharedFilesMenu.EnableMenuItem(MP_REMOVE, iCompleteFileSelected > 0 ? MF_ENABLED : MF_GRAYED);
	//m_SharedFilesMenu.EnableMenuItem(MP_UNSHAREFILE, bContainsUnshareableFile ? MF_ENABLED : MF_GRAYED);
	//m_SharedFilesMenu.SetDefaultItem(bSingleCompleteFileSelected ? MP_SAVE_AS : -1);
	//m_SharedFilesMenu.EnableMenuItem(MP_CMT, (!bContainsShareableFiles && iSelectedItems > 0) ? MF_ENABLED : MF_GRAYED);
	//m_SharedFilesMenu.EnableMenuItem(MP_DETAIL, iSelectedItems > 0 ? MF_ENABLED : MF_GRAYED);
	////m_SharedFilesMenu.EnableMenuItem(thePrefs.GetShowCopyEd2kLinkCmd() ? MP_GETED2KLINK : MP_SHOWED2KLINK, (!bContainsOnlyShareableFile && iSelectedItems > 0) ? MF_ENABLED : MF_GRAYED);
	////m_SharedFilesMenu.EnableMenuItem(MP_SEARCHRELATED, theApp.emuledlg->searchwnd->CanSearchRelatedFiles() ? MF_ENABLED : MF_GRAYED);

	//// Xman: IcEcRacKer Copy UL-feedback
	//m_SharedFilesMenu.EnableMenuItem(MP_ULFEEDBACK, (!bContainsOnlyShareableFile && iSelectedItems > 0) ? MF_ENABLED : MF_GRAYED);
	////Xman end

	//m_SharedFilesMenu.EnableMenuItem(MP_FIND, GetItemCount() > 0 ? MF_ENABLED : MF_GRAYED);

	////Xman Mass Rename (Morph)
	//m_SharedFilesMenu.EnableMenuItem(MP_MASSRENAME, (!bContainsOnlyShareableFile && iSelectedItems > 0) ? MF_ENABLED : MF_GRAYED);
	////Xman end

	//m_CollectionsMenu.EnableMenuItem(MP_MODIFYCOLLECTION, (!bContainsShareableFiles && pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL ) ? MF_ENABLED : MF_GRAYED);
	//m_CollectionsMenu.EnableMenuItem(MP_VIEWCOLLECTION, (!bContainsShareableFiles && pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL ) ? MF_ENABLED : MF_GRAYED);
//	m_CollectionsMenu.EnableMenuItem(MP_SEARCHAUTHOR, (!bContainsShareableFiles && pSingleSelFile != NULL && ((CKnownFile*)pSingleSelFile)->m_pCollection != NULL 
//		&& !((CKnownFile*)pSingleSelFile)->m_pCollection->GetAuthorKeyHashString().IsEmpty()) ? MF_ENABLED : MF_GRAYED);
//#if defined(_DEBUG)
//	if (thePrefs.IsExtControlsEnabled()){
//	//JOHNTODO: Not for release as we need kad lowID users in the network to see how well this work work. Also, we do not support these links yet.
//		m_SharedFilesMenu.EnableMenuItem(MP_GETKADSOURCELINK, (iSelectedItems > 0 && theApp.IsConnected() && theApp.IsFirewalled() && theApp.clientlist->GetBuddy()) ? MF_ENABLED : MF_GRAYED);
//	}
//#endif
//	m_SharedFilesMenu.EnableMenuItem(Irc_SetSendLink, (!bContainsOnlyShareableFile && iSelectedItems == 1 && theApp.emuledlg->ircwnd->IsConnected()) ? MF_ENABLED : MF_GRAYED);
//
	////CTitleMenu WebMenu;
	////WebMenu.CreateMenu();
	////WebMenu.AddMenuTitle(NULL, true);
	////size_t iWebMenuEntries = theWebServices.GetFileMenuEntries(&WebMenu);
	////UINT flag2 = (iWebMenuEntries == 0 || iSelectedItems != 1) ? MF_GRAYED : MF_STRING;
	////m_SharedFilesMenu.AppendMenu(flag2 | MF_POPUP, (UINT_PTR)WebMenu.m_hMenu, GetResString(IDS_WEBSERVICES), _T("WEB"));

	////GetPopupMenuPos(*this, point);
	////m_SharedFilesMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,point.x,point.y,this);

	////m_SharedFilesMenu.RemoveMenu(m_SharedFilesMenu.GetMenuItemCount()-1,MF_BYPOSITION);
	////VERIFY( WebMenu.DestroyMenu() );
	////if (uInsertedMenuItem)
	////	VERIFY( m_SharedFilesMenu.RemoveMenu(uInsertedMenuItem, MF_BYCOMMAND) );
}

BOOL CSharedFilesCtrl::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	wParam = LOWORD(wParam);
	vector<int64> vecSelectList;//删除多条用这个
	CAtlList<CRecordItem*> selectedList;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int index = GetNextSelectedItem(pos);
		if (index >= 0)
		{
			CRecordItem* record = NULL;
			record = (CRecordItem*)GetItemData(index);
			selectedList.AddTail(record);
			vecSelectList.push_back(record->ID);
		}
	}

	if (   wParam == MP_ADD_RECORD
		//|| wParam == MP_FIND ||  wParam == MP_CREATECOLLECTION
		|| selectedList.GetCount() > 0)
	{
		CRecordItem* record = NULL;
		if (selectedList.GetCount() == 1)
			record = selectedList.GetHead();

		switch (wParam)
		{
			case MP_ADD_RECORD:
				{
					//MessageBox(L"添加记录11111");
					//theApp.wyytdlg->ShowRecPPDlg(ECOO_ADD);					
					//ShowRecPPDlg(ECOO_ADD);
					//CString cST;
					//int nSize = 1125899906842624;
					//cST.Format(L"%d",nSize);
					//int64 nSize64 = 1125899906842624;
					//cST.Format(L"%lld",nSize64);
					ShowRecDetailsDlg_ADD();
					ReloadRecordList();
					//CRecordDetailsClassDlg dlg;
					//dlg.DoModal();
				}
				break;
			case MP_MOD_RECORD:
				{					
					if (record )
					{
						//MessageBox(L"修改记录");
						//theApp.wyytdlg->ShowRecPPDlg(ECOO_MOD);
						//ShowRecPPDlg(ECOO_MOD);
						ShowRecDetailsDlg_MOD(record);
					}
				}				
				break; 
			case IDA_ENTER:
			case MP_SHOW_COMMENT:
			case MPG_F2:
			case MP_SHOW_RECORD:
				{
					ShowFileDialog(selectedList);
					//AfxMessageBox(L"查看记录");
					//MessageBox(L"查看记录");
					//if (pKnownFile && !pKnownFile->IsPartFile()){
					//	InputBox inputbox;
					//	CString title = GetResString(IDS_RENAME);
					//	title.Remove(_T('&'));
					//	inputbox.SetLabels(title, GetResString(IDS_DL_FILENAME), pKnownFile->GetFileName());
					//	inputbox.SetEditFilenameMode();
					//	inputbox.DoModal();
					//	CString newname = inputbox.GetInput();
					//	if (!inputbox.WasCancelled() && newname.GetLength()>0)
					//	{
					//		// at least prevent users from specifying something like "..\dir\file"
					//		static const TCHAR _szInvFileNameChars[] = _T("\\/:*?\"<>|");
					//		if (newname.FindOneOf(_szInvFileNameChars) != -1){
					//			AfxMessageBox(GetErrorMessage(ERROR_BAD_PATHNAME));
					//			break;
					//		}

					//		CString newpath;
					//		PathCombine(newpath.GetBuffer(MAX_PATH), file->GetPath(), newname);
					//		newpath.ReleaseBuffer();
					//		if (_trename(pKnownFile->GetFilePath(), newpath) != 0){
					//			CString strError;
					//			strError.Format(GetResString(IDS_ERR_RENAMESF), file->GetFilePath(), newpath, _tcserror(errno));
					//			AfxMessageBox(strError);
					//			break;
					//		}

					//		if (IsCPartFile(pKnownFile)/*pKnownFile->IsKindOf(RUNTIME_CLASS(CPartFile))*/)
					//		{
					//			pKnownFile->SetFileName(newname);
					//			//STATIC_DOWNCAST(CPartFile, pKnownFile)->SetFullName(newpath); 
					//			((CPartFile*)pKnownFile)->SetFullName(newpath); 
					//		}
					//		else
					//		{
					//			//theApp.sharedfiles->RemoveKeywords(pKnownFile);// X: [BF] - [Bug Fix] Don't need to remove & add
					//			pKnownFile->SetFileName(newname);
					//			//theApp.sharedfiles->AddKeywords(pKnownFile);
					//		}
					//		pKnownFile->SetFilePath(newpath);
					//		UpdateFile(pKnownFile);
					//	}
					//}
					//else
					//	MessageBeep(MB_OK);
				}
				
				break;
			case MP_REMOVE:
			case MPG_DELETE:
			case MP_DEL_RECORD:
			{
				//MessageBox(L"删除记录");
				if (IDNO == AfxMessageBox(GetResString(IDS_CONFIRM_RECORDDELETE),MB_ICONWARNING | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_YESNO))
					return TRUE;
				//if ( vecSelectList.size() <= 1 ) //删除一条的方案  全部使用下一条方案，否则删除后内容加载不全(不安选中的分类信息加载)
				//{
				//	SetRedraw(FALSE);
				//	bool bRemovedItems = false;
				//	while (!selectedList.IsEmpty())
				//	{
				//		CRecordItem* myrecord = selectedList.RemoveHead();
				//		if (!myrecord )
				//			continue;
				//		RecordDelete(myrecord);
				//		bRemovedItems = true;
				//	}
				//	SetRedraw(TRUE);
				//}
				//else //删除多条时的方案，上面方案会造成列表中不能全部删除
				if ( vecSelectList.size() >= 1 )
				{
					RecordDelete(vecSelectList);
					ReloadRecordList();
				}

				
				//if (bRemovedItems) {
				//	AutoSelectItem();
				//	// Depending on <no-idea> this does not always cause a
				//	// LVN_ITEMACTIVATE message sent. So, explicitly redraw
				//	// the item.
				//	theApp.emuledlg->sharedfileswnd->ShowSelectedFilesDetails();
				//	theApp.emuledlg->sharedfileswnd->OnSingleFileShareStatusChanged(); // might have been a single shared file
				//}
				break; 
			}
			default:
				break;
		}
	}
	return TRUE;
}

void CSharedFilesCtrl::OnLvnColumnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW *pNMListView = (NMLISTVIEW *)pNMHDR;
	bool sortAscending;
	if (GetSortItem() != pNMListView->iSubItem)
	{
		switch (pNMListView->iSubItem)
		{
			case 3:  // Priority
			case 10: // Complete Sources
			case 11: // Shared
				//sortAscending = false;
				//break;
			case 5:  // Requests
			case 6:  // Accepted Requests
			case 7:  // Transferred Data
				// Keep the current 'm_aSortBySecondValue' for that column, but reset to 'descending'
				sortAscending = false;
				break;
			default:
				sortAscending = true;
				break;
		}
	}
	else
		sortAscending = !GetSortAscending();

	// Ornis 4-way-sorting
	int adder = 0;
	//if (pNMListView->iSubItem >= 5 && pNMListView->iSubItem <= 7) // 5=IDS_SF_REQUESTS, 6=IDS_SF_ACCEPTS, 7=IDS_SF_TRANSFERRED
	//{
	//	ASSERT( pNMListView->iSubItem - 5 < _countof(m_aSortBySecondValue) );
	//	if (GetSortItem() == pNMListView->iSubItem && !sortAscending) // check for 'descending' because the initial sort order is also 'descending'
	//		m_aSortBySecondValue[pNMListView->iSubItem - 5] = !m_aSortBySecondValue[pNMListView->iSubItem - 5];
	//	adder = m_aSortBySecondValue[pNMListView->iSubItem - 5] ? 100 : 0;
	//}
	//else if (pNMListView->iSubItem == 11) // 11=IDS_SHAREDTITLE
	//{
	//	ASSERT( 3 < _countof(m_aSortBySecondValue) );
	//	if (GetSortItem() == pNMListView->iSubItem && !sortAscending) // check for 'descending' because the initial sort order is also 'descending'
	//		m_aSortBySecondValue[3] = !m_aSortBySecondValue[3];
	//	adder = m_aSortBySecondValue[3] ? 100 : 0;
	//}

	// Sort table
	if (adder == 0)
		SetSortArrow(pNMListView->iSubItem, sortAscending); //单向箭头
	else
		SetSortArrow(pNMListView->iSubItem, sortAscending ? arrowDoubleUp : arrowDoubleDown);//双向箭头
	UpdateSortHistory(pNMListView->iSubItem + (sortAscending ? 0 : 20) + adder, 20);
	SortItems(SortProc, pNMListView->iSubItem + (sortAscending ? 0 : 20) + adder);

	*pResult = 0;
}

int CSharedFilesCtrl::SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	const CRecordItem* item1 = (CRecordItem*)lParam1;
	const CRecordItem* item2 = (CRecordItem*)lParam2;

	bool bSortAscending;
	LPARAM iColumn;
	if (lParamSort >= 100) //双向箭头
	{
		bSortAscending = lParamSort < 120;
		iColumn = bSortAscending ? lParamSort : lParamSort - 20;
	}
	else//单向箭头
	{
		bSortAscending = lParamSort < 20;
		iColumn = bSortAscending ? lParamSort : lParamSort - 20;
	}
	
	int iResult = 0;
	bool bExtColumn = false;
	switch (iColumn)
	{
		case 0: //title
			iResult = CompareLocaleStringNoCase(item1->Titlel.c_str(), item2->Titlel.c_str());
			break;
		case 1: //编号
			iResult = CompareLocaleStringNoCase(item1->SID.c_str(), item2->SID.c_str());
			break;
		case 2: //摘要
			iResult = CompareLocaleStringNoCase(item1->KeyValues.c_str(), item2->KeyValues.c_str());
			break;
		case 3: //filesize
			iResult = CompareUnsigned64(item1->AttachSpaceSize, item2->AttachSpaceSize);
			break;
		case 4: //修改时间
			iResult = CompareUnsigned64(DETimeValue(item1->ModTime).sec(), DETimeValue(item2->ModTime).sec());			
			break;
		case 5: //添加时间
			iResult = CompareUnsigned64(DETimeValue(item1->InputTime).sec(), DETimeValue(item2->InputTime).sec());			
			break;

		//case 2: //filetype
		//	iResult = item1->GetFileTypeDisplayStr().Compare(item2->GetFileTypeDisplayStr());
		//	// if the type is equal, subsort by extension
		//	if (iResult == 0)
		//	{
		//		LPCTSTR pszExt1 = PathFindExtension(item1->GetFileName());
		//		LPCTSTR pszExt2 = PathFindExtension(item2->GetFileName());
		//		if ((pszExt1 == NULL) ^ (pszExt2 == NULL))
		//			iResult = pszExt1 == NULL ? 1 : (-1);
		//		else
		//			iResult = pszExt1 != NULL ? _tcsicmp(pszExt1, pszExt2) : 0;
		//	}
		//	break;

		//case 9: //folder
		//	iResult = CompareLocaleStringNoCase(item1->GetPath(), item2->GetPath());
		//	break;
		default:
			bExtColumn = true;
			break;
	}

	//if (bExtColumn)
	//{
	//	if (IsKindOfCKnownFile(item1) && !IsKindOfCKnownFile(item2)/*item1->IsKindOf(RUNTIME_CLASS(CKnownFile)) && !item2->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//		iResult = (-1);
	//	else if (!IsKindOfCKnownFile(item1) && IsKindOfCKnownFile(item2)/*!item1->IsKindOf(RUNTIME_CLASS(CKnownFile)) && item2->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//		iResult = 1;
	//	else if (IsKindOfCKnownFile(item1) && IsKindOfCKnownFile(item2)/*item1->IsKindOf(RUNTIME_CLASS(CKnownFile)) && item2->IsKindOf(RUNTIME_CLASS(CKnownFile))*/)
	//	{
	//		CKnownFile* kitem1 = (CKnownFile*)item1;
	//		CKnownFile* kitem2 = (CKnownFile*)item2;

	//		switch (iColumn)
	//		{
	//			case 3:{//prio
	//				uint8 p1 = kitem1->GetUpPriority() + 1;
	//				if (p1 == 5)
	//					p1 = 0;
	//				uint8 p2 = kitem2->GetUpPriority() + 1;
	//				if (p2 == 5)
	//					p2 = 0;
	//				iResult = p1 - p2;
	//				break;
	//			}

	//			case 4: //fileID
	//				iResult = memcmp(kitem1->GetFileHash(), kitem2->GetFileHash(), 16);
	//				break;

	//			case 5: //requests
	//				iResult = CompareUnsigned(kitem1->statistic.GetRequests(), kitem2->statistic.GetRequests());
	//				break;
	//			
	//			case 6: //acc requests
	//				iResult = CompareUnsigned(kitem1->statistic.GetAccepts(), kitem2->statistic.GetAccepts());
	//				break;
	//			
	//			case 7: //all transferred
	//				iResult = CompareUnsigned64(kitem1->statistic.GetTransferred(), kitem2->statistic.GetTransferred());
	//				break;

	//			case 10: //complete sources
	//				iResult = CompareUnsigned(kitem1->m_nCompleteSourcesCount, kitem2->m_nCompleteSourcesCount);
	//				break;

	//			case 11: //ed2k shared
	//				iResult = kitem1->GetPublishedED2K() - kitem2->GetPublishedED2K();
	//				break;

	//			case 12:
	//				iResult = CompareOptLocaleStringNoCaseUndefinedAtBottom(kitem1->GetStrTagValue(FT_MEDIA_ARTIST), kitem2->GetStrTagValue(FT_MEDIA_ARTIST), bSortAscending);
	//				break;
	//		
	//			case 13:
	//				iResult = CompareOptLocaleStringNoCaseUndefinedAtBottom(kitem1->GetStrTagValue(FT_MEDIA_ALBUM), kitem2->GetStrTagValue(FT_MEDIA_ALBUM), bSortAscending);
	//				break;

	//			case 14:
	//				iResult = CompareOptLocaleStringNoCaseUndefinedAtBottom(kitem1->GetStrTagValue(FT_MEDIA_TITLE), kitem2->GetStrTagValue(FT_MEDIA_TITLE), bSortAscending);
	//				break;

	//			case 15:
	//				iResult = CompareUnsignedUndefinedAtBottom(kitem1->GetIntTagValue(FT_MEDIA_LENGTH), kitem2->GetIntTagValue(FT_MEDIA_LENGTH), bSortAscending);
	//				break;

	//			case 16:
	//				iResult = CompareUnsignedUndefinedAtBottom(kitem1->GetIntTagValue(FT_MEDIA_BITRATE), kitem2->GetIntTagValue(FT_MEDIA_BITRATE), bSortAscending);
	//				break;

	//			case 17:
	//				iResult = CompareOptLocaleStringNoCaseUndefinedAtBottom(GetCodecDisplayName(kitem1->GetStrTagValue(FT_MEDIA_CODEC)), GetCodecDisplayName(kitem2->GetStrTagValue(FT_MEDIA_CODEC)), bSortAscending);
	//				break;
	//			//Xman see OnUploadqueue
	//			case 18:
	//				iResult = CompareUnsigned(kitem1->GetOnUploadqueue(), kitem2->GetOnUploadqueue());
	//				break;
	//			//Xman end

	//			//Xman advanced upload-priority
	//			//sort by the second value-> faster
	//			case 19:
	//				{
	//					float it1value = kitem1->statistic.GetAllTimeTransferred()/(float)kitem1->GetFileSize()*1000; //sort one number after ,
	//					float it2value = kitem2->statistic.GetAllTimeTransferred()/(float)kitem2->GetFileSize()*1000;
	//					iResult=CompareFloat(it1value, it2value);
	//					break;
	//				}
	//			//Xman end

	//			case 105: //all requests
	//				iResult = CompareUnsigned(kitem1->statistic.GetAllTimeRequests(), kitem2->statistic.GetAllTimeRequests());
	//				break;

	//			case 106: //all acc requests
	//				iResult = CompareUnsigned(kitem1->statistic.GetAllTimeAccepts(), kitem2->statistic.GetAllTimeAccepts());
	//				break;

	//			case 107: //all transferred
	//				iResult = CompareUnsigned64(kitem1->statistic.GetAllTimeTransferred(), kitem2->statistic.GetAllTimeTransferred());
	//				break;

	//			case 111:{ //kad shared
	//				uint64 tNow = time(NULL);
	//				int i1 = (tNow < kitem1->GetLastPublishTimeKadSrc()) ? 1 : 0;
	//				int i2 = (tNow < kitem2->GetLastPublishTimeKadSrc()) ? 1 : 0;
	//				iResult = i1 - i2;
	//				break;
	//			}
	//		}
	//	}
	//}

	if (!bSortAscending)
		return -iResult;
	//// SLUGFILLER: multiSort remove - handled in parent class
	///*
	////call secondary sortorder, if this one results in equal
	//int dwNextSort;
	//if (iResult == 0 && (dwNextSort = theApp.emuledlg->sharedfileswnd->sharedfilesctrl.GetNextSortOrder(lParamSort)) != (-1)){
	//	iResult= SortProc(lParam1, lParam2, dwNextSort);
	//}
	//*/
	return iResult;
}

void CSharedFilesCtrl::OpenFile(const CShareableFile* file)
{
	//if(IsKindOfCKnownFile(file)/*file->IsKindOf(RUNTIME_CLASS(CKnownFile))*/ && ((CKnownFile*)file)->m_pCollection)
	//{
	//	// NEO: MLD - [ModelesDialogs] -- Xanatos -->
	//	CCollectionViewDialog* dialog = new CCollectionViewDialog(); 
	//	dialog->SetCollection(((CKnownFile*)file)->m_pCollection);
	//	dialog->OpenDialog();
	//	// NEO: MLD END <-- Xanatos --
	//	//CCollectionViewDialog dialog;
	//	//dialog.SetCollection(((CKnownFile*)file)->m_pCollection);
	//	//dialog.DoModal();
	//}
	//else
	//	ShellOpenFile(file->GetFilePath(), NULL);
}

void CSharedFilesCtrl::OnNmDblClk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int iSel = GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
	if (iSel != -1)
	{
		//MessageBox(L"查看记录");
		CRecordItem* record = (CRecordItem*)GetItemData(iSel);
		if (record)
		{
			//if (GetKeyState(VK_MENU) & 0x8000)
			{
				CAtlList<CRecordItem*> aFiles;
				aFiles.AddHead(record);
				ShowFileDialog(aFiles);
			}
			/*else if (!record->IsPartFile())
				OpenFile(record);*/
		}
	}
	//if (iSel != -1)
	//{
	//	CShareableFile* file = (CShareableFile*)GetItemData(iSel);
	//	if (file)
	//	{
	//		if (GetKeyState(VK_MENU) & 0x8000)
	//		{
	//			CAtlList<CShareableFile*> aFiles;
	//			aFiles.AddHead(file);
	//			ShowFileDialog(aFiles);
	//		}
	//		else if (!file->IsPartFile())
	//			OpenFile(file);
	//	}
	//}
	*pResult = 0;
}

void CSharedFilesCtrl::CreateMenues()
{

	//if (m_PrioMenu) VERIFY( m_PrioMenu.DestroyMenu() );
	//if (m_CollectionsMenu) VERIFY( m_CollectionsMenu.DestroyMenu() );
	//if (m_SharedFilesMenu) VERIFY( m_SharedFilesMenu.DestroyMenu() );
	if (m_RecordsMenu) VERIFY( m_RecordsMenu.DestroyMenu() );
	
	m_RecordsMenu.CreatePopupMenu();
	m_RecordsMenu.AddMenuTitle(GetResString(IDS_SYS_RESOURCES), true);

	m_RecordsMenu.AppendMenu(MF_STRING,MP_ADD_RECORD, GetResString(IDS_ADDRECORD), _T("ADDRECORD"));
	m_RecordsMenu.AppendMenu(MF_STRING,MP_MOD_RECORD, GetResString(IDS_MODRECORD), _T("MODRECORD"));
	m_RecordsMenu.AppendMenu(MF_STRING,MP_DEL_RECORD, GetResString(IDS_DELRECORD), _T("DELRECORD"));
	m_RecordsMenu.AppendMenu(MF_STRING,MP_SHOW_RECORD, GetResString(IDS_SHOWRECORD), _T("SHOWRECORD"));

	//m_PrioMenu.CreateMenu();
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOVERYLOW,GetResString(IDS_PRIOVERYLOW));
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOLOW,GetResString(IDS_PRIOLOW));
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIONORMAL,GetResString(IDS_PRIONORMAL));
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOHIGH, GetResString(IDS_PRIOHIGH));
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOVERYHIGH, GetResString(IDS_PRIORELEASE));
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOPOWER, GetResString(IDS_POWERRELEASE)); //Xman PowerRelease
	//m_PrioMenu.AppendMenu(MF_STRING,MP_PRIOAUTO, GetResString(IDS_PRIOAUTO));//UAP

	/*m_CollectionsMenu.CreateMenu();
	m_CollectionsMenu.AddMenuTitle(NULL, true);
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_CREATECOLLECTION, GetResString(IDS_CREATECOLLECTION), _T("COLLECTION_ADD"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_MODIFYCOLLECTION, GetResString(IDS_MODIFYCOLLECTION), _T("COLLECTION_EDIT"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_VIEWCOLLECTION, GetResString(IDS_VIEWCOLLECTION), _T("COLLECTION_VIEW"));
	m_CollectionsMenu.AppendMenu(MF_STRING,MP_SEARCHAUTHOR, GetResString(IDS_SEARCHAUTHORCOLLECTION), _T("COLLECTION_SEARCH"));*/

	//m_SharedFilesMenu.CreatePopupMenu();
	//m_SharedFilesMenu.AddMenuTitle(GetResString(IDS_SYS_RESOURCES), true);

	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_SAVE_AS, GetResString(IDS_SAVE_AS), _T("SAVE_AS")); //SAVE_AS 图标名称
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_SHOW_ALTER_VERSION, GetResString(IDS_SHOW_ALTER_VERSION), _T("ALTER_VERSION"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_SHOW_COMMENT, GetResString(IDS_SHOW_COMMENT), _T("COMMENT"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_REMOVE, GetResString(IDS_DELETE), _T("DELETE"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_UNSHAREFILE, GetResString(IDS_UNSHARE), _T("KADBOOTSTRAP")); // TODO: better icon
	//if (thePrefs.IsExtControlsEnabled())
	//	m_SharedFilesMenu.AppendMenu(MF_STRING,Irc_SetSendLink,GetResString(IDS_IRC_ADDLINKTOIRC), _T("IRCCLIPBOARD"));

	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);
	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)m_PrioMenu.m_hMenu, GetResString(IDS_PRIORITY) + _T(" (") + GetResString(IDS_PW_CON_UPLBL) + _T(')'), _T("FILEPRIORITY"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);

	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)m_CollectionsMenu.m_hMenu, GetResString(IDS_META_COLLECTION), _T("AABCollectionFileType"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR); 	

	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_DETAIL, GetResString(IDS_SHOWDETAILS), _T("FILEINFO"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_CMT, GetResString(IDS_CMT_ADD), _T("FILECOMMENTS"));
	//if (thePrefs.GetShowCopyEd2kLinkCmd())
	//	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_GETED2KLINK, GetResString(IDS_DL_LINK1), _T("ED2KLINK") );
	//else
	//	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_SHOWED2KLINK, GetResString(IDS_DL_SHOWED2KLINK), _T("ED2KLINK") );
	//m_SharedFilesMenu.AppendMenu(MF_STRING, MP_SEARCHRELATED, GetResString(IDS_SEARCHRELATED), _T("KadFileSearch"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_FIND, GetResString(IDS_FIND), _T("Search"));
	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);

	//// Xman: IcEcRacKer Copy UL-feedback
	//m_SharedFilesMenu.AppendMenu(MF_STRING,MP_ULFEEDBACK, _T("Copy UL-Feedback"), _T("FILECOMMENTS")); 
	//m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR); 
	////Xman end

	////Xman Mass Rename (Morph)
	//if (thePrefs.IsExtControlsEnabled())
	//{
	//	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_MASSRENAME,GetResString(IDS_MR), _T("FILEMASSRENAME"));
	//	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR); 
	//}
	////Xman end

#if defined(_DEBUG)
	//if (thePrefs.IsExtControlsEnabled()){
	//	//JOHNTODO: Not for release as we need kad lowID users in the network to see how well this work work. Also, we do not support these links yet.
	//	m_SharedFilesMenu.AppendMenu(MF_STRING,MP_GETKADSOURCELINK, _T("Copy eD2K Links To Clipboard (Kad)"));
	//	m_SharedFilesMenu.AppendMenu(MF_STRING|MF_SEPARATOR);
	//}
#endif
}

void CSharedFilesCtrl::ShowSelectedFileComments()
{
	POINT point;
	::GetCursorPos(&point);
	CPoint p = point; 
	ScreenToClient(&p); 
	int it = HitTest(p); 
	if (it == -1)
		return;

	SetItemState(-1, 0, LVIS_SELECTED);
	SetItemState(it, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	SetSelectionMark(it);   // display selection mark correctly! 

	CShareableFile* file=(CShareableFile*)GetItemData(GetSelectionMark());
	if (file)
	{
		CAtlList<CShareableFile*> aFiles;
		aFiles.AddHead(file);
//		ShowFileDialog(aFiles, IDD_COMMENT);
	}
}

void CSharedFilesCtrl::OnLvnGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (CwyytDlg::IsRunning()) {
		// Although we have an owner drawn listview control we store the text for the primary item in the listview, to be
		// capable of quick searching those items via the keyboard. Because our listview items may change their contents,
		// we do this via a text callback function. The listview control will send us the LVN_DISPINFO notification if
		// it needs to know the contents of the primary item.
		//
		// But, the listview control sends this notification all the time, even if we do not search for an item. At least
		// this notification is only sent for the visible items and not for all items in the list. Though, because this
		// function is invoked *very* often, do *NOT* put any time consuming code in here.
		//
		// Vista: That callback is used to get the strings for the label tips for the sub(!) items.
		//
		NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
		if (pDispInfo->item.mask & LVIF_TEXT) 
		{
			const CRecordItem* pRecItem = reinterpret_cast<CRecordItem*>(pDispInfo->item.lParam);
			if (pRecItem != NULL)
				GetItemDisplayText(pRecItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText, pDispInfo->item.cchTextMax);
		}
	}
	*pResult = 0;
}

void CSharedFilesCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'C' && (GetKeyState(VK_CONTROL) & 0x8000))
	{
		// Ctrl+C: Copy listview items to clipboard
		SendMessage(WM_COMMAND, MP_GETED2KLINK);
		return;
	}
	else if (nChar == VK_F5)
		ReloadFileList();
	else if (nChar == VK_SPACE && CheckBoxesEnabled())
	{
		// Toggle Checkboxes
		// selection and item position might change during processing (shouldn't though, but lets make sure), so first get all pointers instead using the selection pos directly
		SetRedraw(FALSE);
		CAtlList<CShareableFile*> selectedList;
		POSITION pos = GetFirstSelectedItemPosition();
		while (pos != NULL){
			int index = GetNextSelectedItem(pos);
			if (index >= 0)
				selectedList.AddTail((CShareableFile*)GetItemData(index));
		}
		while (!selectedList.IsEmpty())
		{
			CheckBoxClicked(FindFile(selectedList.RemoveHead()));
		}
		SetRedraw(TRUE);
	}

	CMuleListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSharedFilesCtrl::ShowFileDialog(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage)
{
	if (aFiles.GetCount() > 0)
	{
		CSharedFileDetailsSheet dialog(aFiles, uPshInvokePage, this);
		dialog.DoModal();
	}
}


void CSharedFilesCtrl::SetClassFilter(CClassInfoItem* pNewFilter, bool bRefresh)
{
		if (m_pClassInfoFilter == pNewFilter)
			return;
		m_pClassInfoFilter = pNewFilter;
		if (bRefresh)
			ReloadRecordList();
}
void CSharedFilesCtrl::SetClassFilter(vector<int64> nVecSelClassIDs, bool bRefresh)
{
	
	ReloadRecordList(nVecSelClassIDs);
	
}
void CSharedFilesCtrl::SetDirectoryFilter(CDirectoryItem* pNewFilter, bool bRefresh){
	if (m_pDirectoryFilter == pNewFilter)
		return;
	m_pDirectoryFilter = pNewFilter;
	if (bRefresh)
		ReloadFileList();
}

void CSharedFilesCtrl::OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	if (pGetInfoTip->iSubItem == 0)
	{
		LVHITTESTINFO hti = {0};
		::GetCursorPos(&hti.pt);
		ScreenToClient(&hti.pt);
		if (SubItemHitTest(&hti) == -1 || hti.iItem != pGetInfoTip->iItem || hti.iSubItem != 0){
			// don' show the default label tip for the main item, if the mouse is not over the main item
			if ((pGetInfoTip->dwFlags & LVGIT_UNFOLDED) == 0 && pGetInfoTip->cchTextMax > 0 && pGetInfoTip->pszText[0] != _T('\0'))
				pGetInfoTip->pszText[0] = _T('\0');
			return;
		}

		const CShareableFile* pFile = (CShareableFile*)GetItemData(pGetInfoTip->iItem);
		if (pFile && pGetInfoTip->pszText && pGetInfoTip->cchTextMax > 0)
		{
		/*	CString strInfo = pFile->GetInfoSummary();
			strInfo += TOOLTIP_AUTOFORMAT_SUFFIX_CH;
			_tcsncpy(pGetInfoTip->pszText, strInfo, pGetInfoTip->cchTextMax);
			pGetInfoTip->pszText[pGetInfoTip->cchTextMax-1] = _T('\0');*/
		}
	}
	*pResult = 0;
}

void CSharedFilesCtrl::SetToolTipsDelay(DWORD dwDelay)
{
	CToolTipCtrl* tooltip = GetToolTips();
	if (tooltip)
		tooltip->SetDelayTime(TTDT_INITIAL, dwDelay);
}

bool CSharedFilesCtrl::IsSharedInKad(const CKnownFile *file) const
{
	return false;
	/*return (uint64)time(NULL) < file->GetLastPublishTimeKadSrc() &&
		(!(theApp.IsFirewalled() && theApp.IsConnected()))
			|| ((theApp.clientlist->GetBuddy() && (file->GetLastPublishBuddy() == theApp.clientlist->GetBuddy()->GetIP()))
				|| (Kademlia::CKademlia::IsRunning() && !Kademlia::CUDPFirewallTester::IsFirewalledUDP(true) && Kademlia::CUDPFirewallTester::IsVerified()));*/
}

void CSharedFilesCtrl::AddSelClassIDbyCalc(int64 nID)
{
	vector<int64>::iterator it = find( m_vecSelClassIDsByCalc.begin(), m_vecSelClassIDsByCalc.end(), nID);
	if ( it ==  m_vecSelClassIDsByCalc.end() )
		m_vecSelClassIDsByCalc.push_back(nID);
}
void CSharedFilesCtrl::GetPCID2AllSubClassIDs(int64 nParentCID)
{
	//先以父节点的姿态添加当前选中的节点 调用此函数前已经调用
	//AddSelClassIDbyCalc(nParentCID);
	//遍历查找直接子类
	for ( vector<stSysClassInfoItem>::iterator it = wyytExtendDataManage::Instance().m_vecClassInfos.begin(); it != wyytExtendDataManage::Instance().m_vecClassInfos.end(); it++ )
	{
		if ( nParentCID == it->ParentID )
		{
			//添加直接节点
			AddSelClassIDbyCalc(it->ID);
			//对直接节点进行递归调用
			GetPCID2AllSubClassIDs(it->ID);
		}
	}
}

void CSharedFilesCtrl::AddRecordDatas_PreDeal(vector<int64> nVecSelClassIDs)
{
	m_vecSelClassIDsByCalc.clear();
	for (vector<int64>::iterator it = nVecSelClassIDs.begin(); it !=  nVecSelClassIDs.end(); it++ )
	{
		AddSelClassIDbyCalc(*it);
		GetPCID2AllSubClassIDs(*it); //这里注释掉了，这样只有真正选中的类(即类前的复选框选中)下的记录才会被添加到列表中
	}
	AddRecordDatasByClassIDs();	
}

bool CSharedFilesCtrl::HasSelectedOrSubClass(int64 nCID)
{
	vector<int64>::iterator it = find( m_vecSelClassIDsByCalc.begin(), m_vecSelClassIDsByCalc.end(), nCID);
	if ( it !=  m_vecSelClassIDsByCalc.end() )
		return true;
	return false;
}
void CSharedFilesCtrl::AddRecordDatasByClassIDs()
{
	if ( m_vecSelClassIDsByCalc.size() == 0 )
	{
		m_vecSelClassIDsByCalc.push_back(SCII_NOTES);
	}
	SetRedraw(FALSE);
	for (vector<CRecordItem>::iterator it = wyytExtendDataManage::Instance().m_vecRecordItems.begin(); it !=  wyytExtendDataManage::Instance().m_vecRecordItems.end(); it++ )
	{
		bool bAdd = false;
		for (vector<int64>::iterator itclass = it->nAllClassID.begin(); itclass !=  it->nAllClassID.end(); itclass++ )
		{
			if ( HasSelectedOrSubClass(*itclass))//过滤分类，即判断是否是选中的分类
			{
				if ( IsFilteredItem(&(*it)) )
				{
					bAdd = true;
					break;
				}
				
			}
		}
		if ( bAdd )
		{
			AddRecord(&(*it)); //真添加到列表控件中
		}
	}
	SetRedraw(TRUE); //这个很重要，否则不会调用重绘函数OnChildNotify->DrawItem

}
void CSharedFilesCtrl::AddRecordDatas()
{
	//先清除原来的所有记录
	////while (!liTempShareableFilesInDir.IsEmpty())	// cleanup old filelist
	////	delete liTempShareableFilesInDir.RemoveHead();

	SetRedraw(FALSE);

	//对不需要显示的文件进行过滤
	////	if (ff.IsDirectory() || ff.IsDots() || ff.IsSystem() || ff.IsTemporary() || ff.GetLength()==0 || ff.GetLength()>MAX_EMULE_FILE_SIZE
	////		||(thePrefs.dontsharext&&checkExt(ff.GetFileName(), thePrefs.shareExt)))// X: [DSE] - [DontShareExt]
	////		continue;
	//构造添加的记录选项结构体指针
	//CRecordItem* pNewTempFile = new CRecordItem();
	////		pNewTempFile->SetFilePath(strFoundFilePath);
	////		pNewTempFile->SetFileName(strFoundFileName);
	////		pNewTempFile->SetPath(strFoundDirectory);
	////		pNewTempFile->SetFileSize(ullFoundFileSize);
	////		uchar aucMD4[16];
	////		md4clr(aucMD4);
	////		pNewTempFile->SetFileHash(aucMD4);
	////		liTempShareableFilesInDir.AddTail(pNewTempFile);
	for (vector<CRecordItem>::iterator it = wyytExtendDataManage::Instance().m_vecRecordItems.begin(); it !=  wyytExtendDataManage::Instance().m_vecRecordItems.end(); it++ )
	{
		AddRecord(&(*it)); //真添加到列表控件中
	}
	SetRedraw(TRUE); //这个很重要，否则不会调用重绘函数OnChildNotify->DrawItem
	
}
void CSharedFilesCtrl::AddShareableFiles(CString strFromDir)
{
	////while (!liTempShareableFilesInDir.IsEmpty())	// cleanup old filelist
	////	delete liTempShareableFilesInDir.RemoveHead();

	////CString strSearchPath(strFromDir);
	////PathAddBackslash(strSearchPath.GetBuffer(strFromDir.GetLength() + 1));
	////strSearchPath.ReleaseBuffer();
	////strSearchPath += _T("*");
	////CFileFind ff;
	////bool end = !ff.FindFile(strSearchPath, 0);
	////if (end) {
	////	DWORD dwError = GetLastError();
	////	if (dwError != ERROR_FILE_NOT_FOUND)
	////		DebugLogError(_T("Failed to find files for SharedFilesListCtrl in %s, %s"), strFromDir, GetErrorMessage(dwError));
	////	return;
	////}

	////SetRedraw(FALSE);
	////while (!end)
	////{
	////	end = !ff.FindNextFile();
	////	if (ff.IsDirectory() || ff.IsDots() || ff.IsSystem() || ff.IsTemporary() || ff.GetLength()==0 || ff.GetLength()>MAX_EMULE_FILE_SIZE
	////		||(thePrefs.dontsharext&&checkExt(ff.GetFileName(), thePrefs.shareExt)))// X: [DSE] - [DontShareExt]
	////		continue;

	////	CString strFoundFileName(ff.GetFileName());
	////	CString strFoundFilePath(ff.GetFilePath());
	////	CString strFoundDirectory(strFoundFilePath.Left(ff.GetFilePath().ReverseFind('\\') + 1));
	////	ULONGLONG ullFoundFileSize = ff.GetLength();
	////	CTime tFoundFileTime;
	////	try{
	////		ff.GetLastWriteTime(tFoundFileTime);
	////	}
	////	catch(CException* ex){
	////		ex->Delete();
	////	}

	////	// ignore real(!) LNK files
	////	TCHAR szExt[_MAX_EXT];
	////	_tsplitpath_s(strFoundFileName, NULL, 0, NULL, 0, NULL, 0, szExt, _countof(szExt));
	////	if (_tcsicmp(szExt, _T(".lnk")) == 0){
	////		SHFILEINFO info;
	////		if (SHGetFileInfo(strFoundFilePath, 0, &info, sizeof(info), SHGFI_ATTRIBUTES) && (info.dwAttributes & SFGAO_LINK)){
	////			continue;
	////		}
	////	}

	////	// ignore real(!) thumbs.db files -- seems that lot of ppl have 'thumbs.db' files without the 'System' file attribute
	////	if (strFoundFileName.CompareNoCase(_T("thumbs.db")) == 0)
	////	{
	////		// if that's a valid 'Storage' file, we declare it as a "thumbs.db" file.
	////		CComPtr<IStorage> pStorage;
	////		if (StgOpenStorage(strFoundFilePath, NULL, STGM_READ | STGM_SHARE_DENY_WRITE, NULL, 0, &pStorage) == S_OK)
	////		{
	////			CComPtr<IEnumSTATSTG> pEnumSTATSTG;
	////			if (SUCCEEDED(pStorage->EnumElements(0, NULL, 0, &pEnumSTATSTG)))
	////			{
	////				STATSTG statstg = {0};
	////				if (pEnumSTATSTG->Next(1, &statstg, 0) == S_OK)
	////				{
	////					CoTaskMemFree(statstg.pwcsName);
	////					statstg.pwcsName = NULL;
	////					continue;
	////				}
	////			}
	////		}
	////	}

	////	uint64 fdate = tFoundFileTime.GetTime();// X: [64T] - [64BitTime]
	////	if (fdate == 0)
	////		fdate = (uint64)-1;
	////	if (fdate == -1){
	////		if (thePrefs.GetVerbose())
	////			AddDebugLogLine(false, _T("Failed to get file date of \"%s\""), strFoundFilePath);
	////	}
	////	else
	////		AdjustNTFSDaylightFileTime(fdate, strFoundFilePath);


	////	CKnownFile* toadd = theApp.knownfiles->FindKnownFile(strFoundFileName, fdate, ullFoundFileSize);
	////	if (toadd != NULL && theApp.sharedfiles->GetFileByID(toadd->GetFileHash()) != NULL) // check for shared
	////	{
	////		// this file is already shared and should be on the list, nothing to do
	////	}
	////	else if (toadd != NULL) // for known
	////	{
	////		toadd->SetFilePath(strFoundFilePath);
	////		toadd->SetPath(strFoundDirectory);
	////		AddFile(toadd); // known, could be on the list already
	////	}
	////	else // not known or shared, create
	////	{
	////		CShareableFile* pNewTempFile = new CShareableFile();
	////		pNewTempFile->SetFilePath(strFoundFilePath);
	////		pNewTempFile->SetFileName(strFoundFileName);
	////		pNewTempFile->SetPath(strFoundDirectory);
	////		pNewTempFile->SetFileSize(ullFoundFileSize);
	////		uchar aucMD4[16];
	////		md4clr(aucMD4);
	////		pNewTempFile->SetFileHash(aucMD4);
	////		liTempShareableFilesInDir.AddTail(pNewTempFile);
	////		AddFile(pNewTempFile);
	////	}
	////}
	////SetRedraw(TRUE);
	////ff.Close();
}

BOOL CSharedFilesCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (CheckBoxesEnabled()) // do we have checkboxes?
	{
		NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		int iItem = HitTest(pNMListView->ptAction);
		if (iItem != -1){
			// determine if the checkbox was clicked
			CRect recItem;
			if(GetItemRect(iItem, recItem, LVIR_BOUNDS))
			{
				CPoint pointHit = pNMListView->ptAction;
				ASSERT( recItem.PtInRect(pointHit) );
				recItem.left += sm_iSubItemInset;
				recItem.right = recItem.left + 16;
				recItem.top += (recItem.Height() > 16) ? ((recItem.Height() - 16) / 2) : 0;
				recItem.bottom = recItem.top + 16;
				if (recItem.PtInRect(pointHit)){
					// user clicked on the checkbox
					CheckBoxClicked(iItem);
				}
			}

		}
	}

	*pResult = 0;
	return FALSE; // pass on to parent window
}

void CSharedFilesCtrl::CheckBoxClicked(int iItem)
{
	if (iItem == (-1))
	{
		ASSERT( false );
		return;
	}
	// check which state the checkbox (should) currently have
	//const CShareableFile* pFile = (CShareableFile*)GetItemData(iItem);
	//if (pFile->IsShellLinked())
	//	return; // no interacting with shelllinked files
	//if (theApp.sharedfiles->ShouldBeShared(pFile->GetPath(), pFile->GetFilePath(), false)){// file should be shared
	//	if (theApp.sharedfiles->ShouldBeShared(pFile->GetPath(), _T("")/*pFile->GetFilePath()*/, true)){// file must be shared
	//		if(IsKindOfCKnownFile(pFile)/*pFile->IsKindOf(RUNTIME_CLASS(CKnownFile))*/ ||// X: [QOH] - [QueryOnHashing] not allowed to unshare this known file
	//			!theApp.sharedfiles->CheckAndAddSingleFile(pFile->GetFilePath()))
	//			return;
	//		// this is shared just now
	//		if(!thePrefs.m_bHistoryShowShared){
	//			theApp.emuledlg->sharedfileswnd->historylistctrl.RemoveFileFromView((CKnownFile*)pFile);
	//			//theApp.emuledlg->sharedfileswnd->ShowFilesCount(true);
	//		}
	//	}
	//	else if(IsKindOfCKnownFile(pFile)){// file was shared
	//		// this is currently shared so unshare it
	//		VERIFY( theApp.sharedfiles->ExcludeFile(pFile->GetFilePath()) );
	//		if(!thePrefs.m_bHistoryShowShared){
	//			theApp.emuledlg->sharedfileswnd->historylistctrl.AddFile((CKnownFile*)pFile);
	//			//theApp.emuledlg->sharedfileswnd->ShowFilesCount(true);
	//		}
	//	}
	//	else if(!theApp.sharedfiles->CheckAndAddSingleFile(pFile->GetFilePath()))// file was unshared
	//		return;
	//}
	//else// file was excluded
	//{
	//	// SLUGFILLER: SafeHash remove - removed installation dir unsharing
	//	/*
	//	if (!thePrefs.IsShareableDirectory(pFile->GetPath()))
	//		return; // not allowed to share
	//	*/
	//	if(!theApp.sharedfiles->AddSingleSharedFile(pFile->GetFilePath()))// X: [QOH] - [QueryOnHashing]
	//		return;
	//	UpdateFile(pFile);
	//	if(!thePrefs.m_bHistoryShowShared){
	//		theApp.emuledlg->sharedfileswnd->historylistctrl.RemoveFileFromView((CKnownFile*)pFile);
	//		//theApp.emuledlg->sharedfileswnd->ShowFilesCount(true);
	//	}
	//}
	//// update GUI stuff
	//theApp.emuledlg->sharedfileswnd->ShowFilesCount();
	//theApp.emuledlg->sharedfileswnd->ShowSelectedFilesDetails();
	//theApp.emuledlg->sharedfileswnd->OnSingleFileShareStatusChanged();
}

bool CSharedFilesCtrl::CheckBoxesEnabled() const
{
	return false;
	//return m_pDirectoryFilter != NULL && m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY;
	//return m_pDirectoryFilter != NULL && 
	//	(
	//	m_pDirectoryFilter->m_eItemType == SDI_UNSHAREDDIRECTORY ||
	//	m_pDirectoryFilter->m_eItemType == SDI_NO || 
	//	m_pDirectoryFilter->m_eItemType == SDI_INCOMING || 
	//	m_pDirectoryFilter->m_eItemType == SDI_CATINCOMING
	//	); // X: [QOH] - [QueryOnHashing] Exclude files
}

void CSharedFilesCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// highlighting Checkboxes
	if (CheckBoxesEnabled())
	{
		// are we currently on any checkbox?
		int iItem = HitTest(point);
		if (iItem != (-1))
		{
			CRect recItem;
			if(GetItemRect(iItem, recItem, LVIR_BOUNDS))
			{
				ASSERT( recItem.PtInRect(point) );
				recItem.left += sm_iSubItemInset;
				recItem.right = recItem.left + 16;
				recItem.top += (recItem.Height() > 16) ? ((recItem.Height() - 16) / 2) : 0;
				recItem.bottom = recItem.top + 16;
				if (recItem.PtInRect(point)){
					// is this checkbox already hot?
					if (m_pHighlightedItem != (CRecordItem*)GetItemData(iItem))
					{
						// update old highlighted item
						CRecordItem* pOldItem = m_pHighlightedItem;
						m_pHighlightedItem = (CRecordItem*)GetItemData(iItem);
						//UpdateFile(pOldItem, false);
						UpdateRecord(pOldItem, false);
						// highlight current item
						InvalidateRect(recItem);
					}
					CMuleListCtrl::OnMouseMove(nFlags, point);
					return;
				}
			}
		}
		// no checkbox should be hot
		if (m_pHighlightedItem != NULL)
		{
			CRecordItem* pOldItem = m_pHighlightedItem;
			m_pHighlightedItem = NULL;
			UpdateRecord(pOldItem, false);
		}
	}
	CMuleListCtrl::OnMouseMove(nFlags, point);
}

CSharedFilesCtrl::CShareDropTarget::CShareDropTarget()
{
	m_piDropHelper = NULL;
	m_pParent = NULL;
    m_bUseDnDHelper = SUCCEEDED(CoCreateInstance(CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper, (void**) &m_piDropHelper));
}

CSharedFilesCtrl::CShareDropTarget::~CShareDropTarget()
{
    if (m_piDropHelper != NULL)
        m_piDropHelper->Release();
}

DROPEFFECT CSharedFilesCtrl::CShareDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD /*dwKeyState*/, CPoint point)
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;

	if (pDataObject->IsDataAvailable(CF_HDROP))
		dwEffect = DROPEFFECT_COPY;

    if (m_bUseDnDHelper)
    {
        IDataObject* piDataObj = pDataObject->GetIDataObject(FALSE); 
        m_piDropHelper->DragEnter (pWnd->GetSafeHwnd(), piDataObj, &point, dwEffect);
    }

    return dwEffect;
}

DROPEFFECT CSharedFilesCtrl::CShareDropTarget::OnDragOver(CWnd* /*pWnd*/, COleDataObject* pDataObject, DWORD /*dwKeyState*/, CPoint point)
{
	DROPEFFECT dwEffect = DROPEFFECT_NONE;

	if (pDataObject->IsDataAvailable(CF_HDROP))
		dwEffect = DROPEFFECT_COPY;

    if (m_bUseDnDHelper)
    {
		m_piDropHelper->DragOver(&point, dwEffect);
    }

    return dwEffect;
}

BOOL CSharedFilesCtrl::CShareDropTarget::OnDrop(CWnd* /*pWnd*/, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_HDROP);
	if (hGlobal != NULL)
	{
		HDROP hDrop = (HDROP)GlobalLock(hGlobal);
		if (hDrop != NULL)
		{
			CString strFilePath;
			CFileFind ff;
			CAtlList<CString> liToAddFiles; // all files too add
			CAtlList<CString> liToAddDirs; // all directories to add
			bool bFromSingleDirectory = true; // are all files from within the same directory
			CString strSingleDirectory = _T(""); // which would be this one
			
			UINT nFileCount = DragQueryFile(hDrop, (UINT)(-1), NULL, 0);
			//for (UINT nFile = 0; nFile < nFileCount; nFile++ )
			//{
			//	if (DragQueryFile(hDrop, nFile, strFilePath.GetBuffer(MAX_PATH), MAX_PATH) > 0 )
			//	{
			//		strFilePath.ReleaseBuffer();
			//		if (ff.FindFile(strFilePath, 0))
			//		{
			//			ff.FindNextFile();
			//			// just a quick pre check, complete check is done later in the share function itself
			//			if (ff.IsDots() || ff.IsSystem() || ff.IsTemporary()
			//				|| (!ff.IsDirectory() && (ff.GetLength()==0 || ff.GetLength()>MAX_EMULE_FILE_SIZE))
			//				// SLUGFILLER: SafeHash remove - removed installation dir unsharing
			//				/*
			//				|| (ff.IsDirectory() && !thePrefs.IsShareableDirectory(ff.GetFilePath() + _T('\\')))
			//				*/
			//				|| (ff.IsDirectory() && theApp.sharedfiles->ShouldBeShared(ff.GetFilePath()+ _T('\\'), _T(""), false))
			//				|| (!ff.IsDirectory() && theApp.sharedfiles->ShouldBeShared(ff.GetFilePath(), ff.GetFilePath().Left(ff.GetFilePath().ReverseFind('\\') + 1), false)) )
			//			{
			//				DebugLog(_T("Drag&Drop'ed shared File ignored (%s)"), ff.GetFilePath()); 
			//				ff.Close();
			//				continue;
			//			}
			//			if (ff.IsDirectory())
			//			{
			//				DEBUG_ONLY( DebugLog(_T("Drag'n'Drop'ed directory: %s"), ff.GetFilePath()+ _T('\\'))  );
			//				liToAddDirs.AddTail(ff.GetFilePath() + _T('\\'));
			//			}
			//			else
			//			{
			//				DEBUG_ONLY( DebugLog(_T("Drag'n'Drop'ed file: %s"), ff.GetFilePath()) );
			//				liToAddFiles.AddTail(ff.GetFilePath());
			//				if (bFromSingleDirectory)
			//				{
			//					if (strSingleDirectory.IsEmpty())
			//						strSingleDirectory = ff.GetFilePath().Left(ff.GetFilePath().ReverseFind('\\') + 1);
			//					else if (strSingleDirectory.CompareNoCase(ff.GetFilePath().Left(ff.GetFilePath().ReverseFind('\\') + 1)) != NULL)
			//						bFromSingleDirectory = false;
			//				}
			//			}
			//		}
			//		else
			//		{
			//			DebugLogError(_T("Drag&Drop'ed shared File not found (%s)"), strFilePath); 
			//		}
			//		ff.Close();

			//	}
			//	else
			//	{
			//		ASSERT( false );
			//		strFilePath.ReleaseBuffer();
			//	}
			//}
			//if (!liToAddFiles.IsEmpty() || !liToAddDirs.IsEmpty())
			//{
			//	// add the directories first as they could
			//	// make single file adds invalid if they are contained in one of those dirs already 
			//	for (POSITION pos = liToAddDirs.GetHeadPosition(); pos != NULL; )
			//		VERIFY( theApp.sharedfiles->AddSingleSharedDirectory(liToAddDirs.GetNext(pos)) ); // should always succeed

			//	bool bHaveFiles = false;
			//	while (!liToAddFiles.IsEmpty())
			//		bHaveFiles |= theApp.sharedfiles->AddSingleSharedFile(liToAddFiles.RemoveHead()); // could fail, due to the dirs added above

			//	// GUI updates
			//	if (!liToAddDirs.IsEmpty())
			//		theApp.emuledlg->sharedfileswnd->m_ctlSharedDirTree.Reload(true);
			//	if (bHaveFiles)
			//		theApp.emuledlg->sharedfileswnd->OnSingleFileShareStatusChanged();
			//	theApp.emuledlg->sharedfileswnd->ShowFilesCount();

			//	if (bHaveFiles && liToAddDirs.IsEmpty() && bFromSingleDirectory)
			//	{
			//		// if we added only files from the same directory, show and select this in the filesystem tree
			//		ASSERT( !strSingleDirectory.IsEmpty() );
			//		VERIFY( theApp.emuledlg->sharedfileswnd->m_ctlSharedDirTree.ShowFileSystemDirectory(strSingleDirectory) );
			//	}
			//	else if (!liToAddDirs.IsEmpty() && !bHaveFiles)
			//	{
			//		// only directories added, if only one select the specific shared dir, otherwise the Shared Directories section
			//		if (liToAddDirs.GetCount() == 1)
			//			theApp.emuledlg->sharedfileswnd->m_ctlSharedDirTree.ShowSharedDirectory(liToAddDirs.GetHead());
			//		else
			//			theApp.emuledlg->sharedfileswnd->m_ctlSharedDirTree.ShowSharedDirectory(_T(""));
			//	}
			//	else
			//	{
			//		// otherwise select the All Shared Files category
			//		theApp.emuledlg->sharedfileswnd->m_ctlSharedDirTree.ShowAllSharedFiles();
			//	}
			//}
			GlobalUnlock(hGlobal);
		}
		GlobalFree(hGlobal);
	}

    if (m_bUseDnDHelper)
    {
        IDataObject* piDataObj = pDataObject->GetIDataObject(FALSE); 
        m_piDropHelper->Drop(piDataObj, &point, dropEffect);
    }
    
    return TRUE;
}

void CSharedFilesCtrl::CShareDropTarget::OnDragLeave(CWnd* /*pWnd*/)
{
    if (m_bUseDnDHelper)
        m_piDropHelper->DragLeave();
}
void CSharedFilesCtrl::RecordDelete( int64 ID )
{
	//先删除附件信息
	map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(ID);
	if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
	{
		for (vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++ )
		{
			CString csFileFullName;
			csFileFullName.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,it->CodeID.c_str(),it->FileSuffix.c_str());
			wyytExtendDataManage::Instance().DelSysRecordAttachFile(csFileFullName);
		}

	}
	//再删除数据库及内存信息
	wyytExtendDataManage::Instance().DelSysRecordInfo(ID);
}
void CSharedFilesCtrl::RecordDelete( vector<int64> vecSelected )
{
	for (vector<int64>::iterator it = vecSelected.begin(); it != vecSelected.end(); it++ )
	{
		RecordDelete(*it);
	}
}
void CSharedFilesCtrl::RecordDelete( const CRecordItem* pRecord )
{
	int iItem = FindRecord(pRecord);
	if (iItem != -1)
	{
		if ( DeleteItem(iItem) )
		{
			RecordDelete(pRecord->ID);
		}
	}
}
int CSharedFilesCtrl::ShowRecDetailsDlg_MOD(CRecordItem* pRecord)
{
	CRecordDetails dlgDetails;	
	dlgDetails.SetCurOperOption(ECOO_MOD);
	dlgDetails.InitCurRecordInfo(pRecord);
	if(dlgDetails.DoModal()  == IDOK)
	{
		dlgDetails.ReallySave_Attachs(ECOO_MOD);
		wyytExtendDataManage::Instance().ModSysRecordInfo(pRecord);
		
		//CString csNewClassName;
		//ciDlg.GetResult(csNewClassName);

		////添加到树控件中
		////AddTreeClassInfoChildItem(pRoot, bTopLevel, stCII);
		//SetItemText(hSelect, csNewClassName);
		////更新到内存中//添加的数据库中
		//wyytExtendDataManage::Instance().ModClassInfo(pRoot->m_ID,csNewClassName.GetBufferSetLength(csNewClassName.GetLength()));
	}
	else
	{
		dlgDetails.ReallyCancel_Attach();
	}
	return 0;
}
int CSharedFilesCtrl::ShowRecDetailsDlg_ADD()
{
	CRecordItem stRI;
	CRecordDetails dlgDetails;	
	dlgDetails.SetCurOperOption(ECOO_ADD);
	wyytExtendDataManage::Instance().GetSysRecordMaxID(stRI.ID);
	dlgDetails.InitCurRecordInfo(&stRI);
	if(dlgDetails.DoModal()  == IDOK)
	{
		CSID CodeID(E_SMT_SYS, stRI.MainClassID, stRI.ID, 0);
		stRI.SID = CodeID.FiniToString();
		dlgDetails.ReallySave_Attachs(ECOO_ADD);
		wyytExtendDataManage::Instance().InsertSysRecordInfo(stRI);
		
		//CString csNewClassName;
		//ciDlg.GetResult(csNewClassName);

		////添加到树控件中
		////AddTreeClassInfoChildItem(pRoot, bTopLevel, stCII);
		//SetItemText(hSelect, csNewClassName);
		////更新到内存中//添加的数据库中
		//wyytExtendDataManage::Instance().ModClassInfo(pRoot->m_ID,csNewClassName.GetBufferSetLength(csNewClassName.GetLength()));
	}
	return 0;
}
INT_PTR CSharedFilesCtrl::ShowRecPPDlg(int nOpt, UINT uStartPageID)
{
	CRecordInfoSheetDlg RecInfPPSheetDlg;
	if (uStartPageID != (UINT)-1)
		RecInfPPSheetDlg.SetStartPage(uStartPageID);
	RecInfPPSheetDlg.SetCurOperOption(nOpt);
	//RecInfPPSheetDlg.OpenDialog(SW_SHOW, FALSE);
	if (RecInfPPSheetDlg.DoModal() == IDOK)
	{
		return 0;
	}


	//CRecordInfoSheetDlg* m_pRecInfPPSheetDlg;
	//m_pRecInfPPSheetDlg = new CRecordInfoSheetDlg;
	//if (uStartPageID != (UINT)-1)
	//	m_pRecInfPPSheetDlg->SetStartPage(uStartPageID);
	//m_pRecInfPPSheetDlg->SetCurOperOption(nOpt);
	//m_pRecInfPPSheetDlg->OpenDialog(SW_SHOW, FALSE);
	//m_pRecInfPPSheetDlg->CloseDialog(); 
	//delete m_pRecInfPPSheetDlg;
	return 0;

}