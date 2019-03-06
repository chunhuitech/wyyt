// AttachsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "AttachsDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "Preferences.h"
#include "TitleMenu.h"
#include "MenuCmds.h"
#include "AttachAddDlg.h"
// CAttachsDlg 对话框

IMPLEMENT_DYNAMIC(CAttachsDlg, CResizablePage)

CAttachsDlg::CAttachsDlg(CWnd* pParent /*=NULL*/)
	: CResizablePage(CAttachsDlg::IDD, 0)
{
	m_bReducedDlg = false;
	m_bDataChanged = false;
	m_pRI = NULL; 
	m_strCaption = GetResString(IDS_RECORD_OPERTYPE_ATTACH);
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;
}

CAttachsDlg::~CAttachsDlg()
{
}

void CAttachsDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATTACHS, m_listAttachs);
}


BEGIN_MESSAGE_MAP(CAttachsDlg, CResizablePage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ATTACHS, &CAttachsDlg::OnNMClickListAttachs)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ATTACHS, &CAttachsDlg::OnNMRClickListAttachs)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ATTACHS, &CAttachsDlg::OnLvnItemchangedListAttachs)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ATTACHS, &CAttachsDlg::OnNMDblclkListAttachs)
END_MESSAGE_MAP()


// CAttachsDlg 消息处理程序
void CAttachsDlg::Localize()
{
	//SetDlgItemText(IDC_BUTTON_ADD,GetResString(IDS_ADD));
	//SetDlgItemText(IDC_BUTTON_DEL,GetResString(IDS_DEL));

	//SetDlgItemText(IDC_STATIC_FULLNAME_UP,GetResString(IDS_STATIC_FULLNAME_UP));
	SetDlgItemText(IDC_STATIC_FULLNAME_SAVE,GetResString(IDS_STATIC_FULLNAME_SAVE));
	SetDlgItemText(IDC_STATIC_DOWNADDRESS,GetResString(IDS_STATIC_DOWNADDRESS));
	SetDlgItemText(IDC_STATIC_CODEID,GetResString(IDS_STATIC_CODEID));

	HDITEM hdi;// X: [AL] - [Additional Localize]
	hdi.mask = HDI_TEXT;
	CString strRes;
	strRes=GetResString(IDS_CODE);//编号
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(0, &hdi);

	strRes=GetResString(IDS_FILENAME);//文件名
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(1, &hdi);

	strRes=GetResString(IDS_FILESIZE);//文件大小
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(2, &hdi);

	strRes=GetResString(IDS_FILESTATUS);///文件状态
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(3, &hdi);
}

