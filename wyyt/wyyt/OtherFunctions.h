#pragma once
using namespace std;
#include <string>
enum EFileStates{
	E_FS_UNDOWNED = 0,//未下载
	E_FS_DOWNED,//已下载
	E_FS_UNKNOWN
};

///////////////////////////////////////////////////////////////////////////////
// Win32 specifics
//

#define _WINVER_2K_		0x0500	// 5.0
#define _WINVER_XP_		0x0501	// 5.1
#define _WINVER_2003_	0x0502	// 5.2
#define _WINVER_VISTA_	0x0600	// 6.0
#define _WINVER_7_		0x0601	// 6.1
#define	_WINVER_S2008_	0x0601	// 6.1




//!检测系统版本
WORD		DetectWinVersion();
int			GetAppImageListColorFlag();
int			GetDesktopColorDepth();

///////////////////////////////////////////////////////////////////////////////
// URL conversion
//
CString RemoveAmbersand(const CString& rstr);
///////////////////////////////////////////////////////////////////////////////
// GUI helpers
//
CString ShellGetFolderPath(int iCSIDL);
void InitWindowStyles(CWnd* pWnd);

void GetPopupMenuPos(CListCtrl& lv, CPoint& point);
void GetPopupMenuPos(CTreeCtrl& tv, CPoint& point);

int  FontPointSizeToLogUnits(int nPointSize);
bool CreatePointFont(CFont &rFont, int nPointSize, LPCTSTR lpszFaceName);
bool CreatePointFontIndirect(CFont &rFont, const LOGFONT *lpLogFont);
bool CheckFileOpen(LPCTSTR pszFilePath, LPCTSTR pszFileTitle = NULL);
void ShellOpenFile(CString name);
void ShellOpenFile(CString name, LPCTSTR pszVerb);
///////////////////////////////////////////////////////////////////////////////
// Low level str
//
TCHAR *stristr(const TCHAR *str1, const TCHAR *str2);
CString GetNextString(const CString& rstr, LPCTSTR pszTokens, int& riStart);
CString GetNextString(const CString& rstr, TCHAR chToken, int& riStart);

///////////////////////////////////////////////////////////////////////////////
// File/Path string helpers
//
void MakeFoldername(CString &path);
int CompareLocaleString(LPCTSTR psz1, LPCTSTR psz2);
int CompareLocaleStringNoCase(LPCTSTR psz1, LPCTSTR psz2);
int __cdecl CompareCStringPtrLocaleString(const void* p1, const void* p2);
int __cdecl CompareCStringPtrLocaleStringNoCase(const void* p1, const void* p2);
void Sort(CAtlArray<CString>& astr, int (__cdecl *pfnCompare)(const void*, const void*) = CompareCStringPtrLocaleStringNoCase);
bool		_tmakepathlimit(TCHAR *path, const TCHAR *drive, const TCHAR *dir, const TCHAR *fname, const TCHAR *ext);
bool SelectDir(HWND hWnd, LPTSTR pszPath, LPCTSTR pszTitle = NULL, LPCTSTR pszDlgTitle = NULL);

///////////////////////////////////////////////////////////////////////////////
// Resource strings
//
#ifdef USE_STRING_IDS  //字符串标志ID 
#define	RESSTRIDTYPE		LPCTSTR
#define	IDS2RESIDTYPE(id)	#id
#define GetResString(id)	_GetResString(#id)
CString _GetResString(RESSTRIDTYPE StringID);
#else//USE_STRING_IDS
#define	RESSTRIDTYPE		UINT
#define	IDS2RESIDTYPE(id)	id
CString GetResString(RESSTRIDTYPE StringID);
#define _GetResString(id)	GetResString(id)
#endif//!USE_STRING_IDS

#ifdef _DEBUG
void InitThreadLocale();
#else
#define InitThreadLocale()          ((void)0)
#endif


