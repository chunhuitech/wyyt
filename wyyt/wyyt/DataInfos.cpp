#include "stdafx.h"
#include "wyyt.h"
#include "DataInfos.h"


// CClassInfoItem
CClassInfoItem::CClassInfoItem(CString strName, HTREEITEM htItem , ESpecialClassInfoItems eItemType, int64 ID, int64 ParentID , short sHaseNode, short sHaseRecomm )
{
	m_strName = strName;
	m_htItem = htItem;
	m_eItemType = eItemType;
	m_ID = ID;
	m_ParentID = ParentID;
	m_sHaseNode = sHaseNode;
	m_sHaseRecomm = sHaseRecomm;
	sHaseSel = 0;
	//liSubDirectories.RemoveAll();
}

CClassInfoItem::~CClassInfoItem()
{
	//while(liSubDirectories.IsEmpty())
	while (liSubDirectories.GetHeadPosition() != NULL)
	{
		 //��������ӷ��ര���л����  �������ַ�����������  ��Ϊ CDirectoryTreeCtrl::OnTvnDeleteItem��������ɾ��������ظ�ɾ��
		delete liSubDirectories.RemoveHead(); 
		//CClassInfoItem* pToDelete = liSubDirectories.RemoveHead();
		//if ( pToDelete )
		//{
		//	delete pToDelete;
		//}
	}
}

// search tree for a given filter
HTREEITEM CClassInfoItem::FindItem(CClassInfoItem* pContentToFind) const
{
	if (pContentToFind == NULL)
	{
		ASSERT( false );
		return NULL;
	}

	if (pContentToFind->m_eItemType == m_eItemType /*&& pContentToFind->m_strName == m_strName */&& pContentToFind->m_ID == m_ID)
		return m_htItem;

	POSITION pos = liSubDirectories.GetHeadPosition();
	while (pos != NULL)
	{
		CClassInfoItem* pCurrent = liSubDirectories.GetNext(pos);
		HTREEITEM htResult;
		if ( (htResult = pCurrent->FindItem(pContentToFind)) != NULL)
			return htResult;
	}
	return NULL;
}