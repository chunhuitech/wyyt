// PriResManageWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "PriResManageWnd.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "HighColorTab.hpp"
#include "ArchivePreviewDlg.h"
#include "AttachsDlg.h"
#include "UserMsgs.h"
#include "preferences.h"

#define IDC_SPLITTER_PRIRESMANAGE       2890

// CPriResManageWnd 对话框

IMPLEMENT_DYNAMIC(CPriResManageWnd, CDialog)

CPriResManageWnd::CPriResManageWnd(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CPriResManageWnd::IDD, pParent),m_dlgDetails(EMT_PRI)
{
	m_bDetailsVisible = true;
	m_dlgDetails.SetModuleType(EMT_PRI);
}

CPriResManageWnd::~CPriResManageWnd()
{
}

void CPriResManageWnd::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRI_RECORD_FILTER, m_ctlFilter_Record);
	DDX_Control(pDX, IDC_PRM_CLASSINFOTREE, m_ctlClassInfoTree);
	DDX_Control(pDX, IDC_PRM_RECORDLIST, m_ctlRecordsList);
	DDX_Control(pDX, IDC_PRM_FILTER, m_ctlFilter);
}


BEGIN_MESSAGE_MAP(CPriResManageWnd, CResizableDialog)
	ON_MESSAGE(UM_DELAYED_EVALUATE, OnChangeFilter)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PRM_RECORDLIST, &CPriResManageWnd::OnLvnItemchangedPrmRecordlist)
END_MESSAGE_MAP()


// CPriResManageWnd 消息处理程序
LRESULT CPriResManageWnd::OnChangeFilter(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor curWait; // this may take a while
	CFilterItem*fi;
	fi=&m_ctlRecordsList;
	bool bColumnDiff = (fi->m_nFilterColumn != (uint32)wParam);
	fi->m_nFilterColumn = wParam;
	CAtlArray<CString> astrFilter;//空格 - 分隔 子串
	CString strFullFilterExpr = (LPCTSTR)lParam;
	int iPos = 0;
	CString strFilter(strFullFilterExpr.Tokenize(_T(" "), iPos));
	while (!strFilter.IsEmpty()) 
	{
		if (strFilter != _T('-'))
			astrFilter.Add(strFilter);
		strFilter = strFullFilterExpr.Tokenize(_T(" "), iPos);
	}
	CAtlArray<CString>& m_astrFilter=fi->m_astrFilter;
	bool bFilterDiff = (astrFilter.GetCount() != m_astrFilter.GetCount());
	if (!bFilterDiff) 
	{
		for (size_t i = 0; i < astrFilter.GetCount(); i++) 
		{
			if (astrFilter[i] != m_astrFilter[i]) 
			{
				bFilterDiff = true;
				break;
			}
		}
	}

	if (!bColumnDiff && !bFilterDiff)
		return 0;
	m_astrFilter.RemoveAll();
	m_astrFilter.Append(astrFilter);
	fi->ReloadRecordList();
	return 0;
}

void CPriResManageWnd::SetToolTipsDelay(DWORD dwDelay)
{
	m_ctlRecordsList.SetToolTipsDelay(dwDelay);
}

