#include "StdAfx.h"
#include <io.h>
#include "wyyt.h"
#include "Preferences.h"
#include "Opcodes.h"
#include "wyytdlg.h"
/** \file Preferences.cpp
******************************************************************************** 
<PRE> 
模块名 : < wyyt >
文件名 : <Preferences.cpp> 
相关文件 : < Preferences.h >
文件实现功能 : 参数配置
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





#include "XMessageBox.h"
#include "Ini2.h"
#include "MuleToolbarCtrl.h"
#include <crypto/osrng.h>

/**
 * ... 全局唯一参数配置变量 ...
 */
CPreferences thePrefs;


//!类外定义静态成员
WORD	CPreferences::m_wWinVer;
CString	CPreferences::m_strConfigDir;
CAtlArray<CString> CPreferences::m_ArrayAllDir;
CString CPreferences::m_astrDefaultDirs[100];
bool	CPreferences::m_abDefaultDirsCreated[100] = { 0};
int		CPreferences::m_nCurrentUserDirMode = -1;
CString	CPreferences::m_strVerInfoFile;
CString		CPreferences::m_nVerInfoNum = L"10101";
CString CPreferences::m_strNewDataVerInfo=L"  ...";
CString CPreferences::m_strNewProgVerInfo=L"  ...";

CString	CPreferences::m_strSysAttachPath;
CString	CPreferences::m_strPriAttachPath;

CString	CPreferences::m_strUserName;
CString	CPreferences::m_strUserPwd;

bool	CPreferences::m_bPriPwdCheckSupported;

WORD	CPreferences::m_wLanguageID;
bool	CPreferences::prefReadonly; // X: [ROP] - [ReadOnlyPreference]
EToolbarLabelType CPreferences::m_nToolbarLabels;
CString	CPreferences::m_sToolbarBitmap;
CString	CPreferences::m_sToolbarBitmapFolder;
CString	CPreferences::m_sToolbarSettings;
bool	CPreferences::m_bReBarToolbar;
CSize	CPreferences::m_sizToolbarIconSize;

CString	CPreferences::m_strSkinProfile;
CString	CPreferences::m_strSkinProfileDir;

WINDOWPLACEMENT CPreferences::EmuleWindowPlacement;
uchar	CPreferences::userhash[16];
Preferences_Ext_Struct* CPreferences::prefsExt;
uint8	CPreferences::smartidstate;

LOGFONT CPreferences::m_lfHyperText;
LOGFONT CPreferences::m_lfLogText;

UINT	CPreferences::splitterbarPositionShared;
bool	CPreferences::m_bShowSharedFilesDetails;


bool	CPreferences::mintotray;
BOOL	CPreferences::m_bIsRunningAeroGlass;

DWORD	CPreferences::m_adwStatsColors[13];

UINT	CPreferences::m_iToolDelayTime;
//Xman narrow font at transferwindow
bool CPreferences::m_bUseNarrowFont;
//Xman end
bool	CPreferences::m_bUseSystemFontForMainControls;
//log
int		CPreferences::iMaxLogBuff;
COLORREF CPreferences::m_crLogError = RGB(255, 0, 0);
COLORREF CPreferences::m_crLogWarning = RGB(128, 0, 128);
COLORREF CPreferences::m_crLogSuccess = RGB(0, 0, 255);
CString	CPreferences::m_strDateTimeFormat4Log;
bool	CPreferences::log2disk;
bool	CPreferences::debug2disk;
bool	CPreferences::m_bVerbose;
int		CPreferences::m_byLogLevel;

bool	CPreferences::confirmExit;

CPreferences::CPreferences(void)
{
}

CPreferences::~CPreferences(void)
{
		delete prefsExt;
}


WORD CPreferences::InitWinVersion()
{
	return (m_wWinVer = DetectWinVersion());	
}

