#include "StdAfx.h"
#include "SQLiteDB.h"
#include <malloc.h>
#include <assert.h>

BOOL WriteLog(const char *pszModuleName, const char *pszFormat, ...)
{
	return TRUE;
}


CSQLiteDB::CSQLiteDB(void)
:m_pSQLiteDB(NULL)
{
}

CSQLiteDB::~CSQLiteDB(void)
{
}


int CSQLiteDB::DBOpen(const char *pszName)
{
	assert(pszName);

	int nRet = -1;
	if(pszName)
	{
		//打开数据库文件，如果文件不存在不创建这个文件
		nRet = sqlite3_open_v2(pszName, &m_pSQLiteDB, SQLITE_OPEN_READWRITE, NULL);
		if(m_pSQLiteDB)
		{
			//sqlite3_key(m_pSQLiteDB, "123", 16);
		}
		//nRet = sqlite3_open(pszName, &m_pSQLiteDB);
	}

	return nRet;
}

void CSQLiteDB::GetDBTables(std::vector<std::string> &vctTables)
{
	//这里加载所有可能创建的表
	std::string str = "CREATE TABLE BioWinUser(WinUser varchar(64)  NOT NULL, Password varchar(64), FilePass varchar(64) NOT NULL, BakPass varchar(64), primary key(WinUser) )";
	vctTables.push_back(str);
	str = "CREATE TABLE BioUserFP(WinUser varchar(64)  NOT NULL, FPIndex int  NOT NULL, FP blob NOT NULL, AppLanuchPath varchar(1024) COLLATE NOCASE, primary key(WinUser, FPIndex) )";
	vctTables.push_back(str);
	str = "CREATE TABLE BioAppInfo(WinUser varchar(64)  NOT NULL, Identifier varchar(1024)  NOT NULL, ItemName varchar(1024)  NOT NULL,  Ctrls blob NOT NULL, primary key(WinUser, Identifier, ItemName) )";
	vctTables.push_back(str);
}

void CSQLiteDB::CreateTables()
{
	std::vector<std::string> vctTables;

	//获取所有的要创建的表的脚步文件
	GetDBTables(vctTables);

	std::vector<std::string>::iterator itor = vctTables.begin();
	for(itor; itor != vctTables.end(); itor++)
	{
		std::string strSQL = (*itor);
		//执行创建表的操作
		int nRes = DBCommand(strSQL.c_str() );

		WriteLog("BioSQLite", "CreateTables:%d", nRes);
	}
}

int CSQLiteDB::DBCommand(const char *pSQL)
{
	assert(pSQL);

	int nRet = -1;
	if(pSQL)
	{
		//直接执行SQL语句，非查询以外的SQL语句
		nRet =sqlite3_exec(m_pSQLiteDB, pSQL, 0, 0, &m_pErr);
		if(nRet != SQLITE_OK)
		{	
			WriteLog("WyytSQLiteModule", "DBCommand执行结果为%s", m_pErr);
		}
	}
	return nRet;
}

int CSQLiteDB::DBInsertBlob(const char *pSQL,
							const void *pBuffer,
							const int nBufferSize)
{
	assert(pSQL && pBuffer);

	AcquireWrite();

	int nRet = -1;
	if(pSQL && pBuffer)
	{
		//写二进制数据时要用的结构
		sqlite3_stmt  *stat = NULL;                       

		//准备插入数据
		sqlite3_prepare(m_pSQLiteDB, pSQL, -1, &stat, 0);

		//把内容和字段绑定
		sqlite3_bind_blob(stat, 1, pBuffer, nBufferSize, NULL); 

		//执行
		nRet = sqlite3_step(stat);   //返回值为 SQLITE_DONE才算正常
		if(SQLITE_DONE == nRet)
		{
			nRet = SQLITE_OK;
		}

		//释放内存
		sqlite3_finalize(stat);
	}

	ReleaseWrite();

	return nRet;
}

int CSQLiteDB::GetRecord(const char *pSQL,
						 char ***pppResult,
						 int *pnRow,
						 int *pnColumn)
{
	assert(pSQL && pppResult && pnRow && pnColumn);

	AcquireRead();

	int nRet = -1;
	if(pSQL && pppResult && pnRow && pnColumn)
	{
		//执行查询
		nRet = sqlite3_get_table(m_pSQLiteDB, pSQL, pppResult, pnRow, pnColumn, &m_pErr);
		if(nRet != SQLITE_OK)
		{
			WriteLog("WyytSQLiteModule", "GetRecord的结果为%s", m_pErr);		
		}	
	}

	ReleaseRead();

	return nRet;
}

void CSQLiteDB::FreeTable(/*[in]*/ char **ppResult)
{
	sqlite3_free_table(ppResult);
}

int CSQLiteDB::ReadBlob(const char *pSQL,
						const int nColumn,
						void **ppData, 
						int &nLength)
{
	assert(pSQL);

	AcquireRead();	

	int nRet = -1;
	if(pSQL)
	{
		//读二进制数据时要用的结构
		sqlite3_stmt *stat = NULL;

		//准备读取数据
		sqlite3_prepare(m_pSQLiteDB, pSQL, -1, &stat, 0 );

		//执行
		nRet = sqlite3_step(stat);//返回值为SQLITE_ROW才算正常
		if(SQLITE_ROW == nRet)
		{			
			//对应二进制数据的字节长度
			nLength = sqlite3_column_bytes(stat, nColumn);

			void *pData = malloc(nLength);
			(*ppData) = pData;

			//取对应字段的二进制数据
			memcpy(pData ,sqlite3_column_blob(stat, nColumn), nLength);

			sqlite3_finalize(stat); 

			nRet = SQLITE_OK;
		}		
	}

	ReleaseRead();

	return nRet ;
}

void CSQLiteDB::FreeBlob(/*[in]*/ LPVOID &lpData)
{
	assert(lpData);

	if(lpData)
	{
		free(lpData);
		lpData = NULL;
	}
}


int CSQLiteDB::BeginTrans()
{
	int nRet = sqlite3_exec(m_pSQLiteDB, "begin transaction", 0, 0, &m_pErr); //开始一个事务
	if(nRet != SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "BeginTrans失败：%s", m_pErr);
	}
	return nRet;
}

int CSQLiteDB::CommitTrans()
{
	int nRet = sqlite3_exec(m_pSQLiteDB, "commit transaction", 0, 0, &m_pErr); //递交一个事务
	if(nRet != SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "CommitTrans失败：%s", m_pErr);
	}
	return nRet;
}

int CSQLiteDB::RollbackTrans()
{
	int nRet = sqlite3_exec(m_pSQLiteDB, "rollback transaction", 0, 0, &m_pErr); //回滚一个事务
	if(nRet !=  SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "RollbackTrans失败：%s", m_pErr);		
	}
	return nRet;
}

int CSQLiteDB::DBClose()
{
	//关闭数据库
	return sqlite3_close(m_pSQLiteDB);	
}