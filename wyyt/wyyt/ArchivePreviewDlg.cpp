// ArchivePreviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "ArchivePreviewDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"

// CArchivePreviewDlg �Ի���

IMPLEMENT_DYNAMIC(CArchivePreviewDlg, CResizablePage)

CArchivePreviewDlg::CArchivePreviewDlg()
	: CResizablePage(CArchivePreviewDlg::IDD, 0)
{
	m_paFiles = NULL;
	m_pRI = NULL; 
	m_bDataChanged = false;
	m_bReducedDlg = false;
	m_strCaption = GetResString(IDS_CONTENT_INFO);
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;
}

CArchivePreviewDlg::~CArchivePreviewDlg()
{
}

void CArchivePreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_NR, m_ctlRENr);
}


BEGIN_MESSAGE_MAP(CArchivePreviewDlg, CResizablePage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CArchivePreviewDlg ��Ϣ�������
void CArchivePreviewDlg::Localize(void)
{
	/*if (!m_bReducedDlg)
	{
	SetDlgItemText(IDC_READARCH,	GetResString(IDS_SV_UPDATE) );
	SetDlgItemText(IDC_RESTOREARCH, GetResString(IDS_AP_CREATEPREVCOPY) );
	SetDlgItemText(IDC_ARCP_TYPE ,	GetResString(IDS_ARCHTYPE)+_T(':') );
	SetDlgItemText(IDC_ARCP_STATUS,	GetResString(IDS_STATUS)+_T(':') );
	SetDlgItemText(IDC_ARCP_ATTRIBS,GetResString(IDS_INFO)+_T(':')  );
	}*/
}

BOOL CArchivePreviewDlg::OnInitDialog()
{
	CWaitCursor curWait; // we may get quite busy here..
	CResizablePage::OnInitDialog();
	//	ReplaceRichEditCtrl(&m_fi, this, GetDlgItem(IDC_FD_XI1)->GetFont());// X: [CI] - [Code Improvement] Don't need replace under VS2005+
	InitWindowStyles(this);

	//if (!m_bReducedDlg)
	//{
	//	AddAnchor(m_ctlRENr, TOP_LEFT, BOTTOM_RIGHT);

	//	m_ctlRENr.LimitText(/*afxIsWin95() ? 0xFFFF : */0x7FFFFFFF);
	//	m_ctlRENr.SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(3, 3));
	//	m_ctlRENr.SetAutoURLDetect();
	//	m_ctlRENr.SetEventMask(m_ctlRENr.GetEventMask() | ENM_LINK);
	//}
	//else
	//{

	//}

	AddAnchor(m_ctlRENr, TOP_LEFT, BOTTOM_RIGHT);

	m_ctlRENr.LimitText(/*afxIsWin95() ? 0xFFFF : */0x7FFFFFFF);
	m_ctlRENr.SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELONG(3, 3));
	m_ctlRENr.SetAutoURLDetect();
	m_ctlRENr.SetEventMask(m_ctlRENr.GetEventMask() | ENM_LINK);
	CResizablePage::UpdateData(FALSE);
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CArchivePreviewDlg::OnDestroy()
{
	// This property sheet's window may get destroyed and re-created several times although
	// the corresponding C++ class is kept -> explicitly reset ResizeableLib state
	RemoveAllAnchors();
	CResizablePage::OnDestroy();

}


BOOL CArchivePreviewDlg::OnSetActive()
{
	if (!CResizablePage::OnSetActive())
		return FALSE;
	//if ( m_pRI == NULL )
	//{
	//	if ( m_paFiles && m_paFiles->GetSize() >= 1 )
	//	{
	//		m_pRI = (const CRecordItem*)((*m_paFiles)[0]);
	//		m_bDataChanged = true;	
	//	}
	//}
	if ( m_bDataChanged && m_pRI )
	{
		//������������Ϣ
		CString csMainClassName;
		if ( EMT_SYS == m_emt )
		{
			csMainClassName = wyytExtendDataManage::Instance().GetClassFullName(m_pRI->MainClassID).c_str();
		}
		else
		{
			csMainClassName = wyytExtendDataManagePri::Instance().GetClassFullName(m_pRI->MainClassID).c_str();
		}
		
		//����������Ϣ
		CString csOtherClassNames;
		for ( vector<int64>::const_iterator it = m_pRI->nAllClassID.begin(); it != m_pRI->nAllClassID.end(); it++)
		{
			if ( m_pRI->MainClassID == (*it) )
			{
				continue;
			}
			CString csTempName;
			if ( EMT_SYS == m_emt )
			{
				csTempName = wyytExtendDataManage::Instance().GetClassFullName((*it)).c_str();
			}
			else
			{
				csTempName = wyytExtendDataManagePri::Instance().GetClassFullName((*it)).c_str();
			}
			//m_RECSelClaInfo.SetSel(-1, -1);     //������ƶ������
			csOtherClassNames += csTempName;
			csOtherClassNames += L"\n";
		}
		if (csOtherClassNames.GetLength() == 0 )
		{
			csOtherClassNames = L"\n";
		}
		

		CString csNr;
		csNr.Format(L"                    %s\n�ؼ���:%s\n%s\n%s  %s\n\n����:\n%s\n��¼��ţ�%s",m_pRI->Titlel.c_str(),m_pRI->KeyValues.c_str(),csMainClassName,csOtherClassNames,m_pRI->Content.c_str(),m_pRI->Reference.c_str(),m_pRI->SID.c_str());
		m_ctlRENr.SetWindowText(csNr);

		int nCurLength =  m_pRI->Titlel.length() + 20;
		/////////////////////////////////���ñ�������/////////////////////////////////////////
		CHARFORMAT cf_Title;
		ZeroMemory(&cf_Title, sizeof(CHARFORMAT));
		cf_Title.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf_Title.dwEffects = CFE_BOLD; // ���壨������CFE_ITALIC��CFE_STRIKEOUT ��CFE_UNDERLINE��
		cf_Title.yHeight = 460;//200 18*18;//���ָ߶�
		//cf_Title.crTextColor = RGB(0, 0, 255); //������ɫ--------��
		_tcsncpy(cf_Title.szFaceName, _T("Fixedsys"), _countof(cf_Title.szFaceName));//����
		
		m_ctlRENr.SetSel(0,nCurLength); //���ô�������
		m_ctlRENr.SetSelectionCharFormat(cf_Title);
		/////////////////////////////////���ùؼ�������/////////////////////////////////////////
		int nOldLenth = nCurLength;
		nCurLength += (1 + 4);//�س����� �ؼ���:  
		nCurLength +=  m_pRI->KeyValues.length();
		
		CHARFORMAT cf_KeyValue;
		ZeroMemory(&cf_KeyValue, sizeof(CHARFORMAT));
		cf_KeyValue.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf_KeyValue.dwEffects = CFE_BOLD; // ���壨������CFE_ITALIC��CFE_STRIKEOUT ��CFE_UNDERLINE��
		//cf_KeyValue.yHeight = 460;//200 18*18;//���ָ߶�
		cf_KeyValue.crTextColor = RGB(0, 0, 255); //������ɫ--------��
		_tcsncpy(cf_KeyValue.szFaceName, _T("Fixedsys"), _countof(cf_KeyValue.szFaceName));//����

		m_ctlRENr.SetSel(nOldLenth,nCurLength); //���ô�������
		m_ctlRENr.SetSelectionCharFormat(cf_KeyValue);

		/////////////////////////////////������������/////////////////////////////////////////
		nOldLenth = nCurLength;
		nCurLength += 1;//�س�����  
		nCurLength +=  csMainClassName.GetLength();

		CHARFORMAT cf_MainClassName;
		ZeroMemory(&cf_MainClassName, sizeof(CHARFORMAT));
		cf_MainClassName.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf_MainClassName.dwEffects = CFE_BOLD; // ���壨������CFE_ITALIC��CFE_STRIKEOUT ��CFE_UNDERLINE��
		//cf_KeyValue.yHeight = 460;//200 18*18;//���ָ߶�
		cf_MainClassName.crTextColor = RGB(255, 0, 0); //������ɫ--------��
		_tcsncpy(cf_MainClassName.szFaceName, _T("Fixedsys"), _countof(cf_MainClassName.szFaceName));//����

		m_ctlRENr.SetSel(nOldLenth,nCurLength); //���ô�������
		m_ctlRENr.SetSelectionCharFormat(cf_MainClassName);

		/////////////////////////////////����������������/////////////////////////////////////////
		nOldLenth = nCurLength;
		nCurLength += 1;//�س�����  
		nCurLength +=  csOtherClassNames.GetLength();
		cf_MainClassName.crTextColor = RGB(255, 150, 0); //������ɫ--------��

		m_ctlRENr.SetSel(nOldLenth,nCurLength); //���ô�������
		m_ctlRENr.SetSelectionCharFormat(cf_MainClassName);
		m_bDataChanged = false;
	}
	/*
	if (m_bDataChanged)
	{
		CString strWait = GetResString(IDS_FSTAT_WAITING);
		//SetDlgItemText(IDC_FORMAT, strWait);
		m_ctlRENr.SetWindowText(strWait);

		for (int i = 0; i < m_paFiles->GetSize(); i++)
			//m_aFiles.Add(STATIC_DOWNCAST(CShareableFile, (*paFiles)[i]));
			m_aFiles.Add((CRecordItem*)(*m_paFiles)[i]);

		//CString strNr;
		//for (int i = 0; i < m_aFiles.GetSize(); i++)
		//{
		//	strNr +=  m_aFiles[i]->Content.c_str();
		//	
		//}
		m_ctlRENr.SetWindowText(strNr);
		m_bDataChanged = false;
	}
	*/
	return TRUE;
}
