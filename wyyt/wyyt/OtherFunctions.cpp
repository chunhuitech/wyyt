#include "StdAfx.h"
#include "OtherFunctions.h"

/** \file OtherFunctions.cpp
******************************************************************************** 
<PRE> 
模块名 : < wyyt >
文件名 : <OtherFunctions.cpp> 
相关文件 : < OtherFunctions.h >
文件实现功能 : 其它通用函数
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
YYYY/MM/DD    X.Y        <IHAS>     < 修改内容 > 
</PRE> 
*******************************************************************************/



WORD DetectWinVersion()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (!GetVersionEx((OSVERSIONINFO*)&osvi))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO*)&osvi))
			return 0;
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_NT:
			if (osvi.dwMajorVersion <= 4)
				return 0;
//				return _WINVER_NT4_;
			if (osvi.dwMajorVersion == 5){
				if(osvi.dwMinorVersion == 0)
					return _WINVER_2K_;
				if (osvi.dwMinorVersion == 1)
					return _WINVER_XP_;
				//if (osvi.dwMinorVersion == 2)
				return _WINVER_2003_;
			}
			if (osvi.dwMajorVersion == 6){
				if(osvi.dwMinorVersion == 0)
					return _WINVER_VISTA_;
				if(osvi.dwMinorVersion == 1)
					return _WINVER_7_;
			}
			return _WINVER_7_;

		//case VER_PLATFORM_WIN32_WINDOWS:
			//return 0;
/*			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				return _WINVER_95_; 
			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				return _WINVER_98_; 
			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				return _WINVER_ME_; 
			break;
*/		
		default:
			return 0;
//			return _WINVER_95_;		// there should'nt be anything lower than this
	}
	
}

int GetDesktopColorDepth()
{
	HDC hdcScreen = ::GetDC(HWND_DESKTOP);
	int iColorBits = GetDeviceCaps(hdcScreen, BITSPIXEL) * GetDeviceCaps(hdcScreen, PLANES);
	::ReleaseDC(HWND_DESKTOP, hdcScreen);
	return iColorBits;
}

int GetAppImageListColorFlag()
{
	int iColorBits = GetDesktopColorDepth();
	if (iColorBits >= 32) // Avi-3k
		return ILC_COLOR32;
	if (iColorBits >= 24)
		return ILC_COLOR24;
	if (iColorBits >= 16)
		return ILC_COLOR16;
	if (iColorBits >= 8)
		return ILC_COLOR8;
	if (iColorBits >= 4)
		return ILC_COLOR4;
	return ILC_COLOR;
}
CString ShellGetFolderPath(int iCSIDL)
{
/*	CString strFolderPath;

	// Try the Unicode version from "shell32" *and* examine the function result - just the presence of that
	// function does not mean that it returns the requested path.
	//
	// Win98: 'SHGetFolderPathW' is available in 'shell32.dll', but it does not support all of the CSIDL values.
	HRESULT (WINAPI *pfnSHGetFolderPathW)(HWND, int, HANDLE, DWORD, LPWSTR);
	(FARPROC&)pfnSHGetFolderPathW = GetProcAddress(GetModuleHandle(_T("shell32")), "SHGetFolderPathW");
	if (pfnSHGetFolderPathW)
	{
		WCHAR wszPath[MAX_PATH];
		if ((*pfnSHGetFolderPathW)(NULL, iCSIDL, NULL, SHGFP_TYPE_CURRENT, wszPath) == S_OK)
			strFolderPath = wszPath;
	}

	if (strFolderPath.IsEmpty())
	{
		HMODULE hLibShFolder = LoadLibrary(_T("shfolder.dll"));
		if (hLibShFolder)
		{
			HRESULT (WINAPI *pfnSHGetFolderPathW)(HWND, int, HANDLE, DWORD, LPWSTR);
			(FARPROC&)pfnSHGetFolderPathW = GetProcAddress(hLibShFolder, "SHGetFolderPathW");
			if (pfnSHGetFolderPathW)
			{
				WCHAR wszPath[MAX_PATH];
				if ((*pfnSHGetFolderPathW)(NULL, iCSIDL, NULL, SHGFP_TYPE_CURRENT, wszPath) == S_OK)
					strFolderPath = wszPath;
			}

			if (strFolderPath.IsEmpty())
			{
				HRESULT (WINAPI *pfnSHGetFolderPathA)(HWND, int, HANDLE, DWORD, LPSTR);
				(FARPROC&)pfnSHGetFolderPathA = GetProcAddress(hLibShFolder, "SHGetFolderPathA");
				if (pfnSHGetFolderPathA)
				{
					CHAR aszPath[MAX_PATH];
					if ((*pfnSHGetFolderPathA)(NULL, iCSIDL, NULL, SHGFP_TYPE_CURRENT, aszPath) == S_OK)
						strFolderPath = CString(aszPath);
				}
			}
			FreeLibrary(hLibShFolder);
		}
	}
	return strFolderPath;*/
	WCHAR wszPath[MAX_PATH];
	VERIFY(SHGetFolderPath(NULL, iCSIDL, NULL, SHGFP_TYPE_CURRENT, wszPath) == S_OK);
	return wszPath;
}
CString GetFileState(int nStateFlage)
{
	CString csFileStateInfo;
	switch ( nStateFlage )
	{
	case E_FS_UNDOWNED:
		csFileStateInfo = GetResString(IDS_FS_UNDOWNED);
		break;
	case E_FS_DOWNED:
		csFileStateInfo = GetResString(IDS_FS_DOWNED);
		break;
	default:
		break;
	}
	return csFileStateInfo;
}


