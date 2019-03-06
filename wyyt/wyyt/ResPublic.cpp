#include "stdafx.h"
#include "wyyt.h"
#include "ResPublic.h"
#include "preferences.h"
#include "otherfunctions.h"
#include "SharedFilesCtrl.h"
//#include "Knownfile.h"
#include "MenuCmds.h"
//#include "partfile.h"
#include "wyytdlg.h"
//#include "TransferDlg.h"
//#include "SharedFileList.h"
//#include "SharedFilesWnd.h"
//#include "AddModClassInfoDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**********************************************************************************
// CDirectoryItem

CDirectoryItem::CDirectoryItem(CString strFullPath, HTREEITEM htItem, ESpecialDirectoryItems eItemType, int nCatFilter){
	m_htItem = htItem;
	m_strFullPath = strFullPath;
	m_eItemType = eItemType;
	m_nCatFilter = nCatFilter;
}
	
CDirectoryItem::~CDirectoryItem(){
	while (liSubDirectories.GetHeadPosition() != NULL){
		delete liSubDirectories.RemoveHead();
	}
}

// search tree for a given filter
HTREEITEM CDirectoryItem::FindItem(CDirectoryItem* pContentToFind) const
{
	if (pContentToFind == NULL){
		ASSERT( false );
		return NULL;
	}

	if (pContentToFind->m_eItemType == m_eItemType && pContentToFind->m_strFullPath == m_strFullPath && pContentToFind->m_nCatFilter == m_nCatFilter)
		return m_htItem;

	POSITION pos = liSubDirectories.GetHeadPosition();
	while (pos != NULL){
		CDirectoryItem* pCurrent = liSubDirectories.GetNext(pos);
		HTREEITEM htResult;
		if ( (htResult = pCurrent->FindItem(pContentToFind)) != NULL)
			return htResult;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////

bool NeedArchiveInfoPage(const CSimpleArray<void*>* paItems);
void UpdateFileDetailsPages(CListViewPropertySheet *pSheet,
							CResizablePage *pArchiveInfo, CResizablePage *pAttachInfo);


//////////////////////////////////////////////////////////////////////////////


LPCTSTR CSharedFileDetailsSheet::m_pPshStartPage;

IMPLEMENT_DYNAMIC(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)

BEGIN_MESSAGE_MAP(CSharedFileDetailsSheet, CListViewWalkerPropertySheet)
	ON_WM_DESTROY()
	ON_MESSAGE(UM_DATA_CHANGED, OnDataChanged)
END_MESSAGE_MAP()

CSharedFileDetailsSheet::CSharedFileDetailsSheet(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage, CListCtrlItemWalk* pListCtrl,EModuleType emt)
	: CListViewWalkerPropertySheet(pListCtrl),m_emt(emt)
{
	m_uPshInvokePage = uPshInvokePage;
	POSITION pos = aFiles.GetHeadPosition();
	while (pos)
		m_aItems.Add(aFiles.GetNext(pos));
	m_psh.dwFlags &= ~PSH_HASHELP;

	//m_wndFileComments.m_psp.dwFlags &= ~PSP_HASHELP;
	//m_wndFileComments.m_psp.dwFlags |= PSP_USEICONID;
	//m_wndFileComments.m_psp.pszIcon = _T("FileComments");
	//m_wndFileComments.SetFiles(&m_aItems);
	//AddPage(&m_wndFileComments);

	m_wndArchiveInfo.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndArchiveInfo.m_psp.dwFlags |= PSP_USEICONID;
	m_wndArchiveInfo.m_psp.pszIcon = _T("ARCHIVE_PREVIEW");
	m_wndArchiveInfo.SetModuleType(m_emt);
	m_wndArchiveInfo.SetFiles(&m_aItems);
	AddPage(&m_wndArchiveInfo);
	m_wndAttachsInfo.m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndAttachsInfo.m_psp.dwFlags |= PSP_USEICONID;
	m_wndAttachsInfo.m_psp.pszIcon = _T("ATTACH_PREVIEW");
	m_wndAttachsInfo.SetModuleType(m_emt);
	m_wndAttachsInfo.SetFiles(&m_aItems);
	AddPage(&m_wndAttachsInfo);
	/*if (NeedArchiveInfoPage(&m_aItems))
	AddPage(&m_wndArchiveInfo);
	else
	{
	AddPage(&m_wndMediaInfo);
	}*/

	//m_wndMetaData.m_psp.dwFlags &= ~PSP_HASHELP;
	//m_wndMetaData.m_psp.dwFlags |= PSP_USEICONID;
	//m_wndMetaData.m_psp.pszIcon = _T("METADATA");
	//if (m_aItems.GetSize() == 1 && thePrefs.IsExtControlsEnabled()) {
	//	m_wndMetaData.SetFiles(&m_aItems);
	//	AddPage(&m_wndMetaData);
	//}

	//m_wndFileLink.m_psp.dwFlags &= ~PSP_HASHELP;
	//m_wndFileLink.m_psp.dwFlags |= PSP_USEICONID;
	//m_wndFileLink.m_psp.pszIcon = _T("ED2KLINK");
	//m_wndFileLink.SetFiles(&m_aItems);
	//AddPage(&m_wndFileLink);

	LPCTSTR pPshStartPage = m_pPshStartPage;
	if (m_uPshInvokePage != 0)
		pPshStartPage = MAKEINTRESOURCE(m_uPshInvokePage);
	for (int i = 0; i < m_pages.GetSize(); i++)
	{
		CPropertyPage* pPage = GetPage(i);
		if (pPage->m_psp.pszTemplate == pPshStartPage)
		{
			m_psh.nStartPage = i;
			break;
		}
	}
}

CSharedFileDetailsSheet::~CSharedFileDetailsSheet()
{
}

void CSharedFileDetailsSheet::OnDestroy()
{
	if (m_uPshInvokePage == 0)
		m_pPshStartPage = GetPage(GetActiveIndex())->m_psp.pszTemplate;
	CListViewWalkerPropertySheet::OnDestroy();
}

BOOL CSharedFileDetailsSheet::OnInitDialog()
{
	EnableStackedTabs(FALSE);
	BOOL bResult = CListViewWalkerPropertySheet::OnInitDialog();
	HighColorTab::UpdateImageList(*this);
	InitWindowStyles(this);
	EnableSaveRestore(_T("SharedFileDetailsSheet"), !thePrefs.prefReadonly); // call this after(!) OnInitDialog // X: [ROP] - [ReadOnlyPreference]
	UpdateTitle();
	return bResult;
}

LRESULT CSharedFileDetailsSheet::OnDataChanged(WPARAM, LPARAM)
{
	UpdateTitle();
	//UpdateFileDetailsPages(this, &m_wndArchiveInfo, &m_wndAttachsInfo);
	return 1;
}

void CSharedFileDetailsSheet::UpdateTitle()
{
	CString text = GetResString(IDS_DETAILS);
	if(m_aItems.GetSize() == 1)
		//text.AppendFormat(_T(": %s"), STATIC_DOWNCAST(CAbstractFile, m_aItems[0])->GetFileName());
		///text.AppendFormat(_T(": %s"), ((CAbstractFile*)m_aItems[0])->GetFileName());
	SetWindowText(text);
}

BOOL CSharedFileDetailsSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_APPLY_NOW)
	{
//		CPrivateSharedRecordsCtrl* pSharedFilesCtrl = DYNAMIC_DOWNCAST(CPrivateSharedRecordsCtrl, m_pListCtrl->GetListCtrl());
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
	}

	return CListViewWalkerPropertySheet::OnCommand(wParam, lParam);
}

