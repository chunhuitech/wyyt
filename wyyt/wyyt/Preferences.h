#pragma once
#include "OtherFunctions.h"
/** \file Preferences.h
******************************************************************************** 
<PRE> 
相关配置参数定义
</PRE> 
*******************************************************************************/
//默认工具栏上设置的工具按钮显示情况
const CString strDefaultToolbar = _T("0099010203040506070899091011");
/*! 相关目录设置 enum */
enum WDefaultDirectory
{
	WYYT_ROOTDIR = 99, /*!< 应用程序根目录*/
	WYYT_CONFIGDIR = 0,   /*!< 默认配置文件preferences.ini路径 config */
	WYYT_ATTACHMENTS_SYS_DIR, /*!< 系统类附件的文件存放路径 attachs_sys*/
	WYYT_ATTACHMENTS_PRIVATE_DIR,/*!< 个人类附件的文件存放路径 attachs_private*/
	WYYT_DBSDIR, /*!< 数据文件存放路径 dbs*/
	WYYT_ATTACHMENTS_GROUPS_DIR,/*!< 群组类附件的文件存放路径 attachs_group*/
	WYYT_TEMPDIR ,    /*!< 临时文件路径 temp */ 
	
	WYYT_LOGDIR,/*!< 日志文件存放路径 logs*/
	WYYT_LANGDIR, /*!< 语言文件存放路径 lang*/
	WYYT_SKINDIR,	/*!< skins文件存放路径 skins*/
	WYYT_EXPANSIONDIR, /*!< 扩展目录*/
	WYYT_TOOLBARDIR, /*!< ToolbarBitmapFolder 工具栏位图存放路径*/
	WYYT_ENDEXP /*!< 作为最后一项 不实际创建目录*/

	//WYYT_ADDLANGDIR, //lang directories with languages installed by the wyyt (parent: WYYT_EXPANSIONDIR)
	//WYYT_INSTLANGDIR, // directories with languages installed by the user or installer (parent: WYYT_EXECUTEABLEDIR)
	//WYYT_WEBSERVERDIR, /*!< webserver文件存放路径 webserver*/
	//
	//WYYT_DATABASEDIR,/*!< 选择的数据文件存放基目录*/
	//WYYT_CONFIGBASEDIR, // the parent directory of the config folder 
	//WYYT_EXECUTEABLEDIR, /*!< 可执行程序(exe)安装目录　 assumed to be not writeable (!)*/ 
	
	//WYYT_EXPANSIONDIR  // this is a base directory accessable for all users for things wyyt installs
};
/*! \enum WDefaultDirectory
 * 相关目录设置 enum type.
 */

enum EToolbarLabelType;

#pragma pack(1)
struct Preferences_Ext_Struct{
	uint8	version;
	uchar	userhash[16];
	WINDOWPLACEMENT EmuleWindowPlacement;
};
#pragma pack()

/*! ____参数配置类.____ */
class CPreferences
{
public:
	CPreferences(void);
	~CPreferences(void);

	friend class CPPgDirSet;
	friend class CPPgGeneral;
	friend class CPPgSecurity;
	static	CString	m_strVerInfoFile;
	static CString m_nVerInfoNum;
	static CString m_strNewDataVerInfo;
	static CString m_strNewProgVerInfo;

	static	CString	m_strSysAttachPath;
	static	CString	m_strPriAttachPath;

	static	CString	m_strUserName;
	static	CString	m_strUserPwd;

	static	void	Init();
	static	void	Uninit();
	static	bool	Save();
	static bool		LoadVerInfo(CString& strAllVerInfo);

	static	LPCTSTR GetSelectDir(size_t id = 0)				{return (LPCTSTR)m_ArrayAllDir.GetAt((id < m_ArrayAllDir.GetCount()) ? id : 0);}
	static	size_t	GetAllDirCount()					{return m_ArrayAllDir.GetCount();}

	//
	static	UINT	splitterbarPositionShared;
	static	UINT	GetSplitterbarPositionShared()		{return splitterbarPositionShared;}
	static	void	SetSplitterbarPositionShared(UINT pos) {splitterbarPositionShared=pos;}
	static	bool	m_bShowSharedFilesDetails;
	static	bool	GetShowSharedFilesDetails()			{return m_bShowSharedFilesDetails;}
	static	void	SetShowSharedFilesDetails(bool bIn) {m_bShowSharedFilesDetails = bIn;}
public:

