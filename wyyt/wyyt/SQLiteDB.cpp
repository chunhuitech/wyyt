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
		//�����ݿ��ļ�������ļ������ڲ���������ļ�
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
	//����������п��ܴ����ı�
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

	//��ȡ���е�Ҫ�����ı�ĽŲ��ļ�
	GetDBTables(vctTables);

	std::vector<std::string>::iterator itor = vctTables.begin();
	for(itor; itor != vctTables.end(); itor++)
	{
		std::string strSQL = (*itor);
		//ִ�д�����Ĳ���
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
		//ֱ��ִ��SQL��䣬�ǲ�ѯ�����SQL���
		nRet =sqlite3_exec(m_pSQLiteDB, pSQL, 0, 0, &m_pErr);
		if(nRet != SQLITE_OK)
		{	
			WriteLog("WyytSQLiteModule", "DBCommandִ�н��Ϊ%s", m_pErr);
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
		//д����������ʱҪ�õĽṹ
		sqlite3_stmt  *stat = NULL;                       

		//׼����������
		sqlite3_prepare(m_pSQLiteDB, pSQL, -1, &stat, 0);

		//�����ݺ��ֶΰ�
		sqlite3_bind_blob(stat, 1, pBuffer, nBufferSize, NULL); 

		//ִ��
		nRet = sqlite3_step(stat);   //����ֵΪ SQLITE_DONE��������
		if(SQLITE_DONE == nRet)
		{
			nRet = SQLITE_OK;
		}

		//�ͷ��ڴ�
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
		//ִ�в�ѯ
		nRet = sqlite3_get_table(m_pSQLiteDB, pSQL, pppResult, pnRow, pnColumn, &m_pErr);
		if(nRet != SQLITE_OK)
		{
			WriteLog("WyytSQLiteModule", "GetRecord�Ľ��Ϊ%s", m_pErr);		
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
		//������������ʱҪ�õĽṹ
		sqlite3_stmt *stat = NULL;

		//׼����ȡ����
		sqlite3_prepare(m_pSQLiteDB, pSQL, -1, &stat, 0 );

		//ִ��
		nRet = sqlite3_step(stat);//����ֵΪSQLITE_ROW��������
		if(SQLITE_ROW == nRet)
		{			
			//��Ӧ���������ݵ��ֽڳ���
			nLength = sqlite3_column_bytes(stat, nColumn);

			void *pData = malloc(nLength);
			(*ppData) = pData;

			//ȡ��Ӧ�ֶεĶ���������
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
	int nRet = sqlite3_exec(m_pSQLiteDB, "begin transaction", 0, 0, &m_pErr); //��ʼһ������
	if(nRet != SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "BeginTransʧ�ܣ�%s", m_pErr);
	}
	return nRet;
}

int CSQLiteDB::CommitTrans()
{
	int nRet = sqlite3_exec(m_pSQLiteDB, "commit transaction", 0, 0, &m_pErr); //�ݽ�һ������
	if(nRet != SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "CommitTransʧ�ܣ�%s", m_pErr);
	}
	return nRet;
}

int CSQLiteDB::RollbackTrans()
{
	int nRet = sqlite3_exec(m_pSQLiteDB, "rollback transaction", 0, 0, &m_pErr); //�ع�һ������
	if(nRet !=  SQLITE_OK)
	{
		WriteLog("WyytSQLiteModule", "RollbackTransʧ�ܣ�%s", m_pErr);		
	}
	return nRet;
}

int CSQLiteDB::DBClose()
{
	//�ر����ݿ�
	return sqlite3_close(m_pSQLiteDB);	
}