CString CastItoXBytes(uint16 count, bool isK, bool isPerSec, uint32 decimal){
	return CastItoXBytes((double)count, isK, isPerSec, decimal);
}

CString CastItoXBytes(uint32 count, bool isK, bool isPerSec, uint32 decimal){
	return CastItoXBytes((double)count, isK, isPerSec, decimal);
}

CString CastItoXBytes(uint64 count, bool isK, bool isPerSec, uint32 decimal){
	return CastItoXBytes((double)count, isK, isPerSec, decimal);
}

CString CastItoXBytes(float count, bool isK, bool isPerSec, uint32 decimal){
	return CastItoXBytes((double)count, isK, isPerSec, decimal);
}
CString CastItoXBytes(double count, bool isK, bool isPerSec, uint32 decimal){
	if( count <= 0.0 )
		return _T("0 ") + GetResString(isPerSec?(/*thePrefs.GetForceSpeedsToKB()*/true?IDS_KBYTESPERSEC:IDS_BYTESPERSEC):IDS_BYTES);
	if( isK ){
		if( count >  1.7E+300 )
			count =  1.7E+300;
		else
			count *= 1024.0;
	}
	CString buffer;
	if( isPerSec )
	{
		//Xman Xtreme Mod
		//use other system if decimal=99 (standard)
		if(decimal==99)
		{
			if (/*thePrefs.GetForceSpeedsToKB()*/true || count < 1024000.0)
				buffer.Format(_T("%.1f %s"), count/1024.0, GetResString(IDS_KBYTESPERSEC));
			else if (count < 1048576000.0)
				buffer.Format(_T("%.2f %s"), count/1048576.0, GetResString(IDS_MBYTESPERSEC));
			else
				buffer.Format(_T("%.3f %s"), count/1073741824.0, GetResString(IDS_GBYTESPERSEC));
		}
		else
		{
			if (/*thePrefs.GetForceSpeedsToKB()*/true )
				buffer.Format(_T("%.*f %s"), decimal, count/1024.0, GetResString(IDS_KBYTESPERSEC));
			else if (count < 1024.0)
				buffer.Format(_T("%.0f %s"), count, GetResString(IDS_BYTESPERSEC));
			else if (count < 1024000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1024.0, GetResString(IDS_KBYTESPERSEC));
			else if (count < 1048576000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1048576.0, GetResString(IDS_MBYTESPERSEC));
			else if (count < 1073741824000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1073741824.0, GetResString(IDS_GBYTESPERSEC));
			else 
				buffer.Format(_T("%.*f %s"), decimal, count/1099511627776.0, GetResString(IDS_TBYTESPERSEC));
			//Xman end
		}
	}
	else
	{
		//Xman Xtreme Mod
		//use other system if decimal=99 (standard)
		if(decimal==99)
		{
			if (count < 1024)
				buffer.Format(_T("%.0f %s"),count, GetResString(IDS_BYTES));
			else if (count < 1048576)
				buffer.Format(_T("%.1f %s"),count/1024.0f, GetResString(IDS_KBYTES));
			else if (count < 1073741824)
				buffer.Format(_T("%.2f %s"),count/1048576.0f, GetResString(IDS_MBYTES));
			else if (count < 1099511627776)
				buffer.Format(_T("%.2f %s"),count/1073741824.0f, GetResString(IDS_GBYTES));
			else 
				buffer.Format(_T("%.3f %s"),count/1099511627776.0f, GetResString(IDS_TBYTES));
		}
		else
		{
			if (count < 1024.0)
				buffer.Format(_T("%.0f %s"), count, GetResString(IDS_BYTES));
			else if (count < 1024000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1024.0, GetResString(IDS_KBYTES));
			else if (count < 1048576000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1048576.0, GetResString(IDS_MBYTES));
			else if (count < 1073741824000.0)
				buffer.Format(_T("%.*f %s"), decimal, count/1073741824.0, GetResString(IDS_GBYTES));
			else 
				buffer.Format(_T("%.*f %s"), decimal, count/1099511627776.0, GetResString(IDS_TBYTES));
		}
		//Xman end
	}
	return buffer;
}
int GetSystemErrorString(DWORD dwError, CString &rstrError)
{
	// FormatMessage language flags:
	//
	// - MFC uses: MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT)
	//				SUBLANG_SYS_DEFAULT = 0x02 (system default)
	//
	// - SDK uses: MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
	//				SUBLANG_DEFAULT		= 0x01 (user default)
	//
	//
	// Found in "winnt.h"
	// ------------------
	//  Language IDs.
	//
	//  The following two combinations of primary language ID and
	//  sublanguage ID have special semantics:
	//
	//    Primary Language ID   Sublanguage ID      Result
	//    -------------------   ---------------     ------------------------
	//    LANG_NEUTRAL          SUBLANG_NEUTRAL     Language neutral
	//    LANG_NEUTRAL          SUBLANG_DEFAULT     User default language
	//    LANG_NEUTRAL          SUBLANG_SYS_DEFAULT System default language
	//
	// *** SDK notes also:
	// If you pass in zero, 'FormatMessage' looks for a message for LANGIDs in 
	// the following order:
	//
	//	1) Language neutral 
	//	2) Thread LANGID, based on the thread's locale value 
	//  3) User default LANGID, based on the user's default locale value 
	//	4) System default LANGID, based on the system default locale value 
	//	5) US English 
	LPTSTR pszSysMsg = NULL;
	DWORD dwLength = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		(LPTSTR)&pszSysMsg, 0, NULL);
	if (pszSysMsg != NULL && dwLength != 0)
	{
		if (dwLength >= 2 && pszSysMsg[dwLength - 2] == _T('\r'))
			pszSysMsg[dwLength - 2] = _T('\0');
		rstrError = pszSysMsg;
		rstrError.Replace(_T("\r\n"), _T(" ")); // some messages contain CRLF within the message!?
	}
	else {
		rstrError.Empty();
	}

	if (pszSysMsg)
		LocalFree(pszSysMsg);

	return rstrError.GetLength();
}

