#include "wyytExtendDataManage.h"
#include "GlobalFunction.h"
#include "DataAccess.h"
#include <io.h>




wyytExtendDataManage::wyytExtendDataManage(void)
{
}


wyytExtendDataManage::~wyytExtendDataManage(void)
{
	ClearSysRecordInfoDatas();
	ClearSysRecAttachsDatas();
}

wyytExtendDataManage& wyytExtendDataManage::Instance()
{
	static wyytExtendDataManage s_WyytExtendDataManage;
	return s_WyytExtendDataManage;
}
int64 wyytExtendDataManage::GetSysClassMaxID()
{
	int64 nMaxID = 0;
	for ( vector<stSysClassInfoItem>::iterator it = m_vecClassInfos.begin(); it != m_vecClassInfos.end(); it++ )
	{
		if ( it->ID > nMaxID )
		{
			nMaxID = it->ID;
		}
	}
	return (nMaxID + 1);
}

void wyytExtendDataManage::InsertNewClassInfo( stSysClassInfoItem stNewCII )
{
	m_vecClassInfos.push_back(stNewCII);
	DataAccess::Instance().ISysClassOper->InsertClass(stNewCII);
}
void wyytExtendDataManage::ModClassInfo(int64 nID, wstring wstrNewClassName)
{
	for ( vector<stSysClassInfoItem>::iterator it = m_vecClassInfos.begin(); it != m_vecClassInfos.end(); it++ )
	{
		if ( it->ID == nID )
		{
			it->cname = wstrNewClassName;
		}
	}
	DataAccess::Instance().ISysClassOper->ModClass(nID, wstrNewClassName);
}

void wyytExtendDataManage::DelClassInfo(int64 nID)
{
	vector<stSysClassInfoItem>::iterator it = find_if( m_vecClassInfos.begin(), m_vecClassInfos.end(), IsDelClass(nID) );
	if (it != m_vecClassInfos.end())
	{
		m_vecClassInfos.erase(it);
	}

	//这种方法为什么不行
	//for ( vector<stClassInfoItem>::iterator it = m_vecClassInfos.begin(); it != m_vecClassInfos.end();)
	//{
	//	if ( it->ID == nID )
	//	{
	//		m_vecClassInfos.erase(it++);
	//	}
	//	else
	//	{
	//		++it;
	//	}
	//};
	DataAccess::Instance().ISysClassOper->DelClass(nID);
}
void wyytExtendDataManage::SetClassInfoRecomm(int64 nID)
{
	vector<stSysClassInfoItem>::iterator it = find_if( m_vecClassInfos.begin(), m_vecClassInfos.end(), IsDelClass(nID) );
	if (it != m_vecClassInfos.end())
	{
		it->sHaseRecomm = 1;
	}
	DataAccess::Instance().ISysClassOper->SetClassRecomm(nID, 1);
}
bool wyytExtendDataManage::IsSysClassID(int64 nID)
{
	for(vector<int64>::iterator it =  m_vecSysClassInfoIDs.begin(); it != m_vecSysClassInfoIDs.end(); it++ )
	{
		if ( nID == *it )
		{
			return true;
		}
	}
	return false;
}
bool wyytExtendDataManage::IfHaseSubClass(int64 nParentID)
{
	for(vector<stSysClassInfoItem>::iterator it = m_vecClassInfos.begin(); it != m_vecClassInfos.end(); it++ )
	{
		if ( nParentID == it->ParentID )
		{
			return true;
		}
	}
	return false;
}
wstring wyytExtendDataManage::GetClassFullName(int64 nCID)
{
	wstring strFullName;
	vector<wstring> vecAllNames;
	GetClassAllName(nCID,vecAllNames);
	for (vector<wstring>::reverse_iterator itr = vecAllNames.rbegin(); itr != vecAllNames.rend(); itr++ )
	{
		strFullName +=(*itr) + L"-->";
	}
	//for (vector<wstring>::iterator itr = vecAllNames.begin(); itr != vecAllNames.end(); itr++ )
	//{
	//	strFullName +=(*itr) + L"-->";
	//}
	string::size_type idx = strFullName.rfind(L"-->");
	return strFullName.substr(0,idx);
}
bool wyytExtendDataManage::GetClassAllName(int64 nCID, vector<wstring>& vecAllNames)
{
	//HTREEITEM hParent_Parent = GetParentItem(hParent);
	//遍历查找直接子类
	for ( vector<stSysClassInfoItem>::iterator it = m_vecClassInfos.begin(); it != m_vecClassInfos.end(); it++ )
	{
		if ( nCID == it->ID )
		{
			vecAllNames.push_back(it->cname);
			return GetClassAllName(it->ParentID,vecAllNames);
		}
	}
	return true;
}


