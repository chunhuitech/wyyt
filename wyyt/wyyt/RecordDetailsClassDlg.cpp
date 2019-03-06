// RecordDetailsClassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wyyt.h"
#include "RecordDetailsClassDlg.h"
#include "afxdialogex.h"
#include "OtherFunctions.h"

#define	STATUS_MSG_COLOR		RGB(0,128,0)		// dark green
#define	INFO_MSG_COLOR			RGB(192,0,0)		// mid red
#define	SENT_TARGET_MSG_COLOR	RGB(0,192,0)		// bright green
#define	RECV_SOURCE_MSG_COLOR	RGB(0,128,255)		// bright cyan/blue

// CRecordDetailsClassDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDetailsClassDlg, CDialog)

CRecordDetailsClassDlg::CRecordDetailsClassDlg(CWnd* pParent /*=NULL*/,EModuleType emt/* = EMT_SYS*/)
	: CResizableDialog(CRecordDetailsClassDlg::IDD, pParent),m_emt(emt)
{
	
}

CRecordDetailsClassDlg::~CRecordDetailsClassDlg()
{
}

void CRecordDetailsClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CLASSES, m_ClassSelector);
	
}


BEGIN_MESSAGE_MAP(CRecordDetailsClassDlg, CResizableDialog)
END_MESSAGE_MAP()


// CRecordDetailsClassDlg 消息处理程序


BOOL CRecordDetailsClassDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	InitWindowStyles(this);
	// TODO:  在此添加额外的初始化
	AddAnchor(IDC_TREE_CLASSES, TOP_LEFT,BOTTOM_RIGHT);
	

	//m_RECSelClaInfo.SetWindowText(L"fdfdfdfdfd");
	
	//LoadSelectorClasses();
	Localize();


	m_ClassSelector.InitDataSoruce(m_emt);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRecordDetailsClassDlg::Localize()
{
	SetDlgItemText(IDC_STATIC_PROPINFO,GetResString(IDS_PROP_SELBYCTL));
}
//void CRecordDetailsClassDlg::InitClassInfos(void)
//{
//	m_vecSelClass
//}

void CRecordDetailsClassDlg::LoadSelectorClasses(vector<int64>* vecSelClass, int64 nMainClassID)
{
	//SetDlgItemText(IDC_INCFILES,thePrefs.m_strIncomingDir);	
	m_ClassSelector.SetSelectedClasses(vecSelClass, nMainClassID);

}
bool CRecordDetailsClassDlg::GetSelectedClasses(vector<int64>* vecSelClass,int64& nMainClassID)
{
	return m_ClassSelector.GetSelectedClasses(vecSelClass,nMainClassID);
}


BOOL CRecordDetailsClassDlg::OnApply()
{
	return FALSE;
}

BOOL CRecordDetailsClassDlg::PreTranslateMessage(MSG* pMsg)
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
