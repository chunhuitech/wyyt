// RecordDetailsContentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecordDetailsContentDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "StringConversion.h"
// CRecordDetailsContentDlg �Ի���

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


// CRecordDetailsContentDlg ��Ϣ�������
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	AddAnchor(IDC_RICHEDIT2_SELCLASSINFO, TOP_LEFT,TOP_RIGHT);
	AddAnchor(IDC_RICHEDIT2_CONTENT/*IDC_EDIT_CONTENT*/, TOP_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC_REF, BOTTOM_LEFT,BOTTOM_RIGHT);
	AddAnchor(IDC_EDIT_REF, BOTTOM_LEFT,BOTTOM_RIGHT);
	
	Localize();



	m_RECSelClaInfo.SetTargetDevice(NULL, 1); //m_nWordWrap == WrapNone
	m_RECSelClaInfo.SetTargetDevice(NULL, 0);//m_nWordWrap == WrapToWindow

	BliandData();
	ShowSelectedClasses(m_pRI->nAllClassID,m_pRI->MainClassID);


	//��������
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
	//cf.dwEffects = CFE_BOLD; // ���壨������CFE_ITALIC��CFE_STRIKEOUT ��CFE_UNDERLINE��
	//cf.crTextColor = RGB(0, 0, 255); //������ɫ--------��
	_tcsncpy(cf.szFaceName ,_T("Fixedsys"), _countof(cf.szFaceName) );//����
	cf.yHeight = 200;//18*18;//���ָ߶�
	m_REContent.SetDefaultCharFormat(cf); 
	m_REContent.SetBackgroundColor(FALSE,RGB(204, 232, 207));   //�豳��ɫ���� 
	

	//m_REContent.SetWordCharFormat(cf); //Ϊ��Ҫ����������趨����

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CResizableDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	//m_pRI->Content = L"�����ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->����������ࣺ����һ��-->IVR��ǧ�����ǧ����-->�����";
	
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
	m_RECSelClaInfo.SetSel(-1, -1);     //������ƶ������

	/*vector<int64> vecSelClass;
	int64 nMainClassID = ERROR_CLASSID;*/
	//m_ClassSelector.GetSelectedClasses(&vecSelClass,nMainClassID);

	CString csTemp;
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	m_RECSelClaInfo.GetSelectionCharFormat(cf);
	//��������
	//strcpy(cf.szFaceName ,_T("����"));//��������
	cf.dwMask |= CFM_FACE;
	LOGFONT lf = {0};
	theApp.m_fontSymbol.GetLogFont(&lf);	
	_tcsncpy(cf.szFaceName, lf.lfFaceName, _countof(cf.szFaceName) - 1);
	cf.szFaceName[_countof(cf.szFaceName) - 1] = _T('\0');
	m_RECSelClaInfo.SetWordCharFormat(cf);//������������

	memset(&cf, 0, sizeof(cf));
	m_RECSelClaInfo.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(255,0,0);//������ɫ
	m_RECSelClaInfo.SetSelectionCharFormat(cf);//���õ�ǰ����
	
	if ( ERROR_CLASSID == nMainClassID )
	{
		//m_RECSelClaInfo.SetSel(-1, -1);     //������ƶ������
		m_RECSelClaInfo.ReplaceSel(GetResString(IDS_PROP_INFO_FIRST_SEL_MAINCLASS) +L"\r\n" );
	}
	else
	{
		//csTemp.Format(L"%lld ",nMainClassID);
		//m_RECSelClaInfo.SetSel(-1, -1);     //������ƶ������
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
		//m_RECSelClaInfo.SetSel(-1, -1);     //������ƶ������
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
