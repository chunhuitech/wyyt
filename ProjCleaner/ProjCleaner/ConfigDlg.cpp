// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "ConfigDlg.h"


// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	m_pTreeListHead = NULL;
}

CConfigDlg::~CConfigDlg()
{
	//if ( m_pTreeListHead ) {
	//	FreeTreeList();
	//}
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH_TREE, m_PathTree);
	DDX_Control(pDX, IDC_TREE_SELECT, m_TreeSelect);
	DDX_Control(pDX, IDC_DEL_RECYCLE_CHECK, m_DelRecycleBtn);
	DDX_Control(pDX, IDC_END_CLOSE_CHECK, m_EndCloseBtn);
	DDX_Control(pDX, IDC_MIN_TRAY_CHECK, m_MinTrayBtn);
	DDX_Control(pDX, IDC_ASS_R_MENU_CHECK, m_AssRMenuBtn);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVE_LIST, &CConfigDlg::OnBnClickedSaveList)
	ON_BN_CLICKED(IDC_LOAD_LIST, &CConfigDlg::OnBnClickedLoadList)
	ON_BN_CLICKED(IDC_RES_DEFAULT, &CConfigDlg::OnBnClickedResDefault)
	ON_BN_CLICKED(IDC_DEL_ALL, &CConfigDlg::OnBnClickedDelAll)
	ON_BN_CLICKED(IDC_APPLY_BTN, &CConfigDlg::OnBnClickedApplyBtn)
	ON_BN_CLICKED(IDC_DEL_RECYCLE_CHECK, &CConfigDlg::OnBnClickedDelRecycleCheck)
	ON_BN_CLICKED(IDC_END_CLOSE_CHECK, &CConfigDlg::OnBnClickedEndCloseCheck)
	ON_BN_CLICKED(IDC_MIN_TRAY_CHECK, &CConfigDlg::OnBnClickedMinTrayCheck)
	ON_BN_CLICKED(IDC_SET_DEFAULT, &CConfigDlg::OnBnClickedSetDefault)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SMING_BTN, &CConfigDlg::OnBnClickedSmingBtn)
	ON_BN_CLICKED(IDC_MINZE_BTN, &CConfigDlg::OnBnClickedMinzeBtn)
END_MESSAGE_MAP()


// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//for the profile;
	CProfileInfo  m_Profile;
	GetRegProfile(m_Profile);
	(m_Profile.m_bDelToRecycle == TRUE) ? m_DelRecycleBtn.SetCheck(BST_CHECKED) : m_DelRecycleBtn.SetCheck(BST_UNCHECKED);
	(m_Profile.m_bEndClose == TRUE) ? m_EndCloseBtn.SetCheck(BST_CHECKED) : m_EndCloseBtn.SetCheck(BST_UNCHECKED);
	(m_Profile.m_bMinTray == TRUE) ? m_MinTrayBtn.SetCheck(BST_CHECKED) : m_MinTrayBtn.SetCheck(BST_UNCHECKED);
	(m_Profile.m_bAssRMenu == TRUE) ? m_AssRMenuBtn.SetCheck(BST_CHECKED) : m_AssRMenuBtn.SetCheck(BST_UNCHECKED);
	//end ;
	CBitmap m_Bmp;
	m_TreeImgList.Create(IDB_FOLDER_BMP, 16, 1, RGB(255, 255, 255));
	m_Bmp.LoadBitmap(IDB_FILE_BMP);
	m_TreeImgList.Add(&m_Bmp, RGB(255, 255, 255));
	// 0 - means close folder == FOLDER_CLOSE_IMAGE;
	// 1 - means open folder == FOLDER_OPEN_IMAGE;
	// 2 - means files == FILE_IMAGE;
	m_PathTree.SetImageList(&m_TreeImgList, TVSIL_NORMAL);
	m_PathTree.Initialize(TRUE);
	m_PathTree.SetHasCheckBoxes(TRUE);
	m_PathTree.SetSmartCheckBox(TRUE);
	m_PathTree.SetHtml(TRUE);
	m_PathTree.SetStripHtml(TRUE);
	m_PathTree.SetImages(TRUE);
	m_PathTree.SetParentWnd(this);
	//////////////////////////////////////////////////////////////
	BOOL bInitIsDefault = TRUE;
	if ( m_Profile.m_strInitTree.GetLength() > 0 && PathFileExists(m_Profile.m_strInitTree) ) {
		m_PathTree.LoadXmlFromFile(m_Profile.m_strInitTree, CXHtmlTree::NoConvertAction);
		bInitIsDefault = FALSE;
	}else{
		m_PathTree.LoadXmlFromResource(NULL, MAKEINTRESOURCE(RES_XML_DEFAULT), _T("RES_XML"), CXHtmlTree::NoConvertAction);
		bInitIsDefault = TRUE;
	}
	m_PathTree.Expand(m_PathTree.GetRootItem(), TVE_EXPAND);
	//////////////////////////////////////////////////////////////
	m_TreeSelect.InsertString(m_TreeSelect.GetCount(), _T("Ĭ��Ŀ¼���ṹ ..."));
	m_TreeSelect.InsertString(m_TreeSelect.GetCount(), _T("�����Զ����б� ..."));
	CStringArray arrString;
	RegReadWriteStrings(_T("TreesList"), arrString);
	for( INT_PTR idx = 0; idx < arrString.GetCount(); idx++ ) {
		m_TreeSelect.InsertString(m_TreeSelect.GetCount()-1, arrString.GetAt(idx));
	}
	if ( bInitIsDefault ) {
		m_TreeSelect.SetCurSel(0);
	}else{
		int iFind = m_TreeSelect.FindString(-1, m_Profile.m_strInitTree);
		if ( iFind != -1 ) {
			m_TreeSelect.SetCurSel(iFind);
		}else{
			m_TreeSelect.SetCurSel(m_TreeSelect.InsertString(m_TreeSelect.GetCount()-1, m_Profile.m_strInitTree));
		}
	}
	m_TreeSelect.SetParentWnd(this);
	::EnableWindow(GetDlgItem(IDC_APPLY_BTN)->m_hWnd, FALSE);
	return TRUE;
}

