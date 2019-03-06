#include "StdAfx.h"
#include "ExportMange.h"
#include "SQLiteDB.h"

static CSQLiteDB *g_pSQLiteDB = NULL;  //声明一个到SQLite数据库的对象
static CSQLiteDB *g_pSQLiteDB_Pri = NULL;  //声明一个Pri到SQLite数据库的对象

int InitSQLiteModule(/*[in]*/const std::wstring &strDBFile, const std::wstring &strDBFile_Pri)
{
	int nRet = DB_RESULT_ERROR;
	if(!g_pSQLiteDB)
	{
		g_pSQLiteDB = new CSQLiteDB;

		std::string strFile = wstrToUtf8(ReplaceAllQuote(strDBFile) );		
		nRet = g_pSQLiteDB->DBOpen(strFile.c_str() );
		//g_pSQLiteDB->CreateTables();
	}
	nRet = DB_RESULT_ERROR;
	if(!g_pSQLiteDB_Pri)
	{
		g_pSQLiteDB_Pri = new CSQLiteDB;

		std::string strFile = wstrToUtf8(ReplaceAllQuote(strDBFile_Pri) );		
		nRet = g_pSQLiteDB_Pri->DBOpen(strFile.c_str() );
		//g_pSQLiteDB->CreateTables();
	}
	return nRet;
}

