
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif
#include "wyyt_site_config.h"
#include "resource.h"

// MSDN: Using the Windows Headers
// ===========================================================
//Windows Vista			_WIN32_WINNT>=0x0600	WINVER>=0x0600
//Windows Server 2003	_WIN32_WINNT>=0x0502    WINVER>=0x0502
//Windows XP			_WIN32_WINNT>=0x0501	WINVER>=0x0501
//Windows 2000			_WIN32_WINNT>=0x0500    WINVER>=0x0500
//Windows NT 4.0		_WIN32_WINNT>=0x0400	WINVER>=0x0400
//Windows Me			_WIN32_WINDOWS=0x0500	WINVER>=0x0500
//Windows 98			_WIN32_WINDOWS>=0x0410	WINVER>=0x0410
//Windows 95			_WIN32_WINDOWS>=0x0400	WINVER>=0x0400
//
//IE 7.0				_WIN32_IE>=0x0700 
//IE 6.0 SP2			_WIN32_IE>=0x0603 
//IE 6.0 SP1			_WIN32_IE>=0x0601 
//IE 6.0				_WIN32_IE>=0x0600 
//IE 5.5				_WIN32_IE>=0x0550 
//IE 5.01				_WIN32_IE>=0x0501 
//IE 5.0, 5.0a, 5.0b	_WIN32_IE>=0x0500 
//IE 4.01				_WIN32_IE>=0x0401 
//IE 4.0				_WIN32_IE>=0x0400 
//IE 3.0, 3.01, 3.02	_WIN32_IE>=0x0300 

#if _MSC_VER >= 1500   //vs2008?
#ifndef WINVER
#define WINVER 0x0501		// at least xp ... for vs2008. not good but.... compil
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NN or later.                   
#define _WIN32_WINNT WINVER	// Change this to the appropriate value.
#endif

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows NN or later.
#define _WIN32_WINDOWS WINVER   // Change this to the appropriate value.
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0560		// 0x0560 == Internet Explorer 5.6 -> Comctl32.dll v5.8 (same as MFC internally used)
#endif

#endif

#include "targetver.h"

#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <..\src\mfc\afximpl.h>
#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#pragma comment(lib,"netapi32.lib")
#pragma comment(lib,"htmlcxx.lib")

#include <atlcoll.h> //֧�� CAtlArray<CString>
#include "types.h"
#include <afxdlgs.h>