///////////////////////////////////////////////////////////////////////////////
// String conversion
//
//Xman Xtreme Mod
//default is 99, this means, we use the old method (from 0.30)
CString CastItoXBytes(uint16 count, bool isK = false, bool isPerSec = false, uint32 decimal = 99);
CString CastItoXBytes(uint32 count, bool isK = false, bool isPerSec = false, uint32 decimal = 99);
CString CastItoXBytes(uint64 count, bool isK = false, bool isPerSec = false, uint32 decimal = 99);
CString CastItoXBytes(float count, bool isK = false, bool isPerSec = false, uint32 decimal = 99);
CString CastItoXBytes(double count, bool isK = false, bool isPerSec = false, uint32 decimal = 99);
CString GetFileState(int nStateFlage);
///////////////////////////////////////////////////////////////////////////////
// Error strings, Debugging, Logging
//
int GetSystemErrorString(DWORD dwError, CString &rstrError);
int GetModuleErrorString(DWORD dwError, CString &rstrError, LPCTSTR pszModule);
int GetErrorMessage(DWORD dwError, CString &rstrErrorMsg, DWORD dwFlags = 0);
CString GetErrorMessage(DWORD dwError, DWORD dwFlags = 0);



///////////////////////////////////////////////////////////////////////////////
// MD4 helpers
//

__inline BYTE toHex(const BYTE &x){
	return x > 9 ? x + 55: x + 48;
}

//Xman 
// Maella -Code Improvement-
// md4cmp -- replacement for memcmp(hash1,hash2,16)
// Like 'memcmp' this function returns 0, if hash1==hash2, and !0, if hash1!=hash2.
// NOTE: Do *NOT* use that function for determining if hash1<hash2 or hash1>hash2.
__inline int md4cmp(const void* const hash1, const void* const hash2) {
	return !(((uint64*)hash1)[0] == ((uint64*)hash2)[0] &&
		((uint64*)hash1)[1] == ((uint64*)hash2)[1]);
}

__inline bool isnulmd4(const void* hash) {
	return  (((uint64*)hash)[0] == 0 &&
		((uint64*)hash)[1] == 0);
}
// md4clr -- replacement for memset(hash,0,16)
__inline void md4clr(const void* hash) {
	((uint64*)hash)[1] = ((uint64*)hash)[0] = 0;
}

// md4cpy -- replacement for memcpy(dst,src,16)
__inline void md4cpy(void* dst, const void* src) {
	((uint64*)dst)[0] = ((uint64*)src)[0];
	((uint64*)dst)[1] = ((uint64*)src)[1];
}

///////////////////////////////////////////////////////////////////////////////
// Compare helpers
//
#ifdef _WIN64
#define CompareUnsigned_PTR CompareUnsigned64
#define GetFormatedUInt_PTR GetFormatedUInt64
#else
#define CompareUnsigned_PTR CompareUnsigned
#define GetFormatedUInt_PTR GetFormatedUInt
#endif

__inline int CompareUnsigned(uint32 uSize1, uint32 uSize2)
{
	if (uSize1 < uSize2)
		return -1;
	if (uSize1 > uSize2)
		return 1;
	return 0;
}

__inline int CompareUnsignedUndefinedAtBottom(uint32 uSize1, uint32 uSize2, bool bSortAscending)
{
	if (uSize1 == 0 && uSize2 == 0)
		return 0;
	if (uSize1 == 0)
		return bSortAscending ? 1 : -1;
	if (uSize2 == 0)
		return bSortAscending ? -1 : 1;
	return CompareUnsigned(uSize1, uSize2);
}

__inline int CompareUnsigned64(uint64 uSize1, uint64 uSize2)
{
	if (uSize1 < uSize2)
		return -1;
	if (uSize1 > uSize2)
		return 1;
	return 0;
}

__inline int CompareFloat(float uSize1, float uSize2)
{
	if (uSize1 < uSize2)
		return -1;
	if (uSize1 > uSize2)
		return 1;
	return 0;
}

__inline int CompareOptLocaleStringNoCase(LPCTSTR psz1, LPCTSTR psz2)
{
	if (psz1 && psz2)
		return CompareLocaleStringNoCase(psz1, psz2);
	if (psz1)
		return -1;
	if (psz2)
		return 1;
	return 0;
}

__inline int CompareOptLocaleStringNoCaseUndefinedAtBottom(const CString &str1, const CString &str2, bool bSortAscending)
{
	if (str1.IsEmpty() && str2.IsEmpty())
		return 0;
	if (str1.IsEmpty())
		return bSortAscending ? 1 : -1;
	if (str2.IsEmpty())
		return bSortAscending ? -1 : 1;
	return CompareOptLocaleStringNoCase(str1, str2);
}



std::string StringEncrypt(const char* srcstr, int srclen);
std::string StringDecrypt(const char* srcstr);