int UnitSQLiteModule()
{
	int nRet = DB_RESULT_ERROR;
	if(g_pSQLiteDB)
	{
		nRet = g_pSQLiteDB->DBClose();
		delete g_pSQLiteDB;
		g_pSQLiteDB = NULL;
	}
	nRet = DB_RESULT_ERROR;
	if(g_pSQLiteDB_Pri)
	{
		nRet = g_pSQLiteDB_Pri->DBClose();
		delete g_pSQLiteDB_Pri;
		g_pSQLiteDB_Pri = NULL;
	}
	return nRet;
}
int InsertSysRecordAttachInfo_DLL( CRecordAttachs& AttInfo , EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( !AttInfo.FileName.empty() ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecAttachs",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecAttachs",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"insert into %s(RecordID,IndexID,CodeID,PathIndex,SrcPath,FileName,HashValue,FileSize,FileState,FileSuffix,DownloadAddress,Note,InputTime) values(%lld,%d,'%s',%d,'%s','%s','%s',%lld,%d,'%s','%s','%s',strftime('%%Y-%%m-%%d  %%H:%%M:%%S','now','localtime'));",szTableName,AttInfo.RecordID, AttInfo.IndexID,wstrToUtf8(ReplaceAllQuote(AttInfo.CodeID) ).c_str(),AttInfo.PathIndex,wstrToUtf8(ReplaceAllQuote(AttInfo.SrcPath) ).c_str(), wstrToUtf8(ReplaceAllQuote(AttInfo.FileName) ).c_str(), wstrToUtf8(ReplaceAllQuote(AttInfo.HashValue) ).c_str(),AttInfo.FileSize,AttInfo.FileState,wstrToUtf8(ReplaceAllQuote(AttInfo.FileSuffix) ).c_str(),wstrToUtf8(ReplaceAllQuote(AttInfo.DownloadAddress) ).c_str(),wstrToUtf8(ReplaceAllQuote(AttInfo.Note) ).c_str());
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int InsertSysRecordInfo_DLL(CRecordItem& record , EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( !record.Titlel.empty() ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecords",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecords",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};//
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"insert into %s(ID,CodeID,MainClassID,Title,KeyValues,HaseAttach,AttachSpaceSize,HaseRecomm,Content,ModFlag,Reference,ModTime,InputTime) values(%lld,'%s',%lld,'%s','%s',%d,%lld,%d,'%s',%d,'%s',strftime('%%Y-%%m-%%d  %%H:%%M:%%S','now','localtime'),strftime('%%Y-%%m-%%d  %%H:%%M:%%S','now','localtime'));",szTableName,record.ID, wstrToUtf8(ReplaceAllQuote(record.SID) ).c_str(),record.MainClassID, wstrToUtf8(ReplaceAllQuote(record.Titlel) ).c_str(), wstrToUtf8(ReplaceAllQuote(record.KeyValues) ).c_str(),record.HaseAttach,record.AttachSpaceSize,record.HaseRecomm,wstrToUtf8(ReplaceAllQuote(record.Content) ).c_str(),0,wstrToUtf8(ReplaceAllQuote(record.Reference) ).c_str());
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int InsertSysRecToClass_DLL( int64 nRecordID,  int64 nClassID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nRecordID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecToClass",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecToClass",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"insert into %s(RecordID,ClassID) values(%lld,%lld);",szTableName,nRecordID,nClassID);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int AddSysClassInfos(stSysClassInfoItem& SysClassInfos, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
//
	if( !SysClassInfos.cname.empty() ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysClassInfos",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriClassInfos",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"insert into %s(ID,ParentID, HaseNode, HaseRecomm,	HaseCollect,CName,EName,Note,InputTime) values(%lld, %lld, %d, %d, %d, '%s','%s','%s',strftime('%%Y-%%m-%%d  %%H:%%M:%%S','now','localtime'));",szTableName,SysClassInfos.ID,SysClassInfos.ParentID, SysClassInfos.sHaseNode, SysClassInfos.sHaseRecomm,SysClassInfos.sHaseCollect,
			wstrToUtf8(ReplaceAllQuote(SysClassInfos.cname) ).c_str(), 
			wstrToUtf8(ReplaceAllQuote(SysClassInfos.ename) ).c_str(),
			wstrToUtf8(ReplaceAllQuote(SysClassInfos.Note)).c_str()  
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int SetSysClassRecomm(int64 nID, int nRecommFlage, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysClassInfos",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriClassInfos",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"update %s set HaseRecomm=%d where ID = %lld;",szTableName,
			nRecommFlage,
			nID  
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int DelSysRecToClass_DLL( int64 nRecordID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nRecordID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecToClass",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecToClass",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"delete  from %s where RecordID = %lld;",szTableName,nRecordID
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int DelSysClassInfos(int64 nID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysClassInfos",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PridClassInfos",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"delete  from %s where ID = %lld;",szTableName,nID
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int DelSysRecordAttachInfo_DLL(int64 nID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecAttachs",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecAttachs",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"delete  from %s  where RecordID = %lld;",szTableName,nID
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int DelSysRecordInfo_DLL(int64 nID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( nID != 0 ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecords",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecords",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"delete  from %s where ID = %lld;",szTableName,nID
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int ModSysRecordInfo_DLL(CRecordItem* record, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( !record->Titlel.empty() ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecords",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecords",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"update %s set Title='%s',KeyValues='%s',HaseAttach=%d,AttachSpaceSize=%lld,HaseRecomm=%d,Content='%s',ModFlag=%d,MainClassID=%lld,Reference='%s',ModTime=strftime('%%Y-%%m-%%d  %%H:%%M:%%S','now','localtime') where ID = %lld;",szTableName,
			wstrToUtf8(ReplaceAllQuote(record->Titlel) ).c_str(),wstrToUtf8(ReplaceAllQuote(record->KeyValues) ).c_str(),record->HaseAttach,record->AttachSpaceSize,record->HaseRecomm,wstrToUtf8(ReplaceAllQuote(record->Content) ).c_str(),1,record->MainClassID,wstrToUtf8(ReplaceAllQuote(record->Reference) ).c_str(),record->ID );
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int ModSysClassInfos(int64 nID, const wstring& wstrNewClassName, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;
	//
	if( !wstrNewClassName.empty() ) 
	{
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysClassInfos",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriClassInfos",sizeof(szTableName));
		}
		char szSQL[MAX_SQL_SIZE] = {0};
		sprintf_s(
			szSQL,
			MAX_SQL_SIZE,
			"update %s set CName='%s' where ID = %lld;",szTableName,
			wstrToUtf8(ReplaceAllQuote(wstrNewClassName) ).c_str(),
			nID  
			);
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->DBCommand(szSQL);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->DBCommand(szSQL);
		}
	}
	return nRet;
}
int ExportGetSysRecordMaxID(int64& nID, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;

		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecords",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecords",sizeof(szTableName));
		}
		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select ifnull(max(ID),0)+1 as MaxID from %s",szTableName
			);

		// 查询记录条数
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);	
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->GetRecord(pBuf, &dbResult, &nRow, &nColumn);	
		}
		
		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			nID = _atoi64(dbResult [index++]);
		}

		if( dbResult  )
		{
			if ( EMT_SYS == emt )
			{
				g_pSQLiteDB->FreeTable( dbResult );
			}
			else
			{
				g_pSQLiteDB_Pri->FreeTable( dbResult );
			}
			
			dbResult = NULL;
		}

	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		if ( EMT_SYS == emt )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		}
		else
		{
			g_pSQLiteDB_Pri->FreeTable( dbResult );
		}
		
		dbResult = NULL;
	}
	return nRet;
}
int GetSysRecToClassInfos_DLL(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecToClass",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecToClass",sizeof(szTableName));
		}
		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select RecordID,ClassID from %s",szTableName
			);

		// 查询记录条数
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);	
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->GetRecord(pBuf, &dbResult, &nRow, &nColumn);	
		}		
		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			int64 nRecordID = _atoi64(dbResult [index++]);
			int64 nClassID = _atoi64(dbResult [index++]);
			for (vector<CRecordItem>::iterator it = vecSysRecordInfos.begin(); it != vecSysRecordInfos.end(); it++ )
			{
				if ( nRecordID == it->ID )
				{
					it->nAllClassID.push_back(nClassID);
					break;
				}
			}
		}

		if( dbResult  )
		{
			if ( EMT_SYS == emt )
			{
				g_pSQLiteDB->FreeTable( dbResult );
			}
			else
			{
				g_pSQLiteDB_Pri->FreeTable( dbResult );
			}
			
			dbResult = NULL;
		}

	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		if ( EMT_SYS == emt )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		}
		else
		{
			g_pSQLiteDB_Pri->FreeTable( dbResult );
		}
		dbResult = NULL;
	}
	//dll中需要这里用投机的方法，动态用非法类ID把nAllClassID容器增加到SUBCLASS_MAXCOUNT,以容纳用户的设置，否则若用reserve(10)的方法，则会在释放时报错
	//int nInitSize = 0;
	//for (vector<CRecordItem>::iterator it = vecSysRecordInfos.begin(); it != vecSysRecordInfos.end(); it++ )
	//{
	//	nInitSize = it->nAllClassID.size();
	//	for ( int i = 0; i < (SUBCLASS_MAXCOUNT - nInitSize ); i++ )
	//	{
	//		it->nAllClassID.push_back(ERROR_CLASSID);
	//	}
	//	
	//	
	//}
	return nRet;
}
int GetSysRecAttachsInfos_DLL(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;

		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecAttachs",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecAttachs",sizeof(szTableName));
		}
		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select RecordID,IndexID,CodeID,PathIndex,SrcPath,FileName,HashValue,FileSize,FileState,FileSuffix,DownloadAddress,Note,InputTime from %s",szTableName
			);

		// 查询记录条数
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
			
		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			CRecordAttachs stRA;
			stRA.RecordID = _atoi64(dbResult [index++]);
			stRA.IndexID = atoi(dbResult [index++]);
			stRA.CodeID = utf8toWStr(dbResult [index++]);
			stRA.PathIndex = atoi(dbResult [index++]);
			stRA.SrcPath =  dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRA.FileName =  dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRA.HashValue = utf8toWStr(dbResult [index++]);
			stRA.FileSize = _atoi64(dbResult [index++]);
			stRA.FileState = atoi(dbResult [index++]);
			stRA.FileSuffix = dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRA.DownloadAddress = dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRA.Note = utf8toWStr(dbResult [index++]);
			stRA.InputTime = utf8toWStr(dbResult [index++]);
			vecAllAttachs.push_back(stRA);
		}

		if( dbResult  )
		{
			if ( EMT_SYS == emt )
			{
				g_pSQLiteDB->FreeTable( dbResult );
			}
			else
			{
				g_pSQLiteDB_Pri->FreeTable( dbResult );
			}
			
			dbResult = NULL;
		}

	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		if ( EMT_SYS == emt )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		}
		else
		{
			g_pSQLiteDB_Pri->FreeTable( dbResult );
		}
		dbResult = NULL;
	}
	return nRet;
}
int ExportGetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos)
{
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;
		char szTableName[50]={0};
		strncpy(szTableName,"T_SysDataUpdateList",sizeof(szTableName));
		
		

		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select ID,Version,VerUpDate,Notes,InputTime from %s order by InputTime desc",szTableName
			);

		// 查询记录条数
		nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		

		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			stSysDataVerInfoItem stRI;
			stRI.ID = _atoi64(dbResult [index++]);
			stRI.Version = utf8toWStr(dbResult [index++]);
			stRI.VerUpDate = utf8toWStr(dbResult [index++]);
			stRI.Notes = utf8toWStr(dbResult [index++]);
			stRI.InputTime = utf8toWStr(dbResult [index++]);
			//stRI.nAllClassID.reserve(10);
			vecSysVerInfos.push_back(stRI);
		}

		if( dbResult  )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		

			dbResult = NULL;
		}

	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		g_pSQLiteDB->FreeTable( dbResult );
		
		dbResult = NULL;
	}
	return nRet;
}
int ExportGetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt )
{
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysRecords",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriRecords",sizeof(szTableName));
		}
		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select ID,CodeID,Title,KeyValues,HaseAttach,AttachSpaceSize,HaseRecomm,Content,MainClassID,Reference,ModTime,InputTime from %s order by ModTime desc",szTableName
			);

		// 查询记录条数
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
			
		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			CRecordItem stRI;
			stRI.ID = _atoi64(dbResult [index++]);
			stRI.SID = utf8toWStr(dbResult [index++]);
			stRI.Titlel = utf8toWStr(dbResult [index++]);
			stRI.KeyValues = dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRI.HaseAttach = atoi(dbResult [index++]);
			stRI.AttachSpaceSize = _strtoui64(dbResult [index++],NULL, 10);
			stRI.HaseRecomm =atoi(dbResult [index++]);
			stRI.Content = dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRI.MainClassID = _atoi64(dbResult [index++]);
			stRI.Reference = dbResult [index] ? utf8toWStr(dbResult [index++]) : L"";
			stRI.ModTime = utf8toWStr(dbResult [index++]);
			stRI.InputTime = utf8toWStr(dbResult [index++]);
			//stRI.nAllClassID.reserve(10);
			vecSysRecordInfos.push_back(stRI);
		}

		if( dbResult  )
		{
			if ( EMT_SYS == emt )
			{
				g_pSQLiteDB->FreeTable( dbResult );
			}
			else
			{
				g_pSQLiteDB_Pri->FreeTable( dbResult );
			}
			
			dbResult = NULL;
		}

	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		if ( EMT_SYS == emt )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		}
		else
		{
			g_pSQLiteDB_Pri->FreeTable( dbResult );
		}
		dbResult = NULL;
	}
	return nRet;
}

