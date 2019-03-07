
#include "stdafx.h"
#include "Inc.h"
#include "ProjCleanerDlg.h"
#include "ShellFileOp/ShellFileOp.h"

BOOL m_bStartFromCmd = FALSE;
CProjCleanerDlg *m_gpMainWnd = NULL;
DWORD dwTotal = 0, dwFiles = 0, dwFolders = 0, dwSucceed = 0, dwFailed = 0;

BOOL FindItemInTree(CString strText, CTreeItem *pTreeList, BOOL bFile)
{
	CTreeItem *pCurr = pTreeList, *pNext = NULL;
	while( pCurr ) {
		if ( pCurr->m_pFirstChild ) {
			if ( FindItemInTree(strText, pCurr->m_pFirstChild, bFile) ) {
				return TRUE;
			}
		}
		pNext = pCurr->m_pNext;
		if (bFile == !pCurr->m_bFolder && pCurr->m_iState == TRUE) {
			if ( bFile ) { //is files;
				CString strItemParent;
				CString strExt = _T("*.") + CShellFileOp::GetFileExt(strText);
				CString strParent = CShellFileOp::GetFileParent(strText);
				if ( pCurr->m_pParent && pCurr->m_pParent->m_szItemName ) {
					strItemParent = pCurr->m_pParent->m_szItemName;
				}
				if ( _tcscmp(pCurr->m_szItemName, strExt) == 0 ) {
					if ( (strParent == _T("Debug") || strParent == _T("debug") || strParent == _T("Release") || strParent == _T("release"))) {
						if ( strParent == strItemParent ) {
							return TRUE;
						}
					}else{
						return TRUE;
					}
				}
			}else{ //is folder;
				if ( _tcscmp(pCurr->m_szItemName, strText) == 0 ) {
					return TRUE;
				}
			}
		}
		pCurr = pNext;
	}
	return FALSE;
}

BOOL IsDeleteFile(CString strPath, CTreeItem *pTreeList)
{
	return FindItemInTree(strPath, pTreeList, TRUE);
}

BOOL IsDeleteFolder(CString strPath, CTreeItem *pTreeList)
{
	CString strTmp = strPath;
	DEL_ENDER(strTmp, '\\');
	strTmp = strTmp.Right(strTmp.GetLength()-strTmp.ReverseFind('\\')-1);
	if ( FindItemInTree(strTmp, pTreeList, FALSE) ) {
		return TRUE;
	}
	return FALSE;
}

CTreeItem *GetTreeList()
{
	CString strPath;
	CConfigDlg  m_dlg;
	m_dlg.Create(IDD_CONFIG_DLG);
	RegReadWriteStrings(_T("CurrentTreeFile"), strPath, TRUE);
	CTreeItem *pList = m_dlg.GetTreeListByFile(strPath);
	m_dlg.DestroyWindow();
	return pList;
}

void CleanStart(CTreeItem *pTreeList, CString strPath, BOOL bDelToRecycle)
{
	BOOL bRet = FALSE;
	CFileFind finder;
	ADD_ENDER(strPath, '\\');
	BOOL bfinder = finder.FindFile(strPath + _T("*.*"), 0);
	while(bfinder){
		bfinder = finder.FindNextFile();
		if (finder.IsDots()){
			continue;
		}else{
			if (finder.IsDirectory()){
				CleanStart(pTreeList, finder.GetFilePath(), bDelToRecycle);
			}else{
				if (m_bStartFromCmd == FALSE) {
					m_gpMainWnd->dwPos++;
				}
				CString strFilePath = finder.GetFilePath();
				if ( IsDeleteFile(finder.GetFilePath(), pTreeList) ) {
					dwTotal++;
					dwFiles++;
					bRet = CShellFileOp::RemoveFile(finder.GetFilePath(), bDelToRecycle);
					if ( bRet == TRUE ) {
						dwSucceed++;
					}else{
						dwFailed++;
					}
					if (m_bStartFromCmd == FALSE) {
						m_gpMainWnd->ShowLogMsg(strFilePath, FALSE, bRet);
					}
				}
			}
		}
	}
	finder.Close();
	// if the folder is empty, delete it ;
	if (m_bStartFromCmd == FALSE) {
		m_gpMainWnd->dwPos++;
	}
	if ( IsDeleteFolder(strPath, pTreeList) && CShellFileOp::FolderIsEmpty(strPath) ) {
		dwTotal++;
		dwFolders++;
		bRet = CShellFileOp::RemoveFolder(strPath);
		if ( bRet == TRUE ) {
			dwSucceed++;
		}else{
			dwFailed++;
		}
		if (m_bStartFromCmd == FALSE) {
			m_gpMainWnd->ShowLogMsg(strPath, TRUE, bRet);
		}
	}
}