	//安全
	static bool		m_bPriPwdCheckSupported;
	static	bool		IsPriPwdCheckSupported()		{return m_bPriPwdCheckSupported;}

	/** @name 查看系统版本相关成员_Group********************************************************************
	*   系统版本相关分组成员. 
	*/
	//@{


	/**
	*  初始化检测Windows系统版本
	*@param void
	*@return WORD
	*   如果返回0 则系统版本太低 must be running on Windows 2000 or higer
	*/
	static	WORD	InitWinVersion();
	//! 获取系统版本.
	static	WORD	GetWindowsVersion(){ return m_wWinVer;}


	//@}


	static	CString	GetMuleDirectory(WDefaultDirectory eDirectory, bool bCreate = true);
	static	void	SetMuleDirectory(WDefaultDirectory eDirectory, CString strNewDir);

protected:
	static CString	GetDefaultDirectory(WDefaultDirectory eDirectory, bool bCreate = true);

public:
	static	CString	m_strConfigDir;
	static	CAtlArray<CString>	m_ArrayAllDir;

	static	WORD	m_wLanguageID;

	static	bool	prefReadonly; // X: [ROP] - [ReadOnlyPreference]
	// toolbar
	static	EToolbarLabelType m_nToolbarLabels;
	static	CString	m_sToolbarBitmap;
	static	CString	m_sToolbarBitmapFolder;
	//0099010203040506070899091011  两位表示一个位图所在的索引 99表示分隔符
	static	CString	m_sToolbarSettings;
	static	bool	m_bReBarToolbar;
	static	CSize	m_sizToolbarIconSize;

	//theApp.ApplySkin(thePrefs.GetSkinProfile());  
	//WM_SYSCOLORCHANGE 根据这个消息设置相应的位图，CwyytApp::LoadImage
	static	CString	m_strSkinProfile;
	static	CString	m_strSkinProfileDir;

	static	UINT	m_iToolDelayTime;	// tooltip delay time in seconds
	static	UINT	GetToolTipDelay(void)				{return m_iToolDelayTime;}

public:
	static	void	SetLanguage();
	static	void	SetLanguageID(WORD lid);
	static	WORD	GetLanguageID();

	static	void	GetLanguages(CAtlArray<WORD>& aLanguageIDs);
	static	bool	IsLanguageSupported(LANGID lidSelected, bool bUpdateBefore);
	static	CString GetLangNameByID(LANGID lidSelected);

#ifdef _DEBUG
	static	void	InitThreadLocale();
	static	void	SetRtlLocale(LCID lcid);
#endif


	//Toolbar
	static	const CString& GetToolbarSettings()					{return m_sToolbarSettings;}
	static	void	SetToolbarSettings(const CString& in)		{m_sToolbarSettings = in;}
	static	const CString& GetToolbarBitmapSettings()			{return m_sToolbarBitmap;}
	static	void	SetToolbarBitmapSettings(const CString& path){m_sToolbarBitmap = path;}
	static	EToolbarLabelType GetToolbarLabelSettings()			{return m_nToolbarLabels;}
	static	void	SetToolbarLabelSettings(EToolbarLabelType eLabelType) {m_nToolbarLabels = eLabelType;}
	static	bool	GetReBarToolbar()							{return m_bReBarToolbar;}
	static	bool	GetUseReBarToolbar();
	static	CSize	GetToolbarIconSize()				{return m_sizToolbarIconSize;}
	static	void	SetToolbarIconSize(CSize siz)		{m_sizToolbarIconSize = siz;}

	static	const CString& GetSkinProfile()				{return m_strSkinProfile;}
	static	void	SetSkinProfile(LPCTSTR pszProfile)	{m_strSkinProfile = pszProfile;}

	static	WINDOWPLACEMENT EmuleWindowPlacement;
	static	WINDOWPLACEMENT GetEmuleWindowPlacement()	{return EmuleWindowPlacement;}
	static	void	SetWindowLayout(WINDOWPLACEMENT in) {EmuleWindowPlacement=in;}

	static	LOGFONT m_lfHyperText;
	static	LOGFONT m_lfLogText;

