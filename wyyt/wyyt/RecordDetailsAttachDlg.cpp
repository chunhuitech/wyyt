// RecordDetailsAttachDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecordDetailsAttachDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"
#include "Preferences.h"
#include "AttachAddDlg.h"
#include "TitleMenu.h"
#include "MenuCmds.h"
// CRecordDetailsAttachDlg �Ի���

class IsDelRecordAttach
{
public:
	IsDelRecordAttach( int64& nRecordID , int nIndexID) : m_nRecordID(nRecordID),m_nIndexID(nIndexID)
	{
	}
	bool operator()(CRecordAttachs& cra)
	{
		return (cra.RecordID == m_nRecordID && cra.IndexID == m_nIndexID);
	}
private:
	int64 m_nRecordID;
	int m_nIndexID;
};

IMPLEMENT_DYNAMIC(CRecordDetailsAttachDlg, CDialog)

CRecordDetailsAttachDlg::CRecordDetailsAttachDlg(CWnd* pParent /*=NULL*/,EModuleType emt/* = EMT_SYS*/)
	: CResizableDialog(CRecordDetailsAttachDlg::IDD, pParent),m_emt(emt)
{
	m_nCurRecordID = -1;
	m_nExistAttCount = 0;
	m_nCurAttCount = 1;
}

CRecordDetailsAttachDlg::~CRecordDetailsAttachDlg()
{
	m_vecModOldFiles.clear();
}

void CRecordDetailsAttachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATTACHS, m_listAttachs);
}


BEGIN_MESSAGE_MAP(CRecordDetailsAttachDlg, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRecordDetailsAttachDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CRecordDetailsAttachDlg::OnBnClickedButtonDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ATTACHS, &CRecordDetailsAttachDlg::OnNMClickListAttachs)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ATTACHS, &CRecordDetailsAttachDlg::OnNMRClickListAttachs)
END_MESSAGE_MAP()


// CRecordDetailsAttachDlg ��Ϣ�������
void CRecordDetailsAttachDlg::Localize()
{
	SetDlgItemText(IDC_BUTTON_ADD,GetResString(IDS_ADD));
	SetDlgItemText(IDC_BUTTON_DEL,GetResString(IDS_DEL));

	SetDlgItemText(IDC_STATIC_FULLNAME_UP,GetResString(IDS_STATIC_FULLNAME_UP));
	SetDlgItemText(IDC_STATIC_FULLNAME_SAVE,GetResString(IDS_STATIC_FULLNAME_SAVE));
	SetDlgItemText(IDC_STATIC_DOWNADDRESS,GetResString(IDS_STATIC_DOWNADDRESS));
	SetDlgItemText(IDC_STATIC_CODEID,GetResString(IDS_STATIC_CODEID));
	

	HDITEM hdi;// X: [AL] - [Additional Localize]
	hdi.mask = HDI_TEXT;
	CString strRes;
	strRes=GetResString(IDS_CODE);//���
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(0, &hdi);

	strRes=GetResString(IDS_FILENAME);//�ļ���
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(1, &hdi);

	strRes=GetResString(IDS_FILESIZE);//�ļ���С
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(2, &hdi);

	strRes=GetResString(IDS_FILESTATUS);///�ļ�״̬
	hdi.pszText = const_cast<LPTSTR>((LPCTSTR)strRes);
	m_listAttachs.GetHeaderCtrl()->SetItem(3, &hdi);

}

void CRecordDetailsAttachDlg::FillAttachsList() 
{

	m_listAttachs.DeleteAllItems();
	
	size_t index=0;
	for ( vector<CRecordAttachs>::iterator it = m_vecTempCurAttachs.begin(); it != m_vecTempCurAttachs.end(); it++, index++ )
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
	/*map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(m_nCurRecordID);
	if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
	{
		size_t index=0;
		for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++, index++ )
		{
			m_listAttachs.InsertItem(index , it->CodeID.c_str() );
			m_listAttachs.SetItemText(index, 1, it->FileName.c_str());
			m_listAttachs.SetItemText(index, 2, CastItoXBytes((uint32)it->FileSize, false, false));
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
	*/
}

