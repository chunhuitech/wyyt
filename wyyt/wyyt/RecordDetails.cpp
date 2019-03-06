// RecordDetails.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecordDetails.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "UserMsgs.h"
// CRecordDetails 对话框

IMPLEMENT_DYNAMIC(CRecordDetails, CDialog)

CRecordDetails::CRecordDetails(CWnd* pParent /*=NULL*/,EModuleType emt/* = EMT_SYS*/)
	: CResizableDialog(CRecordDetails::IDD, pParent), m_emt(emt)
{
	m_nCurOperOPtion = ECOO_ADD;

	activewnd = NULL;
	m_pRecDetClaDlg = NULL;
	m_pRecDetClaDlg = new CRecordDetailsClassDlg(NULL,m_emt);
	
	m_pRecDetContDlg = NULL;
	m_pRecDetContDlg = new CRecordDetailsContentDlg(NULL,m_emt);

	m_pRecDetAttachDlg = NULL;
	m_pRecDetAttachDlg = new CRecordDetailsAttachDlg(NULL,m_emt);

	m_pRI = NULL;
}

CRecordDetails::~CRecordDetails()
{
//	ReleaseOperItems();
	if ( m_pRecDetClaDlg != NULL )
	{
		delete m_pRecDetClaDlg;
		m_pRecDetClaDlg = NULL;
	}
	if ( m_pRecDetContDlg != NULL )
	{
		delete m_pRecDetContDlg;
		m_pRecDetContDlg = NULL;
	}
	if ( m_pRecDetAttachDlg != NULL )
	{
		delete m_pRecDetAttachDlg;
		m_pRecDetAttachDlg = NULL;
	}
	
}

void CRecordDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OPER_OPTION, m_listOperOption);
}


BEGIN_MESSAGE_MAP(CRecordDetails, CResizableDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OPER_OPTION, &CRecordDetails::OnNMClickListOperOption)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void DialogCreateIndirect_R(CDialog *pWnd, UINT uID)
{
#if 0
	// This could be a nice way to change the font size of the main windows without needing
	// to re-design the dialog resources. However, that technique does not work for the
	// SearchWnd and it also introduces new glitches (which would need to get resolved)
	// in almost all of the main windows.
	CDialogTemplate dlgTempl;
	dlgTempl.Load(MAKEINTRESOURCE(uID));
	dlgTempl.SetFont(_T("MS Shell Dlg"), 8);
	pWnd->CreateIndirect(dlgTempl.m_hTemplate);
	FreeResource(dlgTempl.Detach());
#else
	pWnd->Create(uID);
#endif
}

// CRecordDetails 消息处理程序


BOOL CRecordDetails::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	InitWindowStyles(this);

	// TODO:  在此添加额外的初始化
	AddAnchor(IDC_LIST_OPER_OPTION, TOP_LEFT,BOTTOM_LEFT);
	//AddAnchor(IDC_STATIC_FRAME_INFO, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_LEFT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);


	InitListCtrl( m_listOperOption );

	//InitOperItems();
