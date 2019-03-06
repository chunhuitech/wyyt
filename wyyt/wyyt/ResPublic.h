#pragma once
#include "map_inc.h"
#include "TitleMenu.h"
#include "wyytExtendDataManage.h"
#include "DataInfos.h"
#include "ListViewWalkerPropertySheet.h"
//#include "../interface/DB/DbDefine.h"

#include "OtherFunctions.h"
#include "ArchivePreviewDlg.h"
#include "AttachsDlg.h"
#include "HighColorTab.hpp"

#include "UserMsgs.h"
#include "ResizableLib/ResizableSheet.h"
#include "MemDC.h"
#include "MenuCmds.h"
#include "RecordInfoSheetDlg.h"
#include "RecordDetails.h"



#define	SPLITTER_RANGE_MIN		260
#define	SPLITTER_RANGE_MAX		450

#define	SPLITTER_MARGIN			0
#define	SPLITTER_WIDTH			4

enum ESpecialDirectoryItems{
	SDI_NO = 0,
	SDI_ALL,				// "All Shared Files" node
	SDI_INCOMING,			// "Incoming Files" node
	SDI_TEMP,				// "Incomplete Files" node
	SDI_DIRECTORY,			// "Shared Directories" node
	SDI_CATINCOMING,		// Category subnode in the "Incoming Files" node
	SDI_UNSHAREDDIRECTORY,
	SDI_FILESYSTEMPARENT	// "All Directories" (the file system)
};


//**********************************************************************************
// CDirectoryItem

class CDirectoryItem{
public:
	CDirectoryItem(CString strFullPath, HTREEITEM htItem = TVI_ROOT, ESpecialDirectoryItems eItemType = SDI_NO, int m_nCatFilter = -1);
	~CDirectoryItem();
	CDirectoryItem*		CloneContent() { return new CDirectoryItem(m_strFullPath, 0, m_eItemType, m_nCatFilter); }
	HTREEITEM			FindItem(CDirectoryItem* pContentToFind) const;

	CString		m_strFullPath;
	HTREEITEM	m_htItem;
	int			m_nCatFilter;
	CAtlList<CDirectoryItem*> liSubDirectories;
	ESpecialDirectoryItems m_eItemType;
};


// CSharedFileDetailsSheet

class CSharedFileDetailsSheet : public CListViewWalkerPropertySheet
{
	DECLARE_DYNAMIC(CSharedFileDetailsSheet)

public:
	CSharedFileDetailsSheet(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage = 0, CListCtrlItemWalk* pListCtrl = NULL,EModuleType emt = EMT_SYS);
	virtual ~CSharedFileDetailsSheet();

protected:
	
	//CMetaDataDlg		m_wndMetaData;
	//CED2kLinkDlg		m_wndFileLink;
	//CCommentDialog		m_wndFileComments;
	CArchivePreviewDlg	m_wndArchiveInfo;
	CAttachsDlg		m_wndAttachsInfo;

	UINT m_uPshInvokePage;
	static LPCTSTR m_pPshStartPage;

	void UpdateTitle();

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg LRESULT OnDataChanged(WPARAM, LPARAM);

	EModuleType m_emt;
};

CString GetFolderLabel(const CString &strFolderPath, bool bTopFolder, bool bAccessible);



class CArchivePreviewDlg;
class CAttachsDlg;
class CTestDlg;
class CSharedFileDetailsModelessSheet : public CListViewPropertySheet
{
	DECLARE_DYNAMIC(CSharedFileDetailsModelessSheet)

public:
	CSharedFileDetailsModelessSheet(EModuleType emt = EMT_SYS);
	virtual ~CSharedFileDetailsModelessSheet();
	void SetFiles(CAtlList<CRecordItem*>& aRecords);
	void LocalizeAll();
	void UpdateMetaDataPage(); //>>> WiZaRd::FiX

	void SetModuleType(EModuleType emt)
	{
		m_emt = emt;
	}
protected:
	//CFileDetailDlgStatistics*	m_wndStatistics;
	//CED2kLinkDlg*				m_wndFileLink;
	CArchivePreviewDlg*			m_wndArchiveInfo;
	CAttachsDlg*			m_wndAttachsInfo;
	//CMetaDataDlg*				m_wndMetaData;

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnDataChanged(WPARAM, LPARAM);
	EModuleType m_emt;

};