BOOL CConfigDlg::GetRegProfile(CProfileInfo &m_Profile)
{
	m_Profile.m_bDelToRecycle = RegReadWriteDword(_T("DeleteToRecycle"), TRUE);
	m_Profile.m_bEndClose = RegReadWriteDword(_T("EndClose"), TRUE);
	m_Profile.m_bMinTray = RegReadWriteDword(_T("MiniTray"), TRUE);
	m_Profile.m_bAssRMenu = RegReadWriteDword(_T("AssRMenu"), TRUE);
	m_Profile.m_bSavePathList = RegReadWriteDword(_T("SavePathList"), TRUE);
	RegReadWriteStrings(_T("CurrentTreeFile"), m_Profile.m_strCurrTree, TRUE);
	RegReadWriteStrings(_T("InitStartTree"), m_Profile.m_strInitTree, TRUE);
	return TRUE;
}

/*
CTreeItem *CConfigDlg::GetDefaultTreeList()
{
	CTreeItem *m_pTreeRoot = new CTreeItem;
	if ( m_pTreeListHead )
		FreeTreeList();
	m_pTreeListHead = m_pTreeRoot;
	_tcscpy_s(m_pTreeRoot->m_szName, _MAX_FNAME, _T("Project"));
	m_pTreeRoot->m_iState = ITEM_STATE_MIDSELECTED;
	
	TCHAR *szRootChilds[] = {_T("Debug\\>"), _T("Release\\?"), _T("debug\\>"), _T("release\\?"), _T("res\\<"),
		_T("*.aps>"), _T("*.clw>"), _T("*.ncb>"), _T("*.opt>"), _T("*.plg>"), _T("*.positions>"), _T("*.suo>"),
		_T("*.user>"), _T("*.c<"), _T("*.cpp<"), _T("*.h<"), _T("*.rc<"), _T("*.vcproj<"), _T("*.dsp<"), _T("*.dsw<"),
		_T("*.sln<"), _T("*.txt<")};
	FillParentChildItems(m_pTreeRoot, szRootChilds, sizeof(szRootChilds)/sizeof(TCHAR*));
	TCHAR *szDebugChilds[] = {_T("*.dep>"), _T("*.exe>"), _T("*.htm>"), _T("*.idb>"), _T("*.ilk>"), _T("*.manifest>"), _T("*.obj>"), _T("*.pch>"), _T("*.pdb>"), _T("*.res>")};
	TCHAR *szReleaseChilds[] = {_T("*.dep>"), _T("*.exe<"), _T("*.htm>"), _T("*.idb>"), _T("*.manifest>"), _T("*.obj>"), _T("*.pch>"), _T("*.pdb>"), _T("*.res>"), _T("*.sbr>")};
	TCHAR *szdebugChilds[] = {_T("*.exe>"), _T("*.ilk>"), _T("*.pdb>")};
	TCHAR *szreleaseChilds[] = {_T("*.exe<"), _T("*.pdb>")};
	TCHAR *szresChilds[] = {_T("*.ico<"), _T("*.rc2<"), _T("*.bmp<")};
	CTreeItem *pItem = m_pTreeRoot->m_pChild;
	while( pItem ) {
		if ( _tcscmp(pItem->m_szName, _T("Debug")) == 0 ) {
			FillParentChildItems(pItem, szDebugChilds, sizeof(szDebugChilds)/sizeof(TCHAR*));
		}else if (_tcscmp(pItem->m_szName, _T("Release")) == 0) {
			FillParentChildItems(pItem, szReleaseChilds, sizeof(szReleaseChilds)/sizeof(TCHAR*));
		}else if (_tcscmp(pItem->m_szName, _T("debug")) == 0) {
			FillParentChildItems(pItem, szdebugChilds, sizeof(szdebugChilds)/sizeof(TCHAR*));
		}else if (_tcscmp(pItem->m_szName, _T("release")) == 0) {
			FillParentChildItems(pItem, szreleaseChilds, sizeof(szreleaseChilds)/sizeof(TCHAR*));
		}else if (_tcscmp(pItem->m_szName, _T("res")) == 0) {
			FillParentChildItems(pItem, szresChilds, sizeof(szresChilds)/sizeof(TCHAR*));
		}
		pItem = pItem->m_pNext;
	}

	return m_pTreeRoot;
}

BOOL CConfigDlg::FillParentChildItems(CTreeItem *in_pParent, TCHAR *szString[], size_t tLen)
{
	if ( !in_pParent || !szString || tLen <= 0 ) {
		return FALSE;
	}
	CString strT;
	CTreeItem *pLast = NULL, *pNext = NULL;
	for( size_t idx = 0; idx < tLen; idx++ ) {
		pNext = new CTreeItem;
		if ( !in_pParent->m_pChild ) {
			in_pParent->m_pChild = pNext;
		}
		strT.Format(_T("%s"), szString[idx]);
		if ( strT[strT.GetLength()-2] != '\\' ) {
			pNext->m_bFolder  = FALSE;
		}
		switch (strT[strT.GetLength()-1])
		{
		case '?':
			pNext->m_iState = ITEM_STATE_MIDSELECTED;
			break;
		default:
		case '>':
			pNext->m_iState = ITEM_STATE_SELECTED;
			break;
		case '<':
			pNext->m_iState = ITEM_STATE_UNSELECTED;
			break;
		}
		strT = strT.SpanExcluding(_T("?><\\"));
		_tcscpy_s(pNext->m_szName, _MAX_FNAME, strT);
		pNext->m_pParent = in_pParent;
		if (pLast) {
			pLast->m_pNext = pNext;
		}
		pLast = pNext;
	}
	return TRUE;
}

void CConfigDlg::FillTreeListWithItems(CTreeItem *in_Root, HTREEITEM in_Parent)
{
	ASSERT(in_Root != NULL && in_Root != NULL);
	if ( !in_Parent ) {
		m_PathTree.DeleteAllItems();
	}
	HTREEITEM hParent = NULL;
	CTreeItem *pCurr = in_Root, *pNext = NULL;
	while( pCurr ) {
		in_Parent = (in_Parent == NULL) ? TVI_ROOT : in_Parent;
		hParent = (pCurr->m_bFolder == TRUE) ? m_PathTree.InsertItem(pCurr->m_szName, 0, 1, in_Parent) : m_PathTree.InsertItem(pCurr->m_szName, 2, 2, in_Parent);
		pCurr->m_hTreeItem = hParent;
		if ( pCurr->m_pChild ) {
			FillTreeListWithItems(pCurr->m_pChild, hParent);
		}
		pCurr = pCurr->m_pNext;
	}
}
*/
void CConfigDlg::OnBnClickedSaveList()
{
	HTREEITEM hRootItem = m_PathTree.GetRootItem();
	if ( !hRootItem ) {
		return;
	}
	CFileDialog m_fileDlg(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xml_Files (*.xml)|*.xml|All_Files (*.*)|*.*||"));
	if ( m_fileDlg.DoModal() == IDOK ) {
		m_PathTree.SaveXml(hRootItem, m_fileDlg.GetPathName(), TRUE);
	}
}

void CConfigDlg::OnBnClickedLoadList()
{
	CFileDialog m_fileDlg(TRUE, _T("xml"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xml_Files (*.xml)|*.xml|All_Files (*.*)|*.*||"));
	if ( m_fileDlg.DoModal() == IDOK ) {
		m_PathTree.DeleteAllItems();
		m_PathTree.LoadXmlFromFile(m_fileDlg.GetPathName(), CXHtmlTree::NoConvertAction);
		m_PathTree.Expand(m_PathTree.GetRootItem(), TVE_EXPAND);
		int idx = m_TreeSelect.FindString(-1, m_fileDlg.GetPathName());
		if ( idx != -1 ) {
			m_TreeSelect.SetCurSel(idx);
		}else{
			m_TreeSelect.SetCurSel(m_TreeSelect.InsertString(m_TreeSelect.GetCount()-1, m_fileDlg.GetPathName()));
		}
	}
}

void CConfigDlg::OnBnClickedResDefault()
{
	m_PathTree.DeleteAllItems();
	m_PathTree.LoadXmlFromResource(NULL, MAKEINTRESOURCE(RES_XML_DEFAULT), _T("RES_XML"), CXHtmlTree::NoConvertAction);
	m_PathTree.Expand(m_PathTree.GetRootItem(), TVE_EXPAND);
}

void CConfigDlg::OnBnClickedDelAll()
{
	m_PathTree.DeleteAllItems();
}

void CConfigDlg::OnBnClickedApplyBtn()
{
	CString strText;
	m_TreeSelect.GetWindowText(strText);
	if ( strText == _T("Ĭ��Ŀ¼���ṹ ...") ) {
		OnBnClickedResDefault();
	} else if ( PathFileExists(strText) ) {
		m_PathTree.DeleteAllItems();
		m_PathTree.LoadXmlFromFile(strText, CXHtmlTree::NoConvertAction);
		m_PathTree.Expand(m_PathTree.GetRootItem(), TVE_EXPAND);
	}else{
		//
	}
	::EnableWindow(GetDlgItem(IDC_APPLY_BTN)->m_hWnd, FALSE);
}

CTreeItem *CConfigDlg::GetTreeListByCtrl(HTREEITEM hParent)
{
	if ( !hParent ) {
		return NULL;
	}
	CString strText;
	CTreeItem *pParent = NULL, *pPreChild = NULL, *pNextChild = NULL;
	pParent = new CTreeItem;
	ASSERT(pParent != NULL );
	//////////////////////////////////////////////////////////////////////////
	// Make the node;
	int nImage = -1, nSelectedImage = -1;
	strText = m_PathTree.GetItemText(hParent);
	if ( strText.GetLength() > 0 ) {
		_tcscpy_s(pParent->m_szItemName, _MAX_FNAME, strText);
		pParent->m_hTreeItem = hParent;
		m_PathTree.GetItemImage(hParent, nImage, nSelectedImage);
		pParent->m_bFolder = (nImage == FOLDER_CLOSE_IMAGE) ? TRUE : FALSE;
		pParent->m_iState = m_PathTree.GetCheck(hParent);
	}
	//////////////////////////////////////////////////////////////////////////
	HTREEITEM hChildItem = m_PathTree.GetChildItem(hParent);
	while( hChildItem != NULL ) {
		pNextChild = GetTreeListByCtrl(hChildItem);
		pNextChild->m_pParent = pParent;
		if ( !pParent->m_pFirstChild ) {
			pParent->m_pFirstChild = pNextChild;
		}
		if ( pPreChild ) {
			pPreChild->m_pNext = pNextChild;
		}
		hChildItem = m_PathTree.GetNextItem(hChildItem, TVGN_NEXT);
		pPreChild = pNextChild;
	}
	return pParent;
}

CTreeItem *CConfigDlg::GetTreeListByFile(LPCTSTR lpszFileName)
{
	m_PathTree.DeleteAllItems();
	if ( !lpszFileName || !PathFileExists(lpszFileName) ) {
		m_PathTree.LoadXmlFromResource(NULL, MAKEINTRESOURCE(RES_XML_DEFAULT), _T("RES_XML"), CXHtmlTree::NoConvertAction);
	}else{
		m_PathTree.LoadXmlFromFile(lpszFileName, CXHtmlTree::NoConvertAction);
	}
	if ( m_pTreeListHead ) {
		FreeTreeList();
	}
	m_pTreeListHead = GetTreeListByCtrl(m_PathTree.GetRootItem());
	return m_pTreeListHead;
}

void CConfigDlg::FreeTreeList(CTreeItem *in_pParent)
{
	if ( in_pParent == NULL ) {
		in_pParent = m_pTreeListHead;
	}
	if ( !in_pParent ) {
		return;
	}
	CTreeItem *pCurr = in_pParent, *pNext = NULL;
	if ( !pCurr ) {
		return;
	}
	while( pCurr ) {
		if ( pCurr->m_pFirstChild ) {
			FreeTreeList(pCurr->m_pFirstChild);
		}
		pNext = pCurr->m_pNext;
		delete pCurr;
		pCurr = pNext;
	}
	m_pTreeListHead = NULL;
}
void CConfigDlg::OnBnClickedDelRecycleCheck()
{
}

void CConfigDlg::OnBnClickedEndCloseCheck()
{
}

void CConfigDlg::OnBnClickedMinTrayCheck()
{
}

void CConfigDlg::OnBnClickedSetDefault()
{
	CString strText;
	GetDlgItemText(IDC_TREE_SELECT, strText);
	if ( RegReadWriteStrings(_T("InitStartTree"), strText, FALSE) ) {
		MessageBox(_T("�趨�ɹ���"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
	}else{
		MessageBox(_T("�趨ʧ�ܣ�"), _T("��ʾ"), MB_OK | MB_ICONERROR);
	}
}

void CConfigDlg::OnCancel()
{
	CString strText;
	CStringArray arrString;
	for( int idx = 1; idx < m_TreeSelect.GetCount()-1; idx++ ) {
		m_TreeSelect.GetLBText(idx, strText);
		arrString.Add(strText);
	}
	RegReadWriteStrings(_T("TreesList"), arrString, FALSE);
	CDialog::OnCancel();
}

BOOL CConfigDlg::RegSetAssociateRMenu(BOOL bAssociate)
{
	if (bAssociate == FALSE) {
		if (RegDeleteKey(HKEY_CLASSES_ROOT, _T("Directory\\shell\\ProjCleaner")) == ERROR_SUCCESS) {
			return TRUE;
		}else{
			return FALSE;
		}
	}
	BOOL bRet = FALSE;
	HKEY hKey = NULL;
	DWORD dwLen = 0;
	TCHAR szValue[MAX_PATH] = {0};
	_tcscpy_s(szValue, MAX_PATH, _T("����Ŀ¼������"));
	dwLen = (DWORD)_tcslen(szValue)*sizeof(TCHAR);

	LONG LRet =  RegCreateKey(HKEY_CLASSES_ROOT, _T("Directory\\shell\\ProjCleaner"), &hKey);
	if ( LRet == ERROR_SUCCESS ) {
		LRet = RegSetValueEx(hKey, NULL, 0, REG_SZ, (LPBYTE)szValue, dwLen);
		if (LRet == ERROR_SUCCESS) {
			RegCloseKey(hKey);
			ZeroMemory(szValue, MAX_PATH);
			GetModuleFileName(NULL, szValue, MAX_PATH);
			_tcscat_s(szValue, MAX_PATH, _T(" \"%1\""));
			dwLen = (DWORD)_tcslen(szValue)*sizeof(TCHAR);
			LRet =  RegCreateKey(HKEY_CLASSES_ROOT, _T("Directory\\shell\\ProjCleaner\\command"), &hKey);
			if (LRet == ERROR_SUCCESS) {
				LRet = RegSetValueEx(hKey, NULL, 0, REG_SZ, (LPBYTE)szValue, dwLen);
				if (LRet == ERROR_SUCCESS) {
					bRet = TRUE;
				}
			}
		}
	}

__Error_End:
	RegCloseKey(hKey);
	return bRet;
}

void CConfigDlg::OnBnClickedOk()
{
	//m_DelRecycleBtn button;
	int nCheck = m_DelRecycleBtn.GetCheck();
	if ( nCheck == BST_CHECKED ) {
		RegReadWriteDword(_T("DeleteToRecycle"), FALSE, TRUE);
	}else if ( nCheck == BST_UNCHECKED ) {
		RegReadWriteDword(_T("DeleteToRecycle"), FALSE, FALSE);
	}
	//m_EndCloseBtn button;
	nCheck = m_EndCloseBtn.GetCheck();
	if ( nCheck == BST_CHECKED ) {
		RegReadWriteDword(_T("EndClose"), FALSE, TRUE);
	}else if ( nCheck == BST_UNCHECKED ) {
		RegReadWriteDword(_T("EndClose"), FALSE, FALSE);
	}
	//m_MinTrayBtn button;
	nCheck = m_MinTrayBtn.GetCheck();
	if ( nCheck == BST_CHECKED ) {
		RegReadWriteDword(_T("MiniTray"), FALSE, TRUE);
	}else if ( nCheck == BST_UNCHECKED ) {
		RegReadWriteDword(_T("MiniTray"), FALSE, FALSE);
	}
	//m_AssRMenuBtn button;
	nCheck = m_AssRMenuBtn.GetCheck();
	if ( nCheck == BST_CHECKED ) {
		RegReadWriteDword(_T("AssRMenu"), FALSE, TRUE);
		RegSetAssociateRMenu(TRUE);
	}else if ( nCheck == BST_UNCHECKED ) {
		RegReadWriteDword(_T("AssRMenu"), FALSE, FALSE);
		RegSetAssociateRMenu(FALSE);
	}
	CString strPath;
	GetDlgItemText(IDC_TREE_SELECT, strPath);
	RegReadWriteStrings(_T("CurrentTreeFile"), strPath, FALSE);
	OnOK();
}

void CConfigDlg::OnBnClickedSmingBtn()
{
	CString str;
	str = _T("\r\n                       ����Ŀ¼������ v1.0\r\n\r\n�� ���ܼ�飺\r\n\r\n        ��ҿ��ܻ�������")
		_T("��д��һ���������������˷���Դ�������������ع���Դ��ʱ��ѹ������Ŀ¼��õ���ѹ���ļ���ܴ�")
		_T("Я���������ϴ���ܲ����㣬������Ĭ�Ͽ�������Microsoft Visual C++ �������ڱ��롢���ӹ��������������м��ļ���")
		_T("�磺obj��ilk �ȵȡ�����������������趨��Ŀ¼���ṹ�󣬿���֧���������ֱ�������VB��C#��Delphi �ȵȡ�")
		_T("֮�󻹿��Ա���� xml �ļ����´μ��ؼ��ɡ�\r\n\r\n�� ʹ��˵����\r\n\r\n        ����Լ���ĥ���ðɣ�( ^_^ )\r\n\r\n")
		_T("�� Bug�����飺\r\n\r\n        �������ʹ�õĹ���������ʲôBug����ʲô�õĽ��飬����ϵ�ң�\r\n\r\n        QQ��7657318\r\n        Mail��7657318@qq.com\r\n                  net_sy@sina.com.cn")
		_T("\r\n\r\n                                                     ллʹ�ã�\r\n");
	CDutyDlg dlg;
	dlg.SetShowString(str, _T("ʹ��˵��"));
	dlg.DoModal();
}

void CConfigDlg::OnBnClickedMinzeBtn()
{
	CString str;
	str = _T("\r\n                                ��������\r\n\r\n        1. �����Ϊ�����������Ա����ɵ�ʹ�á�������")
		_T("Ϊ�������ߣ��벻Ҫ�����޸ģ����󼰷���࣬���޸��뱣��������Ϣ��\r\n\r\n")
		_T("        2. ��ʹ�ñ�����޸����Ĺ���ʱ������֮ǰ���ñ��ݣ����ⶪʧ��")
		_T("���������ɵ��κ���ʧ�ͺ���������ге��������߲����κθ���\r\n\r\n")
		_T("        3. ������Ѿ���ʼʹ�ñ��������Ĭ�����Ѿ�ͬ�Ȿ�����������ге����Σ�")
		_T("\r\n\r\n\r\n\r\n                                                           ���ߣ�Syc\r\n                                                            ����-����\r\n                                                           2009-06-06");
	CDutyDlg dlg;
	dlg.SetShowString(str, _T("��������"));
	dlg.DoModal();
}
