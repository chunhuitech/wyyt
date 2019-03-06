#ifndef __WYYT_DATAACCESS_H__
#define __WYYT_DATAACCESS_H__
#include "interface/DB/I_T_SysClassInfos_Oper.h"
#include "Sqlite_T_SysClassInfos_Oper.h"
#include "MySql_T_SysClassInfos_Oper.h"

enum EDBType{
	DT_Sqlite = 0,
	DT_MySql
};

class DataAccess
{
public:
	DataAccess(void);
	~DataAccess(void);
	//���ض�ϵͳ�������Ӧ�Ĳ�����ӿ�
	static I_T_SysClassInfos_Oper* CreateSysClassOper();

	//
	static DataAccess& Instance();
public:
	static	int		m_nCurrentDBType; //��ǰ���ݿ�����
	static I_T_SysClassInfos_Oper* ISysClassOper;
public:
	void InitDb();
	void UnitDb();
};

#endif