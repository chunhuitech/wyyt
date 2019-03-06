/** \file wyyt.cpp
******************************************************************************** 
<PRE> 
模块名 : <wyyt> 
文件名 : <wyyt.cpp> 
相关文件 : < 与此文件相关的其它文件 > 
文件实现功能 :定义应用程序的类行为。
 * 1- 
 * 2- 
 作者 : <IHAS> 
 版本 : <1.0> 
 版权 : Copyright (c) 2012 IHAS
 创建日期 : <2012/07/29>
-------------------------------------------------------------------------------- 
备注 : <其它说明> 
-------------------------------------------------------------------------------- 
修改记录 : 
日 期         版本           修改人           修改内容 
YYYY/MM/DD    X.Y        <IHAS>     < 修改内容 > 
</PRE> 
*******************************************************************************/
// wyyt.cpp : 
//

#include "stdafx.h"
#include "wyyt.h"
#include "opcodes.h"
#include "wyytDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Preferences.h"
#include "ModInfo.h"
#include "OtherFunctions.h"
#include "enbitmap.h"
#include "GDIPlusUtil.h" // X: [GPUI] - [GDI Plus UI]
#include "VisualStylesXP.h"
#include "wyytExtendDataManagePri.h"


bool g_bLowColorDesktop = false;
CLogFile theLog;
CLogFile theVerboseLog;

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType);

// CwyytApp

BEGIN_MESSAGE_MAP(CwyytApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CwyytApp 构造

//CwyytApp::CwyytApp()
//{
//	// TODO: 在此处添加构造代码，
//	// 将所有重要的初始化放置在 InitInstance 中
//}
CwyytApp::CwyytApp(LPCTSTR lpszAppName) : CWinApp(lpszAppName)
{
	if(thePrefs.InitWinVersion() == 0){
		AfxMessageBox(MOD_FULL_NAME _T(" must be running on Windows 2000 or higer"),	MB_ICONSTOP | MB_SYSTEMMODAL | MB_SETFOREGROUND);
		exit(1);
	}
	//它需要提供一个种子，这个种子会对应一个随机数，如果使用相同的种子后面的rand()函数会出现一样的随机数。如： srand(1); 直接使用1来初始化种子。
	//不过为了防止随机数每次重复常常使用系统时间来初始化，即使用 time函数来获得系统时间，它的返回值为从 00:00:00 GMT, January 1, 1970 到现在所持续的秒数，然后将time_t型数据转化为(unsigned)型再传给srand函数，即： srand((unsigned) time(&t));
	//还有一个经常用法，不需要定义time_t型t变量,即： srand((unsigned) time(NULL)); 直接传入一个空指针，因为你的程序中往往并不需要经过参数获得的t数据。srand((int)getpid()); 使用程序的ID(getpid())来作为初始化种子，在同一个程序中这个种子是固定的。
	srand((uint32)time(NULL));

	m_bAutoStart = false;

	// this is the "base" version number <major>.<minor>.<update>.<build>
	m_dwProductVersionMS = MAKELONG(CwyytApp::m_nVersionMin, CwyytApp::m_nVersionMjr);
	m_dwProductVersionLS = MAKELONG(CwyytApp::m_nVersionBld, CwyytApp::m_nVersionUpd);

	m_ullComCtrlVer = MAKEDLLVERULL(4,0,0,0);
	m_iDfltImageListColorFlags = ILC_COLOR;
	// create a string version (e.g. "0.30a")
	ASSERT( CwyytApp::m_nVersionUpd + 'a' <= 'f' );
/*	m_strCurVersionLongDbg.Format(_T("%u.%u%c.%u"), CemuleApp::m_nVersionMjr, CemuleApp::m_nVersionMin, _T('a') + CemuleApp::m_nVersionUpd, CemuleApp::m_nVersionBld);
#ifdef _DEBUG
	m_strCurVersionLong = m_strCurVersionLongDbg + _T(" DEBUG");
#else
	m_strCurVersionLong.Format(_T("%u.%u%c"), CemuleApp::m_nVersionMjr, CemuleApp::m_nVersionMin, _T('a') + CemuleApp::m_nVersionUpd);
#endif
*/
	// create the protocol version number
	CString strTmp;
	strTmp.Format(_T("0x%u"), m_dwProductVersionMS);
	VERIFY( _stscanf_s(strTmp, _T("0x%x"), &m_uCurVersionShort) == 1 );
	ASSERT( m_uCurVersionShort < 0x99 );

	// create the version check number
	strTmp.Format(_T("0x%u%c"), m_dwProductVersionMS, _T('A') + CwyytApp::m_nVersionUpd);
	VERIFY( _stscanf_s(strTmp, _T("0x%x"), &m_uCurVersionCheck) == 1 );
	ASSERT( m_uCurVersionCheck < 0x999 );
// MOD Note: end

	//Call this member function from within the constructor of your CWinApp-derived class to use HTMLHelp for your application's help.
	EnableHtmlHelp();

	m_sizBigSystemIcon.cx = 32;
	m_sizBigSystemIcon.cy = 32;
}

// 唯一的一个 CwyytApp 对象

CwyytApp theApp(_T("Wyyt"));


// CwyytApp 初始化
#define  IHAVE_A_SET_NAME  _T("I Have A Set_wyyt")

BOOL CwyytApp::InitInstance()
{
#pragma  region  保证只运行一个程序 
	HANDLE hMetex = CreateMutex(NULL, true, IHAVE_A_SET_NAME);
	if(hMetex != NULL)
	{
		DWORD dwErr = GetLastError();
		if(ERROR_ALREADY_EXISTS == dwErr)
		{
			(void)CloseHandle(hMetex);

			// 寻找先前实例的主窗口 
			HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD); 
			while (::IsWindow(hWndPrevious)) 
			{ 
				// 检查窗口是否有预设的标记?有，则是我们寻找的主窗 
				if (::GetProp(hWndPrevious, IHAVE_A_SET_NAME)) 
				{ 
					// 主窗口已最小化，则恢复其大小
					if (!::IsWindowVisible(hWndPrevious)) 
					{
						::ShowWindow(hWndPrevious,SW_RESTORE); 
					}
					::SendMessage(hWndPrevious, WM_SYSCOMMAND, SC_RESTORE, NULL);

					// 将主窗激活 
					(void)::SetForegroundWindow(hWndPrevious); 

					// 将主窗的对话框激活 
					(void)::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 

					// 退出本实例 
					return FALSE; 
				} 
				// 继续寻找下一个窗口 
				hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT);
			} 
			return FALSE;
		}
	}