BOOL CRecordDetailsAttachDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	InitWindowStyles(this);
	m_listAttachs.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	ASSERT( (m_listAttachs.GetStyle() & LVS_SINGLESEL) == 0);
	m_listAttachs.InsertColumn(0, _T("")/*GetResString(IDS_TITLE)*/, LVCFMT_LEFT, 150);//���
	m_listAttachs.InsertColumn(1, _T("")/*GetResString(IDS_S_DAYS)*/, LVCFMT_LEFT, 260);//�ļ���
	m_listAttachs.InsertColumn(2, _T("")/*GetResString(IDS_STARTTIME)*/, LVCFMT_LEFT, 80);//�ļ���С
	m_listAttachs.InsertColumn(3, _T("")/*GetResString(IDS_STARTTIME)*/, LVCFMT_LEFT, 80);//�ļ�״̬

	AddAnchor(IDC_LIST_ATTACHS, TOP_LEFT,BOTTOM_RIGHT);

	Localize();
	FillAttachsList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int CRecordDetailsAttachDlg::GetMaxIndexID()
{
	return m_nCurAttCount++;
	/*if ( 1 == m_nExistAttCount && 1 == m_nCurAttCount )
	{
		return m_nCurAttCount;
	}*/
	
	
	/*int nMaxIndex = 0;	
	size_t index=0;
	for ( vector<CRecordAttachs>::iterator it = m_vecTempCurAttachs.begin(); it != m_vecTempCurAttachs.end(); it++, index++ )
	{
	if ( it->IndexID > nMaxIndex )
	{
	nMaxIndex = it->IndexID;
	}
	}	
	if ( nMaxIndex <= 0 )
	{
	nMaxIndex = 1;
	}
	else
	{
	nMaxIndex += 1;
	}
	return nMaxIndex;*/
}
void CRecordDetailsAttachDlg::ReallyCancel()
{
	//���������ȡ���������ԭ����������Ϊɾ����־���ļ��һ���
	if ( m_vecModOldFiles.size() > 0 )
	{
		for (vector<CString>::iterator it = m_vecModOldFiles.begin(); it != m_vecModOldFiles.end(); it++ )
		{
			CString strOldFile = (*it);
			CString strNewFile;
			int    nPos;   
			nPos=strOldFile.ReverseFind('_');   
			strNewFile=strOldFile.Left(nPos);   
			if ( EMT_SYS == m_emt )
			{
				wyytExtendDataManage::Instance().MoveSysRecordAttachFile(strOldFile,strNewFile);
			}
			else
			{
				wyytExtendDataManagePri::Instance().MoveSysRecordAttachFile(strOldFile,strNewFile);
			}
		}
	}
	//�Զ��ϴ����ļ�����ɾ��
	if ( m_vecMulUpFiles.size() > 0 )
	{
		for (vector<CString>::iterator it = m_vecMulUpFiles.begin(); it != m_vecMulUpFiles.end(); it++ )
		{
			if ( EMT_SYS == m_emt )
			{
				wyytExtendDataManage::Instance().DelSysRecordAttachFile(*it);
			}
			else
			{
				wyytExtendDataManagePri::Instance().DelSysRecordAttachFile(*it);
			}
			
		}
	}
}
void CRecordDetailsAttachDlg::ReallySave(int nOperOption, uint64& getFileAllSize)
{
	//if ( ECOO_ADD == nOperOption )
	//{
	//}
	if ( EMT_SYS == m_emt )
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(m_nCurRecordID);
		if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
		{
			//ͳһ������ݿ���ڴ��еĶ�Ӧ�ļ�¼
			wyytExtendDataManage::Instance().DelSysRecordAttachInfo(m_nCurRecordID);
			//Iter->second.clear();		

			//Iter->second = m_vecTempCurAttachs;
			//size_t index=0;
			//for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++, index++ )
			//{
			//	if ( it->IndexID > nMaxIndex )
			//	{
			//		nMaxIndex = it->IndexID;
			//	}
			//}
		}
		//else //���Ҳ����ʹ�������� ��ֱ�ӡ�ִ���������Ӳ���
		//{
		//}
		//����ͳһ��ӵ����ݿ���ڴ�
		for ( vector<CRecordAttachs>::iterator it = m_vecTempCurAttachs.begin(); it != m_vecTempCurAttachs.end(); it++ )
		{
			getFileAllSize += it->FileSize;
			wyytExtendDataManage::Instance().InsertSysRecordAttachInfo(*it);
		}

		//���������ȷ�ϲ������ԭ����������Ϊɾ����ǵ��ļ�����ɾ��
		if ( m_vecModOldFiles.size() > 0 )
		{
			for (vector<CString>::iterator it = m_vecModOldFiles.begin(); it != m_vecModOldFiles.end(); it++ )
			{ 
				wyytExtendDataManage::Instance().DelSysRecordAttachFile(*it);
			}
		}
	}
	else
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManagePri::Instance().m_mapRec2Attachs.find(m_nCurRecordID);
		if (Iter != wyytExtendDataManagePri::Instance().m_mapRec2Attachs.end() )
		{
			//ͳһ������ݿ���ڴ��еĶ�Ӧ�ļ�¼
			wyytExtendDataManagePri::Instance().DelSysRecordAttachInfo(m_nCurRecordID);
		}
		//����ͳһ��ӵ����ݿ���ڴ�
		for ( vector<CRecordAttachs>::iterator it = m_vecTempCurAttachs.begin(); it != m_vecTempCurAttachs.end(); it++ )
		{
			getFileAllSize += it->FileSize;
			wyytExtendDataManagePri::Instance().InsertSysRecordAttachInfo(*it);
		}

		//���������ȷ�ϲ������ԭ����������Ϊɾ����ǵ��ļ�����ɾ��
		if ( m_vecModOldFiles.size() > 0 )
		{
			for (vector<CString>::iterator it = m_vecModOldFiles.begin(); it != m_vecModOldFiles.end(); it++ )
			{ 
				wyytExtendDataManagePri::Instance().DelSysRecordAttachFile(*it);
			}
		}
	}
	
	
}
void CRecordDetailsAttachDlg::OnBnClickedButtonAdd()
{
	CRecordAttachs AttAdd;
	AttAdd.RecordID = m_nCurRecordID;
	AttAdd.IndexID = GetMaxIndexID();
	if ( m_nCurAttCount >= 999 )
	{
		MessageBox(GetResString(IDS_ALARM_MAXATTACH));
		return;
	}
	//AttAdd.PathIndex = 0;
	CSID Codeid;
	if ( EMT_SYS == m_emt )
	{
		Codeid.Init(E_SMT_SYS, m_nMainClassID, m_nCurRecordID, AttAdd.IndexID);
	}
	else
	{
        Codeid.Init(E_SMT_PRIVATE, m_nMainClassID, m_nCurRecordID, AttAdd.IndexID);
	}
	
	AttAdd.CodeID = Codeid.FiniToString();
	CAttachAddDlg dialog;
	dialog.SetRecAttInitInfo(&AttAdd);
	if (dialog.DoModal() == IDOK)
	{
		CString csFileFullNameUp,csFileFullNameSave;
		if ( EMT_SYS == m_emt )
		{
			csFileFullNameUp.Format(L"%s\\%s%s",AttAdd.SrcPath.c_str(),AttAdd.FileName.c_str(),AttAdd.FileSuffix.c_str());
			csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,AttAdd.CodeID.c_str(),AttAdd.FileSuffix.c_str());		
			if ( wyytExtendDataManage::Instance().AddSysRecordAttachFile(csFileFullNameUp,csFileFullNameSave) )
			{
				m_vecTempCurAttachs.push_back(AttAdd);
				if ( 0 != m_nExistAttCount && AttAdd.IndexID > m_nExistAttCount ) 
				{
					m_vecMulUpFiles.push_back(csFileFullNameSave);
				}
				FillAttachsList();
			}
		}
		else
		{
			csFileFullNameUp.Format(L"%s\\%s%s",AttAdd.SrcPath.c_str(),AttAdd.FileName.c_str(),AttAdd.FileSuffix.c_str());
			csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,AttAdd.CodeID.c_str(),AttAdd.FileSuffix.c_str());		
			if ( wyytExtendDataManagePri::Instance().AddSysRecordAttachFile(csFileFullNameUp,csFileFullNameSave) )
			{
				m_vecTempCurAttachs.push_back(AttAdd);
				if ( 0 != m_nExistAttCount && AttAdd.IndexID > m_nExistAttCount ) 
				{
					m_vecMulUpFiles.push_back(csFileFullNameSave);
				}
				FillAttachsList();
			}
		}
		

	}
}