void wyytExtendDataManage::CancleClassInfoType(int64 nID)
{
	vector<stSysClassInfoItem>::iterator it = find_if( m_vecClassInfos.begin(), m_vecClassInfos.end(), IsDelClass(nID) );
	if (it != m_vecClassInfos.end())
	{
		it->sHaseRecomm = 0;
	}
	DataAccess::Instance().ISysClassOper->SetClassRecomm(nID, 0);
}

void wyytExtendDataManage::InitSysClassIDs()
{
	for (int i = 0; i < ROOT_CLASS_COUNTS; i++ )
	{
		m_vecSysClassInfoIDs.push_back(i);
	}
	//m_vecSysClassInfoIDs.push_back(SCII_ALL_OTHER);
	//m_vecSysClassInfoIDs.push_back(SCII_NOTES);

}
void wyytExtendDataManage::Init()
{
	DataAccess::Instance().InitDb();
}
void wyytExtendDataManage::Unit()
{
	DataAccess::Instance().UnitDb();
}
void wyytExtendDataManage::Reload()
{
	ClearClassInfoDatas();
	InitClassInfoDatas();

	ClearSysRecordInfoDatas();
	InitSysRecordInfoDatas();
}

void wyytExtendDataManage::ClearClassInfoDatas()
{
	m_vecClassInfos.clear();
}

void wyytExtendDataManage::InitClassInfoDatas()
{
	 //ExportClassInfos( m_vecClassInfos); //统一由工厂方法访问数据库接口
	//I_T_SysClassInfos_Oper* pSysClssInfoOper = DataAccess::CreateSysClassOper();
	DataAccess::Instance().ISysClassOper->GetAllClass(m_vecClassInfos);
	//delete pSysClssInfoOper;

	//stClassInfoItem stCII;
	//stCII.ID = 1;
	//stCII.ParentID = SCII_ALL_ID;
	//stCII.sHaseNode = HSN_YES;
	//////string strname = "开发语言";
	//////stCII.name = CGlobalFunction::MultiByteToWideChar(strname.c_str()/*,CP_ACP CP_OEMCPCP_UTF8*/);
	//stCII.name = L"开发语言";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 2;
	//stCII.ParentID = 1;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"C/C++";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 3;
	//stCII.ParentID = 1;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"Java";
	//m_vecClassInfos.push_back(stCII);

	///////
	//stCII.ID = 4;
	//stCII.ParentID = SCII_ALL_ID;
	//stCII.sHaseNode = HSN_YES;
	//stCII.name = L"开发库";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 5;
	//stCII.ParentID = 4;
	//stCII.sHaseNode = HSN_YES;
	//stCII.name = L"界面库";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 6;
	//stCII.ParentID = 5;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"SkinSE";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 7;
	//stCII.ParentID = 4;
	//stCII.sHaseNode = HSN_YES;
	//stCII.name = L"基础类库";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 8;
	//stCII.ParentID = 7;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"MFC";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 9;
	//stCII.ParentID = 7;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"WTL";
	//m_vecClassInfos.push_back(stCII);

	//stCII.ID = 10;
	//stCII.ParentID = 7;
	//stCII.sHaseNode = HSN_NO;
	//stCII.name = L"ATL";
	//stCII.sHaseRecomm = 1;
	//m_vecClassInfos.push_back(stCII);
	

	InitSysClassIDs();

}

