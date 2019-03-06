#ifndef __WYYT_I_T_SYSCLASSINFOS_H__
#define __WYYT_I_T_SYSCLASSINFOS_H__
#include "devlib_include.h"
#include "DbDefine.h"
#include "DataStructDefine.h"




//**********************************************************************************
// 
class I_T_SysClassInfos_Oper
{
public:
	//系统类信息
	virtual int GetAllClass(std::vector<stSysClassInfoItem>& vecClassInfos, EModuleType emt = EMT_SYS) = 0;
	virtual int InsertClass(stSysClassInfoItem cls, EModuleType emt = EMT_SYS) = 0;
	virtual int ModClass(int64 nID, wstring wstrNewClassName, EModuleType emt = EMT_SYS) = 0;
	virtual int DelClass(int64 nID, EModuleType emt = EMT_SYS) = 0;
	virtual int SetClassRecomm(int64 nID, int nRecommFlage, EModuleType emt = EMT_SYS) = 0;

	//系统类记录
	virtual int GetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS) = 0;
	virtual int GetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos) = 0;
	virtual int GetSysRecToClassInfos_DB(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS) = 0;
	virtual int GetSysRecordMaxID(int64& nID, EModuleType emt = EMT_SYS) = 0;
	virtual int InsertSysRecordInfo_DB( CRecordItem record, EModuleType emt = EMT_SYS ) = 0;
	virtual int ModSysRecordInfo_DB(CRecordItem* record, EModuleType emt = EMT_SYS) = 0;
	virtual int DelSysRecordInfo_DB(int64 nID, EModuleType emt = EMT_SYS) = 0;
	virtual int InsertSysRecToClass_DB( int64 nRecordID,  int64 nClassID, EModuleType emt = EMT_SYS) = 0;
	virtual int DelSysRecToClass_DB( int64 nRecordID, EModuleType emt = EMT_SYS) = 0;

	///系统附件
	virtual int GetSysRecAttachsInfos_DB(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt = EMT_SYS) = 0;
	virtual int InsertSysRecordAttachInfo_DB( CRecordAttachs AttInfo, EModuleType emt = EMT_SYS ) = 0;
	virtual int DelSysRecordAttachInfo_DB(int64 nID, EModuleType emt = EMT_SYS) = 0;
};

#endif