//	DialogCreateIndirect_R(m_pRecDetClaDlg, IDD_DIALOG_RECORD_DETAILS_CLASS);
	m_pRecDetClaDlg->Create( CRecordDetailsClassDlg::IDD, this );
	m_pRecDetClaDlg->LoadSelectorClasses(&m_pRI->nAllClassID,m_pRI->MainClassID);
	m_pRecDetContDlg->Create( CRecordDetailsContentDlg::IDD, this );
	//m_pRecDetContDlg->InitCurRecordInfo(m_pRI);//换一个地方调用
	m_pRecDetAttachDlg->Create( CRecordDetailsAttachDlg::IDD, this );
	
	if (activewnd == NULL)
	{
		if ( ECOO_ADD == m_nCurOperOPtion )
		{
			SetActiveDialog(m_pRecDetClaDlg);
		}
		else
		{
			SetActiveDialog(m_pRecDetContDlg);
		}
		
	}

	CRect rcClient, rcList, rcStatusbar;
	GetClientRect(&rcClient);
	m_listOperOption.GetWindowRect(&rcList);
	( GetDlgItem( IDC_STATIC_FRAME_INFO ) )->GetWindowRect( & rcStatusbar );
	rcClient.left += rcList.Width();
	rcClient.left += 5;
	rcClient.bottom -= rcStatusbar.Height();//(rcClient.Height() - rcList.Height()); 

	CWnd* apWnds[] =
	{
		m_pRecDetClaDlg,
		m_pRecDetContDlg,
		m_pRecDetAttachDlg
	};
	for (int i = 0; i < _countof(apWnds); i++)
		apWnds[i]->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER);
	
	// anchors
	AddAnchor(*m_pRecDetClaDlg,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*m_pRecDetContDlg,		TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(*m_pRecDetAttachDlg,		TOP_LEFT, BOTTOM_RIGHT);
	//AddAnchor(*priresmanagewnd,		TOP_LEFT, BOTTOM_RIGHT);

	Localize();	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CRecordDetails::Localize()
{
	CString title;
	if ( ECOO_ADD == m_nCurOperOPtion )
	{
		title = RemoveAmbersand(GetResString(IDS_ADDRECORD));//
		//if(thePrefs.prefReadonly)// X: [ROP] - [ReadOnlyPreference]
		//	title.AppendFormat(_T(" [%s]"), GetResString(IDS_READONLY));

	}
	else if ( ECOO_MOD == m_nCurOperOPtion )
	{
		title = RemoveAmbersand(GetResString(IDS_MODRECORD));//
	}
	SetWindowText(title); 

	m_pRecDetClaDlg->Localize();
	m_pRecDetContDlg->Localize();
	m_pRecDetAttachDlg->Localize();
}
void CRecordDetails::SetCurOperOption(int nOpt)
{
	m_nCurOperOPtion = nOpt;
}
void CRecordDetails::ReallySave_Attachs(int nOperOption)
{
	//重新获取所有附件占用空间大小前，先把原来的值清空
	m_pRI->AttachSpaceSize = 0;
	m_pRecDetAttachDlg->ReallySave( nOperOption, m_pRI->AttachSpaceSize);
}
void CRecordDetails::ReallyCancel_Attach()
{
	m_pRecDetAttachDlg->ReallyCancel();
}
void CRecordDetails::InitCurRecordInfo(CRecordItem* pRI)
{
	m_pRI = pRI;
	m_pRecDetContDlg->InitCurRecordInfo(m_pRI);
	m_pRecDetAttachDlg->InitCurRecordInfo(m_pRI->ID, m_pRI->MainClassID);
}
int CRecordDetails::GetCurOperOption()
{
	return m_nCurOperOPtion;
}
void CRecordDetails::SetActiveDialog(CWnd* dlg)
{
	if (dlg == activewnd)
		return;
	if (activewnd)
		activewnd->ShowWindow(SW_HIDE);
	dlg->ShowWindow(SW_SHOW);
	dlg->SetFocus();
	activewnd = dlg;
	//int iToolbarButtonID = MapWindowToToolbarButton(dlg);
	//if (iToolbarButtonID != -1)
	//	toolbar->PressMuleButton(iToolbarButtonID);
	//if (dlg == groupresmanagewnd)
	//{
	//	/*if (thePrefs.ShowCatTabInfos())
	//		transferwnd->UpdateCatTabTitles();*/
	//}
	//else if (dlg == searchwnd)
	//{
	//	//if(transferwnd->GetDownloadList()->curTab != 0)
	//	//	searchwnd->SetSelectedCat(transferwnd->GetDownloadList()->curTab);// X: [UIC] - [UIChange] follow transfer tab
	//}
	//else if (dlg == chatwnd)
	//{
	///*	chatwnd->chatselector.ShowChat();*/
	//}
	//else if (dlg == statisticswnd)
	//{
	//	//statisticswnd->ShowStatistics();
	//}
}
void CRecordDetails::InitOperItems()
{
	//CRect rect;
	//( GetDlgItem( IDC_STATIC_FRAME_INFO ) )->GetWindowRect( & rect );
	//ScreenToClient( & rect );

	//m_pRecDetClaDlg = NULL;
	//m_pRecDetClaDlg = new CRecordDetailsClassDlg();
	//ASSERT_VALID( m_pRecDetClaDlg );
	//m_pRecDetClaDlg->Create( CRecordDetailsClassDlg::IDD, this );
	//m_pRecDetClaDlg->MoveWindow( & rect );
	//m_pRecDetClaDlg->ShowWindow( TRUE );

	//m_pTreeDlg = NULL;
	//m_pTreeDlg = new CTreeDlg();
	//ASSERT_VALID( m_pTreeDlg );
	//m_pTreeDlg->Create( CTreeDlg::IDD, this );
	//m_pTreeDlg->MoveWindow( & rect );
	//m_pTreeDlg->ShowWindow( FALSE );
}
void CRecordDetails::InitListCtrl( CListCtrl& listCtrl )
{
	DWORD dwExStyle = 0;
	dwExStyle |= LVS_EX_GRIDLINES     | \
		LVS_EX_FLATSB        | \
		LVS_EX_TRACKSELECT;
	listCtrl.SetExtendedStyle( dwExStyle );

	m_ImageList.Create( 16, 16, ILC_COLOR32 | ILC_MASK, 6, 6 );

	m_ImageList.Add( AfxGetApp()->LoadIcon(_T("RECORDCLASSINFOS") ) );
	m_ImageList.Add( AfxGetApp()->LoadIcon( _T("RECORDCONTENTINFOS") ) );
	m_ImageList.Add( AfxGetApp()->LoadIcon( _T("RECORDATTACHINFOS") ) );
	//m_ImageList.Add( AfxGetApp()->LoadIcon( IDR_MAINFRAME ) );
	//m_ImageList.Add( AfxGetApp()->LoadIcon( IDR_MAINFRAME ) );
	//m_ImageList.Add( AfxGetApp()->LoadIcon( IDR_MAINFRAME ) );


	listCtrl.SetImageList( &m_ImageList, LVSIL_SMALL );

	int	hItem = listCtrl.InsertItem( 0, GetResString(IDS_RECORD_OPERTYPE_CLASS), 0 );
	listCtrl.SetItemData(hItem,(DWORD_PTR)ECSO_CLASS);

	hItem = listCtrl.InsertItem( 1,GetResString(IDS_RECORD_OPERTYPE_CONTENT), 1 );
	listCtrl.SetItemData(hItem,(DWORD_PTR)ECSO_CONTENT);
	
	hItem = listCtrl.InsertItem( 2,GetResString(IDS_RECORD_OPERTYPE_ATTACH), 2 );
	listCtrl.SetItemData(hItem,(DWORD_PTR)ECSO_ATTACH);
}

void CRecordDetails::OnNMClickListOperOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listOperOption.GetFirstSelectedItemPosition();
	if ( ! pos )
		return;

	int nItem = m_listOperOption.GetNextSelectedItem( pos );
	ASSERT( nItem >= 0 );

	int nCurSel = (int)m_listOperOption.GetItemData(nItem);
	switch( nCurSel )
	{
	case ECSO_CLASS:
		{
			SetActiveDialog(m_pRecDetClaDlg);			
		}		
		break;
	case ECSO_CONTENT:
		{
			//MessageBox(L"内容");			
			if ( m_pRecDetClaDlg->GetSelectedClasses(&m_pRI->nAllClassID,m_pRI->MainClassID) )
			{
				m_pRecDetContDlg->ShowSelectedClasses(m_pRI->nAllClassID,m_pRI->MainClassID);
				SetActiveDialog(m_pRecDetContDlg);
			}
		
				
		}
		break;
	case ECSO_ATTACH:
		{
			if ( m_pRecDetClaDlg->GetSelectedClasses(&m_pRI->nAllClassID,m_pRI->MainClassID) )
			{
				m_pRecDetAttachDlg->InitCurRecordInfo(m_pRI->ID, m_pRI->MainClassID);
				SetActiveDialog(m_pRecDetAttachDlg);	
			}
			
		}		
		break;
	default:
		break;
	}

	//String strItemText = m_listOperOption.GetItemText( nItem, 0 );
	//ASSERT( ! strItemText.IsEmpty() );
	//if  ( strItemText == L"ClassInfos" ) 
	//{
	//	SetActiveDialog(m_pRecDetClaDlg);
		//ASSERT_VALID( m_pRecDetClaDlg );
		////ASSERT_VALID( m_pTreeDlg );
		//
		//m_pRecDetClaDlg->ShowWindow( TRUE  );
		////m_pTreeDlg->ShowWindow(FALSE);

	//}
	*pResult = 0;
}