BOOL CAttachsDlg::OnInitDialog()
{
	CResizablePage::OnInitDialog();

	InitWindowStyles(this);
	m_listAttachs.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	ASSERT( (m_listAttachs.GetStyle() & LVS_SINGLESEL) == 0);
	m_listAttachs.InsertColumn(0, _T("")/*GetResString(IDS_TITLE)*/, LVCFMT_LEFT, 150);//编号
	m_listAttachs.InsertColumn(1, _T("")/*GetResString(IDS_S_DAYS)*/, LVCFMT_LEFT, 260);//文件名
	m_listAttachs.InsertColumn(2, _T("")/*GetResString(IDS_STARTTIME)*/, LVCFMT_LEFT, 80);//文件大小
	m_listAttachs.InsertColumn(3, _T("")/*GetResString(IDS_STARTTIME)*/, LVCFMT_LEFT, 80);//文件状态

	//if (!m_bReducedDlg)//显示相关控件
	//{
	//	AddAnchor(IDC_LIST_ATTACHS, TOP_LEFT,BOTTOM_RIGHT);
	//}
	//else//不显示相关详细信息控件  即下端显示
	//{
	//	//GetDlgItem(IDC_STATIC_FULLNAME_UP)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_STATIC_FULLNAME_SAVE)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_STATIC_DOWNADDRESS)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_STATIC_CODEID)->ShowWindow(SW_HIDE);

	//	GetDlgItem(IDC_EDIT_CODEID)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_EDIT_FULLNAME_SAVE)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_EDIT_DOWNADDRESS)->ShowWindow(SW_HIDE);
	//	
	//	CRect rcClient;
	//	GetClientRect(&rcClient);
	//	//ScreenToClient(&rcClient);
	//	//CalcInsideRect(rcClient, FALSE);
	//	GetDlgItem(IDC_LIST_ATTACHS)->MoveWindow(&rcClient);
	//}
	AddAnchor(IDC_LIST_ATTACHS, TOP_LEFT,BOTTOM_RIGHT);

	Localize();
	FillAttachsList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAttachsDlg::FillAttachsList() 
{
	SetDlgItemText(IDC_EDIT_CODEID,L"");
	SetDlgItemText(IDC_EDIT_FULLNAME_UP,L"");
	SetDlgItemText(IDC_EDIT_FULLNAME_SAVE,L"");
	SetDlgItemText(IDC_EDIT_DOWNADDRESS,L"");
	m_listAttachs.DeleteAllItems();
	if ( NULL == m_pRI )
	{
		return;
	}
	
	size_t index=0;
	if ( EMT_SYS == m_emt )
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(m_pRI->ID);
		if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
		{
			for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++)
			{
				m_listAttachs.InsertItem(index , it->CodeID.c_str() );
				m_listAttachs.SetItemText(index, 1, (it->FileName + it->FileSuffix).c_str());
				m_listAttachs.SetItemText(index, 2, CastItoXBytes((uint64)it->FileSize, false, false));
				m_listAttachs.SetItemText(index, 3, GetFileState(it->FileState));
				m_listAttachs.SetItemData(index,(LPARAM)(&(*it)));
			}
			if (m_listAttachs.GetItemCount()>0) 
			{
				m_listAttachs.SetSelectionMark(0);
				m_listAttachs.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
				LoadAttach(0);
			}
		}
	}
	else
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManagePri::Instance().m_mapRec2Attachs.find(m_pRI->ID);
		if (Iter != wyytExtendDataManagePri::Instance().m_mapRec2Attachs.end() )
		{
			for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++)
			{
				m_listAttachs.InsertItem(index , it->CodeID.c_str() );
				m_listAttachs.SetItemText(index, 1, (it->FileName + it->FileSuffix).c_str());
				m_listAttachs.SetItemText(index, 2, CastItoXBytes((uint64)it->FileSize, false, false));
				m_listAttachs.SetItemText(index, 3, GetFileState(it->FileState));
				m_listAttachs.SetItemData(index,(LPARAM)(&(*it)));
			}
			if (m_listAttachs.GetItemCount()>0) 
			{
				m_listAttachs.SetSelectionMark(0);
				m_listAttachs.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
				LoadAttach(0);
			}
		}
	}
	
}


void CAttachsDlg::LoadAttach(size_t index) 
{
	CRecordAttachs * pCurAtt = (CRecordAttachs*)m_listAttachs.GetItemData(index);
	CString csFileFullNameUp,csFileFullNameSave;
	csFileFullNameUp.Format(L"%s\\%s%s",pCurAtt->SrcPath.c_str(),pCurAtt->FileName.c_str(),pCurAtt->FileSuffix.c_str());
	if ( EMT_SYS == m_emt )
	{
		csFileFullNameSave.Format(L"%s\\%s%s",/*thePrefs.GetMuleDirectory(WYYT_ATTACHMENTS_SYS_DIR)*/thePrefs.m_strSysAttachPath,pCurAtt->CodeID.c_str(),pCurAtt->FileSuffix.c_str());
	}
	else
	{
		csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,pCurAtt->CodeID.c_str(),pCurAtt->FileSuffix.c_str());
	}
	SetDlgItemText(IDC_EDIT_CODEID,pCurAtt->CodeID.c_str());
	SetDlgItemText(IDC_EDIT_FULLNAME_UP,csFileFullNameUp);
	SetDlgItemText(IDC_EDIT_FULLNAME_SAVE,csFileFullNameSave);
	SetDlgItemText(IDC_EDIT_DOWNADDRESS,pCurAtt->DownloadAddress.c_str());
}

BOOL CAttachsDlg::OnSetActive()
{
	if (!CResizablePage::OnSetActive())
		return FALSE;
	if ( m_bDataChanged && m_pRI )
	{
		FillAttachsList();
		m_bDataChanged = false;
	}
	return TRUE;
}


void CAttachsDlg::OnNMClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (m_listAttachs.GetSelectionMark()>-1) 
		LoadAttach(m_listAttachs.GetSelectionMark());
	*pResult = 0;
}


