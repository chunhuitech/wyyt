// ProjCleanerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "ProjCleanerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CProjCleanerDlg dialog




CProjCleanerDlg::CProjCleanerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjCleanerDlg::IDD, pParent)
{
	dwPos = 0;
	m_CurrIsTrayIcon = FALSE;
	m_bDeling = FALSE;
	m_pTreeList = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProjCleanerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_LIST, m_LogList);
	DDX_Control(pDX, IDC_PATH_LIST, m_PathList);
	DDX_Control(pDX, IDC_PROGRESS, m_ProgCtrl);
	DDX_Control(pDX, IDC_SAVE_PATH_LIST, m_SavePathBtn);
	//DDX_Control(pDX, IDC_ADD_PATH, m_TopPicture);
}

BEGIN_MESSAGE_MAP(CProjCleanerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TRAYICON_MSG, OnTrayCallBackMsg)
	ON_BN_CLICKED(IDC_ADD_PATH, &CProjCleanerDlg::OnBnClickedAddPath)
	ON_BN_CLICKED(IDC_DEL_PATH, &CProjCleanerDlg::OnBnClickedDelPath)
	ON_BN_CLICKED(IDC_SET_INFO, &CProjCleanerDlg::OnBnClickedSetInfo)
	ON_BN_CLICKED(IDC_SCAN_DEL, &CProjCleanerDlg::OnBnClickedScanDel)
	ON_BN_CLICKED(IDC_LOG_BTN, &CProjCleanerDlg::OnBnClickedLogBtn)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE_PATH_LIST, &CProjCleanerDlg::OnBnClickedSavePathList)
	ON_COMMAND(ID_M_SHOW, &CProjCleanerDlg::OnMShow)
	ON_COMMAND(ID_M_EXIT, &CProjCleanerDlg::OnMExit)
END_MESSAGE_MAP()


// CProjCleanerDlg message handlers