int ExportSysClassInfos(std::vector<stSysClassInfoItem>& vecSysClassInfos, EModuleType emt )
{
	
	int nRet = DB_RESULT_ERROR;	

	int  nRow = 0, nColumn = 0, index = 0;	
	char **dbResult = NULL;


	char * pBuf = NULL;
	int iBufLen = MAX_SQL_SIZE;

	do 
	{
		pBuf = new char[ iBufLen ];
		if( NULL == pBuf )
			break;
		char szTableName[50]={0};
		if ( EMT_SYS == emt )
		{
			strncpy(szTableName,"T_SysClassInfos",sizeof(szTableName));
		}
		else
		{
			strncpy(szTableName,"T_PriClassInfos",sizeof(szTableName));
		}
		// 先求元素个数
		sprintf_s( 	pBuf, iBufLen,
			"select ID,ParentID,HaseNode,HaseRecomm,HaseCollect,CName,EName,Note from %s",szTableName
			);

		// 查询记录条数
		if ( EMT_SYS == emt )
		{
			nRet = g_pSQLiteDB->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
		else
		{
			nRet = g_pSQLiteDB_Pri->GetRecord(pBuf, &dbResult, &nRow, &nColumn);
		}
			
		if( SQLITE_OK != nRet )
		{
			break;
		}
		if( nRow == 0 )
		{
			nRet = DB_RESULT_NORECORDS;
			break;
		}
		index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
		//printf( "查到%d条记录\n", nRow );
		for(int i=0; i<nRow; i++)
		{
			//printf( "第 %d 条记录\n", i );
			//for( j = 0 ; j < nColumn; j++ )
			//{
			//	//printf( "字段名:%s ?> 字段值:%s\n", dbResult[j], dbResult [index] );
			//	++index; // dbResult 的字段值是连续的，从第0索引到第 nColumn - 1索引都是字段名称，从第 nColumn 索引开始，后面都是字段值，它把一个二维的表（传统的行列表示法）用一个扁平的形式来表示
			//}
			stSysClassInfoItem stSCI;
			stSCI.ID = atoi(dbResult [index++]);
			stSCI.ParentID = atoi(dbResult [index++]);
			stSCI.sHaseNode = atoi(dbResult [index++]);
			stSCI.sHaseRecomm = atoi(dbResult [index++]);
			stSCI.sHaseCollect = atoi(dbResult [index++]);
			stSCI.cname = utf8toWStr(dbResult [index++]);
			stSCI.ename = utf8toWStr(dbResult [index++]);
			stSCI.Note = utf8toWStr(dbResult [index++]);
			vecSysClassInfos.push_back(stSCI);
		}

		if( dbResult  )
		{
			if ( EMT_SYS == emt )
			{
				g_pSQLiteDB->FreeTable( dbResult );
			}
			else
			{
				g_pSQLiteDB_Pri->FreeTable( dbResult );
			}
			
			dbResult = NULL;
		}
		
	} while ( FALSE );

	if( pBuf )
	{
		delete [] pBuf;
		pBuf = NULL;
	}
	if( dbResult  )
	{
		if ( EMT_SYS == emt )
		{
			g_pSQLiteDB->FreeTable( dbResult );
		}
		else
		{
			g_pSQLiteDB_Pri->FreeTable( dbResult );
		}
		dbResult = NULL;
	}
	return nRet;
	//stSysClassInfoItem stSCI;
	//stSCI.ID = 1;
	//stSCI.ParentID = SCII_ALL_ID;
	//stSCI.sHaseNode = HSN_YES;
	//stSCI.cname = L"开发语言";
	//vecSysClassInfos.push_back(stSCI);

	//stSCI.ID = 2;
	//stSCI.ParentID = 1;
	//stSCI.sHaseNode = HSN_NO;
	//stSCI.cname = L"C/C++";
	//vecSysClassInfos.push_back(stSCI);
	//UnitSQLiteModule();
	//return DB_RESULT_OK;
}

int ExportClassInfos(std::vector<stSysClassInfoItem>& m_vecClassInfos)
{
	stSysClassInfoItem stCII;
	stCII.ID = 1;
	stCII.ParentID = SCII_ALL_OTHER_ID;
	stCII.sHaseNode = HSN_YES;
	//string strname = "开发语言";
	//stCII.name = CGlobalFunction::MultiByteToWideChar(strname.c_str()/*,CP_ACP CP_OEMCPCP_UTF8*/);
	stCII.cname = L"开发语言";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 2;
	stCII.ParentID = 1;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"C/C++";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 3;
	stCII.ParentID = 1;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"Java";
	m_vecClassInfos.push_back(stCII);

	/////
	stCII.ID = 4;
	stCII.ParentID = SCII_ALL_OTHER_ID;
	stCII.sHaseNode = HSN_YES;
	stCII.cname = L"开发库";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 5;
	stCII.ParentID = 4;
	stCII.sHaseNode = HSN_YES;
	stCII.cname = L"界面库";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 6;
	stCII.ParentID = 5;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"SkinSE";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 7;
	stCII.ParentID = 4;
	stCII.sHaseNode = HSN_YES;
	stCII.cname = L"基础类库";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 8;
	stCII.ParentID = 7;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"MFC";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 9;
	stCII.ParentID = 7;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"WTL";
	m_vecClassInfos.push_back(stCII);

	stCII.ID = 10;
	stCII.ParentID = 7;
	stCII.sHaseNode = HSN_NO;
	stCII.cname = L"ATL";
	stCII.sHaseRecomm = 1;
	m_vecClassInfos.push_back(stCII);

	return 1;
}


//功能: Unicode转utf8
static void wstrToUtf8(std::string& dest, const std::wstring& src)
{
	dest.clear();
	for (size_t i = 0; i < src.size(); i++)
	{
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07));
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

//功能: Unicode转utf8
static std::string wstrToUtf8(const std::wstring& str)
{
	std::string result;
	wstrToUtf8(result, str);
	return result;
}

// 功能: 将 wstring 的字符串里的 ' 变成 '' 
static std::wstring ReplaceAllQuote( std::wstring str )
{
	std::string::size_type  iPos, iStart;

	iStart = 0;
	while( true )
	{
		iPos = str.find( L'\'', iStart );
		if( iPos == std::string::npos )
			break;

		str.insert( iPos, L"'" );
		iStart = iPos + 2;
	}
	return str;
}

// 重载
// 功能: 将 string 的字符串里的 ' 变成 ''
static std::string ReplaceAllQuote( std::string str )
{
	std::string::size_type  iPos, iStart;

	iStart = 0;
	while( true )
	{
		iPos = str.find( '\'', iStart );
		if( iPos == std::string::npos )
			break;

		str.insert( iPos, "'" );
		iStart = iPos + 2;
	}
	return str;
}

//功能: utf8转Unicode
static void utf8toWStr(std::wstring& dest, const std::string& src)
{
	int iNeedSpace = ::MultiByteToWideChar( CP_UTF8, 0, src.c_str(), (int)src.length(), 0, 0 );
	++iNeedSpace;
	wchar_t * p = new wchar_t[iNeedSpace];
	p[ iNeedSpace - 1 ] = 0;
	::MultiByteToWideChar( CP_UTF8, 0, src.c_str(), (int)src.length(), p, iNeedSpace );
	dest = p;	
	delete []p;	
}

//功能: utf8转Unicode
static std::wstring utf8toWStr(const std::string& str)
{
	std::wstring result;
	utf8toWStr(result, str);
	return result;
}