UINT __cdecl ScanAndCleanFunc(LPVOID pParam)
{
	CProjCleanerDlg *m_pMainWnd = (CProjCleanerDlg *)pParam;
	ASSERT(m_pMainWnd != NULL);
	dwTotal = 0, dwFiles = 0, dwFolders = 0, dwSucceed = 0, dwFailed = 0;
	m_gpMainWnd = m_pMainWnd;
	m_pMainWnd->m_bDeling = TRUE;
	m_pMainWnd->EnableWindow(IDC_SET_INFO, FALSE);
	m_pMainWnd->EnableWindow(IDC_ADD_PATH, FALSE);
	m_pMainWnd->EnableWindow(IDC_DEL_PATH, FALSE);
	CString strPath;
	LONG nCount = 0;
	for( int idx = 0; idx < m_pMainWnd->m_PathList.GetCount(); idx++ ) {
		m_pMainWnd->m_PathList.GetText(idx, strPath);
		nCount += CShellFileOp::GetSubFilesFolderNums(strPath);
	}
	//for the progress ctrl;
	m_pMainWnd->m_ProgCtrl.SetRange32(0, nCount);
	m_pMainWnd->m_ProgCtrl.SetStep(1);
	m_pMainWnd->SetTimer(1, 50, NULL);
	// end;
	for( int idx = 0; idx < m_pMainWnd->m_PathList.GetCount(); idx++ ) {
		m_pMainWnd->m_PathList.GetText(idx, strPath);
		CleanStart(m_pMainWnd->GetTreeList(), strPath, RegReadWriteDword(_T("DeleteToRecycle"), TRUE, 0));
	}
	m_pMainWnd->KillTimer(1);
	m_pMainWnd->m_ProgCtrl.SetPos(0);
	m_pMainWnd->m_bDeling = FALSE;
	m_pMainWnd->EnableWindow(IDC_SET_INFO, TRUE);
	m_pMainWnd->EnableWindow(IDC_ADD_PATH, TRUE);
	m_pMainWnd->EnableWindow(IDC_DEL_PATH, TRUE);
	CString strT;
	strT.Format(_T("共删除文件和目录：%ld 个， 文件夹：%ld 个、文件：%ld 个；成功：%ld 个、失败：%ld 个；"), dwTotal, dwFolders, dwFiles, dwSucceed, dwFailed);
	m_pMainWnd->ShowLogMsg(_T(""), -1);
	m_pMainWnd->ShowLogMsg(strT, -1);
	if ( RegReadWriteDword(_T("EndClose"), TRUE) == TRUE ) {
		m_pMainWnd->OnCancel();
	}

	return TRUE;
}

//////////////////////////////// Reg operation /////////////////////////////////////
INT RegReadWriteDword(LPTSTR lpszName, BOOL bRead, INT uValue)
{
	INT iResult = -1;
	HKEY hKey = NULL;
	LONG LRet =  RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\ProjCleaner"), &hKey);
	if ( LRet == ERROR_SUCCESS ){
		if ( bRead ) {
			DWORD dwLen = sizeof(iResult);
			LRet = RegQueryValueEx(hKey, lpszName, NULL, NULL, (LPBYTE)&iResult, &dwLen);
			if ( LRet != ERROR_SUCCESS ){
				iResult = -1;
			}
		}else{
			LRet = RegSetValueEx(hKey, lpszName, 0, REG_DWORD, (LPBYTE)&uValue, sizeof(uValue));
			if ( LRet == ERROR_SUCCESS ){
				iResult = uValue;
			}
		}
	}
	RegCloseKey(hKey);
	return iResult;
}