void CPreferences::Init()
{
	srand((uint32)time(0)); // we need random numbers sometimes

	prefsExt = new Preferences_Ext_Struct;
	memset(prefsExt, 0, sizeof *prefsExt);

	
	//初始化相关目录名称
	for (size_t nDir = WYYT_CONFIGDIR; nDir < WYYT_ENDEXP; nDir++ )
	{
		m_ArrayAllDir.Add(GetDefaultDirectory((WDefaultDirectory)nDir, false));		
	}
	

	//验证单独测试config目录的方法
	if (!PathFileExists(GetMuleDirectory(WYYT_CONFIGDIR)) && !::CreateDirectory(GetMuleDirectory(WYYT_CONFIGDIR),0)) 
	{
		XMSGBOXPARAMS params;
		params.nTimeoutSeconds = 5;
		CString strError;
		strError.Format(GetResString(IDS_ERR_CREATE_DIR), GetResString(IDS_INIT_DIR_CONFIG), GetMuleDirectory(WYYT_CONFIGDIR), GetErrorMessage(GetLastError()));
		XMessageBox(NULL, strError, _T("wyyt"), MB_ICONERROR, &params);

		m_strConfigDir = GetDefaultDirectory(WYYT_CONFIGDIR, true); // will also try to create it if needed
		if (!PathFileExists(GetMuleDirectory(WYYT_CONFIGDIR)))
		{
			strError.Format(GetResString(IDS_ERR_CREATE_DIR), GetResString(IDS_INIT_DIR_CONFIG), GetMuleDirectory(WYYT_CONFIGDIR), GetErrorMessage(GetLastError()));
			XMessageBox(NULL, strError, _T("eMule"), MB_ICONERROR, &params);
		}
	}
	//循环判断所有目录并创建
	for (size_t nDir = WYYT_CONFIGDIR; nDir < WYYT_ENDEXP; nDir++ )
	{
		if ( nDir > 3 )
		{
			continue;
		}
		if (!PathFileExists(GetSelectDir(nDir)) && !::CreateDirectory(GetSelectDir(nDir),0)) 
		{
			XMSGBOXPARAMS params;
			params.nTimeoutSeconds = 5;
			CString strError;
			strError.Format(GetResString(IDS_ERR_CREATE_DIR), GetResString(IDS_INIT_DIRS), GetSelectDir(nDir), GetErrorMessage(GetLastError()));
			XMessageBox(NULL, strError, _T("wyyt"), MB_ICONERROR, &params);

			m_ArrayAllDir.SetAt(nDir, GetDefaultDirectory((WDefaultDirectory)nDir, true)); // will also try to create it if needed);
			if (!PathFileExists(GetSelectDir()))
			{
				strError.Format(GetResString(IDS_ERR_CREATE_DIR), GetResString(IDS_INIT_DIR_TEMP), GetSelectDir(), GetErrorMessage(GetLastError()));
				XMessageBox(NULL, strError, _T("wyyt"), MB_ICONERROR, &params);
			}
		}
	}
	m_strSysAttachPath = GetDefaultDirectory(WYYT_ATTACHMENTS_SYS_DIR, true); 
	m_strPriAttachPath = GetDefaultDirectory(WYYT_ATTACHMENTS_PRIVATE_DIR, true);
	
	LoadPreferences();
	
	// load preferences.dat or set standart values
	CString strFullPath;
	strFullPath = GetMuleDirectory(WYYT_CONFIGDIR) + L"preferences.dat";
	FILE* preffile = _tfsopen(strFullPath, L"rb", _SH_DENYWR);

	if (!preffile)
	{
		SetStandartValues();
	}
	else
	{
		if (fread(prefsExt,sizeof(Preferences_Ext_Struct),1,preffile) != 1 || ferror(preffile))
			SetStandartValues();
		else
		{ //Enig123 :: Fix
			md4cpy(userhash, prefsExt->userhash);
			EmuleWindowPlacement = prefsExt->EmuleWindowPlacement;
			//zz_fly :: check userhash after initialized :: Enig123 :: start
			userhash[5] = 0;
			userhash[14] = 0;
			if (isnulmd4(userhash)) //Xman Bugfix by ilmira
				CreateUserHash();
			else{
				userhash[5] = 14;
				userhash[14] = 111;
			}
			//zz_fly :: check userhash after initialized :: Enig123 :: end
		} //Enig123 :: Fix

		fclose(preffile);
		smartidstate = 0;
	}
	CString temp;
	LoadVerInfo(temp);
	
}
bool CPreferences::LoadVerInfo(CString& strAllVerInfo)
{
	//加载版本信息文件内容
	// 打开文件加载
	if(::GetFileAttributes(GetMuleDirectory(WYYT_CONFIGDIR) +L"\\wyytverinfo.txt")==-1)
	{
		return false;//文件不存在
	}
	CStdioFile f(GetMuleDirectory(WYYT_CONFIGDIR) +L"\\wyytverinfo.txt",CFile::typeText ); 
	CString strreceiver;          
	CString strDataUpDate;
	CString strProgUpDate;
	CString strMorInfo;           
	int nlinenum = 0;
	while(f.ReadString(strreceiver))            
	{           
		if ( 0 == nlinenum )
		{
			if ( strreceiver.Left(1) != L"V")
			{
				return false;
			}

			m_strNewDataVerInfo = strreceiver;

		}
		else if ( 1 == nlinenum )
		{
			strDataUpDate = strreceiver;
		}
		else if ( 2 == nlinenum )
		{
			m_strNewProgVerInfo = strreceiver;
		}
		else if ( 3 == nlinenum )
		{
			strProgUpDate = strreceiver;
		}
		else
		{
			strMorInfo += strreceiver+ L"\r\n";   
		}

		nlinenum++;
		              
	}                                           
	f.Close();       
	strAllVerInfo = L"最新数据版本："+ m_strNewDataVerInfo + L" 更新时间："+ strDataUpDate + L"\r\n";
	strAllVerInfo += L"最新程序版本："+ m_strNewProgVerInfo + L" 更新时间："+ strProgUpDate + L"\r\n";
	strAllVerInfo += strMorInfo;
	/*if (m_strNewDataVerInfo.IsEmpty()) 
	m_strNewDataVerInfo = L"";*/

	return true;
}

