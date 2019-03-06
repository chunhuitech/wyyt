#include "StdAfx.h"
#include "MySql_T_SysClassInfos_Oper.h"


MySql_T_SysClassInfos_Oper::MySql_T_SysClassInfos_Oper(void)
{
}


MySql_T_SysClassInfos_Oper::~MySql_T_SysClassInfos_Oper(void)
{
}

int MySql_T_SysClassInfos_Oper::GetAllClass(std::vector<stSysClassInfoItem>& vecClassInfos)
{
	int res = DB_RESULT_ERROR;
	return res;
}
int MySql_T_SysClassInfos_Oper::InsertClass(stSysClassInfoItem cls)
{
	int res = DB_RESULT_ERROR;

	return res;
}
int MySql_T_SysClassInfos_Oper::ModClass(int64 nID, wstring wstrNewClassName)
{
	int res = DB_RESULT_ERROR;

	return res;
}
int MySql_T_SysClassInfos_Oper::DelClass(int64 nID)
{
	int res = DB_RESULT_ERROR;
	return res;
}
int MySql_T_SysClassInfos_Oper::SetClassRecomm(int64 nID, int nRecommFlage)
{
	int res = DB_RESULT_ERROR;
	return res;
}