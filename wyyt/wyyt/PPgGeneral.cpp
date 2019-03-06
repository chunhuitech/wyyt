// PPgGeneral.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "PPgGeneral.h"
//#include "afxdialogex.h"
#include "Preferences.h"
#include "langids.h"
#include "wyytdlg.h"
#include <afxinet.h>
#include "Preferences.h"
#include "jsoncpp-0.10.7/include/json/json.h"
#include "WininetHttp.h"
#include "GlobalFunction.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD dwHttpRequestFlags_DL =
	INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;

DWORD   dwAccessType_DL = PRE_CONFIG_INTERNET_ACCESS;

const TCHAR szHeaders_DL[] =
	_T("Accept: text/*\r\nUser-Agent: MFC_Tear_Sample\r\n");

// CPPgGeneral 对话框

IMPLEMENT_DYNAMIC(CPPgGeneral, CPropertyPage)

CPPgGeneral::CPPgGeneral()
	: CPropertyPage(CPPgGeneral::IDD)
{

}

CPPgGeneral::~CPPgGeneral()
{
}

void CPPgGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGS, m_language);
}


BEGIN_MESSAGE_MAP(CPPgGeneral, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_LANGS, &CPPgGeneral::OnCbnSelchangeLangs)
	ON_BN_CLICKED(IDC_EXIT, &CPPgGeneral::OnSettingsChange)
	ON_BN_CLICKED(IDC_BUTTON_VERINFO, &CPPgGeneral::OnBnClickedButtonVerinfo)
	ON_BN_CLICKED(IDC_BUTTON_UPSYSDATA, &CPPgGeneral::OnBnClickedButtonUpsysdata)
END_MESSAGE_MAP()


// CPPgGeneral 消息处理程序
void CPPgGeneral::Localize(void)
{
	if(m_hWnd)
	{
		SetWindowText(GetResString(IDS_PW_GENERAL));
		SetDlgItemText(IDC_LANG_FRM,GetResString(IDS_PW_LANG));
		SetDlgItemText(IDC_EXIT,GetResString(IDS_PW_PROMPT));
		//SetDlgItemText(IDC_NICK_FRM,GetResString(IDS_QL_USERNAME));
		//SetDlgItemText(IDC_MISC_FRM,GetResString(IDS_PW_MISC));
		//SetDlgItemText(IDC_EXIT,GetResString(IDS_PW_PROMPT));
		//SetDlgItemText(IDC_SPLASHON,GetResString(IDS_PW_SPLASH));
		//SetDlgItemText(IDC_BRINGTOFOREGROUND,GetResString(IDS_PW_FRONT));
		//SetDlgItemText(IDC_ONLINESIG,GetResString(IDS_PREF_ONLINESIG));	
		//SetDlgItemText(IDC_STARTMIN,GetResString(IDS_PREF_STARTMIN));	
		//SetDlgItemText(IDC_WEBSVEDIT,GetResString(IDS_WEBSVEDIT));
		//SetDlgItemText(IDC_ED2KFIX,GetResString(IDS_ED2KLINKFIX));
		//SetDlgItemText(IDC_CHECK4UPDATE,GetResString(IDS_CHECK4UPDATE));
		//SetDlgItemText(IDC_STARTUP,GetResString(IDS_STARTUP));
		//SetDlgItemText(IDC_STARTWIN,GetResString(IDS_STARTWITHWINDOWS));
		//SetDlgItemText(IDC_MINIMULE,GetResString(IDS_ENABLEMINIMULE));
		//SetDlgItemText(IDC_PREVENTSTANDBY,GetResString(IDS_PREVENTSTANDBY));
		////X version check
		//SetDlgItemText(IDC_CHECK4UPDATEMOD,GetResString(IDS_CHECK4UPDATEMOD));
	}
}

BOOL CPPgGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitWindowStyles(this);

	CAtlArray<WORD> aLanguageIDs;
	thePrefs.GetLanguages(aLanguageIDs);
	for (size_t i = 0; i < aLanguageIDs.GetCount(); i++)
	{
		TCHAR szLang[128];
		int ret=GetLocaleInfo(aLanguageIDs[i], LOCALE_SLANGUAGE, szLang, ARRSIZE(szLang));

		if (ret==0)
			switch(aLanguageIDs[i]) 
		{
			case LANGID_UG_CN:
				_tcscpy(szLang,_T("Uyghur") );
				break;
			case LANGID_GL_ES:
				_tcscpy(szLang,_T("Galician") );
				break;
			case LANGID_FR_BR:
				_tcscpy(szLang,_T("Breton (Brezhoneg)") );
				break;
			case LANGID_MT_MT:
				_tcscpy(szLang,_T("Maltese") );
				break;
			case LANGID_ES_AS:
				_tcscpy(szLang,_T("Asturian") );
				break;
			case LANGID_VA_ES:
				_tcscpy(szLang,_T("Valencian") );
				break;
			case LANGID_VA_ES_RACV:
				_tcscpy(szLang, _T("Valencian (RACV)"));
				break;
			default:
				ASSERT(0);
				_tcscpy(szLang,_T("?(unknown language)?") );
				break;
		}

		m_language.SetItemData(m_language.AddString(szLang), aLanguageIDs[i]);
	}

	LoadSettings();
	Localize();
	m_bModified = false; 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPPgGeneral::LoadSettings(void)
{
	for(int i = 0; i < m_language.GetCount(); i++)
		if(m_language.GetItemData(i) == thePrefs.GetLanguageID())
			m_language.SetCurSel(i);

	CheckDlgButton(IDC_EXIT,thePrefs.confirmExit);

}