#pragma  endregion
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	if (!AfxInitRichEdit2())
	{
		if (!AfxInitRichEdit())
			AfxMessageBox(_T("Fatal Error: No Rich Edit control library found!")); // should never happen..
	}
	
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(thePrefs.GetMuleDirectory(WYYT_CONFIGDIR) + _T("preferences.ini"));

	_tsetlocale(LC_ALL, _T(""));		// set all categories of locale to user-default ANSI code page obtained from the OS.
	_tsetlocale(LC_NUMERIC, _T("C"));	// set numeric category to 'C'

	if (ProcessCommandline())
		return false;

	///////////////////////////////////////////////////////////////////////////
	// Common Controls initialization
	//
	//						Mjr Min
	// ----------------------------
	// W98 SE, IE5			5	8
	// W2K SP4, IE6 SP1		5	81
	// XP SP2 				6   0
	// XP SP3				6   0
	// Vista SP1			6   16
	InitCommonControls();
	DWORD dwComCtrlMjr = 4;
	DWORD dwComCtrlMin = 0;
	AtlGetCommCtrlVersion(&dwComCtrlMjr, &dwComCtrlMin);
	m_ullComCtrlVer = MAKEDLLVERULL(dwComCtrlMjr,dwComCtrlMin,0,0);

	
	// create & initialize all the important stuff 
	thePrefs.Init();
	//theStats.Init();



	g_gdiplus.Init(); // X: [GPUI] - [GDI Plus UI]
	UpdateLargeIconSize();
	UpdateDesktopColorDepth();
	CWinApp::InitInstance();


	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	//
	wyytExtendDataManage::Instance().Init();
	wyytExtendDataManage::Instance().InitClassInfoDatas();
	wyytExtendDataManage::Instance().InitSysRecordInfoDatas();
	wyytExtendDataManage::Instance().GetSysDataVerInfo();

	wyytExtendDataManage::Instance().InitSysRecAttachsDatas();


	wyytExtendDataManagePri::Instance().InitClassInfoDatas();
	wyytExtendDataManagePri::Instance().InitSysRecordInfoDatas();
	wyytExtendDataManagePri::Instance().InitSysRecAttachsDatas();

	CwyytDlg dlg;
	wyytdlg = &dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。

	wyytdlg = NULL;
	return FALSE;



	/*---------------原框架生成-----------------
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CwyytDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
	---------------原框架生成-----------------*/
}


bool CwyytApp::CopyTextToClipboard(CString strText)
{
	if (strText.IsEmpty())
		return false;

	HGLOBAL hGlobalT = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
	if (hGlobalT != NULL)
	{
		LPTSTR pGlobalT = static_cast<LPTSTR>(GlobalLock(hGlobalT));
		if (pGlobalT != NULL)
		{
			_tcscpy_s(pGlobalT, strText.GetLength() + 1, strText);
			GlobalUnlock(hGlobalT);
		}
		else
		{
			GlobalFree(hGlobalT);
			hGlobalT = NULL;
		}
	}

	CStringA strTextA(strText);
	HGLOBAL hGlobalA = GlobalAlloc(GHND | GMEM_SHARE, (strTextA.GetLength() + 1) * sizeof(CHAR));
	if (hGlobalA != NULL)
	{
		LPSTR pGlobalA = static_cast<LPSTR>(GlobalLock(hGlobalA));
		if (pGlobalA != NULL)
		{
			strcpy_s(pGlobalA, strTextA.GetLength() + 1, strTextA);
			GlobalUnlock(hGlobalA);
		}
		else
		{
			GlobalFree(hGlobalA);
			hGlobalA = NULL;
		}
	}

	if (hGlobalT == NULL && hGlobalA == NULL)
		return false;

	int iCopied = 0;
	if (OpenClipboard(NULL))
	{
		if (EmptyClipboard())
		{
			if (hGlobalT){
				if (SetClipboardData(CF_UNICODETEXT, hGlobalT) != NULL){
					iCopied++;
				}
				else{
					GlobalFree(hGlobalT);
					hGlobalT = NULL;
				}
			}
			if (hGlobalA){
				if (SetClipboardData(CF_TEXT, hGlobalA) != NULL){
					iCopied++;
				}
				else{
					GlobalFree(hGlobalA);
					hGlobalA = NULL;
				}
			}
		}
		CloseClipboard();
	}

	if (iCopied == 0)
	{
		if (hGlobalT){
			GlobalFree(hGlobalT);
			hGlobalT = NULL;
		}
		if (hGlobalA){
			GlobalFree(hGlobalA);
			hGlobalA = NULL;
		}
	}
	else
		IgnoreClipboardLinks(strText); // this is so eMule won't think the clipboard has ed2k links for adding

	return (iCopied != 0);
}
CString CwyytApp::CopyTextFromClipboard()
{
	if (IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		if (OpenClipboard(NULL))
		{
			bool bResult = false;
			CString strClipboard;
			HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
			if (hMem)
			{
				LPCWSTR pwsz = (LPCWSTR)GlobalLock(hMem);
				if (pwsz)
				{
					strClipboard = pwsz;
					GlobalUnlock(hMem);
					bResult = true;
				}
			}
			CloseClipboard();
			if (bResult)
				return strClipboard;
		}
	}

	if (!IsClipboardFormatAvailable(CF_TEXT))
		return _T("");
	if (!OpenClipboard(NULL))
		return _T("");

	CString	retstring;
	HGLOBAL	hglb = GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		LPCSTR lptstr = (LPCSTR)GlobalLock(hglb);
		if (lptstr != NULL)
		{
			retstring = lptstr;
			GlobalUnlock(hglb);
		}
	}
	CloseClipboard();

	return retstring;
}