int GetModuleErrorString(DWORD dwError, CString &rstrError, LPCTSTR pszModule)
{
	LPTSTR pszSysMsg = NULL;
	DWORD dwLength = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		GetModuleHandle(pszModule), dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		(LPTSTR)&pszSysMsg, 0, NULL);
	if (pszSysMsg != NULL && dwLength != 0)
	{
		if (dwLength >= 2 && pszSysMsg[dwLength - 2] == _T('\r'))
			pszSysMsg[dwLength - 2] = _T('\0');
		rstrError = pszSysMsg;
		rstrError.Replace(_T("\r\n"), _T(" ")); // some messages contain CRLF within the message!?
	}
	else {
		rstrError.Empty();
	}

	if (pszSysMsg)
		LocalFree(pszSysMsg);

	return rstrError.GetLength();
}

int GetErrorMessage(DWORD dwError, CString &rstrErrorMsg, DWORD dwFlags)
{
	int iMsgLen = GetSystemErrorString(dwError, rstrErrorMsg);
	if (iMsgLen == 0)
	{
		if ((long)dwError >= 0)
			rstrErrorMsg.Format(_T("Error %u"), dwError);
		else
			rstrErrorMsg.Format(_T("Error 0x%08x"), dwError);
	}
	else if (dwFlags & 1)
	{
		CString strFullErrorMsg;
		if ((long)dwError >= 0)
			strFullErrorMsg.Format(_T("Error %u: %s"), dwError, rstrErrorMsg);
		else
			strFullErrorMsg.Format(_T("Error 0x%08x: %s"), dwError, rstrErrorMsg);
		rstrErrorMsg = strFullErrorMsg;
	}

	return rstrErrorMsg.GetLength();
}

