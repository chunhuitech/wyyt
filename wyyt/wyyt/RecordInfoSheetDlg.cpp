#include "StdAfx.h"
#include "wyyt.h"
#include "RecordInfoSheetDlg.h"
#include "Preferences.h"
#include "UserMsgs.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRecordInfoSheetDlg, CTreePropSheet)

BEGIN_MESSAGE_MAP(CRecordInfoSheetDlg, CModTreePropSheet) // NEO: MLD - [ModelesDialogs] <-- Xanatos --
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

CRecordInfoSheetDlg::CRecordInfoSheetDlg(void)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_wndClass.m_psp.dwFlags &= ~PSH_HASHELP;

	CTreePropSheet::SetPageIcon(&m_wndClass, _T("RECORDCLASSINFOS"));//

	AddPage(&m_wndClass);

	// The height of the option dialog is already too large for 640x480. To show as much as
	// possible we do not show a page caption (which is an decorative element only anyway).
	SetTreeViewMode(TRUE, GetSystemMetrics(SM_CYSCREEN) >= 600, TRUE);
	SetTreeWidth(170);

	m_pPshStartPage = NULL;
	m_bSaveIniFile = false;
	m_nCurOperOPtion = ECOO_ADD;
}


CRecordInfoSheetDlg::~CRecordInfoSheetDlg(void)
{
}

void CRecordInfoSheetDlg::OnDestroy()
{
	CModTreePropSheet::OnDestroy();
	if (m_bSaveIniFile)
	{
		thePrefs.Save();
		m_bSaveIniFile = false;
	}
	m_pPshStartPage = GetPage(GetActiveIndex())->m_psp.pszTemplate;
}


BOOL CRecordInfoSheetDlg::OnInitDialog()
{
	ASSERT( !m_bSaveIniFile );
	BOOL bResult = CModTreePropSheet::OnInitDialog(); // NEO: MLD - [ModelesDialogs] <-- Xanatos --
	InitWindowStyles(this);

	for (int i = 0; i < m_pages.GetSize(); i++)
	{
		if (GetPage(i)->m_psp.pszTemplate == m_pPshStartPage)
		{
			SetActivePage(i);
			break;
		}
	}

	Localize();	
	return bResult;
}


void CRecordInfoSheetDlg::Localize()
{
	CString title;
	if ( ECOO_ADD == m_nCurOperOPtion )
	{
		title = RemoveAmbersand(GetResString(IDS_ADDRECORD));//
		//if(thePrefs.prefReadonly)// X: [ROP] - [ReadOnlyPreference]
		//	title.AppendFormat(_T(" [%s]"), GetResString(IDS_READONLY));
		
	}
	else if ( ECOO_MOD == m_nCurOperOPtion )
	{
		title = RemoveAmbersand(GetResString(IDS_MODRECORD));//
	}
	SetTitle(title); 

	m_wndClass.Localize();

	int c = 0;

	CTreeCtrl* pTree = GetPageTreeControl();
	if (pTree)
	{
		pTree->SetItemText(GetPageTreeItem(c++), /*RemoveAmbersand(*/GetResString(IDS_PW_GENERAL)/*)*/);
		
	}
	SetDlgItemText(IDOK, GetResString(IDS_TREEOPTIONS_OK));// X: [AL] - [Additional Localize]
	SetDlgItemText(IDCANCEL, GetResString(IDS_CANCEL));
	SetDlgItemText(ID_APPLY_NOW, GetResString(IDS_PW_APPLY));
	SetDlgItemText(IDHELP, GetResString(IDS_EM_HELP));

	UpdateCaption();
}


void CRecordInfoSheetDlg::OnHelp()
{
	int iCurSel = GetActiveIndex();
	if (iCurSel >= 0 && iCurSel<=12) //Xman no crash on F1 (remark: 12 pages are official)
	{
		CPropertyPage* pPage = GetPage(iCurSel);
		if (pPage)
		{
			HELPINFO hi = {0};
			hi.cbSize = sizeof hi;
			hi.iContextType = HELPINFO_WINDOW;
			hi.iCtrlId = 0;
			hi.hItemHandle = pPage->m_hWnd;
			hi.dwContextId = 0;
			pPage->SendMessage(WM_HELP, 0, (LPARAM)&hi);
			return;
		}
	}

	theApp.ShowHelp(0, HELP_CONTENTS);
}

BOOL CRecordInfoSheetDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_HELP)
	{
		OnHelp();
		return TRUE;
	}
	if (wParam == IDOK || wParam == ID_APPLY_NOW)
		m_bSaveIniFile = true;
	return __super::OnCommand(wParam, lParam);
}

BOOL CRecordInfoSheetDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

void CRecordInfoSheetDlg::SetStartPage(UINT uStartPageID)
{
	m_pPshStartPage = MAKEINTRESOURCE(uStartPageID);
}

void CRecordInfoSheetDlg::SetCurOperOption(int nOpt)
{
	m_nCurOperOPtion = nOpt;
}

int CRecordInfoSheetDlg::GetCurOperOption()
{
	return m_nCurOperOPtion;
}