void CwyytApp::UpdateLargeIconSize()
{
	// initialize with system values in case we don't find the Shell's registry key
	m_sizBigSystemIcon.cx = GetSystemMetrics(SM_CXICON);
	m_sizBigSystemIcon.cy = GetSystemMetrics(SM_CYICON);

	// get the Shell's registry key for the large icon size - the large icons which are 
	// returned by the Shell are based on that size rather than on the system icon size
	CRegKey key;
	if (key.Open(HKEY_CURRENT_USER, _T("Control Panel\\desktop\\WindowMetrics"), KEY_READ) == ERROR_SUCCESS)
	{
		TCHAR szShellLargeIconSize[12];
		ULONG ulChars = _countof(szShellLargeIconSize);
		if (key.QueryStringValue(_T("Shell Icon Size"), szShellLargeIconSize, &ulChars) == ERROR_SUCCESS)
		{
			UINT uIconSize = 0;
			if (_stscanf_s(szShellLargeIconSize, _T("%u"), &uIconSize) == 1 && uIconSize > 0)
			{
				m_sizBigSystemIcon.cx = uIconSize;
				m_sizBigSystemIcon.cy = uIconSize;
			}
		}
	}
}
void CwyytApp::CreateAllFonts()
{
	///////////////////////////////////////////////////////////////////////////
	// Symbol font
	//
	//VERIFY( CreatePointFont(m_fontSymbol, 10 * 10, _T("Marlett")) );
	// Creating that font with 'SYMBOL_CHARSET' should be safer (seen in ATL/MFC code). Though
	// it seems that it does not solve the problem with '6' and '9' characters which are
	// shown for some ppl.
	m_fontSymbol.CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"));//Marlett 这种字体支持上下箭头显示//Fixedsys


	///////////////////////////////////////////////////////////////////////////
	// Default GUI Font
	//
	// Fonts which are returned by 'GetStockObject'
	// --------------------------------------------
	// OEM_FIXED_FONT		Terminal
	// ANSI_FIXED_FONT		Courier
	// ANSI_VAR_FONT		MS Sans Serif
	// SYSTEM_FONT			System
	// DEVICE_DEFAULT_FONT	System
	// SYSTEM_FIXED_FONT	Fixedsys
	// DEFAULT_GUI_FONT		MS Shell Dlg (*1)
	//
	// (*1) Do not use 'GetStockObject(DEFAULT_GUI_FONT)' to get the 'Tahoma' font. It does
	// not work...
	//	
	// The documentation in MSDN states that DEFAULT_GUI_FONT returns 'Tahoma' on 
	// Win2000/XP systems. Though this is wrong, it may be true for US-English locales, but
	// it is wrong for other locales. Furthermore it is even documented that "MS Shell Dlg"
	// gets mapped to "MS Sans Serif" on Windows XP systems. Only "MS Shell Dlg 2" would
	// get mapped to "Tahoma", but "MS Shell Dlg 2" can not be used on prior Windows
	// systems.
	//
	// The reason why "MS Shell Dlg" is though mapped to "Tahoma" when used within dialog
	// resources is unclear.
	//
	// So, to get the same font which is used within dialogs which were created via dialog
	// resources which have the "MS Shell Dlg, 8" specified (again, in that special case
	// "MS Shell Dlg" gets mapped to "Tahoma" and not to "MS Sans Serif"), we just query
	// the main window (which is also a dialog) for the current font.
	//
	LOGFONT lfDefault;
	AfxGetMainWnd()->GetFont()->GetLogFont(&lfDefault);
	// WinXP: lfDefault.lfFaceName = "MS Shell Dlg 2" (!)
	// Vista: lfDefault.lfFaceName = "MS Shell Dlg 2"
	//
	// It would not be an error if that font name does not match our pre-determined
	// font name, I just want to know if that ever happens.
	//ASSERT( m_strDefaultFontFaceName == lfDefault.lfFaceName );


	///////////////////////////////////////////////////////////////////////////
	// Bold Default GUI Font
	//
	LOGFONT lfDefaultBold = lfDefault;
	lfDefaultBold.lfWeight = FW_BOLD;
	VERIFY( m_fontDefaultBold.CreateFontIndirect(&lfDefaultBold) );


	///////////////////////////////////////////////////////////////////////////
	// Server Log-, Message- and IRC-Window font
	//
	// Since we use "MS Shell Dlg 2" under WinXP (which will give us "Tahoma"),
	// that font is nevertheless set to "MS Sans Serif" because a scaled up "Tahoma"
	// font unfortunately does not look as good as a scaled up "MS Sans Serif" font.
	//
	// No! Do *not* use "MS Sans Serif" (never!). This will give a very old fashioned
	// font on certain Asian Windows systems. So, better use "MS Shell Dlg" or 
	// "MS Shell Dlg 2" to let Windows map that font to the proper font on all Windows
	// systems.
	//
	LPLOGFONT plfHyperText = thePrefs.GetHyperTextLogFont();
	if (plfHyperText==NULL || plfHyperText->lfFaceName[0]==_T('\0') || !m_fontHyperText.CreateFontIndirect(plfHyperText))
		CreatePointFont(m_fontHyperText, 10 * 10, lfDefault.lfFaceName);

	///////////////////////////////////////////////////////////////////////////
	// Verbose Log-font
	//
	// Why can't this font set via the font dialog??
	//	HFONT hFontMono = CreateFont(10, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Lucida Console"));
	//	m_fontLog.Attach(hFontMono);
	LPLOGFONT plfLog = thePrefs.GetLogFont();
	if (plfLog!=NULL && plfLog->lfFaceName[0]!=_T('\0'))
		m_fontLog.CreateFontIndirect(plfLog);

	///////////////////////////////////////////////////////////////////////////
	// Font used for Message and IRC edit control, default font, just a little
	// larger.
	//
	// Since we use "MS Shell Dlg 2" under WinXP (which will give us "Tahoma"),
	// that font is nevertheless set to "MS Sans Serif" because a scaled up "Tahoma"
	// font unfortunately does not look as good as a scaled up "MS Sans Serif" font.
	//
	// No! Do *not* use "MS Sans Serif" (never!). This will give a very old fashioned
	// font on certain Asian Windows systems. So, better use "MS Shell Dlg" or 
	// "MS Shell Dlg 2" to let Windows map that font to the proper font on all Windows
	// systems.
	//
	CreatePointFont(m_fontChatEdit, 11 * 10, lfDefault.lfFaceName);
}


