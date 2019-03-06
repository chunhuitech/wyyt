#include "StdAfx.h"
#include "Sqlite_T_SysClassInfos_Oper.h"
#include "ExportMange.h" //"../dbSqlite/ExportMange.h"

Sqlite_T_SysClassInfos_Oper::Sqlite_T_SysClassInfos_Oper(void)
{
}


Sqlite_T_SysClassInfos_Oper::~Sqlite_T_SysClassInfos_Oper(void)
{

}
int Sqlite_T_SysClassInfos_Oper::GetSysRecToClassInfos_DB(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = GetSysRecToClassInfos_DLL(vecSysRecordInfos, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::GetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = ExportGetSysRecordInfos(vecSysRecordInfos, emt);
	return res;
}
 int Sqlite_T_SysClassInfos_Oper::GetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos)
 {
	 int res = DB_RESULT_ERROR;
	 res = ExportGetSysDataVerInfo(vecSysVerInfos);
	 return res;
 }
int Sqlite_T_SysClassInfos_Oper::GetSysRecordMaxID(int64& nID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = ExportGetSysRecordMaxID(nID, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::GetAllClass(std::vector<stSysClassInfoItem>& vecClassInfos, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = ExportSysClassInfos(vecClassInfos, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::InsertClass(stSysClassInfoItem cls, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = AddSysClassInfos(cls, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::ModClass(int64 nID, wstring wstrNewClassName, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = ModSysClassInfos(nID, wstrNewClassName, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::ModSysRecordInfo_DB(CRecordItem* record, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	//res = DelSysRecToClass_DLL(record->ID);
	//InsertSysRecToClass_DLL(record->ID,record)
	res = ModSysRecordInfo_DLL(record, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::DelSysRecordInfo_DB(int64 nID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = DelSysRecordInfo_DLL(nID, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::DelSysRecordAttachInfo_DB(int64 nID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = DelSysRecordAttachInfo_DLL(nID, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::DelClass(int64 nID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = DelSysClassInfos(nID, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::SetClassRecomm(int64 nID, int nRecommFlage, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = SetSysClassRecomm(nID, nRecommFlage, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::InsertSysRecordInfo_DB( CRecordItem record, EModuleType emt )
{
	int res = DB_RESULT_ERROR;
	res = InsertSysRecordInfo_DLL(record, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::InsertSysRecordAttachInfo_DB( CRecordAttachs AttInfo, EModuleType emt )
{
	int res = DB_RESULT_ERROR;
	res = InsertSysRecordAttachInfo_DLL( AttInfo, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::InsertSysRecToClass_DB( int64 nRecordID,  int64 nClassID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = InsertSysRecToClass_DLL(nRecordID,nClassID, emt);
	return res;
}
int Sqlite_T_SysClassInfos_Oper::DelSysRecToClass_DB( int64 nRecordID, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = DelSysRecToClass_DLL(nRecordID, emt);
	return res;
}

int Sqlite_T_SysClassInfos_Oper::GetSysRecAttachsInfos_DB(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt)
{
	int res = DB_RESULT_ERROR;
	res = GetSysRecAttachsInfos_DLL(vecAllAttachs, emt);
	return res;
}