CString GetFolderLabel(const CString &strFolderPath, bool bTopFolder, bool bAccessible)
{
	CString strFolder(strFolderPath);
	PathRemoveBackslash(strFolder.GetBuffer());
	strFolder.ReleaseBuffer();

	CString strLabel(strFolder);
	if (strLabel.GetLength() == 2 && strLabel.GetAt(1) == _T(':'))
	{
		ASSERT( bTopFolder );
		strLabel += _T('\\');
	}
	else
	{
		strLabel = strLabel.Right(strLabel.GetLength() - (strLabel.ReverseFind(_T('\\')) + 1));
		if (bTopFolder)
		{
			CString strParentFolder(strFolder);
			PathRemoveFileSpec(strParentFolder.GetBuffer());
			strParentFolder.ReleaseBuffer();
			strLabel += _T("  (") + strParentFolder + _T(')');
		}
	}
	//if (!bAccessible && bTopFolder)
	//	strLabel += _T(" [") + GetResString(IDS_NOTCONNECTED) + _T(']');

	return strLabel;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// CSharedFileDetailsModelessSheet
IMPLEMENT_DYNAMIC(CSharedFileDetailsModelessSheet, CListViewPropertySheet)

BEGIN_MESSAGE_MAP(CSharedFileDetailsModelessSheet, CListViewPropertySheet)
	ON_MESSAGE(UM_DATA_CHANGED, OnDataChanged)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CSharedFileDetailsModelessSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// skip CResizableSheet::OnCreate because we don't the styles and stuff which are set there
	CreateSizeGrip(FALSE); // create grip but dont show it
	return CPropertySheet::OnCreate(lpCreateStruct);
}

