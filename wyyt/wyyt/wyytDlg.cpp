/** \file wyytDlg.cpp
******************************************************************************** 
<PRE> 
模块名 : <wyyt> 
文件名 : <wyytDlg.cpp> 
相关文件 : < 与此文件相关的其它文件 > 
文件实现功能 :主窗口
 * 1- 
 * 2- 
作者 : <IHAS> 
版本 : <1.0> 
版权 : Copyright (c) 2012 IHAS
创建日期 : <2012/07/29>
-------------------------------------------------------------------------------- 
备注 : < 其它说明 > 
-------------------------------------------------------------------------------- 
修改记录 : 
日 期         版本           修改人           修改内容 
YYYY/MM/DD    X.Y        <IHAS>      < 修改内容 > 
</PRE> 
*******************************************************************************/


// wyytDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "wyytDlg.h"
#include "MenuCmds.h"
#include <afxinet.h>

#include "MuleStatusbarCtrl.h"
#include "MuleToolbarCtrl.h"
#include "Preferences.h"

#include "SysResManageWnd.h"
#include "GroupResManageWnd.h"
#include "PriResManageWnd.h"
#include "OtherFunctions.h"
#include "PreferencesDlg.h"
#include "RecordInfoSheetDlg.h"
#include "CheckPwdDlg.h"
#include "XMessageBox.h"

#include "WyytSystrayDlg.h"
#include "DonateDlg.h"
#include "jsoncpp-0.10.7/include/json/json.h"
#include "htmlcxx-0.86/html/ParserDom.h"
#include <string>
#include "WininetHttp.h"
#include "GlobalFunction.h"
#include "StringConversion.h"
#include "PCInfo.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <istream>
#include <sstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <Nb30.h>

using namespace htmlcxx;

#define	SYS_TRAY_ICON_COOKIE_FORCE_UPDATE	(UINT)-1

//DWORD dwHttpRequestFlags_DL =
//	INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
//
//DWORD   dwAccessType_DL = PRE_CONFIG_INTERNET_ACCESS;

const TCHAR szHeaders_DL[] =
	_T("Accept: text/*\r\nUser-Agent: MFC_Tear_Sample\r\n");


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()





// Barry - To find out if app is running or shutting/shut down
bool CwyytDlg::IsRunning()
{
	return (theApp.m_app_state == APP_STATE_RUNNING);
}

// CwyytDlg 对话框
CwyytDlg::CwyytDlg(CWnd* pParent /*=NULL*/)
	: CTrayDialog(CwyytDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = NULL;
	activewnd = NULL;

	toolbar = new CMuleToolbarCtrl;

	statusbar = new CMuleStatusBarCtrl;

	sysresmanagewnd = new CSysResManageWnd;
	groupresmanagewnd = new CGroupResManageWnd;
	priresmanagewnd = new CPriResManageWnd;
	preferenceswnd = new CPreferencesDlg;
	//m_pRecInfPPSheetDlg = new CRecordInfoSheetDlg;

	sharedfileswnd = new CWnd;
	searchwnd = new CWnd;
	chatwnd = new CWnd;
	statisticswnd = new CWnd;
	ircwnd = new CWnd;


	m_uLastSysTrayIconCookie = SYS_TRAY_ICON_COOKIE_FORCE_UPDATE;
	
	m_icoSysTrayConnected = NULL;
	m_icoSysTrayDisconnected = NULL;
	m_icoSysTrayLowID = NULL;
	m_icoSysTrayCurrent = NULL;

	theApp.m_app_state = APP_STATE_RUNNING;


	m_pSystrayDlg = NULL;
}
CwyytDlg::~CwyytDlg()
{
	delete preferenceswnd;
	delete sysresmanagewnd;
	delete groupresmanagewnd;
	delete priresmanagewnd;
	delete sharedfileswnd;
	delete searchwnd;
	delete chatwnd;
	delete statisticswnd;
	delete ircwnd;
	//delete m_pRecInfPPSheetDlg;

	delete toolbar;
	delete statusbar;

	if (m_icoSysTrayConnected) VERIFY( ::DestroyIcon(m_icoSysTrayConnected) );
	if (m_icoSysTrayDisconnected) VERIFY( ::DestroyIcon(m_icoSysTrayDisconnected) );
	if (m_icoSysTrayLowID) VERIFY( ::DestroyIcon(m_icoSysTrayLowID) );

	if (m_hIcon) VERIFY( ::DestroyIcon(m_hIcon) );
	if (m_icoSysTrayCurrent) VERIFY( DestroyIcon(m_icoSysTrayCurrent) );
}


void CwyytDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwyytDlg, CTrayDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MENUCHAR()

	///////////////////////////////////////////////////////////////////////////
	// WM_COMMAND messages
	//
	ON_COMMAND(MP_EXIT, OnClose)
	ON_COMMAND(MP_RESTORE, RestoreWindow)

	// quick-speed changer -- based on xrmb	
	ON_NOTIFY_EX_RANGE(RBN_CHEVRONPUSHED, 0, 0xFFFF, OnChevronPushed)

	ON_MESSAGE(TM_CONSOLETHREADEVENT, OnConsoleThreadEvent)

END_MESSAGE_MAP()


void DialogCreateIndirect(CDialog *pWnd, UINT uID)
{
#if 0
	// This could be a nice way to change the font size of the main windows without needing
	// to re-design the dialog resources. However, that technique does not work for the
	// SearchWnd and it also introduces new glitches (which would need to get resolved)
	// in almost all of the main windows.
	CDialogTemplate dlgTempl;
	dlgTempl.Load(MAKEINTRESOURCE(uID));
	dlgTempl.SetFont(_T("MS Shell Dlg"), 8);
	pWnd->CreateIndirect(dlgTempl.m_hTemplate);
	FreeResource(dlgTempl.Detach());
#else
	pWnd->Create(uID);
#endif
}
// CwyytDlg 消息处理程序

