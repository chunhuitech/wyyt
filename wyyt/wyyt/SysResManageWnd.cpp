// SysResManageWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "SysResManageWnd.h"
#include "wyytDlg.h"
#include "OtherFunctions.h"
#include "HighColorTab.hpp"
#include "ArchivePreviewDlg.h"
#include "AttachsDlg.h"
#include "UserMsgs.h"
#include "preferences.h"
#include "TestDlg.h"


// CSysResManageWnd 对话框

IMPLEMENT_DYNAMIC(CSysResManageWnd, CDialog)

CSysResManageWnd::CSysResManageWnd(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSysResManageWnd::IDD, pParent),m_dlgDetails(EMT_SYS)
{
	m_bDetailsVisible = true;
	m_TestDlg = new CTestDlg;
	//m_nFilterColumn = -1;
	m_dlgDetails.SetModuleType(EMT_SYS);
	
}

CSysResManageWnd::~CSysResManageWnd()
{
	delete m_TestDlg;
}

void CSysResManageWnd::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SRM_CLASSINFOTREE, m_ctlClassInfoTree);
	DDX_Control(pDX, IDC_SRM_FILTER, m_ctlFilter);
	DDX_Control(pDX, IDC_SRM_RECORDLIST, m_ctlRecordsList);
	DDX_Control(pDX, IDC_SRM_RECORD_FILTER, m_ctlFilter_Record);
}


BEGIN_MESSAGE_MAP(CSysResManageWnd, CResizableDialog)
	ON_MESSAGE(UM_DELAYED_EVALUATE, OnChangeFilter)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SRM_CLASSINFOTREE, &CSysResManageWnd::OnTvnSelchangedSrmClassinfotree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SRM_RECORDLIST, OnLvnItemchangedSflist)

	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSysResManageWnd 消息处理程序


BOOL CSysResManageWnd::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	InitWindowStyles(this);

	m_ctlRecordsList.Init();
	//m_ctlRecordsList.dlg = this;
	//m_ctlFilter.OnInit(m_ctlRecordsList);
	m_ctlClassInfoTree.Initalize(&m_ctlRecordsList);
	/*if (thePrefs.GetUseSystemFontForMainControls())
	m_ctlClassInfoTree.SendMessage(WM_SETFONT, NULL, FALSE);*/

	//m_ctlFilter.OnInit();
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
	m_dlgDetails.SetModuleType(EMT_SYS);
	rcSpl.left = rcSpl.right + SPLITTER_MARGIN;
	rcSpl.right = rcSpl.left + SPLITTER_WIDTH;
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rcSpl, this, IDC_SPLITTER_SYSRESMANAGE);

	AddAnchor(IDC_BTN_HIDESHOWDETAILS, BOTTOM_RIGHT);

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

	GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetFont(&theApp.m_fontSymbol);
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetWindowText(_T("6"));
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->BringWindowToTop();

	ShowDetailsPanel(1);//thePrefs.GetShowSharedFilesDetails()
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSysResManageWnd::DoResize(int iDelta)
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
void CSysResManageWnd::UpdateFilterLabel(){
	//m_btnWndS->SetBtnText(MP_VIEWS_FILTER, GetFilterLabel());  //自定义控件
}
void CSysResManageWnd::Localize()// X: [RUL] - [Remove Useless Localize]
{
	m_ctlClassInfoTree.Localize();
	m_ctlRecordsList.SetDirectoryFilter(NULL,true);
	
	SetDlgItemText(IDC_RELOADSCLASSINFOS,GetResString(IDS_SRM_RELOAD));
	SetDlgItemText(IDC_STATIC_PROP_CLASSSEL,GetResString(IDS_PROP_SELBYCTL));
	
}
void CSysResManageWnd::LocalizeAll()// X: [RUL] - [Remove Useless Localize]
{
	Localize();
	//LocalizeToolbars();
	m_ctlRecordsList.Localize();
	//historylistctrl.Localize(); //Xman [MoNKi: -Downloaded History-]
	m_dlgDetails.LocalizeAll();
	//if(m_ctlFilter.m_strLastEvaluatedContent.GetLength() == 0 && m_ctlFilter.GetFocus()!=(CWnd*)&m_ctlFilter)
	//	m_ctlFilter.ShowColumnText(true,true);
}