	static	LPLOGFONT GetHyperTextLogFont()				{return &m_lfHyperText;}
	static	void	SetHyperTextFont(LPLOGFONT plf)		{m_lfHyperText = *plf;}
	static	LPLOGFONT GetLogFont()						{return &m_lfLogText;}
	static	void	SetLogFont(LPLOGFONT plf)			{m_lfLogText = *plf;}

	//托盘
	static	bool	mintotray;
	static	bool	GetMinToTray()						{return mintotray;}
	static	bool*	GetMinTrayPTR()						{return &mintotray;}

	static BOOL		m_bIsRunningAeroGlass;
	static	bool	IsRunningAeroGlassTheme();

	static	DWORD	m_adwStatsColors[13];
	static	DWORD	GetStatsColor(size_t index)			{return m_adwStatsColors[index];}
	static	void	SetStatsColor(size_t index, DWORD value){m_adwStatsColors[index] = value;}
	static	size_t	GetNumStatsColors()					{return ARRSIZE(m_adwStatsColors);}
	static	void	ResetStatsColor(size_t index);

public:
	static	WORD m_wWinVer; /*!< 记录系统版本 */
	static	CString	m_astrDefaultDirs[100];/*!< 系统默认目录 */
	static	bool	m_abDefaultDirsCreated[100]; ;/*!< 系统默认目录是否创建 */
	/**
	 * 当前用户目录类型　即历史安装版本情况
	 * -1 默认 即：从未安装过，第一次安装
	 * 2  ExecuteableDir  exe程序目录  默认设置路径
	 * 0  Multiuser  多用户共享路径　即：系统LocalAppData路径下
	 * 1  Publicuser  公共用户　　即：系统ProgrammData路径下
	 */
	static	int		m_nCurrentUserDirMode; 

	//! an integer value.
	int itest;
	static	LPCTSTR GetConfigFile();

	static	uchar	userhash[16];
	static void	CreateUserHash();
	static	uchar*	GetUserHash()						{return userhash;}

	static	Preferences_Ext_Struct* prefsExt;

	static	uint8	smartidstate;
	static	uint8	GetSmartIdState()					{return smartidstate;}
	static	void	SetSmartIdState(uint8 in_smartidstate) {smartidstate = in_smartidstate;}

	static void	SetStandartValues();

	static void LoadPreferences();
	static void SavePreferences();

	//Xman narrow font at transferwindow
	static	bool m_bUseNarrowFont;
	static	bool UseNarrowFont() {return m_bUseNarrowFont;}
	static	void SetNarrowFont(bool in) {m_bUseNarrowFont=in;}
	//Xman end
	static	bool	m_bUseSystemFontForMainControls;
	static	bool	GetUseSystemFontForMainControls()	{return m_bUseSystemFontForMainControls;}


	//LOG
	static	int		iMaxLogBuff;
	static	int		GetMaxLogBuff()						{return iMaxLogBuff;}
	static	COLORREF m_crLogError;
	static	COLORREF m_crLogWarning;
	static	COLORREF m_crLogSuccess;
	static	COLORREF GetLogErrorColor()					{return m_crLogError;}
	static	COLORREF GetLogWarningColor()				{return m_crLogWarning;}
	static	COLORREF GetLogSuccessColor()				{return m_crLogSuccess;}
	static	CString	m_strDateTimeFormat4Log;
	static	const CString& GetDateTimeFormat4Log()		{return m_strDateTimeFormat4Log;}
	static	bool	m_bVerbose;
	static	bool	log2disk;
	static	bool	debug2disk;
	static	bool	GetLog2Disk()						{return log2disk;}
	static	bool	GetDebug2Disk()						{return m_bVerbose && debug2disk;}

	static	bool	GetVerbose()						{return m_bVerbose;}
	static  int		m_byLogLevel;
	static	int		GetVerboseLogPriority()				{return	m_byLogLevel;}

	//
	static	bool	confirmExit;
	static	bool	IsConfirmExitEnabled()				{return confirmExit;}
	static	void	SetConfirmExit(bool bVal)			{confirmExit = bVal;} 
};

extern CPreferences thePrefs;
extern bool g_bLowColorDesktop;
/*! \class CPreferences Preferences.h "CPreferences.h"
 *  \brief 参数配置类.
 *
 * 通过很多静态成员函数提供相应的功能
 *
 */
