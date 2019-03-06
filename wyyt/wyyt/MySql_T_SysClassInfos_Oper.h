#ifndef __WYYT_MYSQL_T_SYSCLASSINFOS_OPER_H__
#define __WYYT_MYSQL_T_SYSCLASSINFOS_OPER_H__
#include ".\interface\db\i_t_sysclassinfos_oper.h"


class MySql_T_SysClassInfos_Oper :	public I_T_SysClassInfos_Oper
{
public:
	MySql_T_SysClassInfos_Oper(void);
	~MySql_T_SysClassInfos_Oper(void);

public:
	int GetAllClass(std::vector<stSysClassInfoItem>& vecClassInfos);
	int InsertClass(stSysClassInfoItem cls);
	int ModClass(int64 nID, wstring wstrNewClassName);
	int DelClass(int64 nID);
	int SetClassRecomm(int64 nID, int nRecommFlage);
};

#endif