LRESULT CSysResManageWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
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
		// NEO: AKF - [AllKnownFiles] -- Xanatos -->
		// Workaround to solve a glitch with WM_SETTINGCHANGE message
		/*if (m_btnWndS && m_btnWndS->m_hWnd){
		if(thePrefs.IsTransToolbarEnabled()){
		if(m_btnWndS->GetBtnWidth(IDC_FILES_ICO) != WNDS_BUTTON_WIDTH)
		m_btnWndS->SetBtnWidth(IDC_FILES_ICO, WNDS_BUTTON_WIDTH);
		}
		else{
		if(m_btnWndS->GetBtnWidth(IDC_FILES_ICO) != WNDS_BUTTON_WIDTH+WNDS_BUTTON3_WIDTH)
		m_btnWndS->SetBtnWidth(IDC_FILES_ICO, WNDS_BUTTON_WIDTH+WNDS_BUTTON3_WIDTH);
		}
		}*/
		// NEO: AKF END <-- Xanatos --
		break;

	case WM_NOTIFY:
		if (wParam == IDC_SPLITTER_SYSRESMANAGE)
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
	//return __super::DefWindowProc(message, wParam, lParam);
}
void CSysResManageWnd::SetToolTipsDelay(DWORD dwDelay)
{
	m_ctlRecordsList.SetToolTipsDelay(dwDelay);
}
void CSysResManageWnd::Reload(bool bForceTreeReload)
{	
	m_ctlClassInfoTree.Reload(bForceTreeReload); // force a reload of the tree to update

	//m_ctlRecordsList.SetDirectoryFilter(NULL, false); //设置初始目录　加载数据
	//m_ctlClassInfoTree.Reload(bForceTreeReload); // force a reload of the tree to update the 'accessible' state of each directory
	//m_ctlRecordsList.SetDirectoryFilter(m_ctlClassInfoTree.GetSelectedFilter(), false);
	//theApp.sharedfiles->Reload(thePrefs.queryOnHashing!=1);// X: [QOH] - [QueryOnHashing]

	//ShowSelectedFilesDetails();
}

