#include "stdafx.h"
#include "FilterItem.h"
//#include "KnownFile.h"
#include "EditDelayed.h"
//#include "PartFile.h"
#include "Preferences.h"
#include "FilterDlg.h"
#include "OtherFunctions.h" //add de
#include "interface/DB/RecordItem.h"
#include "wyytExtendDataManagePri.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CFilterItem::IsFilteredItem(const CRecordItem* pRecord, EModuleType emt) const
{
	if (m_astrFilter.GetCount() == 0)
		return true;

	// filtering is done by text only for all colums to keep it consistent and simple for the user even if that
	// doesn't allows complex filters
	//TCHAR szFilterTarget[256];
	//GetItemDisplayText(pRecord, m_nFilterColumn,
	//				   szFilterTarget, _countof(szFilterTarget));
	bool bItemFiltered = false;
	//����ֱ����ֱ����С������С������С�ȫ���еȷֱ���д���
	const TCHAR * pszFilterTarget = NULL;
	bool bFindAllItems = false; //�Ƿ����������в�ѯ
	bool bFindAttachs = false;
	switch (m_nFilterColumn)
	{
	case E_EFC_ALL:
		bFindAllItems = true;
		//break;
	case E_EFC_ATTACH:	
		{
			bFindAttachs = true;
			if ( bFindAllItems || bFindAttachs )//��ȫ�ļ��� �򡡸�������
			{
				bItemFiltered = IsAttachItem(pRecord->ID,emt);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	case E_EFC_TITLE:
		{
			pszFilterTarget = pRecord->Titlel.c_str();
			if ( bFindAllItems && pszFilterTarget  )//��ȫ�ļ���
			{
				bItemFiltered = IsQueryItem(pszFilterTarget);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	case E_EFC_CODE:
		{
			pszFilterTarget = pRecord->SID.c_str();
			if ( bFindAllItems && pszFilterTarget  )//��ȫ�ļ���
			{
				bItemFiltered = IsQueryItem(pszFilterTarget);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	case E_EFC_SUMMARY:
		{
			pszFilterTarget = pRecord->KeyValues.c_str();
			if ( bFindAllItems && pszFilterTarget  )//��ȫ�ļ���
			{
				bItemFiltered = IsQueryItem(pszFilterTarget);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	case E_EFC_QUOTE:
		{
			pszFilterTarget = pRecord->Reference.c_str();
			if ( bFindAllItems && pszFilterTarget  )//��ȫ�ļ���
			{
				bItemFiltered = IsQueryItem(pszFilterTarget);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	case E_EFC_CONTENT:
		{
			pszFilterTarget = pRecord->Content.c_str();
			if ( bFindAllItems && pszFilterTarget  )//��ȫ�ļ���
			{
				bItemFiltered = IsQueryItem(pszFilterTarget);
				if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	
	
	if ( !bFindAllItems && !bFindAttachs && pszFilterTarget )  //����ȫ�ļ������Ҳ��Ǹ�����Ϣ����
	{
		bItemFiltered = IsQueryItem(pszFilterTarget);
	}
	
	return bItemFiltered;
}
bool CFilterItem::IsAttachItem(const int64 nRID, EModuleType emt ) const
{
	bool bItemFiltered = false;
	if ( EMT_SYS == emt )
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManage::Instance().m_mapRec2Attachs.find(nRID);//���ҵ����и�����¼
		if (Iter != wyytExtendDataManage::Instance().m_mapRec2Attachs.end() )
		{
			const TCHAR * pszFilterTarget = NULL;
			for (vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++ )//��ÿ��������¼����ƥ���ѯ
			{
				pszFilterTarget = it->CodeID.c_str();
				if ( pszFilterTarget  )//CodeID����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->FileName.c_str();
				if ( pszFilterTarget  )//�ļ�������
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->FileSuffix.c_str();
				if ( pszFilterTarget  )//�ļ���׺����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}
				pszFilterTarget = it->DownloadAddress.c_str();
				if ( pszFilterTarget  )//���ص�ַ����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->Note.c_str();
				if ( pszFilterTarget  )//Note����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		map<int64, vector<CRecordAttachs>>::iterator Iter = wyytExtendDataManagePri::Instance().m_mapRec2Attachs.find(nRID);//���ҵ����и�����¼
		if (Iter != wyytExtendDataManagePri::Instance().m_mapRec2Attachs.end() )
		{
			const TCHAR * pszFilterTarget = NULL;
			for (vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++ )//��ÿ��������¼����ƥ���ѯ
			{
				pszFilterTarget = it->CodeID.c_str();
				if ( pszFilterTarget  )//CodeID����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->FileName.c_str();
				if ( pszFilterTarget  )//�ļ�������
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->FileSuffix.c_str();
				if ( pszFilterTarget  )//�ļ���׺����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}
				pszFilterTarget = it->DownloadAddress.c_str();
				if ( pszFilterTarget  )//���ص�ַ����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}

				pszFilterTarget = it->Note.c_str();
				if ( pszFilterTarget  )//Note����
				{
					bItemFiltered = IsQueryItem(pszFilterTarget);
					if ( bItemFiltered )//��ǰ���Ѿ��ҵ�
					{
						break;
					}
				}
			}
		}
	}
	
	return bItemFiltered;
}

bool CFilterItem::IsQueryItem(const TCHAR *pszFilterTarget) const
{
	bool bItemFiltered = false;
	for (size_t i = 0; i < m_astrFilter.GetCount(); i++)
	{
		const CString& rstrExpr = m_astrFilter.GetAt(i);
		bool bAnd = true;
		LPCTSTR pszText = (LPCTSTR)rstrExpr;
		if (pszText[0] == _T('-')) 
		{
			pszText += 1;
			bAnd = false;
		}

		bool bFound = (stristr(pszFilterTarget, pszText) != NULL);
		if (bFound) 
		{
			bItemFiltered = true;
			break;
		}
	}
	return bItemFiltered;
}
BOOL CFilterItem::SetFilter(size_t newfilter)
{
	// category filter menuitems
	if(filter != newfilter)
	{
		dlg->filter = filter = newfilter;
		ReloadRecordList();
		dlg->UpdateFilterLabel();
	}
	return TRUE;
}

