#ifndef __WYYT_DBDEFINE_H__
#define __WYYT_DBDEFINE_H__

///////////////////////////ͳ�����ݿ�����������//////////////////////////////////////////////

#define DB_RESULT_OK           0   /* Successful result */
#define DB_RESULT_ERROR        1   /* SQL error or missing database */

#define DB_RESULT_NORECORDS    100 //û�в�ѯ���


enum EModuleType //ģ������
{
	EMT_SYS = 0,
	EMT_GROUP,
	EMT_PRI,
};

#endif