void CRecordDetails::ReleaseOperItems()
{
	//if ( m_pRecDetClaDlg != NULL )
	//{
	//	m_pRecDetClaDlg->DestroyWindow();
	//	delete m_pRecDetClaDlg;
	//	m_pRecDetClaDlg = NULL;
	//}
	//ASSERT( m_pRecDetClaDlg == NULL );

	//if ( m_pMsChartDlg != NULL )
	//{
	//	m_pMsChartDlg->DestroyWindow();
	//	delete m_pMsChartDlg;
	//	m_pMsChartDlg = NULL;
	//}
	//ASSERT( m_pMsChartDlg == NULL );

}

void CRecordDetails::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//m_pRecDetClaDlg->OnSize(nType, cx, cy);
}


LRESULT CRecordDetails::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//switch (message)
	//{
	//case WM_SIZE:
	//	if (m_pRecDetClaDlg != NULL )
	//	{
	//		CRect rcWnd;
	//		GetWindowRect(rcWnd);
	//		ScreenToClient(rcWnd);

	//		//CRect rect;
	//		//( GetDlgItem( IDC_STATIC_FRAME_INFO ) )->GetWindowRect( & rect );
	//		//ScreenToClient( & rect );
	//		//m_pRecDetClaDlg->PostMessage(WM_SIZE,wParam, lParam);
	//		//m_pRecDetClaDlg->SetRange(rcWnd.left + 1, rcWnd.left + 1);
	//		//m_wndSplitter.SetRange(rcWnd.left + SPLITTER_RANGE_MIN, rcWnd.left + SPLITTER_RANGE_MAX);
	//	}
	//	break;
	//}
	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}


