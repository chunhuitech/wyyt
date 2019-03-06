#ifndef __VERSION_H__
#define __VERSION_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _T
#define _T(x)	x
#endif

#define _chSTR(x)		_T(#x)
#define chSTR(x)		_chSTR(x)

// *) Specify the version of emule only here with the following defines.
// *) When changing any of those version nr. defines you also have to rebuild the language DLLs.
//
// General format:
//	<major>.<minor>.<update>.<build>
//
// Fields:
//	<major>		major number (e.g. 0)
//	<minor>		minor number (e.g. 30)
//	<update>	update number (e.g. 0='a'  1='b'  2='c'  3='d'  4='e'  5='f' ...)
//	<build>		build number; currently not used
//
// Currently used:
//  <major>.<minor>.<update> is used for the displayed version (GUI) and the version check number
//	<major>.<minor>			 is used for the protocol(!) version
//
#define VERSION_MJR		0
#define VERSION_MIN		50
#define VERSION_UPDATE	0
#define VERSION_BUILD	4
#define VERSION_UPDATE_CHAR	a

// NOTE: This version string is also used by the language DLLs!
#define	SZ_VERSION_NAME		chSTR(VERSION_MJR) _T(".") chSTR(VERSION_MIN) _T(".") chSTR(VERSION_UPDATE)
#define	_SZ_VERSION_NUM		chSTR(VERSION_MJR) _T(".") chSTR(VERSION_MIN) chSTR(VERSION_UPDATE_CHAR)
#define	SZ_VERSION_NUM_WITH_BUILD	_SZ_VERSION_NUM _T(".") chSTR(VERSION_BUILD)
#ifdef _DEBUG
#define	SZ_VERSION_NUM		SZ_VERSION_NUM_WITH_BUILD _T(" DEBUG")
#else
#define	SZ_VERSION_NUM		_SZ_VERSION_NUM
#endif 
//#define SZ_FULL_VERSION	_T("wyyt v") SZ_VERSION_NUM
#define SZ_FULL_VERSION	_T("wyyt v1.1.1.1") 
#endif /* !__VERSION_H__ */



//主版本号 . 子版本号 [. 修正版本号 [. 编译版本号 ]] 
#define MAJOR_VERSION_NUMBER	1
#define MINOR_VERSION_NUMBER	0
#define REVISION_NUMBER		0		//-----新问题的修改改变这个版本号
#define BUILD_NUMBER		1		//	-----同一问题的修改改变这个版本号
#define WYYT_VERSION	"1.1.1.1"