BOOL CwyytDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	theApp.CreateAllFonts();

	CreateToolbarCmdIconMap();

	CWnd* pwndToolbarX = toolbar;
	if (toolbar->Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, IDC_TOOLBAR))
	{
		toolbar->Init();
		if (thePrefs.GetUseReBarToolbar())
		{
			if (m_ctlMainTopReBar.Create(WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
				RBS_BANDBORDERS | RBS_AUTOSIZE | CCS_NODIVIDER, 
				CRect(0, 0, 0, 0), this, AFX_IDW_REBAR))
			{
				CSize sizeBar;
				VERIFY( toolbar->GetMaxSize(&sizeBar) );
				REBARBANDINFO rbbi = {0};
				rbbi.cbSize = sizeof(REBARBANDINFO);//sizeof(rbbi);
				rbbi.fMask = RBBIM_STYLE | RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_ID;
				rbbi.fStyle = RBBS_NOGRIPPER | RBBS_BREAK | RBBS_USECHEVRON;
				rbbi.hwndChild = toolbar->m_hWnd;
				rbbi.cxMinChild = sizeBar.cy;
				rbbi.cyMinChild = sizeBar.cy;
				rbbi.cxIdeal = sizeBar.cx;
				rbbi.cx = rbbi.cxIdeal;
				rbbi.wID = 0;

				////m_ctlMainTopReBar.GetBandInfo(1, &rbbi);
				////m_ctlMainTopReBar.InsertBand(2, &rbbi);   

				VERIFY( m_ctlMainTopReBar.InsertBand((UINT)-1, &rbbi) );
				//同等写法
				//int iresult=::SendMessage(m_ctlMainTopReBar.m_hWnd,RB_INSERTBAND,(WPARAM)-1,(LPARAM)&rbbi); 
				//if(!iresult) 
				//{ 
				//	MessageBox( L"SendMessage   rb_insertband   err ", L"err ",MB_OK); 
				//} 

				///////////////////////////////测试///////////////////////////////////////////
				/*	LPVOID lpMsgBuf;
				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,0,GetLastError(),
				MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
				CString csErrorInfo;
				csErrorInfo.Format(L"%d:%s",GetLastError(),lpMsgBuf);
				MessageBox(csErrorInfo,_T("GetLastError"),MB_OK | MB_ICONINFORMATION);
				//MessageBox((LPTSTR)lpMsgBuf,_T("GetLastError"),MB_OK | MB_ICONINFORMATION);
				::LocalFree(lpMsgBuf);//无效的窗口句柄*/
				////////////////////////////////////
				toolbar->SaveCurHeight();
				toolbar->UpdateBackground();

				pwndToolbarX = &m_ctlMainTopReBar;
			}
		}
	}
	// set title
	//SetWindowText(_T("eMule v") + theApp.m_strCurVersionLong + _T(" ") MOD_VERSION); // Maella -Support for tag ET_MOD_VERSION 0x55
	SetWindowText(SZ_FULL_VERSION);


	// set statusbar
	// the statusbar control is created as a custom control in the dialog resource,
	// this solves font and sizing problems when using large system fonts
	statusbar->SubclassWindow(GetDlgItem(IDC_STATUSBAR)->m_hWnd);
	statusbar->EnableToolTips(true);
	SetStatusBarPartsSize();

	DialogCreateIndirect(sysresmanagewnd, IDD_DIALOG_SYS_RES_MANAGE);
	DialogCreateIndirect(groupresmanagewnd, IDD_DIALOG_GROUP_RES_MANAGER);
	DialogCreateIndirect(priresmanagewnd, IDD_DIALOG_PRI_RES_MANAGE);

	// if still no active window, activate server window
	if (activewnd == NULL)
		SetActiveDialog(sysresmanagewnd);

	SetAllIcons();
	CString strVerinfo;
	//strVerinfo.Format(L"版本:当前数据%s,最新数据%s,程序最新版本:%s",L"1.0",L"2.0",L"2.0.0.1");
	if ( wyytExtendDataManage::Instance().m_vecSysDataVerInfos.size() > 0 )
	{
		strVerinfo.Format(GetResString(IDS_INFO_VERSIONS), wyytExtendDataManage::Instance().m_vecSysDataVerInfos[0].Version.c_str(),thePrefs.m_strNewDataVerInfo);
	}
	else
	{
		strVerinfo.Format(GetResString(IDS_INFO_VERSIONS),L"1.0",L"2.0");
	}
	
	AddLogText(LOG_STATUSBAR,strVerinfo);
	// adjust all main window sizes for toolbar height and maximize the child windows
	CRect rcClient, rcToolbar, rcStatusbar;
	GetClientRect(&rcClient);
	pwndToolbarX->GetWindowRect(&rcToolbar);
	statusbar->GetWindowRect(&rcStatusbar);
	rcClient.top += rcToolbar.Height();
	rcClient.bottom -= rcStatusbar.Height();

	CWnd* apWnds[] =
	{
		sysresmanagewnd,
		groupresmanagewnd,
		priresmanagewnd
	};
	for (int i = 0; i < _countof(apWnds); i++)
		apWnds[i]->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER);
	

	// anchors
	AddAnchor(*sysresmanagewnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*groupresmanagewnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*priresmanagewnd,		TOP_LEFT, BOTTOM_RIGHT);
	/*AddAnchor(*serverwnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*kademliawnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*transferwnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*sharedfileswnd,	TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*searchwnd,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*chatwnd,			TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*ircwnd,			TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*statisticswnd,	TOP_LEFT, BOTTOM_RIGHT);*/
	AddAnchor(*pwndToolbarX,	TOP_LEFT, TOP_RIGHT); //这里不加上窗口大小变化时工具栏不会跟着变化
	AddAnchor(*statusbar,		BOTTOM_LEFT, BOTTOM_RIGHT); //这里不加上窗口大小变化时状态栏不会跟着变化


		///////////////////////////////////////////////////////////////////////////
	// Restore saved window placement
	//
	WINDOWPLACEMENT wp = {0};
	wp.length = sizeof(wp);
	wp = thePrefs.GetEmuleWindowPlacement();
//	if (m_bStartMinimized)
//	{
	
//	}
//	else
//	{
		// Allow only SW_SHOWNORMAL and SW_SHOWMAXIMIZED. Ignore SW_SHOWMINIMIZED to make sure the window
		// becomes visible. If user wants SW_SHOWMINIMIZED, we already have an explicit option for this (see above).
		if (wp.showCmd != SW_SHOWMAXIMIZED)
			wp.showCmd = SW_SHOWNORMAL;
	//	m_bStartMinimizedChecked = true;