void CPreferences::CreateUserHash()
{
	CryptoPP::AutoSeededRandomPool rng;
	rng.GenerateBlock(userhash, 16);
	// mark as emule client. that will be need in later version
	userhash[5] = 14;
	userhash[14] = 111;
}
void CPreferences::SetStandartValues()
{
	CreateUserHash();

	WINDOWPLACEMENT defaultWPM;
	defaultWPM.length = sizeof(WINDOWPLACEMENT);
	defaultWPM.rcNormalPosition.left=10;defaultWPM.rcNormalPosition.top=10;
	defaultWPM.rcNormalPosition.right=900;defaultWPM.rcNormalPosition.bottom=600;
	defaultWPM.showCmd=0;
	EmuleWindowPlacement=defaultWPM;
//	versioncheckLastAutomatic=0;
	//Xman versions check
	//mversioncheckLastAutomatic=0;
	//Xman end

	//	Save();
}

bool CPreferences::Save()
{
	bool error = false;
	CString strFullPath = GetMuleDirectory(WYYT_CONFIGDIR) + L"preferences.dat";

	FILE* preffile = _tfsopen(strFullPath, L"wb", _SH_DENYWR);
	prefsExt->version = PREFFILE_VERSION;
	if (preffile){
		prefsExt->version=PREFFILE_VERSION;
		prefsExt->EmuleWindowPlacement=EmuleWindowPlacement;
		md4cpy(prefsExt->userhash, userhash);

		error = fwrite(prefsExt,sizeof(Preferences_Ext_Struct),1,preffile)!=1;
		if ( !CwyytDlg::IsRunning())
		{
			fflush(preffile); // flush file stream buffers to disk buffers
			(void)_commit(_fileno(preffile)); // commit disk buffers to disk
		}
		fclose(preffile);
	}
	else
		error = true;

	SavePreferences();
	return error;

}