void CRecordDetailsAttachDlg::OnBnClickedButtonDel()
{
	int index=m_listAttachs.GetSelectionMark();

	if (index!=-1) 
	{
		if (index >= m_vecTempCurAttachs.size()) 
			return;
		bool bUpdate = false;
		CRecordAttachs* todel = (CRecordAttachs*)m_listAttachs.GetItemData(index);
		CString csFileFullNameSave;
		
		if ( EMT_SYS == m_emt )
		{
			csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,todel->CodeID.c_str(),todel->FileSuffix.c_str());
			//��ԭ���Ѿ���ŵĸ����Ĳ���
			//������������ɾ����ֻ�ǽ���������������׺����"_del"�����û���ȡ��ȡ����ťʱ���ٻָ�ԭ��������
			if ( 0 != m_nExistAttCount && todel->IndexID <= m_nExistAttCount ) 
			{
				CString strNewName;
				strNewName = csFileFullNameSave + L"_del";
				if ( wyytExtendDataManage::Instance().MoveSysRecordAttachFile(csFileFullNameSave,strNewName) )
				{
					m_vecModOldFiles.push_back(strNewName);
					bUpdate = true;
				}

			}
			else
			{
				if (  wyytExtendDataManage::Instance().DelSysRecordAttachFile(csFileFullNameSave))
				{	
					bUpdate = true;
				}
			}
		}
		else
		{
			csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,todel->CodeID.c_str(),todel->FileSuffix.c_str());
			//��ԭ���Ѿ���ŵĸ����Ĳ���
			//������������ɾ����ֻ�ǽ���������������׺����"_del"�����û���ȡ��ȡ����ťʱ���ٻָ�ԭ��������
			if ( 0 != m_nExistAttCount && todel->IndexID <= m_nExistAttCount ) 
			{
				CString strNewName;
				strNewName = csFileFullNameSave + L"_del";
				if ( wyytExtendDataManagePri::Instance().MoveSysRecordAttachFile(csFileFullNameSave,strNewName) )
				{
					//������û��ɾ���ɹ���Ҫ�����ݿ�ɾ��
				}
				m_vecModOldFiles.push_back(strNewName);
				bUpdate = true;

			}
			else
			{
				if (  wyytExtendDataManagePri::Instance().DelSysRecordAttachFile(csFileFullNameSave))
				{	
					bUpdate = true;
				}
			}
		}

		if ( bUpdate )
		{	
			vector<CRecordAttachs>::iterator it = find_if( m_vecTempCurAttachs.begin(), m_vecTempCurAttachs.end(), IsDelRecordAttach(todel->RecordID,todel->IndexID) );
			if (it != m_vecTempCurAttachs.end())
			{
				m_vecTempCurAttachs.erase(it);
			}
			FillAttachsList();
		}
		
	}
	
}

