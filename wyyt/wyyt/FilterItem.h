#pragma once
#include "../interface/DB/DbDefine.h"
class CEditDelayed;
class CFilterDlg;
class CAbstractFile;
class CRecordItem;
class CFilterItem{// X: [FI] - [FilterItem]
public:
	CFilterItem():m_nFilterColumn(0),IgnoredColums(0),filter(0),dlg(NULL){};

	BOOL SetFilter(size_t newfilter);
	//virtual void	ReloadFileList()=0;
	virtual void	ReloadRecordList() = 0;
	CFilterDlg*		dlg;
	CAtlArray<CString>	m_astrFilter;
	CHeaderCtrl		m_ctlListHeader;
	CString			m_strEvaluatedContent;
	size_t			m_nFilterColumn;
	size_t			IgnoredColums;
	size_t			filter;
protected:
	bool IsFilteredItem(const CRecordItem* pRecord, EModuleType emt = EMT_SYS) const;
	bool IsQueryItem(const TCHAR *pszFilterTarget) const;
	bool IsAttachItem(const int64 nRID, EModuleType emt = EMT_SYS) const;
	//virtual void GetItemDisplayText(const CAbstractFile* file, int iSubItem, LPTSTR pszText, int cchTextMax) const=0;
	virtual void GetItemDisplayText(const CRecordItem* record, int iSubItem, LPTSTR pszText, int cchTextMax) const=0;
};