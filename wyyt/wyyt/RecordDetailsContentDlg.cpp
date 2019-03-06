// RecordDetailsContentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecordDetailsContentDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "StringConversion.h"
// CRecordDetailsContentDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDetailsContentDlg, CDialog)

CRecordDetailsContentDlg::CRecordDetailsContentDlg(CWnd* pParent /*=NULL*/,EModuleType emt/* = EMT_SYS*/)
	: CResizableDialog(CRecordDetailsContentDlg::IDD, pParent),m_emt(emt)
{
	m_pRI = NULL;
}

CRecordDetailsContentDlg::~CRecordDetailsContentDlg()
{
}

void CRecordDetailsContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_SELCLASSINFO, m_RECSelClaInfo);
	DDX_Control(pDX, IDC_RICHEDIT2_CONTENT, m_REContent);
}


BEGIN_MESSAGE_MAP(CRecordDetailsContentDlg, CResizableDialog)
	ON_EN_CHANGE(IDC_EDIT_CONTENT, &CRecordDetailsContentDlg::OnEnChangeEditContent)
END_MESSAGE_MAP()


// CRecordDetailsContentDlg 消息处理程序
void CRecordDetailsContentDlg::Localize()
{
	//GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(_T("5"));
	//( GetDlgItem( IDC_STATIC_TITLE ) )->SetWindowText(GetResString(IDS_PROP_INFO_TITLE));
	SetDlgItemText(IDC_STATIC_TITLE,GetResString(IDS_PROP_INFO_TITLE));
	SetDlgItemText(IDC_STATIC_KEY,GetResString(IDS_PROP_INFO_KEY));
	SetDlgItemText(IDC_STATIC_CONTENT,GetResString(IDS_PROP_INFO_CONTENT));
	SetDlgItemText(IDC_STATIC_REF,GetResString(IDS_PROP_INFO_REF));
	SetDlgItemText(IDC_STATIC_CLASSES,GetResString(IDS_PROP_INFO_CLASSES));
	
}


BOOL CRecordDetailsContentDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	InitWindowStyles(this);
	// TODO:  在此添加额外的初始化
	AddAnchor(IDC_RICHEDIT2_SELCLASSINFO, TOP_LEFT,TOP_RIGHT);
	AddAnchor(IDC_RICHEDIT2_CONTENT/*IDC_EDIT_CONTENT*/, TOP_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC_REF, BOTTOM_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_EDIT_REF, BOTTOM_LEFT,BOTTOM_RIGHT);
	
	Localize();



	m_RECSelClaInfo.SetTargetDevice(NULL, 1); //m_nWordWrap == WrapNone
	m_RECSelClaInfo.SetTargetDevice(NULL, 0);//m_nWordWrap == WrapToWindow

	BliandData();
	ShowSelectedClasses(m_pRI->nAllClassID,m_pRI->MainClassID);


	//设置字体
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize=sizeof(CHARFORMAT2);
	//cf.dwMask |= CFM_FACE|CFM_SIZE|CFM_BOLD;
	//cf.dwEffects = CFE_BOLD;
	//cf.yHeight=222;
	//strcpy_s(cf.szFaceName, "Courier New"); 
	//LOGFONT lf = {0};
	//theApp.m_fontSymbol.GetLogFont(&lf);	
	//_tcsncpy(cf.szFaceName, lf.lfFaceName, _countof(cf.szFaceName) - 1);
	//cf.szFaceName[_countof(cf.szFaceName) - 1] = _T('\0');
	

	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	//cf.dwEffects = CFE_BOLD; // 粗体（或其他CFE_ITALIC，CFE_STRIKEOUT ，CFE_UNDERLINE）
	//cf.crTextColor = RGB(0, 0, 255); //文字颜色--------蓝
	_tcsncpy(cf.szFaceName ,_T("Fixedsys"), _countof(cf.szFaceName) );//字体
	cf.yHeight = 200;//18*18;//文字高度
	m_REContent.SetDefaultCharFormat(cf); 
	m_REContent.SetBackgroundColor(FALSE,RGB(204, 232, 207));   //设背景色可以 
	

	//m_REContent.SetWordCharFormat(cf); //为将要输入的内容设定字体

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRecordDetailsContentDlg::BliandData()
{
	CString csInfos;
	csInfos = m_pRI->Titlel.c_str();
	SetDlgItemText(IDC_EDIT_TITLE,csInfos);
	csInfos = m_pRI->KeyValues.c_str();
	SetDlgItemText(IDC_EDIT_KEY,csInfos);
	csInfos = m_pRI->Content.c_str();
	SetDlgItemText(IDC_RICHEDIT2_CONTENT/*IDC_EDIT_CONTENT*/,csInfos);
	csInfos = m_pRI->Reference.c_str();
	SetDlgItemText(IDC_EDIT_REF,csInfos);
}

void CRecordDetailsContentDlg::OnEnChangeEditContent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CResizableDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CRecordDetailsContentDlg::InitCurRecordInfo(CRecordItem* pRI)
{
	m_pRI = pRI;
}