const CString &CwyytApp::GetDefaultFontFaceName()
{
	if (m_strDefaultFontFaceName.IsEmpty())
	{
		/*OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof(osvi);
		if (GetVersionEx(&osvi)
			&& osvi.dwPlatformId == VER_PLATFORM_WIN32_NT
			&& osvi.dwMajorVersion >= 5) // Win2000/XP or higher*/
			m_strDefaultFontFaceName = _T("MS Shell Dlg 2");
		//else
			//m_strDefaultFontFaceName = _T("MS Shell Dlg");
	}
	return m_strDefaultFontFaceName;
}

void CwyytApp::SearchClipboard()
{
	/*if (m_bGuardClipboardPrompt)
		return;*/

	CString strLinks = CopyTextFromClipboard();
	if (strLinks.IsEmpty() || strLinks.Compare(m_strLastClipboardContents) == 0)
		return;

	// Do not alter (trim) 'strLinks' and then copy back to 'm_strLastClipboardContents'! The
	// next clipboard content compare would fail because of the modified string.
	LPCTSTR pszTrimmedLinks = strLinks;
	while (_istspace((_TUCHAR)*pszTrimmedLinks)) // Skip leading whitespaces
		pszTrimmedLinks++;
	if (_tcsnicmp(pszTrimmedLinks, _T("ed2k://|file|"), 13) == 0)
	{
		//m_bGuardClipboardPrompt = true;

		// NEO: MLD - [ModelesDialogs] -- Xanatos -->
		//if(!emuledlg->directdowndlg){ // X: [UIC] - [UIChange] allow change cat
		//	emuledlg->directdowndlg = new CDirectDownloadDlg();
		//	emuledlg->directdowndlg->ClipboardPromptMode(pszTrimmedLinks);
		//	emuledlg->directdowndlg->OpenDialog();
		//}
		//emuledlg->directdowndlg->AddLink(pszTrimmedLinks);
		//dialog->setw();
		// NEO: MLD END <-- Xanatos --
		// Don't feed too long strings into the MessageBox function, it may freak out..
		/*CString strLinksDisplay;
		if (strLinks.GetLength() > 512)
			strLinksDisplay = strLinks.Left(512) + _T("...");
		else
			strLinksDisplay = strLinks;
		if (AfxMessageBox(GetResString(IDS_ADDDOWNLOADSFROMCB) + _T("\r\n") + strLinksDisplay, MB_YESNO | MB_TOPMOST) == IDYES)
			AddEd2kLinksToDownload(pszTrimmedLinks, emuledlg->transferwnd->GetDownloadList()->curTab, true); //Xman [MoNKi: -Check already downloaded files-]// X: [AC] - [ActionChange] use transferwnd's select cat as default cat*/
	}
	m_strLastClipboardContents = strLinks; // Save the unmodified(!) clipboard contents
}