//	}
	SetWindowPlacement(&wp);

	UploadMachineInformation();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CwyytDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	//else
	//{
	//	CTrayDialog::OnSysCommand(nID, lParam);
	//}

	switch (nID & 0xFFF0)
	{
	case MP_ABOUTBOX: 
	case IDM_ABOUTBOX:
		{
			//CCreditsDlg dlgAbout;
			//dlgAbout.DoModal();
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
			break;
		}
	case MP_VERSIONCHECK:
		//DoVersioncheck(true);
		break;

		//X version check
	case MP_MVERSIONCHECK:
		//DoMVersioncheck(true);
		break;

	case MP_CONNECT:
		//StartConnection();
		break;
	case MP_DISCONNECT:
		//CloseConnection();
		break;
	default:
		CTrayDialog::OnSysCommand(nID, lParam);
		break;
	}

	if ((nID & 0xFFF0) == SC_MINIMIZE		||
		(nID & 0xFFF0) == MP_MINIMIZETOTRAY	||
		(nID & 0xFFF0) == SC_RESTORE		||
		(nID & 0xFFF0) == SC_MAXIMIZE)
	{
		ShowTransferRate(true);

		//AddLogText(LOG_STATUSBAR,L"当前版本 最新版本");

	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwyytDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTrayDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwyytDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwyytDlg::OnTrayRButtonUp(CPoint pt)
{
	if (!IsRunning())
		return;

	//if (m_pMiniMule)
	//{
	//	if (thePrefs.m_bMiniMuleAutoClose)
	//	{
	//		TRACE("%s - thePrefs.m_bMiniMuleAutoClose -> DestroyMiniMule();\n", __FUNCTION__);
	//		DestroyMiniMule();
	//	}
	//	else
	//	{
	//		// Avoid reentrancy problems with main window, options dialog and mini mule window
	//		if (m_pMiniMule->m_hWnd && !m_pMiniMule->IsWindowEnabled()) {
	//			MessageBeep(MB_OK);
	//			return;
	//		}
	//	}
	//}

	if (m_pSystrayDlg) {
		m_pSystrayDlg->BringWindowToTop();
		return;
	}

	m_pSystrayDlg = new CWyytSystrayDlg(this, pt,
										(int)/*thePrefs.GetMaxGraphUploadRate()*/0, (int)/*thePrefs.GetMaxGraphDownloadRate()*/0,
										(int)/*thePrefs.GetMaxUpload()*/0, (int)/*thePrefs.GetMaxDownload()*/0);
	if (m_pSystrayDlg)
	{
		UINT_PTR nResult = m_pSystrayDlg->DoModal();
		delete m_pSystrayDlg;
		m_pSystrayDlg = NULL;
		switch (nResult)
		{
			//case IDC_TOMAX:
			//	QuickSpeedOther();
			//	break;
/*			case IDC_TOMIN:
				QuickSpeedOther(MP_QS_PA);
				break;*/
			case IDC_RESTORE:
				RestoreWindow();
				break;
			//case IDC_CONNECT:
			//	StartConnection();
			//	break;
			//case IDC_DISCONNECT:
			//	CloseConnection();
			//	break;
			case IDC_EXIT:
				OnClose();
				break;
			case IDC_PREFERENCES:
				ShowPreferences();
				break;
			case IDC_ABOUT:
				{
					CAboutDlg dlgAbout;
					dlgAbout.DoModal();
				}				
				break;
		}
	}
}

// Listview helper function
void GetPopupMenuPos(CListCtrl& lv, CPoint& point)
{
	// If the context menu was not opened using the right mouse button,
	// but the keyboard (Shift+F10), get a useful position for the context menu.
	if (point.x == -1 && point.y == -1)
	{
		int iIdxItem = lv.GetNextItem(-1, LVNI_SELECTED | LVNI_FOCUSED);
		if (iIdxItem != -1)
		{
			CRect rc;
			if (lv.GetItemRect(iIdxItem, &rc, LVIR_BOUNDS))
			{
				point.x = rc.left + lv.GetColumnWidth(0) / 2;
				point.y = rc.top + rc.Height() / 2;
				lv.ClientToScreen(&point);
			}
		}
		else
		{
			point.x = 16;
			point.y = 32;
			lv.ClientToScreen(&point);
		}
	}
}

void GetPopupMenuPos(CTreeCtrl& tv, CPoint& point)
{
	// If the context menu was not opened using the right mouse button,
	// but the keyboard (Shift+F10), get a useful position for the context menu.
	if (point.x == -1 && point.y == -1)
	{
		HTREEITEM hSel = tv.GetNextItem(TVI_ROOT, TVGN_CARET);
		if (hSel)
		{
			CRect rcItem;
			if (tv.GetItemRect(hSel, &rcItem, TRUE))
			{
				point.x = rcItem.left;
				point.y = rcItem.top;
				tv.ClientToScreen(&point);
			}
		}
		else
		{
			point.x = 16;
			point.y = 32;
			tv.ClientToScreen(&point);
		}
	}
}
void InitWindowStyles(CWnd* pWnd)
{
	
	//ApplySystemFont(pWnd);
	//if (thePrefs.GetStraightWindowStyles() < 0)
	//	return;
	//	else if (thePrefs.GetStraightWindowStyles() > 0)
	/*StraightWindowStyles(pWnd);*/	// no longer needed
	/*else if (thePrefs.GetStraightWindowStyles() == 0)
	{
		s_bIsXPStyle = g_xpStyle.IsAppThemed() && g_xpStyle.IsThemeActive();
		if (!s_bIsXPStyle)
			FlatWindowStyles(pWnd);
	}*/
}

void CwyytDlg::OnSize(UINT nType, int cx, int cy)
{
	CTrayDialog::OnSize(nType, cx, cy);
	SetStatusBarPartsSize();
	// TODO: 在此处添加消息处理程序代码
}


BOOL CwyytDlg::OnQueryEndSession()
{
	if (!CTrayDialog::OnQueryEndSession())
		return FALSE;

	// TODO:  在此添加专用的查询结束会话代码

	return TRUE;
}


void CwyytDlg::OnEndSession(BOOL bEnding)
{
	CTrayDialog::OnEndSession(bEnding);

	// TODO: 在此处添加消息处理程序代码
}


void CwyytDlg::OnDestroy()
{
	CTrayDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
bool CwyytDlg::CanClose()
{
	if (theApp.m_app_state == APP_STATE_RUNNING && thePrefs.IsConfirmExitEnabled())
	{
		int nResult = XMessageBox(GetSafeHwnd(), GetResString(IDS_MAIN_EXIT), GetResString(IDS_CLOSEWYYT), MB_YESNO | MB_DEFBUTTON2 | MB_DONOTASKAGAIN | MB_ICONQUESTION);
		if ((nResult & MB_DONOTASKAGAIN) > 0)
			thePrefs.SetConfirmExit(false);
		if ((nResult & 0xFF) == IDNO)
			return false;
	}
	return true;
}


void CwyytDlg::OnClose()
{
	if (!CanClose())
		return;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	preferenceswnd->CloseDialog(); // NEO: MLD - [ModelesDialogs] <-- Xanatos --
	// saving data & stuff
	//preferenceswnd->m_wndSecurity.DeleteDDB();
	//m_pRecInfPPSheetDlg->CloseDialog(); 


	// get main window placement
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	GetWindowPlacement(&wp);
	ASSERT( wp.showCmd == SW_SHOWMAXIMIZED || wp.showCmd == SW_SHOWMINIMIZED || wp.showCmd == SW_SHOWNORMAL );
	if (wp.showCmd == SW_SHOWMINIMIZED && (wp.flags & WPF_RESTORETOMAXIMIZED))
		wp.showCmd = SW_SHOWMAXIMIZED;
	wp.flags = 0;
	thePrefs.SetWindowLayout(wp);

	thePrefs.Save();

	//sysresmanagewnd->SaveAllSettings();
	//CTrayDialog::OnClose();//这个托盘不会退出
	CTrayDialog::OnCancel();
}
bool CwyytDlg::IsRecPPDlgOpen() const
{
	return m_pRecInfPPSheetDlg->IsDialogOpen();
}

INT_PTR CwyytDlg::ShowRecPPDlg(int nOpt, UINT uStartPageID)
{
	
	if (IsRecPPDlgOpen())
	{
		m_pRecInfPPSheetDlg->SetForegroundWindow();
		m_pRecInfPPSheetDlg->BringWindowToTop();
		//int ncurOpt = m_RecInfPPSheetDlg->GetCurOperOption();
		m_pRecInfPPSheetDlg->SetCurOperOption(nOpt);
		return -1;
	}
	else
	{
		if (uStartPageID != (UINT)-1)
			m_pRecInfPPSheetDlg->SetStartPage(uStartPageID);
		m_pRecInfPPSheetDlg->OpenDialog(SW_SHOW, FALSE);
		m_pRecInfPPSheetDlg->SetCurOperOption(nOpt);
		return 0;
	}
	
}
void CwyytDlg::RestoreWindow()
{
	// NEO: MLD - [ModelesDialogs] -- Xanatos --
	/*if (IsPreferencesDlgOpen()) {
		MessageBeep(MB_OK);
		preferenceswnd->SetForegroundWindow();
		preferenceswnd->BringWindowToTop();
		return;
	}*/
	if (TrayIsVisible())
		TrayHide();

	/*DestroyMiniMule();

	if (m_wpFirstRestore.length)
	{
	SetWindowPlacement(&m_wpFirstRestore);
	memset(&m_wpFirstRestore, 0, sizeof m_wpFirstRestore);
	SetForegroundWindow();
	BringWindowToTop();
	}
	else
	CTrayDialog::RestoreWindow();
	*/
	CTrayDialog::RestoreWindow();
}


int CwyytDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CTrayDialog::OnCreate(lpCreateStruct);
}


void CwyytDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CTrayDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}


void CwyytDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CTrayDialog::OnSettingChange(uFlags, lpszSection);

	// TODO: 在此处添加消息处理程序代码
}


void CwyytDlg::OnSysColorChange()
{
	CTrayDialog::OnSysColorChange();

	// TODO: 在此处添加消息处理程序代码
}

void CwyytDlg::Localize()
{
	preferenceswnd->Localize();
	sysresmanagewnd->LocalizeAll();
	m_pRecInfPPSheetDlg->Localize();
	//groupresmanagewnd->LocalizeAll();
	//priresmanagewnd->LocalizeAll();
	toolbar->Localize();
}