CString GetErrorMessage(DWORD dwError, DWORD dwFlags)
{
	CString strError;
	GetErrorMessage(dwError, strError, dwFlags);
	return strError;
}


typedef struct
{
	LPCTSTR	pszInitialDir;
	LPCTSTR	pszDlgTitle;
} BROWSEINIT, *LPBROWSEINIT;

extern "C" int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		// Set initial directory
		if ( ((LPBROWSEINIT)lpData)->pszInitialDir != NULL )
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)((LPBROWSEINIT)lpData)->pszInitialDir);

		// Set dialog's window title
		if ( ((LPBROWSEINIT)lpData)->pszDlgTitle != NULL )
			SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)((LPBROWSEINIT)lpData)->pszDlgTitle);
	}

	return 0;
}
bool SelectDir(HWND hWnd, LPTSTR pszPath, LPCTSTR pszTitle, LPCTSTR pszDlgTitle)
{
	bool bResult = false;
	CoInitialize(0);
	LPMALLOC pShlMalloc;
	if (SHGetMalloc(&pShlMalloc) == NOERROR)
	{
		BROWSEINFO BrsInfo = {0};
		BrsInfo.hwndOwner = hWnd;
		BrsInfo.lpszTitle = (pszTitle != NULL) ? pszTitle : pszDlgTitle;
		BrsInfo.ulFlags = BIF_VALIDATE | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_SHAREABLE | BIF_DONTGOBELOWDOMAIN;

		BROWSEINIT BrsInit = {0};
		if (pszPath != NULL || pszTitle != NULL || pszDlgTitle != NULL){
			// Need the 'BrowseCallbackProc' to set those strings
			BrsInfo.lpfn = BrowseCallbackProc;
			BrsInfo.lParam = (LPARAM)&BrsInit;
			BrsInit.pszDlgTitle = (pszDlgTitle != NULL) ? pszDlgTitle : NULL/*pszTitle*/;
			BrsInit.pszInitialDir = pszPath;
		}

		LPITEMIDLIST pidlBrowse;
		if ((pidlBrowse = SHBrowseForFolder(&BrsInfo)) != NULL){
			if (SHGetPathFromIDList(pidlBrowse, pszPath))
				bResult = true;
			pShlMalloc->Free(pidlBrowse);
		}
		pShlMalloc->Release();
	}
	CoUninitialize();
	return bResult;
}

// Wrapper for _tmakepath which ensures that the outputbuffer does not exceed MAX_PATH
// using a smaller buffer  without checking the sizes prior calling this function is not safe
// If the resulting path would be bigger than MAX_PATH-1, it will be empty and return false (similar to PathCombine)
bool _tmakepathlimit(TCHAR *path, const TCHAR *drive, const TCHAR *dir, const TCHAR *fname, const TCHAR *ext){
	if (path == NULL){
		ASSERT( false );
		return false;
	}

	size_t nSize = 64; // the function should actually only add 4 (+1 nullbyte) bytes max extra
	if (drive != NULL)
		nSize += _tcsclen(drive);
	if (dir != NULL)
		nSize += _tcsclen(dir);
	if (fname != NULL)
		nSize += _tcsclen(fname);
	if (ext != NULL)
		nSize += _tcsclen(ext);

	TCHAR* tchBuffer = new TCHAR[nSize];
	_tmakepath(tchBuffer, drive, dir, fname, ext);

	if (_tcslen(tchBuffer) >= MAX_PATH){
		path[0] = _T('\0');
		ASSERT( false );
		delete[] tchBuffer;
		return false;
	}
	else{
		_tcscpy(path, tchBuffer);
		delete[] tchBuffer;
		return true;
	}
}