void CSysResManageWnd::ShowDetailsPanel(bool bShow)
{
	
	if (bShow == m_bDetailsVisible)
		return;
	m_bDetailsVisible = bShow;
	thePrefs.SetShowSharedFilesDetails(bShow);
	RemoveAnchor(m_ctlRecordsList);
	RemoveAnchor(IDC_BTN_HIDESHOWDETAILS);

	CRect rcFile, rcDetailDlg, rcButton;
	m_ctlRecordsList.GetWindowRect(rcFile);
	m_dlgDetails.GetWindowRect(rcDetailDlg);
	GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->GetWindowRect(rcButton);
	ScreenToClient(rcButton);
	const int nOffset = 29;
	if (!bShow)
	{
		m_ctlRecordsList.SetWindowPos(NULL, 0, 0, rcFile.Width(), rcFile.Height() + (rcDetailDlg.Height() - nOffset), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetWindowPos(NULL, rcButton.left, rcButton.top + (rcDetailDlg.Height() - nOffset), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		m_dlgDetails.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetWindowText(_T("5"));
	}
	else
	{
		m_ctlRecordsList.SetWindowPos(NULL, 0, 0, rcFile.Width(), rcFile.Height() - (rcDetailDlg.Height() - nOffset), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetWindowPos(NULL, rcButton.left, rcButton.top - (rcDetailDlg.Height() - nOffset), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
		m_dlgDetails.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_HIDESHOWDETAILS)->SetWindowText(_T("6"));
		
	}
	m_ctlRecordsList.SetFocus();
	AddAnchor(m_ctlRecordsList, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_HIDESHOWDETAILS, BOTTOM_RIGHT);
	ShowSelectedFilesDetails();
	
}

void CSysResManageWnd::ShowSelectedFilesDetails(bool bForce) //Xman [MoNKi: -Downloaded History-]
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
	/*
	CAtlList<CShareableFile*> selectedList;
	bool m_bChanged = false;
	if (m_bDetailsVisible)
	{
		CMuleListCtrl*listctrl = m_ShowAllKnow?(CMuleListCtrl*)&historylistctrl:(CMuleListCtrl*)&sharedfilesctrl; // X: AKF - [AllKnownFiles] <-- Xanatos --
		POSITION pos = listctrl->GetFirstSelectedItemPosition();
		int i = 0;
		while (pos != NULL)
		{
			INT_PTR index = listctrl->GetNextSelectedItem(pos);
			if (index >= 0)
			{
				CShareableFile* file = (CShareableFile*)listctrl->GetItemData(index);
				if (file != NULL)
				{
					selectedList.AddTail(file);
					m_bChanged |= m_dlgDetails.GetItems().GetSize() <= i || m_dlgDetails.GetItems()[i] != file;
					i++;
				}
			}
		}
	}
	m_bChanged |= m_dlgDetails.GetItems().GetSize() != selectedList.GetCount();

	if (m_bChanged || bForce)
		m_dlgDetails.SetFiles(selectedList);
		*/
}


LRESULT CSysResManageWnd::OnChangeFilter(WPARAM wParam, LPARAM lParam)
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
////////////////////////////////
	//CFilterItem*fi;
	//if(m_ShowAllKnow)
	//	fi=&historylistctrl;
	//else
	//	fi=&sharedfilesctrl;
	//bool bColumnDiff = (fi->m_nFilterColumn != (uint32)wParam);
	//fi->m_nFilterColumn = wParam;

	//CAtlArray<CString> astrFilter;
	//CString strFullFilterExpr = (LPCTSTR)lParam;
	//int iPos = 0;
	//CString strFilter(strFullFilterExpr.Tokenize(_T(" "), iPos));
	//while (!strFilter.IsEmpty()) {
	//	if (strFilter != _T('-'))
	//		astrFilter.Add(strFilter);
	//	strFilter = strFullFilterExpr.Tokenize(_T(" "), iPos);
	//}

	//CAtlArray<CString>& m_astrFilter=fi->m_astrFilter; // X: AKF - [AllKnownFiles] <-- Xanatos --
	//bool bFilterDiff = (astrFilter.GetCount() != m_astrFilter.GetCount());
	//if (!bFilterDiff) {
	//	for (size_t i = 0; i < astrFilter.GetCount(); i++) {
	//		if (astrFilter[i] != m_astrFilter[i]) {
	//			bFilterDiff = true;
	//			break;
	//		}
	//	}
	//}

	//if (!bColumnDiff && !bFilterDiff)
	//	return 0;
	//m_astrFilter.RemoveAll();
	//m_astrFilter.Append(astrFilter);
	//fi->ReloadFileList(); // X: AKF - [AllKnownFiles] <-- Xanatos --
	return 0;
}

BOOL CSysResManageWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (wParam)
	{
	case IDC_RELOADSCLASSINFOS:
		{
			//CTestDlg dlg;
			//dlg.DoModal();
			DialogCreateIndirect(m_TestDlg, IDD_DIALOG_TEST);
			m_TestDlg->ShowWindow(SW_SHOW);
			m_TestDlg->SetFocus();
			break;
			CWaitCursor curWait;
			Reload(true);
			break;
		}
	case IDC_BTN_HIDESHOWDETAILS:
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
}


BOOL CSysResManageWnd::PreTranslateMessage(MSG* pMsg)
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


void CSysResManageWnd::OnTvnSelchangedSrmClassinfotree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//m_ctlRecordsList.SetDirectoryFilter(m_ctlClassInfoTree.GetSelectedFilter(), !m_ctlClassInfoTree.IsCreatingTree());
	//m_ctlRecordsList.SetClassFilter(m_ctlClassInfoTree.GetSelectedClass(), !m_ctlClassInfoTree.IsCreatingTree());
	
	*pResult = 0;
}

void CSysResManageWnd::OnLvnItemchangedSflist(NMHDR* /*pNMHDR*/, LRESULT *pResult){	ShowSelectedFilesDetails();	*pResult = 0;}

void CSysResManageWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	__super::OnClose();
}
