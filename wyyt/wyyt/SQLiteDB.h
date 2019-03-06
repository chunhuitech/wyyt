// SQLiteDB.h
// 功能： CSQLiteDB完成到SQLite的所有访问与操作
// 注意： SQLite字符集是utf8格式

#pragma once

extern "C"
{
#include "sqlite3.h"
}

#include <iostream>
#include <vector>
class CSQLiteDB 
{
public:
	CSQLiteDB(void);
	~CSQLiteDB(void);

//方法
public:
	//功能： 打开数据库
	//参数： pszName为打开数据库文件的路径
	int DBOpen(/*[in]*/ const char *pszName); 	

	//功能： 创建你自己的应用所有的数据库表
	void CreateTables();

	//功能： 执行SQL命令
	//参数： pSQL为执行SQL语句        
	int DBCommand(/*[in]*/ const char *pSQL);  

	//功能： 关闭数据库  
	int DBClose();  

	//功能： 插入包含有二进制数据的记录
	//参数： pSQL为SQL语句
	//       pBuffer为插入的数据
	//       nBufferSize为插入数据的字节长度
	int DBInsertBlob(/*[in]*/ const char *pSQL,        
		             /*[in]*/ const void *pBuffer,     
		             /*[in]*/ const int nBufferSize);  

	//功能： 执行查询语句
	//参数： pSQL为SQL语句
	//       pppResult为返回的结果
	//       pnRow为返回的记录行数
	//       pnColumn为返回的记录字段数
	int GetRecord(/*[in]*/ const char *pSQL,
		          /*[out]*/  char ***pppResult,
				  /*[out]*/ int *pnRow,
				  /*[out]*/ int *pnColumn);

	// 功能: 释放 GetRecord 中sqlite3_get_table 中分配的内存
	void FreeTable(/*[in]*/ char **ppResult);

	//功能： 得到二进制字段的数据
	//参数： pSQL为SQL语句
	//       nColumn为读取的字段
	//       ppData为读出的数据，由函数内部分配，用完记得用FreeBlob释放
	//       nLength为ppData中数据的实际字节长度
	int ReadBlob(/*[in]*/ const char *pSQL, 
		         /*[in]*/ const int nColumn,
		         /*[out]*/ void **ppData, 
				 /*[out]*/ int &nLength);

	//功能： 释放由ReadBlob中内部分配的指针
	//参数： pData数据指针
	void FreeBlob(/*[in]*/ LPVOID &lpData);

	//功能： 开始数据库事务
	int BeginTrans();

	//功能： 提交事务
	int CommitTrans();

	//功能： 回滚事务
	int RollbackTrans();

private:	
	//功能： 获取所有该创建的表的脚本
	//参数： vctTables为所有的脚本
	void GetDBTables(std::vector<std::string> &vctTables);

protected:
	//
	// 说明： 这些锁的功能暂时不实现，暂时保留为了以后的扩展	

	// 获取读锁
	void AcquireRead(){}

	// 释放读锁
	void ReleaseRead(){}

	// 获取写锁
	void AcquireWrite(){}

	// 释放写锁
	void ReleaseWrite(){}

//属性
private:
	sqlite3 *m_pSQLiteDB; //数据库对象
	char *m_pErr;         //保存错误信息
};

