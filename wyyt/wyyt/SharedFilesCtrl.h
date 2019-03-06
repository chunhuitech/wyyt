//this file is part of eMule
//Copyright (C)2002-2008 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#pragma once
#include "interface/DB/RecordItem.h"
#include "MuleListCtrl.h"
#include "TitleMenu.h"
#include "ListCtrlItemWalk.h"
#include "FilterItem.h"// X: [FI] - [FilterItem]
#include "wyytExtendDataManage.h"

class CSharedFileList;
class CKnownFile;
class CShareableFile;
class CDirectoryItem;
class CClassInfoItem;
class CToolTipCtrlX;

class CSharedFilesCtrl : public CMuleListCtrl, public CListCtrlItemWalk, public CFilterItem
 {
	friend class CSharedDirsTreeCtrl;
	DECLARE_DYNAMIC(CSharedFilesCtrl)
public:
	class CShareDropTarget: public COleDropTarget  
	{
	public:
		CShareDropTarget();
		virtual ~CShareDropTarget();
		void	SetParent(CSharedFilesCtrl* pParent)					{ m_pParent = pParent; }

		DROPEFFECT	OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
		DROPEFFECT	OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
		BOOL		OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
		void		OnDragLeave(CWnd* pWnd);

	protected:
		IDropTargetHelper*	m_piDropHelper;
		bool				m_bUseDnDHelper;
		BOOL ReadHdropData (COleDataObject* pDataObject);
		CSharedFilesCtrl*   m_pParent;
	};

	CSharedFilesCtrl();
	virtual ~CSharedFilesCtrl();

	void	Init();
	void	SetToolTipsDelay(DWORD dwDelay);
	void	CreateMenues();
	//�����ļ�����
	virtual void	ReloadFileList();
	virtual void	ReloadRecordList();
	void	ReloadRecordList(vector<int64> nVecSelClassIDs);
	//�����ļ���Ŀ
	void	AddFile(const CShareableFile* file);
	void	AddRecord(const CRecordItem* record);
	void	RemoveFile(const CShareableFile* file, bool bDeletedFromDisk);
	void	UpdateFile(const CShareableFile* file, bool bUpdateFileSummary = true/*, bool force=false*/); //Xman advanced upload-priority
	void	UpdateRecord(const CRecordItem* record, bool bUpdateFileSummary = true/*, bool force=false*/);
	void	Localize();
	void	ShowSelectedFileComments();
	void	SetDirectoryFilter(CDirectoryItem* pNewFilter, bool bRefresh = true);
	void	SetClassFilter(CClassInfoItem* pNewFilter, bool bRefresh = true);
	void	SetClassFilter(vector<int64> nVecSelClassIDs, bool bRefresh = true);

	INT_PTR ShowRecPPDlg(int nOpt,UINT uStartPageID = (UINT)-1);
	int ShowRecDetailsDlg_MOD(CRecordItem* pRecord);
	void RecordDelete( const CRecordItem* pRecord );
	void RecordDelete( vector<int64> vecSelected );
	void RecordDelete( int64 ID );
	int ShowRecDetailsDlg_ADD();

protected:
	//CTitleMenu		m_SharedFilesMenu;
	CTitleMenu		m_RecordsMenu;
	//CTitleMenu		m_CollectionsMenu;//�Ӳ˵������ղؼ� �е� �����ղؼ����޸��ղؼ� �ȵ�
	//CMenu			m_PrioMenu; //�Ӳ˵��������ȼ��еĵ͡��ܵ͡���ͨ���ߡ��ȵ�
	bool			m_aSortBySecondValue[4];
	CImageList		m_ImageList;
	CToolTipCtrlX*	m_pToolTip;
	CAtlList<CShareableFile*>	liTempShareableFilesInDir;
	//CShareableFile*	m_pHighlightedItem;
	CRecordItem*	m_pHighlightedItem;
	CShareDropTarget m_ShareDropTarget;
	CDirectoryItem*	m_pDirectoryFilter;
	CClassInfoItem* m_pClassInfoFilter;
	//�洢����ͨ������õ���Ӧ����ʾ��ID��ֵ���������ݵ�ǰѡ�е�IDֵ���������������ڵ�ID ��ȫ������
	vector<int64> m_vecSelClassIDsByCalc;
	
	//����������ص�����
	static int CALLBACK SortProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void OpenFile(const CShareableFile* file);
	void ShowFileDialog(CAtlList<CRecordItem*>& aFiles, UINT uPshInvokePage = 0);
	void SetAllIcons();
	int FindFile(const CShareableFile* pFile);
	int FindRecord(const CRecordItem* pRecord);
	virtual void GetItemDisplayText(const CRecordItem* record, int iSubItem, LPTSTR pszText, int cchTextMax) const;
	bool IsSharedInKad(const CKnownFile *file) const;
	void AddShareableFiles(CString strFromDir);
	void AddRecordDatas();

	void AddRecordDatas_PreDeal(vector<int64> nVecSelClassIDs);//��Ӽ�¼ǰ�Ķ���ID��Ԥ����
	bool HasSelectedOrSubClass(int64 nCID);
	void AddRecordDatasByClassIDs();
	void AddSelClassIDbyCalc(int64 nID);
	void GetPCID2AllSubClassIDs(int64 nParentCID);
	void CheckBoxClicked(int iItem);
	bool CheckBoxesEnabled() const;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLvnColumnClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNmDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

};