int CompareLocaleStringNoCase(LPCTSTR psz1, LPCTSTR psz2)
{
	// SDK says: The 'CompareString' function is optimized to run at the highest speed when 'dwCmpFlags' is set to 0 
	// or NORM_IGNORECASE, and 'cchCount1' and 'cchCount2' have the value -1.
	int iResult = CompareString(GetThreadLocale(), NORM_IGNORECASE, psz1, -1, psz2, -1);
	if (iResult == 0)
		return 0;
	return iResult - 2;
}
void MakeFoldername(CString &rstrPath)
{
	if (!rstrPath.IsEmpty()) // don't canonicalize an empty path, we would get a "\"
	{
		CString strNewPath;
		LPTSTR pszNewPath = strNewPath.GetBuffer(MAX_PATH);
		PathCanonicalize(pszNewPath, rstrPath);
		PathRemoveBackslash(pszNewPath);
		strNewPath.ReleaseBuffer();
		rstrPath = strNewPath;
	}
}
int CompareLocaleString(LPCTSTR psz1, LPCTSTR psz2)
{
	// SDK says: The 'CompareString' function is optimized to run at the highest speed when 'dwCmpFlags' is set to 0 
	// or NORM_IGNORECASE, and 'cchCount1' and 'cchCount2' have the value -1.
	int iResult = CompareString(GetThreadLocale(), 0, psz1, -1, psz2, -1);
	if (iResult == 0)
		return 0;
	return iResult - 2;
}

int __cdecl CompareCStringPtrLocaleStringNoCase(const void* p1, const void* p2)
{
	const CString* pstr1 = (const CString*)p1;
	const CString* pstr2 = (const CString*)p2;
	return CompareLocaleStringNoCase(*pstr1, *pstr2);
}

int __cdecl CompareCStringPtrLocaleString(const void* p1, const void* p2)
{
	const CString* pstr1 = (const CString*)p1;
	const CString* pstr2 = (const CString*)p2;
	return CompareLocaleString(*pstr1, *pstr2);
}


void Sort(CAtlArray<CString>& astr, int (__cdecl *pfnCompare)(const void*, const void*))
{
	qsort(astr.GetData(), astr.GetCount(), sizeof(CString*), pfnCompare);
}


CString GetNextString(const CString& rstr, LPCTSTR pszTokens, int& riStart)
{
	CString strResult;
	if (pszTokens != NULL && riStart != -1)
	{
		int iToken = rstr.Find(pszTokens, riStart);
		if (iToken != -1)
		{
			int iLen = iToken - riStart;
			if (iLen >= 0)
			{
				strResult = rstr.Mid(riStart, iLen);
				riStart += iLen + 1;
			}
		}
		else
		{
			strResult = rstr.Mid(riStart);
			riStart = -1;
		}
	}
	return strResult;
}

CString GetNextString(const CString& rstr, TCHAR chToken, int& riStart)
{
	CString strResult;
	if (chToken != _T('\0') && riStart != -1)
	{
		int iToken = rstr.Find(chToken, riStart);
		if (iToken != -1)
		{
			int iLen = iToken - riStart;
			if (iLen >= 0)
			{
				strResult = rstr.Mid(riStart, iLen);
				riStart += iLen + 1;
			}
		}
		else
		{
			strResult = rstr.Mid(riStart);
			riStart = -1;
		}
	}
	return strResult;
}
TCHAR *stristr(const TCHAR *str1, const TCHAR *str2)
{
	const TCHAR *cp = str1;
	const TCHAR *s1;
	const TCHAR *s2;

	if (!*str2)
		return (TCHAR *)str1;

	while (*cp)
	{
		s1 = cp;
		s2 = str2;

		while (*s1 && *s2 && _totlower((_TUCHAR)*s1) == _totlower((_TUCHAR)*s2))
			s1++, s2++;

		if (!*s2)
			return (TCHAR *)cp;

		cp++;
	}

	return NULL;
}



int FontPointSizeToLogUnits(int nPointSize)
{
	HDC hDC = ::GetDC(HWND_DESKTOP);
	if (hDC)
	{
		POINT pt;
#if 0
		// This is the same math which is performed by "CFont::CreatePointFont",
		// which is flawed because it does not perform any rounding. But without
		// performing the correct rounding one can not get the correct LOGFONT-height
		// for an 8pt font!
		//
		// PointSize	Result
		// -------------------
		// 8*10			10.666 -> 10 (cut down and thus wrong result)
		pt.y = GetDeviceCaps(hDC, LOGPIXELSY) * nPointSize;
		pt.y /= 720;
#else
		// This math accounts for proper rounding and thus we will get the correct results.
		//
		// PointSize	Result
		// -------------------
		// 8*10			10.666 -> 11 (rounded up and thus correct result)
		pt.y = MulDiv(GetDeviceCaps(hDC, LOGPIXELSY), nPointSize, 720);
#endif
		pt.x = 0;
		DPtoLP(hDC, &pt, 1);
		POINT ptOrg = { 0, 0 };
		DPtoLP(hDC, &ptOrg, 1);
		nPointSize = -abs(pt.y - ptOrg.y);
		ReleaseDC(HWND_DESKTOP, hDC);
	}
	return nPointSize;
}

