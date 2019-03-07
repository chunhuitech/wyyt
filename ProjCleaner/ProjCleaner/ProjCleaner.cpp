// ProjCleaner.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "ProjCleanerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProjCleanerApp

BEGIN_MESSAGE_MAP(CProjCleanerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CProjCleanerApp construction

CProjCleanerApp::CProjCleanerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CProjCleanerApp object

CProjCleanerApp theApp;

BOOL m_bEndThread = FALSE;

// CProjCleanerApp initialization

BOOL BringBoxWindowToTop()
{
	HWND hBoxWnd = FindWindow(_T("#32770"), _T("友情提示"));
	if (hBoxWnd) {
		HWND hStaticTextWnd = GetDlgItem(hBoxWnd, 0xFFFF);
		TCHAR szText[MAX_PATH] = {0};
		if (hStaticTextWnd) {
			GetDlgItemText(hBoxWnd, 0xFFFF, szText, MAX_PATH);
			if (_tcscmp(szText, _T("部分文件无法删除，请确认此文件夹中的文件未被使用中！")) == 0) {
				SetWindowPos(hBoxWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				return TRUE;
			}
		}
	}
	return FALSE;
}

UINT __cdecl ThreadProc(LPVOID pParam)
{
	while (!m_bEndThread) {
		BringBoxWindowToTop();
		Sleep(100);
	}
	return TRUE;
}

BOOL CProjCleanerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//for the command line;
	int argc = 0;
	LPWSTR *lpszArgv = NULL;
	LPTSTR szCmdLine = GetCommandLine();
	lpszArgv = CommandLineToArgvW(szCmdLine, &argc);
	if (argc >= 2) {
		WCHAR szPath[500] = {0};
		swprintf(szPath, _T("%s%s%s"), _T("确定要清理 [ "), lpszArgv[1], _T(" ] 目录吗？"));
		if (MessageBox(GetDesktopWindow(), szPath, _T("友情提示"), MB_OKCANCEL|MB_ICONQUESTION) ==  IDOK) {
			m_bStartFromCmd = TRUE;
			CleanStart(GetTreeList(), lpszArgv[1], RegReadWriteDword(_T("DeleteToRecycle"), TRUE, 0));
			if (dwFailed > 0) {
				AfxBeginThread(ThreadProc, NULL);
				MessageBox(GetActiveWindow(), _T("部分文件无法删除，请确认此文件夹中的文件未被使用中！"), _T("友情提示"), MB_OK | MB_ICONWARNING);
				m_bEndThread = TRUE;
			}
		}
		return FALSE;
	}
	//command line end;
	////for the command line;
	//int argc = 0;
	//LPWSTR *lpszArgv = NULL;
	//LPTSTR szCmdLine = GetCommandLine();
	//lpszArgv = CommandLineToArgvW(szCmdLine, &argc);
	//if (argc >= 2) {
	//	int idx = 1;
	//	CString strPath;
	//	for (idx = 1; idx < argc; idx++) {
	//		strPath += lpszArgv[idx];
	//		strPath += _T("\r\n");
	//	}
	//	strPath = _T("确定清除以下目录吗：\r\n") + strPath;
	//	if (MessageBox(NULL, strPath, _T("友情提示"), MB_OKCANCEL|MB_ICONQUESTION) ==  IDOK) {
	//		m_bStartFromCmd = TRUE;
	//		for (idx = 1; idx < argc; idx++) {
	//			CleanStart(GetTreeList(), lpszArgv[idx], RegReadWriteDword(_T("DeleteToRecycle"), TRUE, 0));
	//		}
	//	}
	//	return FALSE;
	//}
	////command line end;

	CProjCleanerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