BOOL CRecordDetails::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if (wParam == ID_HELP)
	//{
	//	OnHelp();
	//	return TRUE;
	//}
	if (wParam == IDOK )
	{
		if ( !m_pRecDetClaDlg->GetSelectedClasses(&m_pRI->nAllClassID,m_pRI->MainClassID) )
			return FALSE;
		m_pRecDetContDlg->GetRecordInfo();
		if ( ERROR_CLASSID == m_pRI->MainClassID )
		{
			MessageBox(GetResString(IDS_PROP_INFO_FIRST_SEL_MAINCLASS));//L"必需选择一个所属主分类"
			return FALSE;
		}
		if ( m_pRI->Titlel.empty() )
		{
			MessageBox(GetResString(IDS_PROP_INFO_TITLE_NULL));//L"标题不能为空"
			return FALSE;
		}
		int nContentLength = m_pRI->Content.length();
		if ( nContentLength > ContentMaxLength )
		{
			MessageBox(GetResString(IDS_PROP_INFO_CONTENTTOOLENGTH));
			return FALSE;
		}
		
	}
	return CResizableDialog::OnCommand(wParam, lParam);
	
}


BOOL CRecordDetails::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(WM_KEYDOWN == pMsg->message ) 
	{ 
		UINT nKey = (int) pMsg->wParam; 
		if( VK_RETURN == nKey || VK_ESCAPE == nKey ) 
			return TRUE ; 
	}
	return CResizableDialog::PreTranslateMessage(pMsg);
}