// General behavior:
//
// WinVer < Vista
// Default: ApplicationDir if preference.ini exists there. If not: user specific dirs if preferences.ini exits there. If not: again ApplicationDir
// Default overwritten by Registry value (see below)
// Fallback: ApplicationDir
//
// WinVer >= Vista:
// Default: User specific Dir if preferences.ini exists there. If not: All users dir, if preferences.ini exists there. If not user specific dirs again
// Default overwritten by Registry value (see below)
// Fallback: ApplicationDir
CString CPreferences::GetDefaultDirectory(WDefaultDirectory eDirectory, bool bCreate)
{
	
	if (m_astrDefaultDirs[0].IsEmpty() ) //尚未创建任务目录
	{
		
		// Get out exectuable starting directory which was our default till Vista  获取可执行程序exe所在根目录　
		TCHAR tchBuffer[MAX_PATH];
		::GetModuleFileName(NULL, tchBuffer, _countof(tchBuffer));
		tchBuffer[_countof(tchBuffer) - 1] = _T('\0');
		LPTSTR pszFileName = _tcsrchr(tchBuffer, L'\\') + 1;
		*pszFileName = L'\0';
		m_astrDefaultDirs[WYYT_ROOTDIR] = tchBuffer;
		CString strSelectedConfigBaseDirectory = m_astrDefaultDirs[WYYT_ROOTDIR];
		CString strSelectedDataBaseDirectory =  m_astrDefaultDirs[WYYT_ROOTDIR];
		
		m_astrDefaultDirs[WYYT_CONFIGDIR] = strSelectedConfigBaseDirectory + CONFIGFOLDER;
		m_strConfigDir = m_astrDefaultDirs[WYYT_CONFIGDIR];
		m_astrDefaultDirs[WYYT_TEMPDIR] = strSelectedDataBaseDirectory + _T("temp");
		m_astrDefaultDirs[WYYT_ATTACHMENTS_SYS_DIR] = strSelectedDataBaseDirectory + _T("attachs_sys");
		m_astrDefaultDirs[WYYT_ATTACHMENTS_GROUPS_DIR] = strSelectedDataBaseDirectory + _T("attachs_group");
		m_astrDefaultDirs[WYYT_ATTACHMENTS_PRIVATE_DIR] = strSelectedDataBaseDirectory + _T("attachs_private");
		m_astrDefaultDirs[WYYT_DBSDIR] = strSelectedDataBaseDirectory + _T("dbs");
		m_astrDefaultDirs[WYYT_LOGDIR] = strSelectedConfigBaseDirectory + _T("logs\\");
		m_astrDefaultDirs[WYYT_LANGDIR] = strSelectedConfigBaseDirectory + _T("lang\\");
		m_astrDefaultDirs[WYYT_SKINDIR] = strSelectedConfigBaseDirectory + _T("skins");
		m_astrDefaultDirs[WYYT_EXPANSIONDIR] = strSelectedConfigBaseDirectory + _T("expansiondir\\");
		
		m_astrDefaultDirs[WYYT_TOOLBARDIR] = m_astrDefaultDirs[WYYT_EXPANSIONDIR] + _T("skins");
	}
	if (bCreate && !m_abDefaultDirsCreated[eDirectory])//分别创建对应的目录
	{
		switch (eDirectory){ // create the underlying directory first - be sure to adjust this if changing default directories
			case WYYT_CONFIGDIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_CONFIGDIR], NULL);
				break;
			case WYYT_TEMPDIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_TEMPDIR], NULL);
				break;
			case WYYT_ATTACHMENTS_SYS_DIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_ATTACHMENTS_SYS_DIR], NULL);
				break;
			case WYYT_ATTACHMENTS_GROUPS_DIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_ATTACHMENTS_GROUPS_DIR], NULL);
				break;
			case WYYT_ATTACHMENTS_PRIVATE_DIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_ATTACHMENTS_PRIVATE_DIR], NULL);
				break;
			case WYYT_DBSDIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_DBSDIR], NULL);
				break;
			case WYYT_LOGDIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_LOGDIR], NULL);
				break;
			case WYYT_LANGDIR:
				::CreateDirectory(m_astrDefaultDirs[WYYT_LANGDIR], NULL);
				break;
				
			default:
				::CreateDirectory(m_astrDefaultDirs[WYYT_EXPANSIONDIR], NULL);
				break;
		}
		::CreateDirectory(m_astrDefaultDirs[eDirectory], NULL);
		m_abDefaultDirsCreated[eDirectory] = true;
	}
	return m_astrDefaultDirs[eDirectory];
}

CString	CPreferences::GetMuleDirectory(WDefaultDirectory eDirectory, bool bCreate){
	switch (eDirectory)
	{
		case WYYT_CONFIGDIR:
			{
				if(!m_strConfigDir.IsEmpty())
					return m_strConfigDir;
				else
				{
					return GetDefaultDirectory(eDirectory, bCreate);
				}
			}
			
		case WYYT_TEMPDIR:
		case WYYT_ATTACHMENTS_SYS_DIR:
		case WYYT_ATTACHMENTS_GROUPS_DIR:
		case WYYT_ATTACHMENTS_PRIVATE_DIR:
		case WYYT_DBSDIR:
		case WYYT_LOGDIR:
		case WYYT_LANGDIR:
		case WYYT_ROOTDIR:
		case WYYT_EXPANSIONDIR:
			return GetSelectDir(eDirectory);
		/*case WYYT_SKINDIR:
			return m_strSkinProfileDir;
		case WYYT_TOOLBARDIR:
			return m_sToolbarBitmapFolder;*/
		default:
			return GetDefaultDirectory(eDirectory, bCreate);
	}
}


