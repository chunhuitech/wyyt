
// wyyt.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "Version.h"		
#define	DEFAULT_TCP_PORT_OLD	4662
#define	DEFAULT_UDP_PORT_OLD	(DEFAULT_TCP_PORT_OLD+10)
#include "Log.h"
// CwyytApp:
// 有关此类的实现，请参阅 wyyt.cpp
//
enum AppState{
	APP_STATE_RUNNING = 0,
	APP_STATE_SHUTTINGDOWN,
	APP_STATE_DONE
};
class CwyytDlg;

class CwyytApp : public CWinApp/*CWinAppEx*/
{
public:
	//CwyytApp();
	CwyytApp(LPCTSTR lpszAppName = NULL);

	CwyytDlg*			wyytdlg;

	volatile AppState		m_app_state; // defines application state for shutdown 
	
public:
	//HANDLE				m_hMutexOneInstance;
	//int					m_iDfltImageListColorFlags;
	CFont				m_fontHyperText;
	CFont				m_fontDefaultBold;
	CFont				m_fontSymbol;
	CFont				m_fontLog;
	CFont				m_fontChatEdit;
	CBrush				m_brushBackwardDiagonal;

	CString		m_strDefaultFontFaceName;
	const CString &GetDefaultFontFaceName();

	void		CreateAllFonts();
	void		UpdateLargeIconSize();
public:
	// because nearly all icons we are loading are 16x16, the default size is specified as 16 and not as 32 nor LR_DEFAULTSIZE
	HICON		LoadIcon(LPCTSTR lpszResourceName, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR) const;
	HICON		LoadIcon(UINT nIDResource) const;
	HBITMAP		LoadImage(LPCTSTR lpszResourceName, LPCTSTR pszResourceType) const;
	HBITMAP		LoadImage(UINT nIDResource, LPCTSTR pszResourceType) const;
	
	void		ApplySkin(LPCTSTR pszSkinProfile);
	bool		IsXPThemeActive() const;
	bool		IsVistaThemeActive() const;

	bool		GetLangHelpFilePath(CString& strResult);
	void		SetHelpFilePath(LPCTSTR pszHelpFilePath);
	void		ShowHelp(UINT uTopic, UINT uCmd = HELP_CONTEXT);
	bool		ShowWebHelp(UINT uTopic);
protected:
	CString		m_strLastClipboardContents;
	bool m_bAutoStart;

	CSize m_sizBigSystemIcon;
public:
	bool			DidWeAutoStart() { return m_bAutoStart; }

	LPCTSTR		GetProfileFile()		{ return m_pszProfileName; }
	CSize		GetBigSytemIconSize() { return m_sizBigSystemIcon; }
public:
	// clipboard (text)
	bool		CopyTextToClipboard(CString strText);
	CString		CopyTextFromClipboard();
	void		IgnoreClipboardLinks(CString strLinks) {m_strLastClipboardContents = strLinks;}
	void		SearchClipboard();

public:
	static const UINT	m_nVersionMjr;
	static const UINT	m_nVersionMin;
	static const UINT	m_nVersionUpd;
	static const UINT	m_nVersionBld;
	DWORD				m_dwProductVersionMS;
	DWORD				m_dwProductVersionLS;
	UINT				m_uCurVersionShort;
	UINT				m_uCurVersionCheck;
	ULONGLONG			m_ullComCtrlVer;
	int					m_iDfltImageListColorFlags;

	HANDLE				m_hMutexOneInstance;
protected:
	bool ProcessCommandline();
// 重写
	public:
	virtual BOOL InitInstance();



// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();


public:
	bool		LoadSkinColor(LPCTSTR pszKey, COLORREF& crColor) const;
	bool		LoadSkinColorAlt(LPCTSTR pszKey, LPCTSTR pszAlternateKey, COLORREF& crColor) const;

	void		UpdateDesktopColorDepth();
};

extern CwyytApp theApp;


//////////////////////////////////////////////////////////////////////////////
// CTempIconLoader

class CTempIconLoader
{
public:
	// because nearly all icons we are loading are 16x16, the default size is specified as 16 and not as 32 nor LR_DEFAULTSIZE
	CTempIconLoader(LPCTSTR pszResourceID, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR);
	CTempIconLoader(UINT uResourceID, int cx = 16, int cy = 16, UINT uFlags = LR_DEFAULTCOLOR);
	~CTempIconLoader();

	operator HICON() const{
		return this == NULL ? NULL : m_hIcon;
	}

protected:
	HICON m_hIcon;
};