BOOL CProjCleanerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//// for the top picture;
	//m_TopPicture.LoadPicture(MAKEINTRESOURCE(IDR_RES_GIF),_T("RES_GIF"));
	//m_TopPicture.Draw();

	m_gpMainWnd = this;

	// Status Bar Control
	m_StatBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[2]= {190, -1};
	m_StatBar.SetParts(2, strPartDim);
	m_StatBar.SetText(_T("                                  ★  VC 驿站 -   WwW.CcTry.Com  ★"), 1, 0);

	//CBitmap mBmp;
	//mBmp.LoadBitmap(IDB_FOLDER_OPEN);
	//m_PathImgList.Create(IDB_FOLDER_CLOSE, 16, 2, RGB(255, 255, 255));
	//m_PathImgList.Add(&mBmp, RGB(255, 255, 255));
	m_PathImgList.Create(IDB_FOLDER_BMP, 16, 2, RGB(255, 255, 255));
	m_PathList.SetImageList(&m_PathImgList);

	m_LogList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LogList.InsertColumn(0, _T("名 称"), LVCFMT_LEFT, 460);
	m_LogList.InsertColumn(1, _T("类 型"), LVCFMT_LEFT, 70);
	m_LogList.InsertColumn(2, _T("状 态"), LVCFMT_LEFT, 80);
	ExtendDiaog(FALSE);

	//for the profile;
	CProfileInfo  m_Profile;
	GetRegProfile(m_Profile);
	(m_Profile.m_bSavePathList == TRUE) ? m_SavePathBtn.SetCheck(BST_CHECKED) : m_SavePathBtn.SetCheck(BST_UNCHECKED);
	if ( m_Profile.m_bSavePathList == TRUE ) {
		CStringArray arrString;
		RegReadWriteStrings(_T("LastPathList"), arrString);
		for( INT_PTR idx = 0; idx < arrString.GetCount(); idx++ ) {
			m_PathList.InsertString(m_PathList.GetCount(), arrString.GetAt(idx), 0);
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProjCleanerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProjCleanerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProjCleanerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProjCleanerDlg::OnBnClickedAddPath()
{
	TCHAR szFolderPath[MAX_PATH] = {0}, szFullPath[MAX_PATH] = {0};
	BROWSEINFO mBroInfo = {0};
	mBroInfo.hwndOwner = m_hWnd;
	mBroInfo.lpszTitle = _T("请选择一文件夹...");
	mBroInfo.pszDisplayName = szFolderPath;
	mBroInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	ITEMIDLIST *pidl = ::SHBrowseForFolder(&mBroInfo);
	if (::SHGetPathFromIDList(pidl, szFullPath)){
		int idx = m_PathList.FindString(-1, szFullPath);
		if ( idx >= 0 ) {
			MessageBox(_T("路径已存在！"), _T("提示"), MB_OK|MB_ICONWARNING);
			return;
		}
		m_PathList.SetCurSel(m_PathList.InsertString(m_PathList.GetCount(), szFullPath, 0));
		m_PathList.SetCurSel(-1);
		m_PathList.RedrawWindow();
	}
}

void CProjCleanerDlg::OnBnClickedDelPath()
{
	int iSelect = m_PathList.GetCurSel();
	if ( iSelect >= 0 ) {
		m_PathList.DeleteString(iSelect);
	}else{
		MessageBox(_T("请先选择一个路径！"), _T("提示"), MB_OK | MB_ICONWARNING);
	}
}

void CProjCleanerDlg::OnBnClickedSetInfo()
{
	CConfigDlg dlg;
	dlg.DoModal();
}

void CProjCleanerDlg::OnBnClickedScanDel()
{
	m_LogList.DeleteAllItems();
	if ( m_PathList.GetCount() <= 0 ) {
		MessageBox(_T("请先选择或添加要清理的目录！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
		return;
	}
	CWinThread *pTread = AfxBeginThread(ScanAndCleanFunc, this);
}

void CProjCleanerDlg::OnBnClickedLogBtn()
{
	ExtendDiaog(!m_bShowAll);
	if ( m_bShowAll ) {
		SetDlgItemText(IDC_LOG_BTN, _T("隐藏日志↑"));
	}else{
		SetDlgItemText(IDC_LOG_BTN, _T("显示日志↓"));
	}
}

void CProjCleanerDlg::ExtendDiaog(BOOL bShow)
{
	static CRect m_DlgRectLarge(0, 0, 0, 0);
	static CRect m_DlgRectSmall(0, 0, 0, 0);

	if ( m_DlgRectLarge.IsRectNull() ) {
		GetWindowRect(&m_DlgRectLarge);
		m_DlgRectSmall = m_DlgRectLarge;
		m_DlgRectSmall.bottom -= 235;
	}
	if ( bShow ) {
		m_bShowAll = TRUE;
		SetWindowPos(NULL, 0, 0, m_DlgRectLarge.Width(), m_DlgRectLarge.Height(), SWP_NOZORDER | SWP_NOMOVE);
	}else{
		m_bShowAll = FALSE;
		SetWindowPos(NULL, 0, 0, m_DlgRectSmall.Width(), m_DlgRectSmall.Height(), SWP_NOZORDER | SWP_NOMOVE);
	}
}

BOOL CProjCleanerDlg::EnableWindow(UINT uID, BOOL bEnable)
{
	return GetDlgItem(uID)->EnableWindow(bEnable);
}

void CProjCleanerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( m_hWnd && m_StatBar.m_hWnd ) {
		m_StatBar.MoveWindow(CRect(0, 0, 100, 100));
	}
	if ( nType == SIZE_MINIMIZED && RegReadWriteDword(_T("MiniTray"), TRUE, 0) == TRUE) {
		TrayMyIcon();
		m_CurrIsTrayIcon = TRUE;
	}
}

void CProjCleanerDlg::ShowLogMsg(CString strPath, INT bFolder, BOOL bSucceed)
{
	CString strText;
	int nCount = m_LogList.GetItemCount();
	m_LogList.InsertItem(nCount, _T(""));
	m_LogList.SetItemText(nCount, 0, strPath);
	if ( bFolder != -1 ) {
		strText = (bFolder == TRUE) ? _T("文件夹") : _T("文件");
		m_LogList.SetItemText(nCount, 1, strText);
		strText = (bSucceed == TRUE) ? _T("成功") : _T("失败");
		m_LogList.SetItemText(nCount, 2, strText);
	}
}

CTreeItem *CProjCleanerDlg::GetTreeList()
{
	CString strPath;
	CConfigDlg  m_dlg;
	m_dlg.Create(IDD_CONFIG_DLG);
	RegReadWriteStrings(_T("CurrentTreeFile"), strPath, TRUE);
	CTreeItem *pList = m_dlg.GetTreeListByFile(strPath);
	m_dlg.DestroyWindow();
	FreeTreeList();
	m_pTreeList = pList;
	return pList;
}

void CProjCleanerDlg::FreeTreeList(CTreeItem *in_pParent)
{
	if ( in_pParent == NULL ) {
		in_pParent = m_pTreeList;
	}
	if ( !in_pParent ) {
		return;
	}
	CTreeItem *pCurr = in_pParent, *pNext = NULL;
	if ( !pCurr ) {
		return;
	}
	while( pCurr ) {
		if ( pCurr->m_pFirstChild ) {
			FreeTreeList(pCurr->m_pFirstChild);
		}
		pNext = pCurr->m_pNext;
		delete pCurr;
		pCurr = pNext;
	}
	m_pTreeList = NULL;
}

void CProjCleanerDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_ProgCtrl.SetPos(dwPos);
	CDialog::OnTimer(nIDEvent);
}

BOOL CProjCleanerDlg::GetRegProfile(CProfileInfo &m_Profile)
{
	m_Profile.m_bDelToRecycle = RegReadWriteDword(_T("DeleteToRecycle"), TRUE);
	m_Profile.m_bEndClose = RegReadWriteDword(_T("EndClose"), TRUE);
	m_Profile.m_bMinTray = RegReadWriteDword(_T("MiniTray"), TRUE);
	m_Profile.m_bAssRMenu = RegReadWriteDword(_T("AssRMenu"), TRUE);
	m_Profile.m_bSavePathList = RegReadWriteDword(_T("SavePathList"), TRUE);
	RegReadWriteStrings(_T("CurrentTreeFile"), m_Profile.m_strCurrTree, TRUE);
	RegReadWriteStrings(_T("InitStartTree"), m_Profile.m_strInitTree, TRUE);
	return TRUE;
}
void CProjCleanerDlg::OnBnClickedSavePathList()
{
	int nCheck = m_SavePathBtn.GetCheck();
	if ( nCheck == BST_CHECKED ) {
		RegReadWriteDword(_T("SavePathList"), FALSE, TRUE);
	}else if ( nCheck == BST_UNCHECKED ) {
		RegReadWriteDword(_T("SavePathList"), FALSE, FALSE);
	}
}

void CProjCleanerDlg::OnCancel()
{
	CString strText;
	CStringArray arrString;
	for( int idx = 0; idx < m_PathList.GetCount(); idx++ ) {
		m_PathList.GetText(idx, strText);
		arrString.Add(strText);
	}
	if ( arrString.GetCount() > 0 ) {
		RegReadWriteStrings(_T("LastPathList"), arrString, FALSE);
	}
	if ( m_CurrIsTrayIcon ) {
		NOTIFYICONDATA tnd = {0};
		tnd.cbSize = sizeof(NOTIFYICONDATA);
		tnd.hWnd = GetSafeHwnd();
		tnd.uID = IDR_MAINFRAME;
		Shell_NotifyIcon(NIM_DELETE, &tnd);
	}
	FreeTreeList();
	CDialog::OnCancel();
}

BOOL CProjCleanerDlg::TrayMyIcon(BOOL bAdd)
{
	BOOL bRet = FALSE;
	NOTIFYICONDATA tnd = {0};
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = GetSafeHwnd();
	tnd.uID = IDR_MAINFRAME;
	if ( bAdd == TRUE ) {
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = WM_TRAYICON_MSG;
		tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		_tcscpy_s(tnd.szTip, sizeof(tnd.szTip), _T("工程目录清理器 v1.0"));
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
		bRet = Shell_NotifyIcon(NIM_ADD, &tnd);
	}else{
		ShowWindow(SW_SHOWNA);
		SetForegroundWindow();
		bRet = Shell_NotifyIcon(NIM_DELETE, &tnd);
	}
	return bRet;
}

LRESULT CProjCleanerDlg::OnTrayCallBackMsg(WPARAM wparam, LPARAM lparam)
{
	switch(lparam)
	{
	case WM_RBUTTONUP:
		{
			CMenu mMenu, *pMenu = NULL;
			CPoint pt;
			mMenu.LoadMenu(IDR_TRAY_MENU);
			pMenu = mMenu.GetSubMenu(0);
			GetCursorPos(&pt);
			SetForegroundWindow();
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
			break;
		}
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_RESTORE);
		SetForegroundWindow();
		TrayMyIcon(FALSE);
		m_CurrIsTrayIcon = FALSE;
		break;
	default:break;
	}
	return NULL;
}
void CProjCleanerDlg::OnMShow()
{
	ShowWindow(SW_RESTORE);
	TrayMyIcon(FALSE);
	m_CurrIsTrayIcon = FALSE;
}

void CProjCleanerDlg::OnMExit()
{
	TrayMyIcon(FALSE);
	m_CurrIsTrayIcon = FALSE;
	OnCancel();
}