void CPreferences::SetMuleDirectory(WDefaultDirectory eDirectory, CString strNewDir){
	switch (eDirectory){
	//case WYYT_INCOMINGDIR:
	//	m_strIncomingDir = strNewDir;
	//	break;
	case WYYT_SKINDIR:
		m_strSkinProfileDir = strNewDir;
		break;
	case WYYT_TOOLBARDIR:
		m_sToolbarBitmapFolder = strNewDir;
		break;
	default:
		ASSERT( false );
		break;
	}
}

void CPreferences::LoadPreferences()
{
	CIni ini(GetConfigFile(), L"wyyt");

	// Toolbar
	m_sToolbarSettings = ini.GetString(L"ToolbarSetting", strDefaultToolbar);
	m_sToolbarBitmap = ini.GetString(L"ToolbarBitmap");
	m_sToolbarBitmapFolder = ini.GetString(L"ToolbarBitmapFolder");
	if (m_sToolbarBitmapFolder.IsEmpty()) // We want GetDefaultDirectory to also create the folder, so we have to know if we use the default or not
		m_sToolbarBitmapFolder = GetDefaultDirectory(WYYT_TOOLBARDIR, false);
	m_nToolbarLabels = (EToolbarLabelType)ini.GetInt(L"ToolbarLabels", CMuleToolbarCtrl::GetDefaultLabelType());
	m_bReBarToolbar = ini.GetBool(L"ReBarToolbar", 1);
	m_sizToolbarIconSize.cx = m_sizToolbarIconSize.cy = ini.GetInt(L"ToolbarIconSize", 32);
	//m_iStraightWindowStyles=ini.GetInt(L"StraightWindowStyles",0);
	//m_bUseSystemFontForMainControls=ini.GetBool(L"UseSystemFontForMainControls",0);
	//m_bRTLWindowsLayout = ini.GetBool(L"RTLWindowsLayout");
	m_strSkinProfile = ini.GetString(L"SkinProfile");
	m_strSkinProfileDir = ini.GetString(L"SkinProfileDir");
	if (m_strSkinProfileDir.IsEmpty()) // We want GetDefaultDirectory to also create the folder, so we have to know if we use the default or not
		m_strSkinProfileDir = GetDefaultDirectory(WYYT_SKINDIR, false);

	LPBYTE pData = NULL;
	size_t uSize = sizeof m_lfHyperText;
	if (ini.GetBinary(L"HyperTextFont", &pData, &uSize) && uSize == sizeof m_lfHyperText)
		memcpy(&m_lfHyperText, pData, sizeof m_lfHyperText);
	else
		memset(&m_lfHyperText, 0, sizeof m_lfHyperText);
	delete[] pData;


	splitterbarPositionShared=ini.GetInt(L"SplitterbarPositionSys",179);
	m_bShowSharedFilesDetails = ini.GetBool(L"ShowSysResDetails", true);

	//托盘相关设置
	// since the minimize to tray button is not working under Aero (at least not at this point),
	// we enable map the minimize to tray on the minimize button by default if Aero is running
	if (IsRunningAeroGlassTheme())
		mintotray=ini.GetBool(L"MinToTray_Aero", true);
	else
		mintotray=ini.GetBool(L"MinToTray", false);

	CString buffer2;
	for (size_t i = 0; i < _countof(m_adwStatsColors); i++) 
	{
		buffer2.Format(L"StatColor%i", i);
		m_adwStatsColors[i] = 0;
		if (_stscanf_s(ini.GetString(buffer2), L"%i", &m_adwStatsColors[i]) != 1)
			ResetStatsColor(i);
	}

	m_wLanguageID=ini.GetWORD(L"Language",0);
	SetLanguage();

	m_iToolDelayTime=ini.GetInt(L"ToolTipDelay",1);

	//
	prefReadonly = ini.GetBool(L"ReadOnly", false); // X: [ROP] - [ReadOnlyPreference]

	//Xman narrow font at transferwindow
	m_bUseNarrowFont=ini.GetBool(L"UseNarrowFont",false);
	//Xman end
	m_bUseSystemFontForMainControls=ini.GetBool(L"UseSystemFontForMainControls",0);

	//log
	iMaxLogBuff = ini.GetInt(L"MaxLogBuff",64) * 1024;
	m_crLogError = ini.GetColRef(L"LogErrorColor", m_crLogError);
	m_crLogWarning = ini.GetColRef(L"LogWarningColor", m_crLogWarning);
	m_crLogSuccess = ini.GetColRef(L"LogSuccessColor", m_crLogSuccess);
	m_strDateTimeFormat4Log = ini.GetString(L"DateTimeFormat4Log", L"%c");
	log2disk = ini.GetBool(L"SaveLogToDisk",false);
	debug2disk=ini.GetBool(L"SaveDebugToDisk",false);

	m_bVerbose=ini.GetBool(L"Verbose",false);
	m_byLogLevel = ini.GetInt(L"DebugLogLevel", DLP_VERYLOW);

	//
	m_strVerInfoFile = ini.GetString(L"SysVerInfoFile");
	if (m_strVerInfoFile.IsEmpty()) 
		//m_strVerInfoFile = L"http://localhost/wyytverinfo.txt";
		m_strVerInfoFile = L"http://verinfo.wyyt.net/wyytverinfo.txt";
	m_nVerInfoNum = ini.GetString(L"SysVerNum");
	if (m_nVerInfoNum.IsEmpty()) 
		m_nVerInfoNum = L"10101";
	m_strSysAttachPath = ini.GetString(L"SysAttachDir");
	if (m_strSysAttachPath.IsEmpty()) // We want GetDefaultDirectory to also create the folder, so we have to know if we use the default or not
		m_strSysAttachPath = GetDefaultDirectory(WYYT_ATTACHMENTS_SYS_DIR, true);
	MakeFoldername(m_strSysAttachPath);

	m_strPriAttachPath = ini.GetString(L"PriAttachDir");
	if (m_strPriAttachPath.IsEmpty()) // We want GetDefaultDirectory to also create the folder, so we have to know if we use the default or not
		m_strPriAttachPath = GetDefaultDirectory(WYYT_ATTACHMENTS_PRIVATE_DIR, true);
	MakeFoldername(m_strPriAttachPath);


	m_bPriPwdCheckSupported = ini.GetBool(L"PriPwdCheckSupported", true);
	m_strUserName = ini.GetString(L"PriUserName");
	m_strUserPwd = ini.GetString(L"PriUserPwd");

	confirmExit=ini.GetBool(L"ConfirmExit",true);
}
void CPreferences::ResetStatsColor(size_t index)
{
	switch(index)
	{
	case  0: m_adwStatsColors[ 0]=RGB(  0,  0, 64);break;
	case  1: m_adwStatsColors[ 1]=RGB(192,192,255);break;
	case  2: m_adwStatsColors[ 2]=RGB(128,255,128);break;
	case  3: m_adwStatsColors[ 3]=RGB(  0,210,  0);break;
	case  4: m_adwStatsColors[ 4]=RGB(  0,128,  0);break;
	case  5: m_adwStatsColors[ 5]=RGB(255,128,128);break;
	case  6: m_adwStatsColors[ 6]=RGB(200,  0,  0);break;
	case  7: m_adwStatsColors[ 7]=RGB(140,  0,  0);break;
	case  8: m_adwStatsColors[ 8]=RGB(150,150,255);break;
	case  9: m_adwStatsColors[ 9]=RGB(192,  0,192);break;
	case 10: m_adwStatsColors[10]=RGB(255,255,128);break;
	case 11: m_adwStatsColors[11]=RGB(  0,  0,  0);/* bHasCustomTaskIconColor = false; */break;
	case 12: m_adwStatsColors[12]=RGB(255,255,255);break;
	}
}

