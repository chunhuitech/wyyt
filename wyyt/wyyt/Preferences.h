#pragma once
#include "OtherFunctions.h"
/** \file Preferences.h
******************************************************************************** 
<PRE> 
������ò�������
</PRE> 
*******************************************************************************/
//Ĭ�Ϲ����������õĹ��߰�ť��ʾ���
const CString strDefaultToolbar = _T("0099010203040506070899091011");
/*! ���Ŀ¼���� enum */
enum WDefaultDirectory
{
	WYYT_ROOTDIR = 99, /*!< Ӧ�ó����Ŀ¼*/
	WYYT_CONFIGDIR = 0,   /*!< Ĭ�������ļ�preferences.ini·�� config */
	WYYT_ATTACHMENTS_SYS_DIR, /*!< ϵͳ�฽�����ļ����·�� attachs_sys*/
	WYYT_ATTACHMENTS_PRIVATE_DIR,/*!< �����฽�����ļ����·�� attachs_private*/
	WYYT_DBSDIR, /*!< �����ļ����·�� dbs*/
	WYYT_ATTACHMENTS_GROUPS_DIR,/*!< Ⱥ���฽�����ļ����·�� attachs_group*/
	WYYT_TEMPDIR ,    /*!< ��ʱ�ļ�·�� temp */ 
	
	WYYT_LOGDIR,/*!< ��־�ļ����·�� logs*/
	WYYT_LANGDIR, /*!< �����ļ����·�� lang*/
	WYYT_SKINDIR,	/*!< skins�ļ����·�� skins*/
	WYYT_EXPANSIONDIR, /*!< ��չĿ¼*/
	WYYT_TOOLBARDIR, /*!< ToolbarBitmapFolder ������λͼ���·��*/
	WYYT_ENDEXP /*!< ��Ϊ���һ�� ��ʵ�ʴ���Ŀ¼*/

	//WYYT_ADDLANGDIR, //lang directories with languages installed by the wyyt (parent: WYYT_EXPANSIONDIR)
	//WYYT_INSTLANGDIR, // directories with languages installed by the user or installer (parent: WYYT_EXECUTEABLEDIR)
	//WYYT_WEBSERVERDIR, /*!< webserver�ļ����·�� webserver*/
	//
	//WYYT_DATABASEDIR,/*!< ѡ��������ļ���Ż�Ŀ¼*/
	//WYYT_CONFIGBASEDIR, // the parent directory of the config folder 
	//WYYT_EXECUTEABLEDIR, /*!< ��ִ�г���(exe)��װĿ¼�� assumed to be not writeable (!)*/ 
	
	//WYYT_EXPANSIONDIR  // this is a base directory accessable for all users for things wyyt installs
};
/*! \enum WDefaultDirectory
 * ���Ŀ¼���� enum type.
 */

enum EToolbarLabelType;

#pragma pack(1)
struct Preferences_Ext_Struct{
	uint8	version;
	uchar	userhash[16];
	WINDOWPLACEMENT EmuleWindowPlacement;
};
#pragma pack()

/*! ____����������.____ */
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

	//��ȫ
	static bool		m_bPriPwdCheckSupported;
	static	bool		IsPriPwdCheckSupported()		{return m_bPriPwdCheckSupported;}

	/** @name �鿴ϵͳ�汾��س�Ա_Group********************************************************************
	*   ϵͳ�汾��ط����Ա. 
	*/
	//@{


	/**
	*  ��ʼ�����Windowsϵͳ�汾
	*@param void
	*@return WORD
	*   �������0 ��ϵͳ�汾̫�� must be running on Windows 2000 or higer
	*/
	static	WORD	InitWinVersion();
	//! ��ȡϵͳ�汾.
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
	//0099010203040506070899091011  ��λ��ʾһ��λͼ���ڵ����� 99��ʾ�ָ���
	static	CString	m_sToolbarSettings;
	static	bool	m_bReBarToolbar;
	static	CSize	m_sizToolbarIconSize;

	//theApp.ApplySkin(thePrefs.GetSkinProfile());  
	//WM_SYSCOLORCHANGE ���������Ϣ������Ӧ��λͼ��CwyytApp::LoadImage
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

	//����
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
	static	WORD m_wWinVer; /*!< ��¼ϵͳ�汾 */
	static	CString	m_astrDefaultDirs[100];/*!< ϵͳĬ��Ŀ¼ */
	static	bool	m_abDefaultDirsCreated[100]; ;/*!< ϵͳĬ��Ŀ¼�Ƿ񴴽� */
	/**
	 * ��ǰ�û�Ŀ¼���͡�����ʷ��װ�汾���
	 * -1 Ĭ�� ������δ��װ������һ�ΰ�װ
	 * 2  ExecuteableDir  exe����Ŀ¼  Ĭ������·��
	 * 0  Multiuser  ���û�����·��������ϵͳLocalAppData·����
	 * 1  Publicuser  �����û���������ϵͳProgrammData·����
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
 *  \brief ����������.
 *
 * ͨ���ܶྲ̬��Ա�����ṩ��Ӧ�Ĺ���
 *
 */
