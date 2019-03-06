#ifndef __WYYT_SQLITE_T_SYSCLASSINFOS_OPER_H__
#define __WYYT_SQLITE_T_SYSCLASSINFOS_OPER_H__

#include ".\interface\db\i_t_sysclassinfos_oper.h"


class Sqlite_T_SysClassInfos_Oper :	public I_T_SysClassInfos_Oper
{
public:
	Sqlite_T_SysClassInfos_Oper(void);
	~Sqlite_T_SysClassInfos_Oper(void);

public:
	int GetAllClass(std::vector<stSysClassInfoItem>& vecClassInfos, EModuleType emt = EMT_SYS);
	int InsertClass(stSysClassInfoItem cls, EModuleType emt = EMT_SYS);
	int ModClass(int64 nID, wstring wstrNewClassName, EModuleType emt = EMT_SYS);
	int DelClass(int64 nID, EModuleType emt = EMT_SYS);
	int SetClassRecomm(int64 nID, int nRecommFlage, EModuleType emt = EMT_SYS);

	//系统类记录
	int GetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
	int GetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos);
	int GetSysRecToClassInfos_DB(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
	int GetSysRecordMaxID(int64& nID, EModuleType emt = EMT_SYS);
	int InsertSysRecordInfo_DB( CRecordItem record , EModuleType emt = EMT_SYS);
	int ModSysRecordInfo_DB(CRecordItem* record, EModuleType emt = EMT_SYS);
	int InsertSysRecToClass_DB( int64 nRecordID,  int64 nClassID, EModuleType emt = EMT_SYS);
	int DelSysRecordInfo_DB(int64 nID, EModuleType emt = EMT_SYS);
	int DelSysRecToClass_DB( int64 nRecordID, EModuleType emt = EMT_SYS);
	///系统附件
	int GetSysRecAttachsInfos_DB(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt = EMT_SYS);
	int InsertSysRecordAttachInfo_DB( CRecordAttachs AttInfo , EModuleType emt = EMT_SYS);
	int DelSysRecordAttachInfo_DB(int64 nID, EModuleType emt = EMT_SYS);
};


#endif