void CAttachsDlg::OnNMRClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	*/
	POINT point;
	::GetCursorPos(&point);

	CTitleMenu m_AttachsMenu;
	//CMenu m_ActionSel;
	//CMenu m_CatActionSel;

	/*bool isCatAction=false;
	if (m_listAttachs.GetSelectionMark()!=-1) 
	{
		DWORD_PTR ac=m_listAttachs.GetItemData(m_listAttachs.GetSelectionMark());
		if (ac==6 || ac==7) isCatAction=true;
	}*/

	m_AttachsMenu.CreatePopupMenu();
	//m_ActionSel.CreatePopupMenu();
	//m_CatActionSel.CreatePopupMenu();
	
	UINT nFlag=MF_STRING;
	if (m_listAttachs.GetSelectionMark()==-1) 
		nFlag=MF_STRING | MF_GRAYED;

	

	m_AttachsMenu.AddMenuTitle(GetResString(IDS_ATTACH));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_OPEN,	GetResString(IDS_OPEN_FILE));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_OPENFOLDER,GetResString(IDS_ATTACH_OPENFOLDER));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_SHOW,GetResString(IDS_ATTACH_SHOW));		
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_SAVEAS,GetResString(IDS_FILE_SAVEAS));


	m_AttachsMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	VERIFY( m_AttachsMenu.DestroyMenu() );

	*pResult = 0;
}


BOOL CAttachsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	wParam = LOWORD(wParam);

	CAtlList<CRecordAttachs*> selectedList;
	POSITION pos = m_listAttachs.GetFirstSelectedItemPosition();
	while (pos != NULL){
		int index = m_listAttachs.GetNextSelectedItem(pos);
		if (index >= 0)
			selectedList.AddTail((CRecordAttachs*)m_listAttachs.GetItemData(index));
	}

	if (   wParam == MP_ATTACH_OPEN
		|| wParam == MP_ATTACH_SAVEAS
		|| selectedList.GetCount() > 0)
	{
		CRecordAttachs* attach = NULL;
		if (selectedList.GetCount() >= 1)
			attach = selectedList.GetHead();
		switch (wParam){
		case MP_ATTACH_OPEN:
			{
				CString csFileFullName;
				if ( EMT_SYS == m_emt )
				{
					csFileFullName.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
				}
				else
				{
					csFileFullName.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
				}
				ShellOpenFile(csFileFullName);
			}
			break;
		case MP_ATTACH_OPENFOLDER:
			{
				CString csFileFullPath;
				if ( EMT_SYS == m_emt )
				{
					csFileFullPath.Format(L"%s\\",thePrefs.m_strSysAttachPath);
				}
				else
				{
					csFileFullPath.Format(L"%s\\",thePrefs.m_strPriAttachPath);
				}
				ShellExecute(NULL, _T("open"), _T("explorer"), csFileFullPath/*_T("/select,\"") + file->GetFilePath() + _T("\"")*/, NULL, SW_SHOW);
			}
			break;
		case MP_ATTACH_SHOW:
			{
				CAttachAddDlg dialog;
				dialog.SetRecAttInitInfo(attach,AOT_SHOW);
				dialog.DoModal();
			}
			break;
		case MP_ATTACH_SAVEAS:
			{
				wstring Suffix;
				int spon = attach->FileSuffix.find_first_of(L".");
				Suffix = attach->FileSuffix.substr(spon+1);
				CString csFiles;
				csFiles.Format(L"%s Files(*.%s)|*.%s||",Suffix.c_str(),Suffix.c_str(),Suffix.c_str());
				CFileDialog dlg(FALSE, Suffix.c_str(), attach->FileName.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFiles, this, 0);
				if (dlg.DoModal() == IDOK)
				{
					CString csFileSrcName;
					if ( EMT_SYS == m_emt )
					{
						csFileSrcName.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
					}
					else
					{
						csFileSrcName.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
					}
					::CopyFile(csFileSrcName, dlg.GetPathName(), false);
				}
			}
			break;
		default: 
			break;
		}
	}
	return CResizablePage::OnCommand(wParam, lParam);
}


void CAttachsDlg::OnLvnItemchangedListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	
	*pResult = 0;
}


void CAttachsDlg::OnNMDblclkListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CAtlList<CRecordAttachs*> selectedList;
	POSITION pos = m_listAttachs.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int index = m_listAttachs.GetNextSelectedItem(pos);
		if (index >= 0)
			selectedList.AddTail((CRecordAttachs*)m_listAttachs.GetItemData(index));
	}
	if ( selectedList.GetCount() > 0 )
	{
		CRecordAttachs* attach = NULL;
		if (selectedList.GetCount() >= 1)
			attach = selectedList.GetHead();
		CString csFileFullName;
		if ( EMT_SYS == m_emt )
		{
			csFileFullName.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
		}
		else
		{
			csFileFullName.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,attach->CodeID.c_str(),attach->FileSuffix.c_str());
		}
		ShellOpenFile(csFileFullName);
	}
	*pResult = 0;
}