BOOL CPriResManageWnd::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	InitWindowStyles(this);

	m_ctlRecordsList.Init();
	m_ctlClassInfoTree.Initalize(&m_ctlRecordsList);
	m_ctlFilter_Record.OnInit(E_EFT_RECORD);

	RECT rcSpl;
	m_ctlClassInfoTree.GetWindowRect(&rcSpl);
	ScreenToClient(&rcSpl);

	CRect rcFilter_Record;
	m_ctlFilter_Record.GetWindowRect(rcFilter_Record);
	ScreenToClient(rcFilter_Record);

	CRect rcFiles;
	m_ctlRecordsList.GetWindowRect(rcFiles);
	ScreenToClient(rcFiles);
	VERIFY( m_dlgDetails.Create(this, DS_CONTROL | DS_SETFONT | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CONTROLPARENT) );
	m_dlgDetails.SetWindowPos(NULL, rcFiles.left - 6, rcFiles.bottom - 2, rcFiles.Width() + 12, rcSpl.bottom + 7 - (rcFiles.bottom - 2), 0);
	AddAnchor(m_dlgDetails, BOTTOM_LEFT, BOTTOM_RIGHT);

	m_dlgDetails.SetModuleType(EMT_PRI);
	rcSpl.left = rcSpl.right + SPLITTER_MARGIN;
	rcSpl.right = rcSpl.left + SPLITTER_WIDTH;
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rcSpl, this, IDC_SPLITTER_PRIRESMANAGE);

	AddAnchor(IDC_BTN_HIDESHOWDETAILS_PRI, BOTTOM_RIGHT);

	AddAnchor(m_wndSplitter, TOP_LEFT);
	AddAnchor(m_ctlFilter_Record, TOP_LEFT);
	AddAnchor(m_ctlRecordsList, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(m_ctlClassInfoTree, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(m_ctlFilter, TOP_LEFT);

	int iPosStatInit = rcSpl.left;
	int iPosStatNew = thePrefs.GetSplitterbarPositionShared();
	if (iPosStatNew > SPLITTER_RANGE_MAX)
		iPosStatNew = SPLITTER_RANGE_MAX;
	else if (iPosStatNew < SPLITTER_RANGE_MIN)
		iPosStatNew = SPLITTER_RANGE_MIN;
	rcSpl.left = iPosStatNew;
	rcSpl.right = iPosStatNew + SPLITTER_WIDTH;
	if (iPosStatNew != iPosStatInit)
	{
		m_wndSplitter.MoveWindow(&rcSpl);
		DoResize(iPosStatNew - iPosStatInit);
	}

	GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetFont(&theApp.m_fontSymbol);
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetWindowText(_T("6"));
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->BringWindowToTop();

	ShowDetailsPanel(1);//thePrefs.GetShowSharedFilesDetails()
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPriResManageWnd::DoResize(int iDelta)
{
	if(!iDelta)
		return;

	CSplitterControl::ChangeWidth(&m_ctlClassInfoTree, iDelta);
	CSplitterControl::ChangeWidth(&m_ctlFilter, iDelta);
	CSplitterControl::ChangePos(&m_ctlFilter_Record, -iDelta, 0);
	CSplitterControl::ChangeWidth(&m_ctlFilter_Record, -iDelta, 0);
	CSplitterControl::ChangePos(&m_ctlRecordsList, -iDelta, 0);
	CSplitterControl::ChangeWidth(&m_ctlRecordsList, -iDelta);
	bool bAntiFlicker = m_dlgDetails.IsWindowVisible() == TRUE;
	if (bAntiFlicker)
		m_dlgDetails.SetRedraw(FALSE);
	CSplitterControl::ChangePos(&m_dlgDetails, -iDelta, 0);
	CSplitterControl::ChangeWidth(&m_dlgDetails, -iDelta);
	if (bAntiFlicker)
		m_dlgDetails.SetRedraw(TRUE);



	RECT rcSpl;
	m_wndSplitter.GetWindowRect(&rcSpl);
	ScreenToClient(&rcSpl);
	thePrefs.SetSplitterbarPositionShared(rcSpl.left);

	RemoveAnchor(m_wndSplitter);
	AddAnchor(m_wndSplitter, TOP_LEFT);

	RemoveAnchor(m_ctlRecordsList);
	RemoveAnchor(m_ctlClassInfoTree);
	RemoveAnchor(m_ctlFilter);
	RemoveAnchor(m_ctlFilter_Record);
	RemoveAnchor(m_dlgDetails);

	AddAnchor(m_ctlFilter_Record, TOP_LEFT);
	AddAnchor(m_ctlRecordsList, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(m_ctlClassInfoTree, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(m_ctlFilter, TOP_LEFT);
	AddAnchor(m_dlgDetails, BOTTOM_LEFT, BOTTOM_RIGHT);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);
	m_wndSplitter.SetRange(rcWnd.left + SPLITTER_RANGE_MIN, rcWnd.left + SPLITTER_RANGE_MAX);

	Invalidate();
	UpdateWindow();
}

void CPriResManageWnd::Localize()// X: [RUL] - [Remove Useless Localize]
{
	m_ctlClassInfoTree.Localize();
	//m_ctlRecordsList.SetDirectoryFilter(NULL,true);

	//SetDlgItemText(IDC_RELOADSCLASSINFOS,GetResString(IDS_SRM_RELOAD));
	SetDlgItemText(IDC_STATIC_PROP_CLASSSEL_PRI,GetResString(IDS_PROP_SELBYCTL));

}
void CPriResManageWnd::LocalizeAll()// X: [RUL] - [Remove Useless Localize]
{
	Localize();
	m_ctlRecordsList.Localize();
	m_dlgDetails.LocalizeAll();
}
void CPriResManageWnd::ShowDetailsPanel(bool bShow)
{

	if (bShow == m_bDetailsVisible)
		return;
	m_bDetailsVisible = bShow;
	thePrefs.SetShowSharedFilesDetails(bShow);
	RemoveAnchor(m_ctlRecordsList);
	RemoveAnchor(IDC_BTN_HIDESHOWDETAILS_PRI);

	CRect rcFile, rcDetailDlg, rcButton;
	m_ctlRecordsList.GetWindowRect(rcFile);
	m_dlgDetails.GetWindowRect(rcDetailDlg);
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->GetWindowRect(rcButton);
	ScreenToClient(rcButton);
	const int nOffset = 29;
	if (!bShow)
	{
		m_ctlRecordsList.SetWindowPos(NULL, 0, 0, rcFile.Width(), rcFile.Height() + (rcDetailDlg.Height() - nOffset), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetWindowPos(NULL, rcButton.left, rcButton.top + (rcDetailDlg.Height() - nOffset), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		m_dlgDetails.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetWindowText(_T("5"));
	}
	else
	{
		m_ctlRecordsList.SetWindowPos(NULL, 0, 0, rcFile.Width(), rcFile.Height() - (rcDetailDlg.Height() - nOffset), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetWindowPos(NULL, rcButton.left, rcButton.top - (rcDetailDlg.Height() - nOffset), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		m_dlgDetails.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS_PRI)->SetWindowText(_T("6"));

	}
	m_ctlRecordsList.SetFocus();
	AddAnchor(m_ctlRecordsList, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_HIDESHOWDETAILS_PRI, BOTTOM_RIGHT);
	ShowSelectedFilesDetails();

}


void CPriResManageWnd::ShowSelectedFilesDetails(bool bForce) 
{
	CAtlList<CRecordItem*> selectedList;
	bool m_bChanged = false;
	if (m_bDetailsVisible)
	{
		CMuleListCtrl*listctrl = (CMuleListCtrl*)&m_ctlRecordsList; 
		POSITION pos = listctrl->GetFirstSelectedItemPosition();
		int i = 0;
		while (pos != NULL)
		{
			INT_PTR index = listctrl->GetNextSelectedItem(pos);
			if (index >= 0)
			{
				CRecordItem* record = (CRecordItem*)listctrl->GetItemData(index);
				if (record != NULL)
				{
					selectedList.AddTail(record);
					m_bChanged |= m_dlgDetails.GetItems().GetSize() <= i || m_dlgDetails.GetItems()[i] != record;
					i++;
				}
			}
		}
	}
	m_bChanged |= m_dlgDetails.GetItems().GetSize() != selectedList.GetCount();

	if (m_bChanged || bForce)
		m_dlgDetails.SetFiles(selectedList);
}


BOOL CPriResManageWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Don't handle Ctrl+Tab in this window. It will be handled by main window.
		if (pMsg->wParam == VK_TAB && GetAsyncKeyState(VK_CONTROL) < 0)
			return FALSE;
		if (pMsg->wParam == VK_ESCAPE)
			return FALSE;
	}
	else if (pMsg->message == WM_KEYUP)
	{
		ShowSelectedFilesDetails();
	}
	else if (/*!thePrefs.GetStraightWindowStyles() && */pMsg->message == WM_MBUTTONUP)
	{
		m_ctlRecordsList.ShowSelectedFileComments();
		return TRUE;
	}

	return CResizableDialog::PreTranslateMessage(pMsg);
}


void CPriResManageWnd::OnLvnItemchangedPrmRecordlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	ShowSelectedFilesDetails();	*pResult = 0;
}


LRESULT CPriResManageWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CResizableDialog::WindowProc(message, wParam, lParam);
}


LRESULT CPriResManageWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_PAINT:
		if (m_wndSplitter)
		{
			CRect rcWnd;
			GetWindowRect(rcWnd);
			if (rcWnd.Width() > 0)
			{
				RECT rcSpl;
				m_ctlClassInfoTree.GetWindowRect(&rcSpl);
				ScreenToClient(&rcSpl);
				rcSpl.left = rcSpl.right + SPLITTER_MARGIN;
				rcSpl.right = rcSpl.left + SPLITTER_WIDTH;

				RECT rcFilter;
				m_ctlFilter.GetWindowRect(&rcFilter);
				ScreenToClient(&rcFilter);
				rcSpl.top = rcFilter.top;
				m_wndSplitter.MoveWindow(&rcSpl, TRUE);
			}
		}
		break;

	case WM_NOTIFY:
		if (wParam == IDC_SPLITTER_PRIRESMANAGE)
		{ 
			SPC_NMHDR* pHdr = (SPC_NMHDR*)lParam;
			DoResize(pHdr->delta);
		}
		break;

	case WM_SIZE:
		if (m_wndSplitter)
		{
			CRect rcWnd;
			GetWindowRect(rcWnd);
			ScreenToClient(rcWnd);
			m_wndSplitter.SetRange(rcWnd.left + SPLITTER_RANGE_MIN, rcWnd.left + SPLITTER_RANGE_MAX);
		}
		break;
	}
	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CPriResManageWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (wParam)
	{
	case IDC_BTN_HIDESHOWDETAILS_PRI:
		ShowDetailsPanel(!m_bDetailsVisible);
		break;

	default:
		{
			ShowSelectedFilesDetails();
			break;
		}
	}

	//return TRUE;
	return __super::OnCommand(wParam, lParam);
	//return CResizableDialog::OnCommand(wParam, lParam);
}
