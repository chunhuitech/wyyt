// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ProjCleaner.h"
#include "MyListBox.h"
#include "ProjCleanerDlg.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_M_ADD_PATH, &CMyListBox::OnMAddPath)
	ON_COMMAND(ID_M_DEL_PATH, &CMyListBox::OnMDelPath)
	ON_COMMAND(ID_M_DEL_PATHS, &CMyListBox::OnMDelPaths)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyListBox message handlers



void CMyListBox::OnRButtonUp(UINT nFlags, CPoint point)
{
	int idx = -1;
	BOOL bOutSide = TRUE;
	idx = ItemFromPoint(point, bOutSide);
	if ( !bOutSide ) {
		SetCurSel(idx);
	}
	/////////////////////////////////////////////////////
	CMenu mMenu, *pMenu = NULL;
	mMenu.LoadMenu(IDR_PATH_MENU);
	pMenu = mMenu.GetSubMenu(0);
	ClientToScreen(&point);
	idx = GetCurSel();
	if ( idx < 0 )
		mMenu.EnableMenuItem(ID_M_DEL_PATH, MF_GRAYED);
	idx = GetCount();
	if ( idx <= 0 )
		mMenu.EnableMenuItem(ID_M_DEL_PATHS, MF_GRAYED);
	if ( m_gpMainWnd->m_bDeling ) {
		mMenu.EnableMenuItem(ID_MENU_ADD_PATH, MF_GRAYED);
		mMenu.EnableMenuItem(ID_M_DEL_PATH, MF_GRAYED);
		mMenu.EnableMenuItem(ID_M_DEL_PATHS, MF_GRAYED);
	}
	SetForegroundWindow();
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	CListBox::OnRButtonUp(nFlags, point);
}

void CMyListBox::OnMAddPath()
{
	TCHAR szFolderPath[MAX_PATH] = {0}, szFullPath[MAX_PATH] = {0};
	BROWSEINFO mBroInfo = {0};
	mBroInfo.hwndOwner = m_hWnd;
	mBroInfo.lpszTitle = _T("��ѡ��һ�ļ���...");
	mBroInfo.pszDisplayName = szFolderPath;
	mBroInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	ITEMIDLIST *pidl = ::SHBrowseForFolder(&mBroInfo);
	if (::SHGetPathFromIDList(pidl, szFullPath)){
		int idx = FindString(-1, szFullPath);
		if ( idx >= 0 ) {
			MessageBox(_T("·���Ѵ��ڣ�"), _T("��ʾ"), MB_OK|MB_ICONWARNING);
			return;
		}
		SetCurSel(InsertString(GetCount(), szFullPath, 0));
		SetCurSel(-1);
	}
}

void CMyListBox::OnMDelPath()
{
	int idx = GetCurSel();
	if ( idx >= 0 ) {
		DeleteString(idx);
	}
}

void CMyListBox::OnMDelPaths()
{
	int idx = GetCount();
	if ( idx > 0 ) {
		ResetContent();
	}
}

void CMyListBox::OnDropFiles(HDROP hDropInfo)
{
	if ( m_gpMainWnd->m_bDeling  ) {
		return;
	}
	TCHAR szFileName[MAX_PATH] = {0};
	int iFileNumber = 0;

	// �õ���ק�����е��ļ�����
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (int i = 0; i < iFileNumber; i++) {
		// �õ�ÿ���ļ���
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		if (FindString(0, szFileName) != LB_ERR)
			continue;
		// ���ļ�����ӵ�list��
		DWORD dwAttr = GetFileAttributes(szFileName);
		if ( dwAttr & FILE_ATTRIBUTE_DIRECTORY ) {
			SetCurSel(InsertString(GetCount(), szFileName, 0));
			SetCurSel(-1);
		}
	}

	CListBox::OnDropFiles(hDropInfo);
}