bool CwyytApp::ProcessCommandline()
{
	bool bIgnoreRunningInstances = (GetProfileInt(_T("wyyt"), _T("IgnoreInstances"), 0) != 0);

	for (int i = 1; i < __argc; i++){
		LPCTSTR pszParam = __targv[i];
		if (pszParam[0] == _T('-') || pszParam[0] == _T('/')){
			pszParam++;
#ifdef _DEBUG
		//	if (_tcsicmp(pszParam, _T("assertfile")) == 0)
		//		_CrtSetReportHook(CrtDebugReportCB);
#endif
			if (_tcsicmp(pszParam, _T("ignoreinstances")) == 0)
				bIgnoreRunningInstances = true;

			if (_tcsicmp(pszParam, _T("AutoStart")) == 0)
				m_bAutoStart = true;
		}
	}

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// If we create our TCP listen socket with SO_REUSEADDR, we have to ensure that there are
	// not 2 emules are running using the same port.
	// NOTE: This will not prevent from some other application using that port!
	UINT uTcpPort = GetProfileInt(_T("wyyt"), _T("Port"), DEFAULT_TCP_PORT_OLD);
	CString strMutextName;
	strMutextName.Format(_T("%s:%u"), WYYT_GUID, uTcpPort);
	m_hMutexOneInstance = ::CreateMutex(NULL, FALSE, strMutextName);

	HWND maininst = NULL;
	bool bAlreadyRunning = false;
/*
	// this codepart is to determine special cases when we do add a link to our eMule
	// because in this case it would be nonsense to start another instance!
	if (!bIgnoreRunningInstances ||
		(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen
		&& (cmdInfo.m_strFileName.Find(_T("://")) > 0
		|| CCollection::HasCollectionExtention(cmdInfo.m_strFileName)))
		){
			bAlreadyRunning = (::GetLastError() == ERROR_ALREADY_EXISTS ||::GetLastError() == ERROR_ACCESS_DENIED);
			if (bAlreadyRunning) EnumWindows(SearchEmuleWindow, (LPARAM)&maininst);
	}

	//>>> Fix for "exit" command (Original by leuk_he)
	if(!bAlreadyRunning)
	{
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen
			&& cmdInfo.m_strFileName.Find(L"exit") > 0)
			return TRUE; //do NOT start!
	}
	//<<< Fix for "exit" command

	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{
		CString* command = new CString(cmdInfo.m_strFileName);
		if (command->Find(_T("://"))>0) {
			sendstruct.cbData = (command->GetLength() + 1)*sizeof(TCHAR);
			sendstruct.dwData = OP_ED2KLINK; 
			sendstruct.lpData = const_cast<LPTSTR>((LPCTSTR)*command); 
			if (maininst){
				SendMessage(maininst, WM_COPYDATA, (WPARAM)0, (LPARAM)(PCOPYDATASTRUCT)&sendstruct);
				delete command;
				return true; 
			} 
			else 
				pstrPendingLink = command;
		}
		else if (CCollection::HasCollectionExtention(*command)) 
		{
			sendstruct.cbData = (command->GetLength() + 1)*sizeof(TCHAR);
			sendstruct.dwData = OP_COLLECTION; 
			sendstruct.lpData = const_cast<LPTSTR>((LPCTSTR)*command); 
			if (maininst){
				SendMessage(maininst, WM_COPYDATA, (WPARAM)0, (LPARAM)(PCOPYDATASTRUCT)&sendstruct);
				delete command;
				return true; 
			} 
			else 
				pstrPendingLink = command;
		}
		else 
		{
			sendstruct.cbData = (command->GetLength() + 1)*sizeof(TCHAR);
			sendstruct.dwData = OP_CLCOMMAND;
			sendstruct.lpData = const_cast<LPTSTR>((LPCTSTR)*command); 
			if (maininst){
				SendMessage(maininst, WM_COPYDATA, (WPARAM)0, (LPARAM)(PCOPYDATASTRUCT)&sendstruct);
				delete command;
				return true;
			}
			// Don't start if we were invoked with 'exit' command.
			if (command->CompareNoCase(_T("exit")) == 0) {
				delete command;
				return true;
			}
			delete command;
		}
	}
	*/
	return (maininst || bAlreadyRunning);
}

bool CwyytApp::IsXPThemeActive() const
{
	// TRUE: If an XP style (and only an XP style) is active
	return theApp.m_ullComCtrlVer < MAKEDLLVERULL(6,16,0,0) && g_xpStyle.IsThemeActive() && g_xpStyle.IsAppThemed();
}

bool CwyytApp::IsVistaThemeActive() const
{
	// TRUE: If a Vista (or better) style is active
	return theApp.m_ullComCtrlVer >= MAKEDLLVERULL(6,16,0,0) && g_xpStyle.IsThemeActive() && g_xpStyle.IsAppThemed();
}
void CwyytApp::ApplySkin(LPCTSTR pszSkinProfile)
{
	thePrefs.SetSkinProfile(pszSkinProfile);
	AfxGetMainWnd()->SendMessage(WM_SYSCOLORCHANGE);
}