void CRecordDetailsContentDlg::GetRecordInfo()
{
	CString csInfos;
	GetDlgItemText(IDC_EDIT_TITLE,csInfos);
	m_pRI->Titlel = csInfos.GetBuffer();
	csInfos.ReleaseBuffer();
	GetDlgItemText(IDC_EDIT_KEY,csInfos);
	m_pRI->KeyValues = csInfos.GetBuffer();
	csInfos.ReleaseBuffer();
	//CString csInfos_NR;
	GetDlgItemText(IDC_RICHEDIT2_CONTENT/*IDC_EDIT_CONTENT*/,csInfos);
	m_pRI->Content = csInfos.GetBuffer();
	csInfos.ReleaseBuffer();
	//string strNr;
	//ConvertCString2string(csInfo_NR,strNr);
	//int n = csInfo_NR.GetLength();
	//m_pRI->Content.reserve(1024);
	//m_pRI->Content = s2ws(strNr);
	//m_pRI->Content = L"主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机主分类：我有一套-->IVR大千世界大千世界-->计算机";
	
	GetDlgItemText(IDC_EDIT_REF,csInfos);
	m_pRI->Reference = csInfos.GetBuffer();
	csInfos.ReleaseBuffer();
}


void CRecordDetailsContentDlg::ShowSelectedClasses(vector<int64>& vecSelClass,int64 nMainClassID)
{
	vector<int64> vecTrueSelClass;
	for (vector<int64>::iterator itt = vecSelClass.begin(); itt != vecSelClass.end(); itt++ )
	{
		if ( ERROR_CLASSID != (*itt) )
		{
			vecTrueSelClass.push_back(*itt);
		}
	}

	m_RECSelClaInfo.SetWindowText(L"");
	m_RECSelClaInfo.SetSel(-1, -1);     //将光标移动到最后

	/*vector<int64> vecSelClass;
	int64 nMainClassID = ERROR_CLASSID;*/
	//m_ClassSelector.GetSelectedClasses(&vecSelClass,nMainClassID);

	CString csTemp;
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	m_RECSelClaInfo.GetSelectionCharFormat(cf);
	//设置字体
	//strcpy(cf.szFaceName ,_T("隶书"));//设置字体
	cf.dwMask |= CFM_FACE;
	LOGFONT lf = {0};
	theApp.m_fontSymbol.GetLogFont(&lf);	
	_tcsncpy(cf.szFaceName, lf.lfFaceName, _countof(cf.szFaceName) - 1);
	cf.szFaceName[_countof(cf.szFaceName) - 1] = _T('\0');
	m_RECSelClaInfo.SetWordCharFormat(cf);//设置所有字体

	memset(&cf, 0, sizeof(cf));
	m_RECSelClaInfo.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(255,0,0);//设置颜色
	m_RECSelClaInfo.SetSelectionCharFormat(cf);//设置当前字体
	
	if ( ERROR_CLASSID == nMainClassID )
	{
		//m_RECSelClaInfo.SetSel(-1, -1);     //将光标移动到最后
		m_RECSelClaInfo.ReplaceSel(GetResString(IDS_PROP_INFO_FIRST_SEL_MAINCLASS) +L"\r\n" );
	}
	else
	{
		//csTemp.Format(L"%lld ",nMainClassID);
		//m_RECSelClaInfo.SetSel(-1, -1);     //将光标移动到最后
		if ( EMT_SYS == m_emt )
		{
			csTemp = wyytExtendDataManage::Instance().GetClassFullName(nMainClassID).c_str();
		}
		else
		{
			csTemp = wyytExtendDataManagePri::Instance().GetClassFullName(nMainClassID).c_str();
		}
		m_RECSelClaInfo.ReplaceSel( GetResString(IDS_PROP_INFO_FIRST_MAINCLASS) + csTemp  + L"\r\n");
	}
	if ( vecTrueSelClass.size() <= 0 )
	{
		return;
	}
	DWORD dwMask = 0;
	for ( vector<int64>::iterator it = vecTrueSelClass.begin(); it != vecTrueSelClass.end(); it++)
	{
		if ( nMainClassID == (*it) )
		{
			continue;
		}
		//csTemp.Format(L"%lld \r\n",(*it));
		if ( EMT_SYS == m_emt )
		{
			csTemp = wyytExtendDataManage::Instance().GetClassFullName((*it)).c_str();
		}
		else
		{
			csTemp = wyytExtendDataManagePri::Instance().GetClassFullName((*it)).c_str();
		}		
		//m_RECSelClaInfo.SetSel(-1, -1);     //将光标移动到最后
		m_RECSelClaInfo.ReplaceSel( csTemp  + L"\r\n" );

		//m_RECSelClaInfo.AddLine(csTemp, csTemp.GetLength(), true);
		//m_RECSelClaInfo.AppendColoredText(csTemp, INFO_MSG_COLOR, CLR_DEFAULT, dwMask);
		//csSelInfos += csTemp;
		//		csSelInfos += it->
	}


	//csSelInfos += csTemp;
	//m_RECSelClaInfo.AddLine(csTemp, csTemp.GetLength(), true);
	//m_RECSelClaInfo.AppendColoredText(csTemp, INFO_MSG_COLOR, CLR_DEFAULT, dwMask);
	//m_RECSelClaInfo.SetWindowText(csSelInfos);
	//m_RECSelClaInfo.ReplaceSel( csTemp );
}

BOOL CRecordDetailsContentDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message ) 
	{ 
		//UINT nll =  LOWORD(pMsg->lParam);//(int) pMsg->lParam; 
		//UINT nlh =  HIWORD(pMsg->lParam);
		HWND hWnd=::GetFocus();
		int iID=::GetDlgCtrlID(hWnd);
		UINT nKey = (int) pMsg->wParam; 
		if( (VK_RETURN == nKey &&  IDC_RICHEDIT2_CONTENT/*IDC_EDIT_CONTENT*/ !=iID && IDC_EDIT_REF != iID)  || VK_ESCAPE == nKey ) 
			return TRUE ; 
	}

	return CResizableDialog::PreTranslateMessage(pMsg);
}