void wyytExtendDataManage::ClearSysRecordInfoDatas()
{
	m_vecRecordItems.clear();
}
void wyytExtendDataManage::GetSysDataVerInfo()
{
	DataAccess::Instance().ISysClassOper->GetSysDataVerInfo(m_vecSysDataVerInfos);
}
void wyytExtendDataManage::InitSysRecordInfoDatas()
{
	//vector<CRecordItem>  vecRecordItems;
	DataAccess::Instance().ISysClassOper->GetSysRecordInfos(m_vecRecordItems);
	//for ( vector<CRecordItem>::iterator it = m_vecRecordItems.begin(); it != m_vecRecordItems.end(); it++ )
	//{
	//	it->nAllClassID.reserve(10);
	//}
	DataAccess::Instance().ISysClassOper->GetSysRecToClassInfos_DB(m_vecRecordItems);
	//for (vector<CRecordItem>::iterator it = vecRecordItems.begin(); it !=  vecRecordItems.end(); it++ )
	//{
	//	CRecordItem RI1;
	//	RI1 = (*it);
	//	m_vecRecordItems.push_back(RI1);
	//}
	//m_vecRecordItems = vecRecordItems;
	//vecRecordItems.clear();

	//CRecordItem RI1;
	//RI1.ID = 1;
	//RI1.SID = L"00000102000001000";
	//RI1.Titlel = L"111";
	//RI1.KeyValues = L"11";
	//RI1.HaseAttach = 0;
	//RI1.AttachSpaceSize =33320 ;
	//RI1.HaseRecomm = 0;
	//RI1.Content = L"tt";
	//RI1.Reference = L"11";
	//RI1.ModTime = L"2013-03-03  23:21:03";
	//RI1.InputTime = L"2013-03-03  23:21:03";
	//RI1.MainClassID = 102;
	//RI1.nAllClassID.push_back(102);
	//m_vecRecordItems.push_back(RI1);
}
void wyytExtendDataManage::ModSysRecordInfo( CRecordItem* record )
{
	DataAccess::Instance().ISysClassOper->DelSysRecToClass_DB(record->ID);
	for (vector<int64>::iterator it = record->nAllClassID.begin(); it != record->nAllClassID.end(); it++ )
	{
		DataAccess::Instance().ISysClassOper->InsertSysRecToClass_DB(record->ID,(*it));
	}
	DataAccess::Instance().ISysClassOper->ModSysRecordInfo_DB(record);
}

void wyytExtendDataManage::DelSysRecordAttachInfo(int64 nRID, bool bDelAttachFile)
{
//	DelSysRecordAttachFile();
	DataAccess::Instance().ISysClassOper->DelSysRecordAttachInfo_DB(nRID);
	map<int64, vector<CRecordAttachs>>::iterator Iter = m_mapRec2Attachs.find(nRID);
	if (Iter != m_mapRec2Attachs.end() )
	{
		//if ( bDelAttachFile )
		//{
		//	for (vector<CRecordAttachs>::iterator it = Iter->second.begin(); it != Iter->second.end(); it++ )
		//	{
		//		CString csFileFullName;
		//		csFileFullName.Format(L"%s\\%s%s",thePrefs.GetMuleDirectory(WYYT_ATTACHMENTS_SYS_DIR),it->CodeID.c_str(),it->FileSuffix.c_str());
		//		DelSysRecordAttachFile(csFileFullName);
		//	}
		//	
		//}
		Iter->second.clear();
	}
}
bool wyytExtendDataManage::DelSysRecordAttachFile(const WCHAR * srcPathFile)
{
	if(/*::access(srcPathFile,0) != -1*/ srcPathFile)
	{
		if (::DeleteFile(srcPathFile) != TRUE) 
		{
			return false;
		}
		return true;
	}
	return true;   
}
bool wyytExtendDataManage::AddSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath)
{
	return ::CopyFile(srcPath,destPath,FALSE);
}
bool wyytExtendDataManage::MoveSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath)
{
	return ::MoveFile(srcPath,destPath);
}
void wyytExtendDataManage::DelSysRecordInfo(int64 nRID)
{
	//删除记录对应的分类
	DataAccess::Instance().ISysClassOper->DelSysRecToClass_DB(nRID);
	//删除记录对应的附件
	DelSysRecordAttachInfo(nRID,true);
	  
	vector<CRecordItem>::iterator it = find_if( m_vecRecordItems.begin(), m_vecRecordItems.end(), IsDelRecord(nRID) );
	if (it != m_vecRecordItems.end())
	{
		m_vecRecordItems.erase(it);
	}
	DataAccess::Instance().ISysClassOper->DelSysRecordInfo_DB(nRID);
}
void wyytExtendDataManage::InsertSysRecordInfo( CRecordItem record )
{
	//DataAccess::Instance().ISysClassOper->GetSysRecordMaxID(record.ID); //单独调用以支持新加的数据记录获取ID以生成SID
	DataAccess::Instance().ISysClassOper->InsertSysRecordInfo_DB(record);
	DataAccess::Instance().ISysClassOper->DelSysRecToClass_DB(record.ID);
	for (vector<int64>::iterator it = record.nAllClassID.begin(); it != record.nAllClassID.end(); it++ )
	{
		DataAccess::Instance().ISysClassOper->InsertSysRecToClass_DB(record.ID,(*it));
	}
	m_vecRecordItems.push_back(record);

}

