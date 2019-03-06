// AttachAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "AttachAddDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"

// CAttachAddDlg 对话框

IMPLEMENT_DYNAMIC(CAttachAddDlg, CDialog)

CAttachAddDlg::CAttachAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttachAddDlg::IDD, pParent)
{
	m_pRA = NULL;
	m_aot = AOT_ADD;
}

CAttachAddDlg::~CAttachAddDlg()
{
}

void CAttachAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAttachAddDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEV, &CAttachAddDlg::OnBnClickedButtonBrowsev)
	ON_BN_CLICKED(IDOK, &CAttachAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAttachAddDlg 消息处理程序
void CAttachAddDlg::Localize()
{
	SetWindowText(GetResString(IDS_ATTACH_ADD));
	SetDlgItemText(IDC_STATIC_SELECTUPFILE,GetResString(IDS_STATIC_SELECTUPFILE));
	SetDlgItemText(IDC_STATIC_FILENAME,GetResString(IDS_STATIC_FILENAME));
	SetDlgItemText(IDC_STATIC_DOWNADRESS,GetResString(IDS_STATIC_DOWNADDRESS));
	SetDlgItemText(IDC_STATIC_INFO,GetResString(IDS_STATIC_INFO));
	SetDlgItemText(IDOK,GetResString(IDS_OK));
	SetDlgItemText(IDCANCEL,GetResString(IDS_CANCEL));
	if ( m_aot == AOT_MOD || AOT_SHOW == m_aot )
	{
		GetDlgItem(IDC_STATIC_SELECTUPFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_UPFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_BROWSEV)->ShowWindow(SW_HIDE);
		if ( m_pRA )
		{
			SetDlgItemText(IDC_EDIT_DOWNADDRESS,m_pRA->DownloadAddress.c_str());
			SetDlgItemText(IDC_EDIT_INFO,m_pRA->Note.c_str());
			SetDlgItemText(IDC_EDIT_FILENAME,m_pRA->FileName.c_str());
		}
		

	}
	if (  AOT_SHOW == m_aot )
	{
		GetDlgItem(IDC_EDIT_FILENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DOWNADDRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INFO)->EnableWindow(FALSE);
	}
	if (  AOT_ADD == m_aot )
	{
		GetDlgItem(IDC_STATIC_FILENAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_FILENAME)->ShowWindow(SW_HIDE);
	}
}
void CAttachAddDlg::SetRecAttInitInfo(CRecordAttachs* pAttInfo,AttachOperType aot)
{
	m_pRA = pAttInfo;
	m_aot = aot;
}
BOOL CAttachAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitWindowStyles(this);
	
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAttachAddDlg::OnBnClickedButtonBrowsev()
{
	
	CString strFilePath;
	GetDlgItemText(IDC_EDIT_UPFILE, strFilePath);
	CFileDialog dlgFile(TRUE, _T("*"), strFilePath, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, _T("所有文件 (*.*)|*.*||"), NULL, 0);
	if (dlgFile.DoModal() == IDOK)
	{
		CString cstem = dlgFile.GetPathName();
		SetDlgItemText(IDC_EDIT_UPFILE,cstem);
		
		//wyyt.pdb
		CFileStatus status;
		CFile::GetStatus(cstem,status);
		ULONGLONG ullFoundFileSize  = status.m_size;
		m_pRA->FileSize = (int64)ullFoundFileSize;

		int    nPos;   
		nPos=cstem.ReverseFind('\\');   
		cstem=cstem.Left(nPos);   
		m_pRA->SrcPath = cstem.GetBufferSetLength(cstem.GetLength());
		/*CFileFind ff;
		bool rel = ff.FindFile(cstem, 0);
		ULONGLONG ullFoundFileSize = ff.GetLength();
		m_pRA->FileSize = (int)ullFoundFileSize;*/
		uchar aucMD4[16];
		md4clr(aucMD4);
		cstem.Format(L"%0x",aucMD4);
		m_pRA->HashValue =  cstem.GetBufferSetLength(cstem.GetLength());
		cstem = dlgFile.GetFileTitle();//wyyt
		m_pRA->FileName = cstem.GetBufferSetLength(cstem.GetLength());
		m_pRA->FileState = E_FS_DOWNED;
		cstem = dlgFile.GetFileExt(); //pdb
		m_pRA->FileSuffix = L"." + cstem;
		
		
		//cstem = dlgFile.GetFileName(); //wyyt.pdb
		
		//OnSettingsChange(); // X: [CI] - [Code Improvement] Apply if modified
	}
}


void CAttachAddDlg::OnBnClickedOk()
{
	if (  AOT_SHOW != m_aot )
	{
		CString strFileDownAddress;
		GetDlgItemText(IDC_EDIT_DOWNADDRESS, strFileDownAddress);
		m_pRA->DownloadAddress = strFileDownAddress.GetBufferSetLength(strFileDownAddress.GetLength());
		CString strFileDownInfo;
		GetDlgItemText(IDC_EDIT_INFO, strFileDownInfo);
		m_pRA->Note = strFileDownInfo.GetBufferSetLength(strFileDownInfo.GetLength());
		if (  AOT_MOD == m_aot )
		{
			CString strFileName;
			GetDlgItemText(IDC_EDIT_FILENAME, strFileName);
			m_pRA->FileName = strFileName.GetBufferSetLength(strFileName.GetLength());
		}
	}
	
	CDialog::OnOK();
}
