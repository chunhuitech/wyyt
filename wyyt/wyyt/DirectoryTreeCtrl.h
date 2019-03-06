#pragma once
#include "devlib_include.h"
#include "wyytExtendDataManagePri.h"
#include "DataInfos.h"
/////////////////////////////////////////////
// written by robert rostek - tecxx@rrs.at //
/////////////////////////////////////////////

#define MP_SHAREDFOLDERS_FIRST	46901
#define MP_RECORD_MAINCLASS		46902
#define MP_RECORD_MAINCLASS_CANCLE		46903



class CDirectoryTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDirectoryTreeCtrl)

public:
	void InitDataSoruce(EModuleType emt = EMT_SYS);
	// initialize control
	void Init(void);
	// get all shared directories
	void GetSharedDirectories(CAtlList<CString>* list);
	// set shared directories
	void SetSharedDirectories(CAtlList<CString>* list);

	void SetSelectedClasses(vector<int64>* vecSel,int64 nMainClassID);
	bool GetSelectedClasses(vector<int64>* vecSel,int64& nMainClassID);
	CClassInfoItem* GetSelectedClass() const;
	//void SetClassInfos(int nDataSourceType);
	//void	InitalizeStandardItems_SYS();

	void Init_ClassInfos(void);
	bool IfSelectedSubclass(int64 nCurCID);
	void  UpDataItemStatus_CurNode(CClassInfoItem* pCurClsInfo); //判断当前节点是否改变状态
	void  UpDataItemStatus_DirectChildrenNode(CClassInfoItem* pCurClsInfo); //判断当前节点直接子节点是否需要改变判断
private:
	CImageList m_image; 
	// add a new item
	HTREEITEM AddChildItem(HTREEITEM hRoot, CString strText);
	// add subdirectory items
	void AddSubdirectories(HTREEITEM hRoot, CString strDir);
	// return the full path of an item (like C:\abc\somewhere\inheaven\)
	CString GetFullPath(HTREEITEM hItem);
	// returns true if strDir has at least one subdirectory
	bool HasSubdirectories(CString strDir);
	// check status of an item has changed
	void CheckChanged(HTREEITEM hItem, bool bChecked);
	// returns true if a subdirectory of strDir is shared
	bool HasSharedSubdirectory(CString strDir);
	// when sharing a directory, make all parent directories bold
	void UpdateParentItems(HTREEITEM hChild);
	void ShareSubDirTree(HTREEITEM hItem, BOOL bShare);

	// share list access
	bool IsShared(CString strDir);
	void AddShare(CString strDir);
	void DelShare(CString strDir);
	void MarkChilds(HTREEITEM hChild,bool mark);

	// return the ID of an item 
	int64 GetClassID(HTREEITEM hItem);
	void AddSelected(int64 nID);
	void DelSelected(int64 nID);

	CAtlList<CString> m_lstShared;
	vector<int64> m_vecSelected;
	CString m_strLastRightClicked;
	bool m_bSelectSubDirs;

	int m_nDataSourceType;
	//CClassInfoItem*		m_pRootAllClassInfos;
	//CClassInfoItem*		m_pRootNotesClassInfos;
	CClassInfoItem*		m_ArrayRootClassInfos[ROOT_CLASS_COUNTS];

	//CAtlList<int64>			m_lstAllSelectIDs; //记录已经选中的ID标记集合
public:
	// construction / destruction
	CDirectoryTreeCtrl();
	virtual ~CDirectoryTreeCtrl();
	virtual BOOL OnCommand(WPARAM wParam,LPARAM lParam );

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnDeleteItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();


	void			DeleteChildItems(CClassInfoItem* pParent);
	//只加载指定父节点下的所有直接关联的子节点
	//注：只有在点开相应节点时才加载相应节点下的数据，否则只加载根节点
	void	AddClassInfoChildItem(CClassInfoItem* pRoot, int64 nParentID, bool bTopLevel);
	void	AddTreeClassInfoChildItem(CClassInfoItem* pRoot, bool bTopLevel, stSysClassInfoItem Cii);
	bool IsSelected(int64 nCID);
	// returns true if a subClass of nCID is selected
	bool HasSelectedSubclass(int64 nParentCID/*, HTREEITEM hParent = NULL*/);
	// select list access
	//bool IsSelected(int64 nCID);
	//void AddSelected(int64 nCID);
	//void DelSelected(int64 nCID);
	EModuleType m_emt;
	int64 m_nMainClassID;
};