void wyytExtendDataManage::GetSysRecordMaxID(int64& nID)
{
	DataAccess::Instance().ISysClassOper->GetSysRecordMaxID(nID);
}


void wyytExtendDataManage::ClearSysRecAttachsDatas()
{
	m_mapRec2Attachs.clear();
}

void wyytExtendDataManage::InitSysRecAttachsDatas()
{
	vector<CRecordAttachs> vecAllAttachs;
	DataAccess::Instance().ISysClassOper->GetSysRecAttachsInfos_DB(vecAllAttachs);
	for ( vector<CRecordAttachs>::iterator itRA = vecAllAttachs.begin(); itRA != vecAllAttachs.end(); itRA++ )
	{
		m_mapRec2Attachs[itRA->RecordID].push_back(*itRA);
	}
	vecAllAttachs.clear();

	//CRecordAttachs cRAtemp;
	//cRAtemp.RecordID = 1;
	//cRAtemp.IndexID = 1;
	//CSID CodeID(E_SMT_SYS, 1, cRAtemp.RecordID, cRAtemp.IndexID);
	//cRAtemp.CodeID = CodeID.FiniToString();
	//cRAtemp.PathIndex = 0;
	//cRAtemp.FileName = L"CDMA技术";
	//cRAtemp.HashValue = L"cccccccsssss";
	//cRAtemp.FileSize = 10000;
	//cRAtemp.FileState = 0;//E_FS_UNDOWNED;
	//cRAtemp.FileSuffix = L".doc";
	//cRAtemp.DownloadAddress = L"www.baidu.com";
	//m_mapRec2Attachs[cRAtemp.RecordID].push_back(cRAtemp);
	//cRAtemp.IndexID = 2;
	//CodeID.Init(E_SMT_SYS, 1, cRAtemp.RecordID, cRAtemp.IndexID);
	//cRAtemp.CodeID = CodeID.FiniToString();
	//cRAtemp.PathIndex = 0;
	//cRAtemp.FileName = L"WCDMA技术";
	//cRAtemp.HashValue = L"cccccccsssssx";
	//cRAtemp.FileSize = 100001;
	//cRAtemp.FileState = 0;//E_FS_UNDOWNED;
	//cRAtemp.FileSuffix = L".rar";
	//cRAtemp.DownloadAddress = L"www.baidu.com?xxx";
	//m_mapRec2Attachs[cRAtemp.RecordID].push_back(cRAtemp);
	
}


bool wyytExtendDataManage::InsertSysRecordAttachInfo( CRecordAttachs AttInfo )
{
	//CString csFileFullName_desc;
	//csFileFullName_desc.Format(L"%s\\%s%s",thePrefs.GetMuleDirectory(WYYT_ATTACHMENTS_SYS_DIR),AttInfo.FileName.c_str(),AttInfo.FileSuffix.c_str());
	//if ( AddSysRecordAttachFile( ) )
	//{
		//添加到数据库
		DataAccess::Instance().ISysClassOper->InsertSysRecordAttachInfo_DB( AttInfo );
		//DataAccess::Instance().ISysClassOper->DelSysRecToClass_DB(record.ID);
		//for (vector<int64>::iterator it = record.nAllClassID.begin(); it != record.nAllClassID.end(); it++ )
		//{
		//	DataAccess::Instance().ISysClassOper->InsertSysRecToClass_DB(record.ID,(*it));
		//}
		//添加到内存
		m_mapRec2Attachs[AttInfo.RecordID].push_back(AttInfo);
		return true;
//	}
	return false;
	

}