void CRecordDetailsAttachDlg::LoadAttach(size_t index) 
{
	CRecordAttachs * pCurAtt = (CRecordAttachs*)m_listAttachs.GetItemData(index);
	CString csFileFullNameUp,csFileFullNameSave;
	csFileFullNameUp.Format(L"%s\\%s%s",pCurAtt->SrcPath.c_str(),pCurAtt->FileName.c_str(),pCurAtt->FileSuffix.c_str());
	if ( EMT_SYS == m_emt )
	{
		csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strSysAttachPath,pCurAtt->CodeID.c_str(),pCurAtt->FileSuffix.c_str());
	}
	else
	{
		csFileFullNameSave.Format(L"%s\\%s%s",thePrefs.m_strPriAttachPath,pCurAtt->CodeID.c_str(),pCurAtt->FileSuffix.c_str());
	}
	
	SetDlgItemText(IDC_EDIT_CODEID,pCurAtt->CodeID.c_str());
	SetDlgItemText(IDC_EDIT_FULLNAME_UP,csFileFullNameUp);
	SetDlgItemText(IDC_EDIT_FULLNAME_SAVE,csFileFullNameSave);
	SetDlgItemText(IDC_EDIT_DOWNADDRESS,pCurAtt->DownloadAddress.c_str());
	
	//Schedule_Struct* schedule=theApp.scheduler->GetSchedule(index);
	//SetDlgItemText(IDC_S_TITLE,schedule->title);

	////time
	//CTime time=time.GetCurrentTime();
	//if (schedule->time>0) time=schedule->time;
	//m_time.SetTime(&time);

	//CTime time2=time2.GetCurrentTime();
	//if (schedule->time2>0) time2=schedule->time2;
	//m_timeTo.SetTime(&time2);

	////time kindof (days)
	//m_timesel.SetCurSel(schedule->day);

	//CheckDlgButton(IDC_S_ENABLE,(schedule->enabled));
	//CheckDlgButton(IDC_CHECKNOENDTIME, schedule->time2==0);

	//OnDisableTime2();

	//m_actions.DeleteAllItems();
	//for (int i=0;i<16;i++) {
	//	if (schedule->actions[i]==0) break;
	//	m_actions.InsertItem(i,GetActionLabel(schedule->actions[i]));
	//	m_actions.SetItemText(i,1,schedule->values[i]);
	//	m_actions.SetItemData(i,schedule->actions[i]);
	//}
}