void CPreferences::SavePreferences()
{
	if(prefReadonly) // X: [ROP] - [ReadOnlyPreference]
		return;

	CString buffer;

	CIni ini(GetConfigFile(), L"wyyt");

	// Toolbar
	ini.WriteString(L"ToolbarSetting", m_sToolbarSettings);
	ini.WriteString(L"ToolbarBitmap", m_sToolbarBitmap );
	ini.WriteString(L"ToolbarBitmapFolder", m_sToolbarBitmapFolder);
	ini.WriteInt(L"ToolbarLabels", m_nToolbarLabels);
	ini.WriteInt(L"ToolbarIconSize", m_sizToolbarIconSize.cx);
	ini.WriteString(L"SkinProfile", m_strSkinProfile);
	ini.WriteString(L"SkinProfileDir", m_strSkinProfileDir);

	ini.WriteBinary(L"HyperTextFont", (LPBYTE)&m_lfHyperText, sizeof m_lfHyperText);
	ini.WriteBinary(L"LogTextFont", (LPBYTE)&m_lfLogText, sizeof m_lfLogText);


	ini.WriteInt(L"SplitterbarPositionSys",splitterbarPositionShared);
	ini.WriteBool(L"ShowSysResDetails", m_bShowSharedFilesDetails);
	//托盘相关设置
	ini.WriteBool((IsRunningAeroGlassTheme()?L"MinToTray_Aero":L"MinToTray"),mintotray);

	CString buffer2;
	for (size_t i=0; i < _countof(m_adwStatsColors); i++) 
	{
		buffer.Format(L"0x%06x",GetStatsColor(i));
		buffer2.Format(L"StatColor%i",i);
		ini.WriteString(buffer2,buffer);
	}
	//语言设置
	ini.WriteWORD(L"Language",m_wLanguageID);

	// tooltip delay time in seconds
	ini.WriteInt(L"ToolTipDelay",m_iToolDelayTime);

	//Xman narrow font at transferwindow
	ini.WriteBool(L"UseNarrowFont", m_bUseNarrowFont);
	//Xman end

	//log
	ini.WriteBool(L"SaveLogToDisk",log2disk);
	ini.WriteBool(L"SaveDebugToDisk",debug2disk);
	ini.WriteBool(L"Verbose", m_bVerbose);
	ini.WriteInt(L"DebugLogLevel", m_byLogLevel);

	ini.WriteString(L"SysVerInfoFile", m_strVerInfoFile);
	ini.WriteString(L"SysVerNum", m_nVerInfoNum);

	ini.WriteString(L"SysAttachDir", m_strSysAttachPath);
	ini.WriteString(L"PriAttachDir", m_strPriAttachPath);

	ini.WriteBool(L"PriPwdCheckSupported", m_bPriPwdCheckSupported);
	ini.WriteString(L"PriUserName", m_strUserName);
	ini.WriteString(L"PriUserPwd", m_strUserPwd);

	ini.WriteBool(L"ConfirmExit",confirmExit);
}


