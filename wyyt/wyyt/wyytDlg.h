/** \file wyytDlg.h
 * 主对话框头文件
 * A more elaborated file description.
 */

// wyytDlg.h : 头文件
//

#pragma once
#include "TrayDialog.h"
#include "MeterIcon.h"
#include "TitleMenu.h"
//#include "map_inc.h" //用到boost库


//class CPreferencesDlg;
class CMuleStatusBarCtrl;
class CMuleToolbarCtrl;
class CSysResManageWnd;
class CRecordInfoSheetDlg;
class CGroupResManageWnd;
class CPriResManageWnd;
class CPreferencesDlg;
class CWyytSystrayDlg;

//#define	EMULE_HOTMENU_ACCEL		'x'
#define	EMULSKIN_BASEEXT		_T("wyytSkin")
void DialogCreateIndirect(CDialog *pWnd, UINT uID);
// CwyytDlg 对话框
class CwyytDlg : public CTrayDialog
{
	friend class CMuleToolbarCtrl;//否则AddAnchor不可访问
// 构造
public:
	CwyytDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CwyytDlg();
	
	static bool IsRunning();
	//窗口托盘支持CTrayDialog虚函数实现	
	uint_ptr		m_uLastSysTrayIconCookie;
	virtual void TrayMinimizeToTrayChange();
	virtual void RestoreWindow();
	void ShowTransferRate(bool forceAll=false);
	void UpdateTrayIcon();
	void ShowTransferStateIcon();
	CMeterIcon		m_TrayIcon;

	HICON			m_icoSysTrayConnected;		// do not use those icons for anything else than the traybar!!!
	HICON			m_icoSysTrayDisconnected;	// do not use those icons for anything else than the traybar!!!
	HICON			m_icoSysTrayLowID;	// do not use those icons for anything else than the traybar!!!
	HICON			m_icoSysTrayCurrent;
	void SetAllIcons();
	//
	void SetStatusBarPartsSize();
// 对话框数据
	enum { IDD = IDD_WYYT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//工具栏相关
	CMuleToolbarCtrl* toolbar;
	CReBarCtrl		m_ctlMainTopReBar;
	//状态栏相关
	CMuleStatusBarCtrl* statusbar;
	

	////聊天弹出消息??????
	//CTaskbarNotifier* m_wndTaskbarNotifier;
public:

	CSysResManageWnd*	sysresmanagewnd; 
	CGroupResManageWnd*	groupresmanagewnd;//	CWnd*	transferwnd;
	CPriResManageWnd*	priresmanagewnd;//	CWnd*		serverwnd;
	CPreferencesDlg* preferenceswnd;

	CRecordInfoSheetDlg* m_pRecInfPPSheetDlg;

	CWnd* sharedfileswnd;
	CWnd*		searchwnd;
	CWnd*		chatwnd;
	CWnd*  statisticswnd;
	CWnd*		ircwnd;
	

	CWnd*			activewnd;
	void ShowToolPopup(bool toolsonly = false, bool speedgraph = false); 

	INT_PTR ShowPreferences(UINT uStartPageID = (UINT)-1);
	bool IsPreferencesDlgOpen() const;

	bool IsRecPPDlgOpen() const;
	INT_PTR ShowRecPPDlg(int nOpt,UINT uStartPageID = (UINT)-1);

protected:
	CWyytSystrayDlg* m_pSystrayDlg;
public:

#ifdef REPLACE_ATLMAP
	unordered_map<UINT, LPCTSTR> m_mapTbarCmdToIcon;
#else
	CAtlMap<UINT, LPCTSTR> m_mapTbarCmdToIcon;
#endif
	void CreateToolbarCmdIconMap();
	LPCTSTR GetIconFromCmdId(UINT uId);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnTrayRButtonUp(CPoint pt);
	DECLARE_MESSAGE_MAP()
public:
	int MapWindowToToolbarButton(CWnd* pWnd) const;
	CWnd* MapToolbarButtonToWindow(int iButtonID) const;
	int GetNextWindowToolbarButton(int iButtonID, int iDirection = 1) const;
	void SetActiveDialog(CWnd* dlg);
	bool IsWindowToolbarButton(int iButtonID) const;
	void Localize();

	// Logging
	void AddLogText(UINT uFlags, LPCTSTR pszText);
	void UploadMachineInformation();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL bEnding);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnChevronPushed(UINT id, NMHDR *pnm, LRESULT *pResult);//工具栏中添加出按钮以外的控件
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Terminal Services
	afx_msg LRESULT OnConsoleThreadEvent(WPARAM wParam, LPARAM lParam);
	protected:
		bool CanClose();
};


enum EWyytAppMsgs
{
	//thread messages
	TM_FINISHEDHASHING = WM_APP + 10,
	TM_HASHFAILED,
	//Xman
	// BEGIN SLUGFILLER: SafeHash - new handling
	TM_PARTHASHEDOK,
	TM_PARTHASHEDOKNOAICH,
	TM_PARTHASHEDCORRUPT,
	TM_PARTHASHEDCORRUPTNOAICH,
	TM_PARTHASHEDOKAICHRECOVER,
	TM_PARTHASHEDCORRUPTAICHRECOVER,
	// END SLUGFILLER: SafeHash
	TM_READBLOCKFROMFILEDONE, // SiRoB: ReadBlockFromFileThread
	TM_FLUSHDONE, // SiRoB: Flush Thread
	TM_DOTIMER, //Xman process timer code via messages (Xanatos)
	TM_SAVEKNOWNDONE, // Threaded Known Files Saving [Stulle] - Stulle
	TM_FRAMEGRABFINISHED,
	TM_FILEALLOCEXC,
	TM_FILECOMPLETED,
	TM_FILEOPPROGRESS,
	TM_CONSOLETHREADEVENT
};