BOOL CwyytDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(wParam)
	{	
	//case MP_HM_KAD:
	case TBBTN_SYS:
		SetActiveDialog(sysresmanagewnd);
		break;
	case TBBTN_GROUP:
	//case MP_HM_SRVR:
		SetActiveDialog(groupresmanagewnd);
		break;
	case TBBTN_PRIVATE:
	//case MP_HM_TRANSFER:
		{
			if ( !thePrefs.IsPriPwdCheckSupported() )
			{
				CCheckPwdDlg dlgCheck;
				if(dlgCheck.DoModal()  == IDCANCEL)
				{
					break;//return FALSE;
				}
			}
			
			SetActiveDialog(priresmanagewnd);
		}		
		break;
	case TBBTN_OPTIONS:
	case MP_HM_PREFS:
		toolbar->CheckButton(TBBTN_OPTIONS, TRUE);
		ShowPreferences();
		toolbar->CheckButton(TBBTN_OPTIONS, FALSE);
		break;
	case TBBTN_TOOLS:
		ShowToolPopup(true);
		break;

	case MP_HM_LINK1: // MOD: dont remove!
		//ShellExecute(NULL, NULL, L"http://www.emule-project.com", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
		ShellExecute(NULL, NULL, L"http://www.chunhuitech.cn/wyyt/", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT); //http://www.wyyt.net
		break;
	case MP_HM_LINK2:
		//ShellExecute(NULL, NULL, L"http://www.emule-project.com"+ _T("/faq/"), NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
		ShellExecute(NULL, NULL, L"http://www.firemail.wang:8088/forum.php?mod=viewthread&tid=9682", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
		break;
		/*case MP_HM_LINK3: {
		CString theUrl;
		theUrl.Format( thePrefs.GetVersionCheckBaseURL() + _T("/en/version_check.php?version=%i&language=%i"),theApp.m_uCurVersionCheck,thePrefs.GetLanguageID());
		ShellExecute(NULL, NULL, theUrl, NULL, thePrefs.GetMuleDirectory(EMULE_EXECUTEABLEDIR), SW_SHOWDEFAULT);
		break;
		}*/

	case MP_HM_CONSTRUCTION:
		{
			//ShellExecute(NULL, _T("open"), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR),NULL, NULL, SW_SHOW); 
			CString csFileFullName;
			csFileFullName.Format(L"%s\\info.pdf",thePrefs.GetMuleDirectory(WYYT_DBSDIR)/*thePrefs.m_strSysAttachPath*/);
			ShellOpenFile(csFileFullName);
		}
		break;
	case MP_HM_DONATEDE:
		{
			CDonateDlg dlg;
			dlg.DoModal();
		}
		break;
		
	case MP_HM_HELP:
	case TBBTN_HELP:
		{
			//ShellExecute(NULL, _T("open"), thePrefs.GetMuleDirectory(EMULE_INCOMINGDIR),NULL, NULL, SW_SHOW); 
			//CString csFileFullName;
			//csFileFullName.Format(L"%s\\help.pdf",thePrefs.GetMuleDirectory(WYYT_DBSDIR)/*thePrefs.m_strSysAttachPath*/);
			//ShellOpenFile(csFileFullName);
			ShellExecute(NULL, NULL, L"http://www.firemail.wang:8088/forum.php?mod=viewthread&tid=9677", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
		}
		break;
	
	}
	
	

	return CTrayDialog::OnCommand(wParam, lParam);
}


LRESULT CwyytDlg::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CTrayDialog::OnMenuChar(nChar, nFlags, pMenu);
}

bool CwyytDlg::IsPreferencesDlgOpen() const
{
	//return (preferenceswnd->m_hWnd != NULL);
	return preferenceswnd->IsDialogOpen(); // NEO: MLD - [ModelesDialogs] <-- Xanatos --
}


void CwyytDlg::ShowToolPopup(bool toolsonly, bool speedgraph)
{
	POINT point;
	::GetCursorPos(&point);

	CTitleMenu menu;
	menu.CreatePopupMenu();
	menu.AddMenuTitle(speedgraph?NULL:(LPCTSTR)(GetResString(toolsonly?IDS_ABOUT_INFOS:IDS_HOTMENU)), true);

	CTitleMenu Links;
	Links.CreateMenu();
	Links.AddMenuTitle(NULL, true);
	Links.AppendMenu(MF_STRING, MP_HM_LINK1, GetResString(IDS_HM_LINKHP), _T("WEB"));
	Links.AppendMenu(MF_STRING, MP_HM_LINK2, GetResString(IDS_HM_LINKFAQ), _T("WEB"));
	//Links.AppendMenu(MF_STRING, MP_HM_LINK3, GetResString(IDS_HM_LINKVC), _T("WEB"));
	//Links.AppendMenu(MF_SEPARATOR);
	//theWebServices.GetGeneralMenuEntries(&Links);
	//Links.AppendMenu(MF_STRING, MP_WEBSVC_EDIT, GetResString(IDS_WEBSVEDIT));
	
	//menu.AppendMenu(MF_STRING,MP_HM_CONSTRUCTION, GetResString(IDS_CONSTRUCTION), _T("CONSTRUCTION"));
	//menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING|MF_POPUP,(UINT_PTR)Links.m_hMenu, GetResString(IDS_LINKS), _T("WEB") );
	//menu.AppendMenu(MF_SEPARATOR);
	//menu.AppendMenu(MF_STRING,MP_HM_DONATEDE, GetResString(IDS_DONATEDE), _T("WEB"));
	menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	VERIFY( Links.DestroyMenu() );
	VERIFY( menu.DestroyMenu() );
}
INT_PTR CwyytDlg::ShowPreferences(UINT uStartPageID)
{
	if (IsPreferencesDlgOpen())
	{
		preferenceswnd->SetForegroundWindow();
		preferenceswnd->BringWindowToTop();
		return -1;
	}
	else
	{
		if (uStartPageID != (UINT)-1)
			preferenceswnd->SetStartPage(uStartPageID);
		// NEO: NCFG - [NeoConfiguration] -- Xanatos --
		//return preferenceswnd->DoModal();
		// NEO: MLD - [ModelesDialogs] -- Xanatos -->
		preferenceswnd->OpenDialog(SW_SHOW, FALSE);
		return 0;
		// NEO: MLD END <-- Xanatos --
	}
}
int CwyytDlg::GetNextWindowToolbarButton(int iButtonID, int iDirection) const
{
	ASSERT( iDirection == 1 || iDirection == -1 );
	int iButtonCount = toolbar->GetButtonCount();
	if (iButtonCount > 0)
	{
		int iButtonIdx = toolbar->CommandToIndex(iButtonID);
		if (iButtonIdx >= 0 && iButtonIdx < iButtonCount)
		{
			int iEvaluatedButtons = 0;
			while (iEvaluatedButtons < iButtonCount)
			{
				iButtonIdx = iButtonIdx + iDirection;
				if (iButtonIdx < 0)
					iButtonIdx = iButtonCount - 1;
				else if (iButtonIdx >= iButtonCount)
					iButtonIdx = 0;

				TBBUTTON tbbt = {0};
				if (toolbar->GetButton(iButtonIdx, &tbbt))
				{
					if (IsWindowToolbarButton(tbbt.idCommand))
						return tbbt.idCommand;
				}
				iEvaluatedButtons++;
			}
		}
	}
	return -1;
}

bool CwyytDlg::IsWindowToolbarButton(int iButtonID) const
{
	switch (iButtonID)
	{
	case TBBTN_SYS:			return true;
	case TBBTN_GROUP:		return true;
	case TBBTN_PRIVATE:	return true;
	case TBBTN_MESSAGES:	return true;
	case TBBTN_IRC:			return true;
	case TBBTN_SHARED:		return true;
	case TBBTN_SEARCH:		return true;
	case TBBTN_STATS:		return true;
	
	}
	return false;
}

BOOL CwyytDlg::PreTranslateMessage(MSG* pMsg)
{
	//if(pMsg->message == WM_LBUTTONDBLCLK /*WM_NCLBUTTONDBLCLK*/) // 非client区域的双击消息
	//{
	//	//if(pMsg->wParam == HTCAPTION) // 光标所在区域为标题栏
	//	//{
	//		if ( pMsg->hwnd == m_ctlMainTopReBar.m_hWnd)//hwndToolBarDlg解释在下面
	//		{
	//			AfxMessageBox( _T("donow"));
	//			return TRUE;
	//		}
	//	//}
	//}
	BOOL bResult = CTrayDialog::PreTranslateMessage(pMsg);
	
	if (pMsg->message == WM_KEYDOWN)
	{
		// Handle Ctrl+Tab and Ctrl+Shift+Tab
		if (pMsg->wParam == VK_TAB && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			int iButtonID = MapWindowToToolbarButton(activewnd);
			if (iButtonID != -1)
			{
				int iNextButtonID = GetNextWindowToolbarButton(iButtonID, GetAsyncKeyState(VK_SHIFT) < 0 ? -1 : 1);
				if (iNextButtonID != -1)
				{
					CWnd* pWndNext = MapToolbarButtonToWindow(iNextButtonID);
					if (pWndNext)
						SetActiveDialog(pWndNext);
				}
			}
		}
	}
	return bResult;
}