bool CreatePointFontIndirect(CFont &rFont, const LOGFONT *lpLogFont)
{
	LOGFONT logFont = *lpLogFont;
	logFont.lfHeight = FontPointSizeToLogUnits(logFont.lfHeight);
	return rFont.CreateFontIndirect(&logFont) != FALSE;
}
bool CheckFileOpen(LPCTSTR pszFilePath, LPCTSTR pszFileTitle)
{
	/*if (thePrefs.GetCheckFileOpen() && !PathIsURL(pszFilePath) && GetDRM(pszFilePath))
	{
	CString strWarning;
	strWarning.Format(GetResString(IDS_FILE_WARNING_DRM), pszFileTitle != NULL ? pszFileTitle : pszFilePath);
	sint_ptr iAnswer = AfxMessageBox(strWarning, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
	if (iAnswer != IDYES)
	return false;
	}*/
	return true;
}

void ShellOpenFile(CString name)
{
	ShellOpenFile(name, _T("open"));
}

void ShellOpenFile(CString name, LPCTSTR pszVerb)
{
	if ((pszVerb == NULL || pszVerb[0] == _T('\0') || _tcsicmp(pszVerb, _T("open"))) && !CheckFileOpen(name, NULL))
		return;
	ShellExecute(NULL, pszVerb, name, NULL, NULL, SW_SHOW);
}
bool CreatePointFont(CFont &rFont, int nPointSize, LPCTSTR lpszFaceName)
{
	LOGFONT logFont = {0};
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = nPointSize;
	lstrcpyn(logFont.lfFaceName, lpszFaceName, _countof(logFont.lfFaceName));
	return CreatePointFontIndirect(rFont, &logFont);
}

CString RemoveAmbersand(const CString& rstr)
{
	CString str(rstr);
	str.Remove(_T('&'));
	return str;
}
//////////////////////

static char UnIndex64(BYTE nIndex) 
{ 
	char ch; 

	nIndex %= 64;                    // 取到[0,64)范围内 

	if (nIndex < 26)                // [0,26)返回大写字母 
	{ 
		ch = (char)('A' + nIndex); 
	} 
	else if (nIndex < 52)            // 26+[0,26)返回小写字母 
	{ 
		ch = (char)('a' + nIndex - 26); 
	} 
	else if (nIndex < 62)            // 52+[0,10)返回数字 
	{ 
		ch = (char)('0' + nIndex - 52); 
	} 
	else if (nIndex == 62)            // 62返回'_' 
	{ 
		ch = '_'; 
	} 
	else if (nIndex == 63)            // 63返回'.' 
	{ 
		ch = '.'; 
	} 
	else 
	{ 
		ch = 'A'; 
	} 

	return ch; 
} 


static BYTE Index64(char ch) 
{ 
	BYTE nIndex; 

	if (_istupper(ch)) 
	{ 
		nIndex = ch - 'A'; 
	} 
	else if (_istlower(ch)) 
	{ 
		nIndex = 26 + ch - 'a'; 
	} 
	else if (_istdigit(ch)) 
	{ 
		nIndex = 52 + ch - '0'; 
	} 
	else if (ch == '_') 
	{ 
		nIndex = 62; 
	} 
	else if (ch == '.') 
	{ 
		nIndex = 63; 
	} 
	else 
	{ 
		nIndex = 0; 
	} 

	return nIndex; 
} 
static void ToBase64(const char* instr, int len, char* outstr) 
{ 
	ASSERT(instr && len > 0 && len % 3 == 0); 

	int i, j; 
	BYTE ch1, ch2, ch3; 

	i = 0; 
	j = 0; 
	while (i + 2 < len) 
	{ 
		ch1 = (BYTE)instr[i]; 
		ch2 = (BYTE)instr[i + 1]; 
		ch3 = (BYTE)instr[i + 2]; 

		outstr[j] = UnIndex64(ch1 >> 2); 
		outstr[j + 1] = UnIndex64(((ch1 & 0x3) << 4) | (ch2 >> 4)); 
		outstr[j + 2] = UnIndex64(((ch2 & 0x0f) << 2) | (ch3 >> 6)); 
		outstr[j + 3] = UnIndex64(ch3 & 0x3f); 

		i += 3; 
		j += 4; 
	} 
	outstr[j] = '\0'; 
} 


