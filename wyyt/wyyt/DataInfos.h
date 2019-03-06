#pragma once
#include "interface/DB/DataStructDefine.h"
//stSysClassInfoItem  内存数据源绑定
// CClassInfoItem  //树控件级绑定
class CClassInfoItem
{
public:
	CClassInfoItem(CString strName, HTREEITEM htItem = TVI_ROOT, ESpecialClassInfoItems eItemType = SCII_NO, int64 ID = 0, int64 ParentID =0 , short sHaseNode = 0, short sHaseRecomm = 0 );
	~CClassInfoItem();
	CClassInfoItem*		CloneContent() { return new CClassInfoItem(m_strName, 0, m_eItemType, m_ID, m_ParentID, m_sHaseNode); }
	HTREEITEM			FindItem(CClassInfoItem* pContentToFind) const;

	CString		m_strName;
	HTREEITEM	m_htItem;
	int64 m_ID;
	int64 m_ParentID;
	short m_sHaseNode; //是否有子节点 1 有 0 没有
	short m_sHaseRecomm; //是否推荐
	short sHaseSel; //是否被选中  1　选中 0 没有选中

	CAtlList<CClassInfoItem*> liSubDirectories;
	ESpecialClassInfoItems m_eItemType;
};


