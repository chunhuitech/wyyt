#include "StdAfx.h"
#include "DataAccess.h"
#include "ExportMange.h" //../dbSqlite/ExportMange.h
#include "Preferences.h"


int		DataAccess::m_nCurrentDBType = DT_Sqlite;
I_T_SysClassInfos_Oper* DataAccess::ISysClassOper = NULL;
DataAccess::DataAccess(void)
{
}


DataAccess::~DataAccess(void)
{
	delete ISysClassOper;
}

DataAccess& DataAccess::Instance()
{
	static DataAccess s_DataAccess;
	s_DataAccess.CreateSysClassOper();
	return s_DataAccess;
}

void DataAccess::InitDb()
{
	CString strdbPath = thePrefs.GetMuleDirectory(WYYT_DBSDIR);
	if (strdbPath.Right(1) != _T('\\'))
		strdbPath += L"\\wyyt.db";
	else
		strdbPath += L"wyyt.db";
	wstring wdbPath =strdbPath.GetString(); strdbPath.GetBuffer(strdbPath.GetLength());

	CString strdbPath_Pri = thePrefs.GetMuleDirectory(WYYT_DBSDIR);
	if (strdbPath_Pri.Right(1) != _T('\\'))
		strdbPath_Pri += L"\\wyyt_pri.db";
	else
		strdbPath_Pri += L"wyyt_pri.db";
	wstring wdbPath_Pri =strdbPath_Pri.GetString(); strdbPath_Pri.GetBuffer(strdbPath_Pri.GetLength());

	InitSQLiteModule(wdbPath,wdbPath_Pri);
}
void DataAccess::UnitDb()
{
	UnitSQLiteModule();
}
I_T_SysClassInfos_Oper* DataAccess::CreateSysClassOper()
{
	if ( ISysClassOper != NULL )
	{
		return ISysClassOper;
	}
	//下面创建后　注意找适当的时间进行释放
	switch( m_nCurrentDBType )
	{
	case DT_Sqlite:
		{
			ISysClassOper = new Sqlite_T_SysClassInfos_Oper();
		}
		break;
	case DT_MySql:
		{
			//ISysClassOper = new MySql_T_SysClassInfos_Oper();
		}
		break;
	}
	return ISysClassOper;
}