bool NeedArchiveInfoPage(const CSimpleArray<void*>* paItems);
void UpdateFileDetailsPages(CListViewPropertySheet *pSheet,
							CResizablePage *pArchiveInfo, CResizablePage *pAttachInfo);

CSharedFileDetailsModelessSheet::CSharedFileDetailsModelessSheet(EModuleType emt):m_emt(emt)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_psh.dwFlags |= PSH_MODELESS;
	m_wndArchiveInfo = new CArchivePreviewDlg();
	m_wndArchiveInfo->m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndArchiveInfo->m_psp.dwFlags |= PSP_USEICONID;
	m_wndArchiveInfo->m_psp.pszIcon = _T("ARCHIVE_PREVIEW");
	m_wndArchiveInfo->SetReducedDialog();
	m_wndArchiveInfo->SetModuleType(m_emt);
	m_wndArchiveInfo->SetFiles(&m_aItems);	
	AddPage(m_wndArchiveInfo);

	m_wndAttachsInfo = new CAttachsDlg();
	m_wndAttachsInfo->m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndAttachsInfo->m_psp.dwFlags |= PSP_USEICONID;
	m_wndAttachsInfo->m_psp.pszIcon = _T("ATTACH_PREVIEW");
	m_wndAttachsInfo->SetReducedDialog();
	m_wndAttachsInfo->SetModuleType(m_emt);
	m_wndAttachsInfo->SetFiles(&m_aItems);	
	AddPage(m_wndAttachsInfo);

	UpdateMetaDataPage();

	m_emt = EMT_SYS;

}

//>>> WiZaRd::FiX
void CSharedFileDetailsModelessSheet::UpdateMetaDataPage()
{
	/*if (thePrefs.IsExtControlsEnabled())
	{
	if(m_wndMetaData)
	return;

	m_wndMetaData = new CMetaDataDlg();
	m_wndMetaData->m_psp.dwFlags &= ~PSP_HASHELP;
	m_wndMetaData->m_psp.dwFlags |= PSP_USEICONID;
	m_wndMetaData->m_psp.pszIcon = _T("METADATA");
	m_wndMetaData->SetFiles(&m_aItems);
	AddPage(m_wndMetaData);
	}
	else
	{
	if(m_wndMetaData == NULL)
	return;

	RemovePage(m_wndMetaData);
	delete m_wndMetaData;
	m_wndMetaData = NULL;
	}*/
}

void  CSharedFileDetailsModelessSheet::LocalizeAll()
{
	// X: [BF] - [Bug Fix]
	static const UINT titleids[] = { IDS_CONTENT_INFO};
	CTabCtrl*tab = GetTabControl();
	TCITEM tabitem;
	tabitem.mask = TCIF_TEXT;
	for (int index = 0; index < tab->GetItemCount(); ++index)
	{
		CString strCaption(GetResString(titleids[index]));
		tabitem.pszText = const_cast<LPTSTR>((LPCTSTR)strCaption);
		tab->SetItem(index,&tabitem);
	}
	
	if(m_wndArchiveInfo && m_wndArchiveInfo->GetSafeHwnd())
		m_wndArchiveInfo->Localize();
	if(m_wndAttachsInfo && m_wndAttachsInfo->GetSafeHwnd())
		m_wndAttachsInfo->Localize();

}
//<<< WiZaRd::FiX

CSharedFileDetailsModelessSheet::~CSharedFileDetailsModelessSheet()
{
	delete m_wndArchiveInfo;
	delete m_wndAttachsInfo;

}

BOOL CSharedFileDetailsModelessSheet::OnInitDialog()
{
	EnableStackedTabs(FALSE);
	BOOL bResult = CListViewPropertySheet::OnInitDialog();
	HighColorTab::UpdateImageList(*this);
	InitWindowStyles(this);
	return bResult;
}

void  CSharedFileDetailsModelessSheet::SetFiles(CAtlList<CRecordItem*>& aRecords)
{
	m_aItems.RemoveAll();
	POSITION pos = aRecords.GetHeadPosition();
	while (pos)
		m_aItems.Add(aRecords.GetNext(pos));
	m_wndArchiveInfo->SetFiles(&m_aItems);
	m_wndAttachsInfo->SetFiles(&m_aItems);
	ChangedData();
}

LRESULT CSharedFileDetailsModelessSheet::OnDataChanged(WPARAM, LPARAM)
{
	//UpdateFileDetailsPages(this, m_wndArchiveInfo, m_wndAttachsInfo);//这里不用调用了，只是起到显示或隐藏相关窗口的问题
	return 1;
}
