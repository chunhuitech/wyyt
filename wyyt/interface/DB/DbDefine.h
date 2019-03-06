#ifndef __WYYT_DBDEFINE_H__
#define __WYYT_DBDEFINE_H__

///////////////////////////统用数据库操作结果定义//////////////////////////////////////////////

#define DB_RESULT_OK           0   /* Successful result */
#define DB_RESULT_ERROR        1   /* SQL error or missing database */

#define DB_RESULT_NORECORDS    100 //没有查询结果


enum EModuleType //模块类型
{
	EMT_SYS = 0,
	EMT_GROUP,
	EMT_PRI,
};

#endif