HBITMAP CwyytApp::LoadImage(LPCTSTR lpszResourceName, LPCTSTR pszResourceType) const
{
	LPCTSTR pszSkinProfile = thePrefs.GetSkinProfile();
	if (pszSkinProfile != NULL && pszSkinProfile[0] != _T('\0'))
	{
		// load resource file specification from skin profile
		TCHAR szSkinResource[MAX_PATH];
		GetPrivateProfileString(_T("Bitmaps"), lpszResourceName, _T(""), szSkinResource, _countof(szSkinResource), pszSkinProfile);
		if (szSkinResource[0] != _T('\0'))
		{
			// expand any optional available environment strings
			TCHAR szExpSkinRes[MAX_PATH];
			if (ExpandEnvironmentStrings(szSkinResource, szExpSkinRes, _countof(szExpSkinRes)) != 0)
			{
				_tcsncpy(szSkinResource, szExpSkinRes, _countof(szSkinResource) - 1);
				szSkinResource[_countof(szSkinResource) - 1] = _T('\0');
			}

			// create absolute path to resource file
			TCHAR szFullResPath[MAX_PATH];
			if (PathIsRelative(szSkinResource))
			{
				TCHAR szSkinResFolder[MAX_PATH];
				_tcsncpy(szSkinResFolder, pszSkinProfile, _countof(szSkinResFolder) - 1);
				szSkinResFolder[_countof(szSkinResFolder) - 1] = _T('\0');
				PathRemoveFileSpec(szSkinResFolder);
				_tmakepathlimit(szFullResPath, NULL, szSkinResFolder, szSkinResource, NULL);
			}
			else
			{
				_tcsncpy(szFullResPath, szSkinResource, _countof(szFullResPath) - 1);
				szFullResPath[_countof(szFullResPath) - 1] = _T('\0');
			}

			CEnBitmap bmp;
			if (bmp.LoadImage(szFullResPath))
				return (HBITMAP)bmp.Detach();
		}
	}

	CEnBitmap bmp;
	if (bmp.LoadImage(lpszResourceName, pszResourceType))
		return (HBITMAP)bmp.Detach();
	return NULL;
}

HICON CwyytApp::LoadIcon(LPCTSTR lpszResourceName, int cx, int cy, UINT uFlags) const
{
	// Test using of 16 color icons. If 'LR_VGACOLOR' is specified _and_ the icon resource
	// contains a 16 color version, that 16 color version will be loaded. If there is no
	// 16 color version available, Windows will use the next (better) color version found.
#ifdef _DEBUG
	if (g_bLowColorDesktop)
		uFlags |= LR_VGACOLOR;
#endif

	HICON hIcon = NULL;
	LPCTSTR pszSkinProfile = thePrefs.GetSkinProfile();
	if (pszSkinProfile != NULL && pszSkinProfile[0] != _T('\0'))
	{
		// load icon resource file specification from skin profile
		TCHAR szSkinResource[MAX_PATH];
		GetPrivateProfileString(_T("Icons"), lpszResourceName, _T(""), szSkinResource, _countof(szSkinResource), pszSkinProfile);
		if (szSkinResource[0] != _T('\0'))
		{
			// expand any optional available environment strings
			TCHAR szExpSkinRes[MAX_PATH];
			if (ExpandEnvironmentStrings(szSkinResource, szExpSkinRes, _countof(szExpSkinRes)) != 0)
			{
				_tcsncpy(szSkinResource, szExpSkinRes, _countof(szSkinResource) - 1);
				szSkinResource[_countof(szSkinResource) - 1] = _T('\0');
			}

			// create absolute path to icon resource file
			TCHAR szFullResPath[MAX_PATH];
			if (PathIsRelative(szSkinResource))
			{
				TCHAR szSkinResFolder[MAX_PATH];
				_tcsncpy(szSkinResFolder, pszSkinProfile, _countof(szSkinResFolder) - 1);
				szSkinResFolder[_countof(szSkinResFolder) - 1] = _T('\0');
				PathRemoveFileSpec(szSkinResFolder);
				_tmakepathlimit(szFullResPath, NULL, szSkinResFolder, szSkinResource, NULL);
			}
			else
			{
				_tcsncpy(szFullResPath, szSkinResource, _countof(szFullResPath) - 1);
				szFullResPath[_countof(szFullResPath) - 1] = _T('\0');
			}

			// check for optional icon index or resource identifier within the icon resource file
			bool bExtractIcon = false;
			CString strFullResPath = szFullResPath;
			int iIconIndex = 0;
			int iComma = strFullResPath.ReverseFind(_T(','));
			if (iComma != -1){
				if (_stscanf_s((LPCTSTR)strFullResPath + iComma + 1, _T("%d"), &iIconIndex) == 1)
					bExtractIcon = true;
				strFullResPath = strFullResPath.Left(iComma);
			}

			if (bExtractIcon)
			{
				if (uFlags != 0 || !(cx == cy && (cx == 16 || cx == 32)))
				{
					/*static UINT (WINAPI *_pfnPrivateExtractIcons)(LPCTSTR, int, int, int, HICON*, UINT*, UINT, UINT) 
						= (UINT (WINAPI *)(LPCTSTR, int, int, int, HICON*, UINT*, UINT, UINT))GetProcAddress(GetModuleHandle(_T("user32")), _TWINAPI("PrivateExtractIcons"));
					if (_pfnPrivateExtractIcons)
					{
						UINT uIconId;
						(*_pfnPrivateExtractIcons)(strFullResPath, iIconIndex, cx, cy, &hIcon, &uIconId, 1, uFlags);
					}*/
					UINT uIconId;
					PrivateExtractIcons(strFullResPath, iIconIndex, cx, cy, &hIcon, &uIconId, 1, uFlags);
				}

				if (hIcon == NULL)
				{
					HICON aIconsLarge[1] = {0};
					HICON aIconsSmall[1] = {0};
					int iExtractedIcons = ExtractIconEx(strFullResPath, iIconIndex, aIconsLarge, aIconsSmall, 1);
					if (iExtractedIcons > 0) // 'iExtractedIcons' is 2(!) if we get a large and a small icon
					{
						// alway try to return the icon size which was requested
						if (cx == 16 && aIconsSmall[0] != NULL)
						{
							hIcon = aIconsSmall[0];
							aIconsSmall[0] = NULL;
						}
						else if (cx == 32 && aIconsLarge[0] != NULL)
						{
							hIcon = aIconsLarge[0];
							aIconsLarge[0] = NULL;
						}
						else
						{
							if (aIconsSmall[0] != NULL)
							{
								hIcon = aIconsSmall[0];
								aIconsSmall[0] = NULL;
							}
							else if (aIconsLarge[0] != NULL)
							{
								hIcon = aIconsLarge[0];
								aIconsLarge[0] = NULL;
							}
						}

						for (int i = 0; i < _countof(aIconsLarge); i++)
						{
							if (aIconsLarge[i] != NULL)
								VERIFY( DestroyIcon(aIconsLarge[i]) );
							if (aIconsSmall[i] != NULL)
								VERIFY( DestroyIcon(aIconsSmall[i]) );
						}
					}
				}
			}
			else
			{
				// WINBUG:ExtractIcon' does not work well on ICO-files when using the color 
				// scheme 'Windows-Standard (extragroß)' -> always try to use 'LoadImage'!
				//
				// If the ICO file contains a 16x16 icon, 'LoadImage' will though return a 32x32 icon,
				// if LR_DEFAULTSIZE is specified! -> always specify the requested size!
				hIcon = (HICON)::LoadImage(NULL, szFullResPath, IMAGE_ICON, cx, cy, uFlags | LR_LOADFROMFILE);
				if (hIcon == NULL && GetLastError() != ERROR_PATH_NOT_FOUND && g_gdiplus.IsInited()) // X: [GPUI] - [GDI Plus UI]
				{
					/*ULONG_PTR gdiplusToken = 0;
					Gdiplus::GdiplusStartupInput gdiplusStartupInput;
					if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) == Gdiplus::Ok)
					{*/
					Gdiplus::Bitmap bmp(szFullResPath);
					bmp.GetHICON(&hIcon);
					//}
					//Gdiplus::GdiplusShutdown(gdiplusToken);
				}
			}
		}
	}

	if (hIcon == NULL)
	{
		if (cx != LR_DEFAULTSIZE || cy != LR_DEFAULTSIZE || uFlags != LR_DEFAULTCOLOR)
			hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), lpszResourceName, IMAGE_ICON, cx, cy, uFlags);
		if (hIcon == NULL)
		{
			//TODO: Either do not use that function or copy the returned icon. All the calling code is designed
			// in a way that the icons returned by this function are to be freed with 'DestroyIcon'. But an 
			// icon which was loaded with 'LoadIcon', is not be freed with 'DestroyIcon'.
			// Right now, we never come here...
			//ASSERT(0);
			hIcon = CWinApp::LoadIcon(lpszResourceName);
		}
	}
	return hIcon;
}
HICON CwyytApp::LoadIcon(UINT nIDResource) const
{
	// use string resource identifiers!!
	return CWinApp::LoadIcon(nIDResource);
}