BOOL RegReadWriteStrings(LPTSTR lpszKeyName, CStringArray &arrString, BOOL bRead)
{
	HKEY hKey = NULL;
	CString strKeyName(lpszKeyName);
	if ( bRead ) {
		strKeyName = _T("Software\\ProjCleaner\\") + strKeyName;
	}else{
		strKeyName = _T("Software\\ProjCleaner\\");
	}
	LONG LRet =  RegCreateKey(HKEY_LOCAL_MACHINE, strKeyName, &hKey);
	if ( LRet == ERROR_SUCCESS ){
		if ( bRead ) {
			DWORD dwValues = 0, dwMaxValueNameLen = 0, dwMaxValueLen = 0;
			LRet = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL);
			dwMaxValueNameLen = 5;
			dwMaxValueLen = MAX_PATH*sizeof(TCHAR);
			TCHAR szValueName[5] = {0};
			TCHAR szValueData[MAX_PATH] = {0};
			for ( DWORD idx = 0; idx < dwValues; idx++ ){
				DWORD dwValueNameLen = dwMaxValueNameLen;
				DWORD dwValueLen = dwMaxValueLen;
				LRet = RegEnumValue(hKey, idx, szValueName, &dwValueNameLen, NULL, NULL, (LPBYTE)szValueData, &dwValueLen);
				if ( LRet == ERROR_SUCCESS ){
					CString strValue(szValueData);
					arrString.Add(strValue);
				}
			}
		}else{
			LRet = SHDeleteKey(hKey, lpszKeyName);
			if ( LRet != ERROR_SUCCESS ) {
				RegCloseKey(hKey);
				return FALSE;
			}
			RegCloseKey(hKey);
			strKeyName.Format(_T("%s"), lpszKeyName);
			strKeyName = _T("Software\\ProjCleaner\\") + strKeyName;
			LRet =  RegCreateKey(HKEY_LOCAL_MACHINE, strKeyName, &hKey);
			if ( LRet != ERROR_SUCCESS ) {
				return FALSE;
			}
			for( int idx = 0; idx < arrString.GetCount(); idx++ ) {
				CString strName;
				strName.Format(_T("%d"), idx+1);
				TCHAR szValue[MAX_PATH] = {0};
				_tcscpy_s(szValue, MAX_PATH, arrString.GetAt(idx));
				LRet = RegSetValueEx(hKey, strName, 0, REG_SZ, (LPBYTE)szValue, (DWORD)_tcslen(szValue)*sizeof(TCHAR));
				if ( LRet != ERROR_SUCCESS ) {
					RegCloseKey(hKey);
					return FALSE;
				}
			}
		}
		
		RegCloseKey(hKey);
		return TRUE;
	}
	RegCloseKey(hKey);
	return FALSE;
}

BOOL RegReadWriteStrings(LPTSTR lpszName, CString &strValue, BOOL bRead)
{
	HKEY hKey = NULL;
	DWORD dwLen = 0;
	TCHAR szValue[MAX_PATH] = {0};
	LONG LRet =  RegCreateKey(HKEY_LOCAL_MACHINE, _T("Software\\ProjCleaner"), &hKey);
	if ( LRet == ERROR_SUCCESS ) {
		if ( bRead ) {
			dwLen = (DWORD)sizeof(szValue);
			LRet = RegQueryValueEx(hKey, lpszName, NULL, NULL, (LPBYTE)szValue, &dwLen);
			if ( LRet == ERROR_SUCCESS ){
				strValue.Format(_T("%s"), szValue);
				RegCloseKey(hKey);
				return TRUE;
			}
		}else{
			_tcscpy_s(szValue, MAX_PATH, strValue);
			dwLen = (DWORD)_tcslen(szValue)*sizeof(TCHAR);
			LRet = RegSetValueEx(hKey, lpszName, 0, REG_SZ, (LPBYTE)szValue, dwLen);
			if ( LRet == ERROR_SUCCESS ){
				RegCloseKey(hKey);
				return TRUE;
			}
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}