BOOL CRecordDetailsAttachDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(WM_KEYDOWN == pMsg->message ) 
	{ 
		UINT nKey = (int) pMsg->wParam; 
		if( VK_RETURN == nKey || VK_ESCAPE == nKey ) 
			return TRUE ; 
	}
	return CResizableDialog::PreTranslateMessage(pMsg);
}


void CRecordDetailsAttachDlg::OnNMClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_listAttachs.GetSelectionMark()>-1) 
		LoadAttach(m_listAttachs.GetSelectionMark());
	*pResult = 0;
}

void CRecordDetailsAttachDlg::InitCurRecordInfo(int64 nCurRecordID, int64 nMainClassID)
{
	m_nCurRecordID = nCurRecordID;
	m_nMainClassID = nMainClassID;

	m_vecTempCurAttachs.clear();
	if ( EMT_SYS == m_emt )
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(m_nCurRecordID);
		if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
		{
			m_vecTempCurAttachs = Iter->second;
			for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++)
			{
				if ( it->IndexID > m_nExistAttCount )
				{
					m_nExistAttCount = it->IndexID;
				}
			}
			m_nCurAttCount = m_nExistAttCount == 0 ? 1 : m_nExistAttCount+1 ;
		}
	}
	else
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManagePri::Instance().m_mapRec2Attachs.find(m_nCurRecordID);
		if (Iter != wyytExtendDataManagePri::Instance().m_mapRec2Attachs.end() )
		{
			m_vecTempCurAttachs = Iter->second;
			for ( vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++)
			{
				if ( it->IndexID > m_nExistAttCount )
				{
					m_nExistAttCount = it->IndexID;
				}
			}
			m_nCurAttCount = m_nExistAttCount == 0 ? 1 : m_nExistAttCount+1 ;
		}
	}
	
	
}

void CRecordDetailsAttachDlg::OnNMRClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//*pResult = 0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POINT point;
	::GetCursorPos(&point);

	CTitleMenu m_AttachsMenu;
	m_AttachsMenu.CreatePopupMenu();
	
	UINT nFlag=MF_STRING;
	if (m_listAttachs.GetSelectionMark()==-1) 
		nFlag=MF_STRING | MF_GRAYED;



	m_AttachsMenu.AddMenuTitle(GetResString(IDS_ATTACH));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_OPEN,	GetResString(IDS_OPEN_FILE));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_SAVEAS,GetResString(IDS_FILE_SAVEAS));
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_SHOW,GetResString(IDS_ATTACH_SHOW));	
	m_AttachsMenu.AppendMenu(nFlag,MP_ATTACH_MOD,GetResString(IDS_FILE_MOD));
	m_AttachsMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	VERIFY( m_AttachsMenu.DestroyMenu() );

	*pResult = 0;
}


BOOL CRecordDetailsAttachDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
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
		case MP_ATTACH_MOD:
			{
				CAttachAddDlg dialog;
				dialog.SetRecAttInitInfo(attach,AOT_MOD);
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

	return CResizableDialog::OnCommand(wParam, lParam);
}