bool CwyytApp::GetLangHelpFilePath(CString& strResult)
{
	// Change extension for help file
	CString strHelpFile = m_pszHelpFilePath;
	CFileFind ff;
	bool bFound;
	int pos = strHelpFile.ReverseFind(_T('\\'));   //CML
	if (thePrefs.GetLanguageID() != MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT)) //SUBLANG_CHINESE_SIMPLIFIED
	{
		CString temp;
		temp.Format(_T("%s\\wyyt.%u.chm"), strHelpFile.Left(pos), thePrefs.GetLanguageID());
		if (pos>0)
			strHelpFile = temp;

		// if not exists, use original help (english)
		if (!ff.FindFile(strHelpFile, 0)){
			strHelpFile = m_pszHelpFilePath;
			bFound = false;
		}
		else
			bFound = true;
		strHelpFile.Replace(_T(".HLP"), _T(".chm"));
	}
	else{
		//int pos = strHelpFile.ReverseFind(_T('\\'));
		CString temp;
		temp.Format(_T("%s\\wyyt.chm"), strHelpFile.Left(pos));
		strHelpFile = temp;
		strHelpFile.Replace(_T(".HLP"), _T(".chm"));
		if (!ff.FindFile(strHelpFile, 0))
			bFound = false;
		else
			bFound = true;
	}
	ff.Close();
	strResult = strHelpFile;
	return bFound;
}

void CwyytApp::SetHelpFilePath(LPCTSTR pszHelpFilePath)
{
	free((void*)m_pszHelpFilePath);
	m_pszHelpFilePath = _tcsdup(pszHelpFilePath);
}
void CwyytApp::ShowHelp(UINT uTopic, UINT uCmd)
{
	CString strHelpFilePath;
	bool bResult = GetLangHelpFilePath(strHelpFilePath);
	if (!bResult){
		if (ShowWebHelp(uTopic))
			return;
	}
	SetHelpFilePath(strHelpFilePath);
	WinHelpInternal(uTopic, uCmd);
}

bool CwyytApp::ShowWebHelp(UINT uTopic)
{
	CString strHelpURL;
	strHelpURL.Format(_T("http://www.baidu.com"), thePrefs.GetLanguageID(), uTopic);
	ShellExecute(NULL, NULL, strHelpURL, NULL, thePrefs.GetMuleDirectory(WYYT_CONFIGDIR), SW_SHOWDEFAULT);
	return true;
}


BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
{
	// *) This function is invoked by the system from within a *DIFFERENT* thread !!
	//
	// *) This function is invoked only, if eMule was started with "RUNAS"
	//		- when user explicitly/manually logs off from the system (CTRL_LOGOFF_EVENT).
	//		- when user explicitly/manually does a reboot or shutdown (also: CTRL_LOGOFF_EVENT).
	//		- when eMule issues a ExitWindowsEx(EWX_LOGOFF/EWX_REBOOT/EWX_SHUTDOWN)
	//
	// NOTE: Windows will in each case forcefully terminate the process after 20 seconds!
	// Every action which is started after receiving this notification will get forcefully
	// terminated by Windows after 20 seconds.


	if (dwCtrlType==CTRL_CLOSE_EVENT || dwCtrlType==CTRL_LOGOFF_EVENT || dwCtrlType==CTRL_SHUTDOWN_EVENT)
	{
		if (theApp.wyytdlg && theApp.wyytdlg->m_hWnd)
		{
			
			// Use 'SendMessage' to send the message to the (different) main thread. This is
			// done by intention because it lets this thread wait as long as the main thread
			// has called 'ExitProcess' or returns from processing the message. This is
			// needed to not let Windows terminate the process before the 20 sec. timeout.
			if (!theApp.wyytdlg->SendMessage(TM_CONSOLETHREADEVENT, dwCtrlType, (LPARAM)GetCurrentThreadId()))
			{
				theApp.m_app_state = APP_STATE_SHUTTINGDOWN; // as a last attempt
				
			}
		}
	}

	// Returning FALSE does not cause Windows to immediatly terminate the process. Though,
	// that only depends on the next registered console control handler. The default seems
	// to wait 20 sec. until the process has terminated. After that timeout Windows
	// nevertheless terminates the process.
	//
	// For whatever unknown reason, this is *not* always true!? It may happen that Windows
	// terminates the process *before* the 20 sec. timeout if (and only if) the console
	// control handler thread has already terminated. So, we have to take care that we do not
	// exit this thread before the main thread has called 'ExitProcess' (in a synchronous
	// way) -- see also the 'SendMessage' above.
	
	return FALSE; // FALSE: Let the system kill the process with the default handler.
}

CTempIconLoader::CTempIconLoader(LPCTSTR pszResourceID, int cx, int cy, UINT uFlags)
{
	m_hIcon = theApp.LoadIcon(pszResourceID, cx, cy, uFlags);
}

CTempIconLoader::CTempIconLoader(UINT uResourceID, int /*cx*/, int /*cy*/, UINT uFlags)
{
	UNREFERENCED_PARAMETER(uFlags);
	ASSERT( uFlags == 0 );
	m_hIcon = theApp.LoadIcon(uResourceID);
}

CTempIconLoader::~CTempIconLoader()
{
	if (m_hIcon)
		VERIFY( DestroyIcon(m_hIcon) );
}



int CwyytApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	wyytExtendDataManage::Instance().Unit();
	return CWinApp::ExitInstance();
}

bool CwyytApp::LoadSkinColor(LPCTSTR pszKey, COLORREF& crColor) const
{
	LPCTSTR pszSkinProfile = thePrefs.GetSkinProfile();
	if (pszSkinProfile != NULL && pszSkinProfile[0] != _T('\0'))
	{
		TCHAR szColor[MAX_PATH];
		GetPrivateProfileString(_T("Colors"), pszKey, _T(""), szColor, _countof(szColor), pszSkinProfile);
		if (szColor[0] != _T('\0'))
		{
			UINT red, grn, blu;
			int iVals = _stscanf_s(szColor, _T("%i , %i , %i"), &red, &grn, &blu);
			if (iVals == 3)
			{
				crColor = RGB(red, grn, blu);
				return true;
			}
		}
	}
	return false;
}

bool CwyytApp::LoadSkinColorAlt(LPCTSTR pszKey, LPCTSTR pszAlternateKey, COLORREF& crColor) const
{
	if (LoadSkinColor(pszKey, crColor))
		return true;
	return LoadSkinColor(pszAlternateKey, crColor);
}


void CwyytApp::UpdateDesktopColorDepth()
{
	g_bLowColorDesktop = (GetDesktopColorDepth() <= 8);
//#ifdef _DEBUG
//	if (!g_bLowColorDesktop)
//		g_bLowColorDesktop = (GetProfileInt(_T("eMule"), _T("LowColorRes"), 0) != 0);
//#endif

	if (g_bLowColorDesktop)
	{
		// If we have 4- or 8-bit desktop color depth, Windows will (by design) load only 
		// the 16 color versions of icons. Thus we force all image lists also to 4-bit format.
		m_iDfltImageListColorFlags = ILC_COLOR4;
	}
	else
	{
		// Get current desktop color depth and derive the image list format from it
		m_iDfltImageListColorFlags = GetAppImageListColorFlag();

		// Don't use 32-bit image lists if not supported by COMCTL32.DLL
		if (m_iDfltImageListColorFlags == ILC_COLOR32 && m_ullComCtrlVer < MAKEDLLVERULL(6,0,0,0))
		{
			// We fall back to 16-bit image lists because we do not provide 24-bit
			// versions of icons any longer (due to resource size restrictions for Win98). We
			// could also fall back to 24-bit image lists here but the difference is minimal
			// and considered not to be worth the additinoal memory consumption.
			//
			// Though, do not fall back to 8-bit image lists because this would let Windows
			// reduce the color resolution to the standard 256 color window system palette.
			// We need a 16-bit or 24-bit image list to hold all our 256 color icons (which
			// are not pre-quantized to standard 256 color windows system palette) without
			// loosing any colors.
			m_iDfltImageListColorFlags = ILC_COLOR16;
		}
	}

	// Doesn't help..
	//m_aExtToSysImgIdx.RemoveAll();
}