void CPPgGeneral::OnCbnSelchangeLangs()
{
	WORD byNewLang = (WORD)m_language.GetItemData(m_language.GetCurSel());
	if (thePrefs.GetLanguageID() != byNewLang)
	{
		if	(!thePrefs.IsLanguageSupported(byNewLang, false))
		{
			
			for(int i = 0; i < m_language.GetCount(); i++)
				if(m_language.GetItemData(i) == thePrefs.GetLanguageID())
					m_language.SetCurSel(i);
		}
		else
			OnSettingsChange();
	}
}

BOOL CPPgGeneral::OnApply()
{
	if(m_bModified)
	{
		if (m_language.GetCurSel() != CB_ERR)
		{
			WORD wNewLang = (WORD)m_language.GetItemData(m_language.GetCurSel());
			if (thePrefs.GetLanguageID() != wNewLang)
			{
				thePrefs.SetLanguageID(wNewLang);
				thePrefs.SetLanguage();

				theApp.wyytdlg->Localize();
			}
		}

		thePrefs.confirmExit = IsDlgButtonChecked(IDC_EXIT)!=0;
		LoadSettings();

		SetModified(FALSE);
		m_bModified = false;
	}
	return CPropertyPage::OnApply();
}

void CPPgGeneral::OnBnClickedButtonVerinfo()
{

	CString strServerName;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	if (!AfxParseURL(thePrefs.m_strVerInfoFile, dwServiceType, strServerName, strObject, nPort) ||
		dwServiceType != INTERNET_SERVICE_HTTP)
	{
		//CException* pEx =  new CException();
		//throw pEx;
	}

	//std::wstring verInfo = thePrefs.m_strVerInfoFile.GetBufferSetLength(thePrefs.m_strVerInfoFile.GetLength());
	std::string strUrl = CGlobalFunction::WideCharToMultiByte(thePrefs.m_strVerInfoFile.GetBuffer(0));// "http://www.chunhuitech.cn/api/admin/api/prodinfo/versioncheck";
	CWininetHttp inetHttp;
	std::string res = inetHttp.RequestJsonInfo(strUrl, Hr_Post, "Content-Type: application/json", "{\"name\":\"wyyt\",\"technologyPlatform\":\"vs\"}");
	Json::Reader reader;
	Json::Value root;
	std::string downAddress;
	CString cNewVersion;
	int num = 0;
	if (reader.parse(res, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		downAddress = root["data"]["downAddress"].asString();
		num = root["data"]["versionNum"].asInt();
		std::string newVersion = root["data"]["version"].asString();
		wstring wstrNewVersion = CGlobalFunction::MultiByteToWideChar(newVersion.c_str());
		cNewVersion = wstrNewVersion.c_str();
	}
	int curNum = _ttoi(thePrefs.m_nVerInfoNum);
	if(curNum < num){
		SetDlgItemText(IDC_EDIT_VERSION,L"发现新版本:" + cNewVersion);
		ShellExecute(NULL, NULL, L"http://www.firemail.wang:8088/forum.php?mod=viewthread&tid=9676", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_VERSION,L"已是最新版本");
	}
	/*
	CInternetSession session(L"wyytversion", 1, PRE_CONFIG_INTERNET_ACCESS);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		if (!AfxParseURL(thePrefs.m_strVerInfoFile, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			//CException* pEx =  new CException();
			//throw pEx;
		}
		pServer = session.GetHttpConnection(strServerName, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags_DL);
		pFile->AddRequestHeaders(szHeaders_DL);
		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if (dwRet != HTTP_STATUS_OK)
		{
			//ThrowTearException_DL(2);
		}
		if (dwRet == HTTP_STATUS_DENIED)
		{
			DWORD dwPrompt;
			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			{
				//ThrowTearException_DL(1);
			}

			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		CString strLength;
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strLength);
		int   len;   
		//sscanf(strLength, L"%d", &len);   
		_stscanf_s(strLength,L"%d",&len);
		//len = _atoi(strLength.GetBuffer(0));
		CFileException pError;

		//	| CFile::typeBinary,
		CFile Readyfile;
		if ( !Readyfile.Open( thePrefs.GetMuleDirectory(WYYT_CONFIGDIR) + L"\\wyytverinfo.txt",
			CFile::modeCreate
			| CFile::modeReadWrite
			| CFile::shareDenyWrite,
			//| CFile::typeText
			&pError ) )
		{
			return ;
		}
		Readyfile.SeekToBegin();

		TCHAR sz[1461]={0};
		int nRead = 0, nTotal = 0;
		while ( (nRead = pFile->Read(sz, 1460)) > 0)
		{
			nTotal += nRead;
			Readyfile.Write(sz, nRead);

		}
		Readyfile.Close();
	
		pFile->Close();
		pServer->Close();

	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		pEx->Delete();
		Sleep(1000);
		
	}
	catch (CException* pEx)
	{
		
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	CString strVerInfos;
	if ( thePrefs.LoadVerInfo(strVerInfos) )
	{
		SetDlgItemText(IDC_EDIT_VERSION,strVerInfos);
	}
	else
	{
		ShellExecute(NULL, NULL, L"http://www.wyyt.net", NULL, thePrefs.GetMuleDirectory(WYYT_EXPANSIONDIR), SW_SHOWDEFAULT);
	}
	*/

}


void CPPgGeneral::OnBnClickedButtonUpsysdata()
{
	// TODO: 在此添加控件通知处理程序代码
}