CWnd* CwyytDlg::MapToolbarButtonToWindow(int iButtonID) const
{
	CWnd* pWnd;
	switch (iButtonID)
	{
	case TBBTN_SYS:			pWnd = sysresmanagewnd;		break;
	case TBBTN_PRIVATE:	pWnd = priresmanagewnd;		break;
	case TBBTN_GROUP:		pWnd = groupresmanagewnd;		break;
	case TBBTN_MESSAGES:	pWnd = chatwnd;			break;
	case TBBTN_IRC:			pWnd = ircwnd;			break;
	case TBBTN_SHARED:		pWnd = sharedfileswnd;	break;
	case TBBTN_SEARCH:		pWnd = searchwnd;		break;
	case TBBTN_STATS:		pWnd = statisticswnd;	break;
	default:				pWnd = NULL; ASSERT(0);	break;
	}
	return pWnd;
}
void CwyytDlg::SetActiveDialog(CWnd* dlg)
{
	if (dlg == activewnd)
		return;
	if (activewnd)
		activewnd->ShowWindow(SW_HIDE);
	dlg->ShowWindow(SW_SHOW);
	dlg->SetFocus();
	activewnd = dlg;
	int iToolbarButtonID = MapWindowToToolbarButton(dlg);
	if (iToolbarButtonID != -1)
		toolbar->PressMuleButton(iToolbarButtonID);
	if (dlg == groupresmanagewnd)
	{
		/*if (thePrefs.ShowCatTabInfos())
			transferwnd->UpdateCatTabTitles();*/
	}
	else if (dlg == searchwnd)
	{
		//if(transferwnd->GetDownloadList()->curTab != 0)
		//	searchwnd->SetSelectedCat(transferwnd->GetDownloadList()->curTab);// X: [UIC] - [UIChange] follow transfer tab
	}
	else if (dlg == chatwnd)
	{
	/*	chatwnd->chatselector.ShowChat();*/
	}
	else if (dlg == statisticswnd)
	{
		//statisticswnd->ShowStatistics();
	}
}


int CwyytDlg::MapWindowToToolbarButton(CWnd* pWnd) const
{
	int iButtonID = -1;
	if (pWnd == priresmanagewnd)        iButtonID = TBBTN_PRIVATE;
	else if (pWnd == groupresmanagewnd)     iButtonID = TBBTN_GROUP;
	else if (pWnd == chatwnd)       iButtonID = TBBTN_MESSAGES;
	else if (pWnd == ircwnd)        iButtonID = TBBTN_IRC;
	else if (pWnd == sharedfileswnd)iButtonID = TBBTN_SHARED;
	else if (pWnd == searchwnd)     iButtonID = TBBTN_SEARCH;
	else if (pWnd == statisticswnd)	iButtonID = TBBTN_STATS;
	else if	(pWnd == sysresmanagewnd)	iButtonID = TBBTN_SYS;
	else ASSERT(0);
	return iButtonID;
}
void CwyytDlg::TrayMinimizeToTrayChange()
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		if (!thePrefs.GetMinToTray())
		{
			// just for safety, ensure that we are not adding duplicate menu entries..
			if (pSysMenu->EnableMenuItem(MP_MINIMIZETOTRAY, MF_BYCOMMAND | MF_ENABLED) == -1)
			{
				ASSERT( (MP_MINIMIZETOTRAY & 0xFFF0) == MP_MINIMIZETOTRAY && MP_MINIMIZETOTRAY < 0xF000);
				VERIFY( pSysMenu->InsertMenu(SC_MINIMIZE, MF_BYCOMMAND, MP_MINIMIZETOTRAY, GetResString(IDS_PW_TRAY)) );
			}
			else
				ASSERT(0);
		}
		else
		{
			(void)pSysMenu->RemoveMenu(MP_MINIMIZETOTRAY, MF_BYCOMMAND);
		}
	}
	CTrayDialog::TrayMinimizeToTrayChange();
}


void CwyytDlg::SetStatusBarPartsSize()
{
//>>> WiZaRd::Workaround
	//if startup takes too long then we might run into invalid calls here...
	if(!statusbar || !statusbar->m_hWnd)
		return;
//<<< WiZaRd::Workaround
	RECT rect;
	statusbar->GetClientRect(&rect);
	//int ussShift = 0;

	//Xman
	//only used for with USS
	/*
	if(thePrefs.IsDynUpEnabled())
	{
        if (thePrefs.IsDynUpUseMillisecondPingTolerance())
            ussShift = 45;
        else
            ussShift = 90;
        }
	*/
	//Xman changed
	/*
	int aiWidths[5] =
	{ 
		rect.right - 675 - ussShift,
		rect.right - 440 - ussShift,
		rect.right - 250 - ussShift,
		rect.right -  25 - ussShift,
		-1
	};
	*/
	int aiWidths[5] =
	{ 
		rect.right - 730,
		rect.right - 495,
		rect.right - 250,
		rect.right -  25,
		-1
	};
	statusbar->SetParts(_countof(aiWidths), aiWidths);
}


BOOL CwyytDlg::OnChevronPushed(UINT id, NMHDR* pNMHDR, LRESULT* plResult)
{
	UNREFERENCED_PARAMETER(id);
	if (!thePrefs.GetUseReBarToolbar())
		return FALSE;

	NMREBARCHEVRON* pnmrc = (NMREBARCHEVRON*)pNMHDR;

	ASSERT( id == AFX_IDW_REBAR );
	ASSERT( pnmrc->uBand == 0 );
	ASSERT( pnmrc->wID == 0 );
#ifdef REPLACE_ATLMAP
	ASSERT( m_mapTbarCmdToIcon.size() != 0 );
#else
	ASSERT( m_mapTbarCmdToIcon.GetCount() != 0 );
#endif

	// get visible area of rebar/toolbar
	CRect rcVisibleButtons;
	toolbar->GetClientRect(&rcVisibleButtons);

	// search the first toolbar button which is not fully visible
	int iButtons = toolbar->GetButtonCount();
	int i;
	for (i = 0; i < iButtons; i++)
	{
		CRect rcButton;
		toolbar->GetItemRect(i, &rcButton);

		CRect rcVisible;
		if (!rcVisible.IntersectRect(&rcVisibleButtons, &rcButton) || !EqualRect(rcButton, rcVisible))
			break;
	}

	// create menu for all toolbar buttons which are not (fully) visible
	BOOL bLastMenuItemIsSep = TRUE;
	CTitleMenu menu;
	menu.CreatePopupMenu();
	menu.AddMenuTitle(_T("wyyt"), true);
	while (i < iButtons)
	{
		TCHAR szString[256];
		szString[0] = _T('\0');
		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize = sizeof tbbi;
		tbbi.dwMask = TBIF_BYINDEX | TBIF_COMMAND | TBIF_STYLE | TBIF_STATE | TBIF_TEXT;
		tbbi.cchText = _countof(szString);
		tbbi.pszText = szString;
		if (toolbar->GetButtonInfo(i, &tbbi) != -1)
		{
			szString[_countof(szString) - 1] = _T('\0');
			if (tbbi.fsStyle & TBSTYLE_SEP)
			{
				if (!bLastMenuItemIsSep)
					bLastMenuItemIsSep = menu.AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)NULL);
			}
			else
			{
				if (szString[0] != _T('\0') && menu.AppendMenu(MF_STRING, tbbi.idCommand, szString, GetIconFromCmdId(tbbi.idCommand)))
				{
					bLastMenuItemIsSep = FALSE;
					if (tbbi.fsState & TBSTATE_CHECKED)
						menu.CheckMenuItem(tbbi.idCommand, MF_BYCOMMAND | MF_CHECKED);
					if ((tbbi.fsState & TBSTATE_ENABLED) == 0)
						menu.EnableMenuItem(tbbi.idCommand, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				}
			}
		}

		i++;
	}

	CPoint ptMenu(pnmrc->rc.left, pnmrc->rc.top);
	ClientToScreen(&ptMenu);
	ptMenu.y += rcVisibleButtons.Height();
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);
	VERIFY( menu.DestroyMenu() );
	*plResult = 1;
	return FALSE;
}


