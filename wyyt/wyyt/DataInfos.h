#pragma once
#include "interface/DB/DataStructDefine.h"
//stSysClassInfoItem  �ڴ�����Դ��
// CClassInfoItem  //���ؼ�����
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
	short m_sHaseNode; //�Ƿ����ӽڵ� 1 �� 0 û��
	short m_sHaseRecomm; //�Ƿ��Ƽ�
	short sHaseSel; //�Ƿ�ѡ��  1��ѡ�� 0 û��ѡ��

	CAtlList<CClassInfoItem*> liSubDirectories;
	ESpecialClassInfoItems m_eItemType;
};


