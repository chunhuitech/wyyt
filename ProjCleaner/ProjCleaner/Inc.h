
#pragma once

#define WM_TRAYICON_MSG (WM_USER+100)

#define ADD_HEADER(self, ch) if(self[0] != ch) self = ch + self    
#define DEL_HEADER(self, ch) if(self[0] == ch) self.Delete(0)
#define ADD_ENDER(self, ch) if(self[self.GetLength()-1] != ch) self += ch
#define DEL_ENDER(self, ch) if(self[self.GetLength()-1] == ch) self = self.Left(self.GetLength()-1)

#define FOLDER_CLOSE_IMAGE 0
#define FOLDER_OPEN_IMAGE 1
#define FILE_IMAGE 2

enum {
	ITEM_STATE_UNKNOWN = -1,
	ITEM_STATE_UNSELECTED,
	ITEM_STATE_SELECTED,
	ITEM_STATE_MIDSELECTED
};

class CTreeItem {
public:
	int m_iState;
	BOOL m_bFolder;
	CTreeItem *m_pParent;
	CTreeItem *m_pFirstChild;
	CTreeItem *m_pNext;
	HTREEITEM m_hTreeItem;
	TCHAR m_szItemName[_MAX_FNAME];
	CTreeItem() {
		m_iState = ITEM_STATE_UNKNOWN;
		m_bFolder = TRUE;
		m_pParent = NULL;
		m_pFirstChild = NULL;
		m_pNext = NULL;
		m_hTreeItem = NULL;
		ZeroMemory(m_szItemName, _MAX_FNAME);
	}
};

class CProfileInfo {
public:
	BOOL m_bAssRMenu;
	BOOL m_bMinTray;
	BOOL m_bDelToRecycle;
	BOOL m_bEndClose;
	BOOL m_bSavePathList;
	CString m_strInitTree;
	CString m_strCurrTree;
	CProfileInfo() {
		m_bAssRMenu = FALSE;
		m_bMinTray = FALSE;
		m_bDelToRecycle = TRUE;
		m_bEndClose = FALSE;
		m_bSavePathList = FALSE;
	}
};
class CProjCleanerDlg;
extern DWORD dwFailed;
extern BOOL m_bStartFromCmd;
extern CProjCleanerDlg *m_gpMainWnd;
CTreeItem *GetTreeList();
UINT __cdecl ScanAndCleanFunc(LPVOID pParam);
void CleanStart(CTreeItem *pTreeList, CString strPath, BOOL bDelToRecycle);
INT RegReadWriteDword(LPTSTR lpszName, BOOL bRead, INT uValue = 0);
BOOL RegReadWriteStrings(LPTSTR lpszKeyName, CStringArray &arrString, BOOL bRead = TRUE);
BOOL RegReadWriteStrings(LPTSTR lpszName, CString &strValue, BOOL bRead);