void CwyytDlg::CreateToolbarCmdIconMap()
{
#ifdef REPLACE_ATLMAP
	m_mapTbarCmdToIcon[TBBTN_CONNECT] = _T("Connect");
	m_mapTbarCmdToIcon[TBBTN_SYS] = _T("Kademlia");
	m_mapTbarCmdToIcon[TBBTN_GROUP] = _T("Server");
	m_mapTbarCmdToIcon[TBBTN_PRIVATE] = _T("Transfer");
	m_mapTbarCmdToIcon[TBBTN_SEARCH] = _T("Search");
	m_mapTbarCmdToIcon[TBBTN_SHARED] = _T("SharedFiles");
	m_mapTbarCmdToIcon[TBBTN_MESSAGES] = _T("Messages");
	m_mapTbarCmdToIcon[TBBTN_IRC] = _T("IRC");
	m_mapTbarCmdToIcon[TBBTN_STATS] = _T("Statistics");
	m_mapTbarCmdToIcon[TBBTN_OPTIONS] = _T("Preferences");
	m_mapTbarCmdToIcon[TBBTN_TOOLS] = _T("Tools");
	m_mapTbarCmdToIcon[TBBTN_HELP] = _T("Help");
#else
	m_mapTbarCmdToIcon.SetAt(TBBTN_CONNECT, _T("Connect"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_SYS, _T("Kademlia"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_GROUP, _T("Server"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_PRIVATE, _T("Transfer"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_SEARCH, _T("Search"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_SHARED, _T("SharedFiles"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_MESSAGES, _T("Messages"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_IRC, _T("IRC"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_STATS, _T("Statistics"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_OPTIONS, _T("Preferences"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_TOOLS, _T("Tools"));
	m_mapTbarCmdToIcon.SetAt(TBBTN_HELP, _T("Help"));
#endif
}

LPCTSTR CwyytDlg::GetIconFromCmdId(UINT uId)
{
#ifdef REPLACE_ATLMAP
	unordered_map<UINT, LPCTSTR>::const_iterator it = m_mapTbarCmdToIcon.find(uId);
	if(it != m_mapTbarCmdToIcon.end())
		return it->second;
#else
	LPCTSTR pszIconId = NULL;
	if (m_mapTbarCmdToIcon.Lookup(uId, pszIconId))
		return pszIconId;
#endif
	return NULL;
}

void CwyytDlg::ShowTransferRate(bool bForceAll)
{
	if (bForceAll)
		m_uLastSysTrayIconCookie = SYS_TRAY_ICON_COOKIE_FORCE_UPDATE;

	CString strTransferRate = L"欢迎使用";

	if (TrayIsVisible() || bForceAll)
	{
		// set trayicon-icon
		UpdateTrayIcon();

		TCHAR buffer2[128];//NOTIFYICONDATA_V2_TIP_SIZE
//>>> WiZaRd::ToolTip-FiX
		//first of all it's not interesting which client we use
		//and second this will create a buggy tool tip because only limited 
		//length is allowed/supported by M$/OS version/IE version
		//It is necessary to change that part in case you are using a rather long mod string/version string (as I do...)
		_sntprintf(buffer2, _countof(buffer2) - 1, _T("%s\r\n%s"), L"wyyt", strTransferRate);
		//buffer2[_countof(buffer2) - 1] = _T('\0');
//<<< WiZaRd::ToolTip-FiX

		// Win98: '\r\n' is not displayed correctly in tooltip
		//if (afxData.bWin95) {
		//	LPTSTR psz = buffer2;
		//	while (*psz) {
		//		if (*psz == _T('\r') || *psz == _T('\n'))
		//			*psz = _T(' ');
		//		psz++;
		//	}
		//}
		TraySetToolTip(buffer2);
	}

	//Xman see all sources
	//if (activewnd == transferwnd && IsWindowVisible())
	//	transferwnd->UpdateFilesCount();
	//Xman end


	if (IsWindowVisible() || bForceAll)
	{
		statusbar->SetText(strTransferRate, SBarUpDown, 0);
		ShowTransferStateIcon();
	}
	
}
void CwyytDlg::ShowTransferStateIcon()
{
	/*if (m_uUpDatarate && m_uDownDatarate)
		statusbar->SetIcon(SBarUpDown, transicons[3]);
	else if (m_uUpDatarate)
		statusbar->SetIcon(SBarUpDown, transicons[2]);
	else if (m_uDownDatarate)
		statusbar->SetIcon(SBarUpDown, transicons[1]);
	else
		statusbar->SetIcon(SBarUpDown, transicons[0]);*/
}
void CwyytDlg::UpdateTrayIcon()
{
	//聊天窗口！
	/*if (m_iMsgIcon!=0 && thePrefs.DoFlashOnNewMessage()==true ) 
	{
		m_iMsgBlinkState = !m_iMsgBlinkState;

		if (m_iMsgBlinkState)
		{
			ASSERT( imicons[1] != NULL );
			if (imicons[1])
			{
				TraySetIcon(imicons[1]);
				TrayUpdate();
				return;
			}
		}
		m_uLastSysTrayIconCookie = SYS_TRAY_ICON_COOKIE_FORCE_UPDATE;
	}*/
	

	//int iDownRateProcent = (int)ceil((m_uDownDatarate/10.24) / thePrefs.GetMaxGraphDownloadRate());
	//if (iDownRateProcent > 100)
	//	iDownRateProcent = 100;
	//uint_ptr pixelDown = (iDownRateProcent > 0) ? (iDownRateProcent*16 + 99)/100 : 0;

	//int iUpRateProcent = (int)ceil((m_uUpDatarate/10.24) / thePrefs.GetMaxGraphUploadRate());
	//if (iUpRateProcent > 100)
	//	iUpRateProcent = 100;
	//uint_ptr pixelUp = (iUpRateProcent > 0) ? (iUpRateProcent*16 + 99)/100 : 0;

	//// compute an id of the icon to be generated
	////UINT uSysTrayIconCookie = (iPercent > 0) ? (16 - ((iPercent*15/100) + 1)) : 0;
	//uint_ptr uSysTrayIconCookie = pixelDown;
	//if (theApp.IsConnected()) 
	//{
	//	if (!theApp.IsFirewalled())
	//		uSysTrayIconCookie += 32;
	//}
	//else
	//	uSysTrayIconCookie += 64;
	//if(pixelDown != 0 && pixelDown >= pixelUp)
	//	iUpRateProcent = 0;
	//else
	//	uSysTrayIconCookie += pixelUp<<8;

	//// dont update if the same icon as displayed would be generated
	//if (m_uLastSysTrayIconCookie == uSysTrayIconCookie)
	//	return;
	//m_uLastSysTrayIconCookie = uSysTrayIconCookie;

	// prepare it up
	COLORREF aColors[2] = { thePrefs.GetStatsColor(5), thePrefs.GetStatsColor(11) }; // set the corresponding color for each level
	m_TrayIcon.Init(
		/*theApp.IsConnected()?
		(theApp.IsFirewalled()?m_icoSysTrayLowID:m_icoSysTrayConnected)
		:*/m_icoSysTrayDisconnected,
		100, 2, 1, 16, 16, aColors);

	// load our limit and color info
	/*static const int aiLimits[1] = { 100 }; // set the limits of where the bar color changes (low-high)
	COLORREF aColors[1] = { thePrefs.GetStatsColor(11) }; // set the corresponding color for each level
	m_TrayIcon.SetColorLevels(aiLimits, aColors, _countof(aiLimits));*/

	// generate the icon (do *not* destroy that icon using DestroyIcon(), that's done in 'TrayUpdate')
	int aiVals[2] = { 0/*iUpRateProcent*/, 0/*iDownRateProcent*/ };
	m_icoSysTrayCurrent = m_TrayIcon.Create(aiVals);
	ASSERT( m_icoSysTrayCurrent != NULL );
	if (m_icoSysTrayCurrent)
	{
		TraySetIcon(m_icoSysTrayCurrent, true);
		TrayUpdate();
	}
}

void CwyytDlg::SetAllIcons()
{
	// application icon (although it's not customizable, we may need to load a different color resolution)
	if (m_hIcon)
		VERIFY( ::DestroyIcon(m_hIcon) );
	// NOTE: the application icon name is prefixed with "AAA" to make sure it's alphabetically sorted by the
	// resource compiler as the 1st icon in the resource table!
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_IHAS);
	SetIcon(m_hIcon, TRUE);
	// this scales the 32x32 icon down to 16x16, does not look nice at least under WinXP
	//SetIcon(m_hIcon, FALSE);

	// connection state
	/*for (int i = 0; i < _countof(connicons); i++){
	if (connicons[i]) VERIFY( ::DestroyIcon(connicons[i]) );
	}
	connicons[0] = theApp.LoadIcon(_T("ConnectedNotNot"), 16, 16);
	connicons[1] = theApp.LoadIcon(_T("ConnectedNotLow"), 16, 16);
	connicons[2] = theApp.LoadIcon(_T("ConnectedNotHigh"), 16, 16);
	connicons[3] = theApp.LoadIcon(_T("ConnectedLowNot"), 16, 16);
	connicons[4] = theApp.LoadIcon(_T("ConnectedLowLow"), 16, 16);
	connicons[5] = theApp.LoadIcon(_T("ConnectedLowHigh"), 16, 16);
	connicons[6] = theApp.LoadIcon(_T("ConnectedHighNot"), 16, 16);
	connicons[7] = theApp.LoadIcon(_T("ConnectedHighLow"), 16, 16);
	connicons[8] = theApp.LoadIcon(_T("ConnectedHighHigh"), 16, 16);
	ShowConnectionStateIcon();*/

	// transfer state
	/*if (transicons[0]) VERIFY( ::DestroyIcon(transicons[0]) );
	if (transicons[1]) VERIFY( ::DestroyIcon(transicons[1]) );
	if (transicons[2]) VERIFY( ::DestroyIcon(transicons[2]) );
	if (transicons[3]) VERIFY( ::DestroyIcon(transicons[3]) );
	transicons[0] = theApp.LoadIcon(_T("UP0DOWN0"), 16, 16);
	transicons[1] = theApp.LoadIcon(_T("UP0DOWN1"), 16, 16);
	transicons[2] = theApp.LoadIcon(_T("UP1DOWN0"), 16, 16);
	transicons[3] = theApp.LoadIcon(_T("UP1DOWN1"), 16, 16);
	ShowTransferStateIcon();
	*/
	// users state
	/*if (usericon) VERIFY( ::DestroyIcon(usericon) );
	usericon = theApp.LoadIcon(_T("StatsClients"), 16, 16);
	ShowUserStateIcon();
*/
	// traybar icons
	if (m_icoSysTrayConnected) VERIFY( ::DestroyIcon(m_icoSysTrayConnected) );
	if (m_icoSysTrayDisconnected) VERIFY( ::DestroyIcon(m_icoSysTrayDisconnected) );
	if (m_icoSysTrayLowID) VERIFY( ::DestroyIcon(m_icoSysTrayLowID) );
	m_icoSysTrayConnected = theApp.LoadIcon(_T("TrayConnected"), 16, 16);
	m_icoSysTrayDisconnected = theApp.LoadIcon(_T("TrayNotConnected"), 16, 16);
	m_icoSysTrayLowID = theApp.LoadIcon(_T("TrayLowID"), 16, 16);
	ShowTransferRate(true);

	//if (imicons[0]) VERIFY( ::DestroyIcon(imicons[0]) );
	//if (imicons[1]) VERIFY( ::DestroyIcon(imicons[1]) );
	//if (imicons[2]) VERIFY( ::DestroyIcon(imicons[2]) );
	//imicons[0] = NULL;
	//imicons[1] = theApp.LoadIcon(_T("Message"), 16, 16);
	//imicons[2] = theApp.LoadIcon(_T("MessagePending"), 16, 16);
	//ShowMessageState(m_iMsgIcon);
}

LRESULT CwyytDlg::OnConsoleThreadEvent(WPARAM wParam, LPARAM lParam)
{
	//AddDebugLogLine(DLP_VERYLOW, _T("%hs: nEvent=%u, nThreadID=%u"), __FUNCTION__, wParam, lParam);

	// If eMule was started with "RUNAS":
	// This message handler receives a 'console event' from the concurrently and thus
	// asynchronously running console control handler thread which was spawned by Windows
	// in case the user logs off/reboots/shutdown. Even if the console control handler thread
	// is waiting on the result from this message handler (is waiting until the main thread
	// has finished processing this inter-application message), the application will get
	// forcefully terminated by Windows after 20 seconds! There is no known way to prevent
	// that. This means, that if we would invoke our standard shutdown code ('OnClose') here
	// and the shutdown takes longer than 20 sec, we will get forcefully terminated by 
	// Windows, regardless of what we are doing. This means, MET-file and PART-file corruption
	// may occure. Because the shutdown code in 'OnClose' does also shutdown Kad (which takes
	// a noticeable amount of time) it is not that unlikely that we run into problems with
	// not being finished with our shutdown in 20 seconds.
	// 
	if (theApp.m_app_state == APP_STATE_RUNNING)
	{
#if 1
		// And it really should be OK to expect that emule can shutdown in 20 sec on almost
		// all computers. So, use the proper shutdown.
		theApp.m_app_state = APP_STATE_SHUTTINGDOWN;
		OnClose();	// do not invoke if shutdown takes longer than 20 sec, read above
#else
		// As a minimum action we at least set the 'shutting down' flag, this will help e.g.
		// the CUploadQueue::UploadTimer to not start any file save actions which could get
		// interrupted by windows and which would then lead to corrupted MET-files.
		// Setting this flag also helps any possible running threads to stop their work.
		theApp.m_app_state = APP_STATE_SHUTTINGDOWN;

#ifdef _DEBUG
		// Simulate some work.
		//
		// NOTE: If the console thread has already exited, Windows may terminate the process
		// even before the 20 sec. timeout!
		//BeBusy(70, __FUNCTION__);
#endif

		// Actually, just calling 'ExitProcess' should be the most safe thing which we can
		// do here. Because we received this message via the main message queue we are 
		// totally in-sync with the application and therefore we know that we are currently
		// not within a file save action and thus we simply can not cause any file corruption
		// when we exit right now.
		//
		// Of course, there may be some data loss. But it's the same amount of data loss which
		// could occure if we keep running. But if we keep running and wait until Windows
		// terminates us after 20 sec, there is also the chance for file corruption.
		/*if (thePrefs.GetDebug2Disk()) 
		{
			theVerboseLog.Logf(_T("%hs: ExitProcess"), __FUNCTION__);
			theVerboseLog.Close();
		}*/
		ExitProcess(0);
#endif
	}

	//AddDebugLogLine(DLP_VERYLOW, _T("%hs: returning"), __FUNCTION__);
	return 1;
}

void CwyytDlg::AddLogText(UINT uFlags, LPCTSTR pszText)
{
	if (uFlags & LOG_STATUSBAR)
	{
		if (statusbar->m_hWnd /*&& ready*/)
		{
			if (theApp.m_app_state == APP_STATE_RUNNING)
				statusbar->SetText(pszText, SBarLog, 0);
		}
	}
//	if (GetCurrentThreadId() != g_uMainThreadId)
//	{
//		theApp.QueueLogLineEx(uFlags, _T("%s"), pszText);
//		return;
//	}
//
//	if (uFlags & LOG_STATUSBAR)
//	{
//		if (statusbar->m_hWnd /*&& ready*/)
//		{
//			if (theApp.m_app_state == APP_STATE_RUNNING)
//				statusbar->SetText(pszText, SBarLog, 0);
//		}
//		else
//		{
//			theApp.SplashHide(SW_HIDE); //Xman new slpash-screen arrangement
//			XMSGBOXPARAMS params;
//			params.nTimeoutSeconds = 5;
//			XMessageBox(NULL, pszText, _T("eMule"), MB_OK | MB_ICONINFORMATION, &params);
//			theApp.SplashHide(SW_SHOW); //Xman new slpash-screen arrangement
//		}
//	}
//#if defined(_DEBUG) || defined(USE_DEBUG_DEVICE)
//	Debug(_T("%s\n"), pszText);
//#endif
//
//	if ((uFlags & LOG_DEBUG) && !thePrefs.GetVerbose())
//		return;
//
//	//Xman Anti-Leecher-Log
//	if ((uFlags & LOG_LEECHER) && !thePrefs.GetVerbose())
//		return;
//
//
//	TCHAR temp[1060];
//	int iLen = _sntprintf(temp, _countof(temp) - 1, _T("%s: %s\r\n"), CTime::GetCurrentTime().Format(thePrefs.GetDateTimeFormat4Log()), pszText);
//	if (iLen < 0){
//		_tcsncpy(temp + _countof(temp) - 4, _T("..."), 4);
//		iLen = _countof(temp) - 1;
//	}
//	if (iLen > 0)
//	{
//		if (!(uFlags & LOG_DEBUG) && !(uFlags & LOG_LEECHER)) //Xman Anti-Leecher-Log
//		{
//			serverwnd->logbox->AddTyped(temp, iLen, uFlags & LOGMSGTYPEMASK);
//			if (IsWindow(serverwnd->StatusSelector) && serverwnd->StatusSelector.GetCurSel() != CServerWnd::PaneLog)
//				serverwnd->StatusSelector.HighlightItem(CServerWnd::PaneLog, TRUE);
//			if (!(uFlags & LOG_DONTNOTIFY) && ready)
//				ShowNotifier(pszText, TBN_LOG);
//			if (thePrefs.GetLog2Disk())
//				theLog.Log(temp, iLen);
//		}
//		else
//			//Xman Anti-Leecher-Log
//			if (thePrefs.GetVerbose() && (uFlags & LOG_LEECHER) )
//			{
//				serverwnd->leecherlog->AddTyped(temp, iLen, uFlags & LOGMSGTYPEMASK);
//				if (IsWindow(serverwnd->StatusSelector) && serverwnd->StatusSelector.GetCurSel() != CServerWnd::PaneLeecherLog)
//					serverwnd->StatusSelector.HighlightItem(CServerWnd::PaneLeecherLog, TRUE);
//
//				if (thePrefs.GetDebug2Disk())
//					theVerboseLog.Log(temp, iLen);
//			}
//			else
//				//Xman end
//				if (thePrefs.GetVerbose() && ((uFlags & LOG_DEBUG) || thePrefs.GetFullVerbose()))
//				{
//					serverwnd->debuglog->AddTyped(temp, iLen, uFlags & LOGMSGTYPEMASK);
//					if (IsWindow(serverwnd->StatusSelector) && serverwnd->StatusSelector.GetCurSel() != CServerWnd::PaneVerboseLog)
//						serverwnd->StatusSelector.HighlightItem(CServerWnd::PaneVerboseLog, TRUE);
//
//					if (thePrefs.GetDebug2Disk())
//						theVerboseLog.Log(temp, iLen);
//				}
//	}
}

int GetMac(char * mac)     
{     
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff   [30];
	}ASTAT,*PASTAT;

	ASTAT Adapter;   

	typedef struct _LANA_ENUM
	{
		UCHAR   length;
		UCHAR   lana[MAX_LANA];
	}LANA_ENUM;

	LANA_ENUM lana_enum;    
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));    
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);

	if(uRetCode != NRC_GOODRET)     
		return uRetCode;     

	for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb); 
		if(uRetCode == NRC_GOODRET)
			break; 
	}

	if(uRetCode != NRC_GOODRET)
		return uRetCode;     

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);

	if(uRetCode != NRC_GOODRET)
		return uRetCode;

	sprintf(mac,"%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);

	return 0;   
}
void ReplaceAll(std::string& strSource, const std::string& strOld, const std::string& strNew)
{
	int nPos = 0;
	while ((nPos = strSource.find(strOld, nPos)) != strSource.npos)
	{
		strSource.replace(nPos, strOld.length(), strNew);
		nPos += strNew.length();
	}
}

//===========================
//    获取本机外网IP及区域信息
//===========================
void GetInternetIPInfo(string &netIP, string & area){
	CInternetSession session(L"wyytversion", 1, PRE_CONFIG_INTERNET_ACCESS);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	string html;
	try
	{
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		if (!AfxParseURL(L"http://whois.pconline.com.cn/", dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			//CException* pEx =  new CException();
			//throw pEx;
		}
		pServer = session.GetHttpConnection(strServerName, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
		pFile->AddRequestHeaders(szHeaders_DL);
		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if (dwRet != HTTP_STATUS_OK)
		{
			//ThrowTearException_DL(2);
		}
		if (dwRet == HTTP_STATUS_DENIED)
		{
			DWORD dwPrompt;
			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			{
				//ThrowTearException_DL(1);
			}

			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		CString strLength;
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strLength);
		int   len;   
		//sscanf(strLength, L"%d", &len);   
		_stscanf_s(strLength,L"%d",&len);
		//len = _atoi(strLength.GetBuffer(0));
		CFileException pError;
		
		char sz[1461]={0};
		int nRead = 0, nTotal = 0;
		while ( (nRead = pFile->Read(sz, 1460)) > 0)
		{
			nTotal += nRead;
			html += string(sz);
		}

		pFile->Close();
		pServer->Close();

	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		pEx->Delete();
		Sleep(1000);

	}
	catch (CException* pEx)
	{

	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	HTML::ParserDom parser;
	//html ="<html><body>测试文字</body></html>";
	tree<HTML::Node> dom = parser.parseTree(html);
	tree<HTML::Node>::iterator it = dom.begin(); 
	tree<HTML::Node>::iterator end = dom.end();

	//输出整棵DOM树
	
	cout<< dom << endl;
	bool bfindIp = false;

	for(; it != end; ++it)
	{
		string tagName = it->tagName();

		if (!bfindIp && tagName == "input")
		{
			it->parseAttributes();
			if (it->attribute("value").first)
			{
				netIP = it->attribute("value").second;
				bfindIp = true;
			}
		}
		wstring wTagName = s2ws(tagName);
		int pos = wTagName.find(L"位置：");
		if(pos >= 0)
		{
			area = tagName;
		}

	}
	cout << endl;
}

void CwyytDlg::UploadMachineInformation()
{
	std::string strUrl = "http://www.firemail.wang:8880/api/admin/api/prodactivity/report";
	char   mac[200]; 
	GetMac(mac);
	string strMac(mac);
	ReplaceAll(strMac, "-", ":");
	CPCInfo pcInfo;
	string netIP="";
	string area="";
	GetInternetIPInfo(netIP, area);
	ReplaceAll(area, "\t", "");
	ReplaceAll(area, " ", "");
	ReplaceAll(area, "\n", "");
	ReplaceAll(area, "\r\n", "");
	ReplaceAll(area, "\r", "");
	area = string_To_UTF8(area);
	time_t tv;
	tv = time(NULL);
	long ltime = (long) tv;
	std::ostringstream os;
	os<<ltime;
	string strTime;
	std::istringstream is(os.str());
	is>>strTime;
	std:string postData = "{\"userId\":\"0\",\"clientFlag\":\""+ strMac+ "\",\"procName\":\"wyyt\",\"procVersion\":\"1.1.1.1\",\"procId\":\"4\",\"os\":\""+ pcInfo.m_strOS +"\",\"eventName\":\"start-up\",\"ip\":\""+pcInfo.m_strIP+"\",\"netIp\":\""+netIP+"\",\"area\":\""+area+"\",\"modifyTime\":\""+strTime+"000\",\"createTime\":\""+strTime+"000\",\"remarks\":\"\"}";
	CWininetHttp inetHttp;
	inetHttp.RequestJsonInfo(strUrl, Hr_Post, "Content-Type: application/json", postData);
}