LPCTSTR CPreferences::GetConfigFile()
{
	return theApp.m_pszProfileName;
}

bool CPreferences::GetUseReBarToolbar()
{
	return GetReBarToolbar() && theApp.m_ullComCtrlVer >= MAKEDLLVERULL(5,8,0,0);
}

bool CPreferences::IsRunningAeroGlassTheme(){
	// This is important for all functions which need to draw in the NC-Area (glass style)
	// Aero by default does not allow this, any drawing will not be visible. This can be turned off,
	// but Vista will not deliver the Glass style then as background when calling the default draw function
	// in other words, its draw all or nothing yourself - eMule chooses currently nothing
	static bool bAeroAlreadyDetected = false;
	if (!bAeroAlreadyDetected)
	{
		bAeroAlreadyDetected = true;
		m_bIsRunningAeroGlass = FALSE;
		if (GetWindowsVersion() >= _WINVER_VISTA_){
			HMODULE hDWMAPI = LoadLibrary(_T("dwmapi.dll"));
			if (hDWMAPI)
			{
				HRESULT (WINAPI *pfnDwmIsCompositionEnabled)(BOOL*);
				(FARPROC&)pfnDwmIsCompositionEnabled = GetProcAddress(hDWMAPI, "DwmIsCompositionEnabled");
				if (pfnDwmIsCompositionEnabled != NULL)
					pfnDwmIsCompositionEnabled(&m_bIsRunningAeroGlass);
				FreeLibrary(hDWMAPI);
			}
		}
	}
	return m_bIsRunningAeroGlass == TRUE ? true : false;
}