static void UnBase64(const char* instr, int len, char* outstr) 
{ 
	ASSERT(instr && len % 4 == 0); 

	int i, j; 
	BYTE ch1, ch2, ch3, ch4; 

	i = 0; 
	j = 0; 
	while (i + 3 < len) 
	{ 
		ch1 = Index64(instr[i]); 
		ch2 = Index64(instr[i + 1]); 
		ch3 = Index64(instr[i + 2]); 
		ch4 = Index64(instr[i + 3]); 

		outstr[j] =  (ch1 << 2) | ((ch2 >> 4) & 0x3); 
		outstr[j + 1] = (ch2 << 4) | ((ch3 >> 2) & 0xf); 
		outstr[j + 2] = (ch3 << 6) | ch4; 

		i += 4; 
		j += 3; 
	} 
	outstr[j] = '\0'; 
} 

static char EncryptChar(char c) 
{ 
	BYTE x = 0; 

	x += (c & 0x80); 
	x += (c & 0x40) >> 6; 
	x += (c & 0x20) >> 4; 
	x += (c & 0x10) >> 2; 
	x += (c & 0x08);// << 3; 
	x += (c & 0x04) << 2; 
	x += (c & 0x02) << 4; 
	x += (c & 0x01) << 6; 

	return x; 
} 


static inline char randchar() 
{ 
	char c = (char)rand(); 
	return 0 == c ? '\xAA' : c; 
} 
std::string StringEncrypt(const char* srcstr, int srclen)
{ 
	if (srclen < 0) 
	{ 
		srclen = (NULL == srcstr) ? 0 : strlen(srcstr); 
	} 

	int inlen = (1 + srclen + 2) + 1; 
	int outlen = (3 + srclen) / 3 * 4 + 1; 

	char *buf = new char[inlen + outlen]; 
	char *inbuf = buf; 
	char *outbuf = buf + inlen; 

	memset(buf, 0, sizeof(char) * (inlen + outlen)); 

	// 复制源串到(inbuf+1)，每个字符进行移位变换 
	for (int i = 0; i < srclen; i++) 
	{ 
		inbuf[i + 1] = EncryptChar(srcstr[i]); 
	} 

	// 设置长度标记字符 inbuf[0] 
	// 移位变换可能产生0字符，用最低两位代表inbuf的长度, 00:3n, 01:3n+1,10:3n+2 
	// 
	srand( (unsigned)time(NULL) ); 
	inbuf[0] = randchar() & (~0x03);    // 最低两位为0时，leadlen % 3 == 0 

	int actlen = srclen + 1; 
	if (actlen % 3 == 1)        // 原长3n+1，补两个随机字符保证inbuf长度为3n 
	{ 
		inbuf[0] |= 0x01; 
		inbuf[actlen] = randchar(); 
		inbuf[actlen + 1] = randchar(); 
		actlen += 2; 
	} 
	else if (actlen % 3 == 2)    // 原长3n+2，补1个随机字符保证inbuf长度为3n 
	{ 
		inbuf[0] |= 0x02; 
		inbuf[actlen] = randchar(); 
		actlen++; 
	} 

	// 从inbuf转换出outbuf，outbuf由字母、数字、'_'、'.'组成，长度为4的倍数 
	ToBase64(inbuf, actlen, outbuf); 

	std::string strResult(outbuf); 

	delete[] buf; 

	return strResult; 
} 


std::string StringDecrypt(const char* srcstr) 
{ 
	int srclen = (NULL == srcstr) ? 0 : strlen(srcstr) / 4 * 4; 
	int len = srclen * 3 / 4; 

	if (0 == len) 
	{ 
		return ""; 
	} 

	char *chBuf = new char[len + 1]; 

	UnBase64(srcstr, srclen, chBuf); 

	if (1 == (chBuf[0] & 0x03)) 
		len -= 2; 
	else if (2 == (chBuf[0] & 0x03)) 
		len--; 
	chBuf[len] = 0; 

	for (int i = 1; i < len; i++) 
	{ 
		chBuf[i] = EncryptChar(chBuf[i]); 
	} 

	std::string strResult(chBuf + 1, len - 1); 

	delete[] chBuf; 

	return strResult; 
}
