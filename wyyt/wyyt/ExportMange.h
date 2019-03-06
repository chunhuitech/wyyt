#pragma once
#include "ExportDefine.h"

#define MAX_SQL_SIZE    (24 * 1024)  //为了保证插入的语句中大的数据不溢出

//初始化及释放 数据库
int InitSQLiteModule(/*[in]*/const std::wstring &strDBFile, const std::wstring &strDBFile_Pri);
int UnitSQLiteModule();

////////////////////////////系统类信息///////////////////////////////////////////////////////////
//获取系统类信息
int ExportSysClassInfos(std::vector<stSysClassInfoItem>& vecSysClassInfos, EModuleType emt = EMT_SYS);
//添加 
int AddSysClassInfos(stSysClassInfoItem& SysClassInfos, EModuleType emt = EMT_SYS);  //这里传入的参数一定要用引用类型
//修改　系统分类信息
int ModSysClassInfos(int64 nID, const wstring& wstrNewClassName, EModuleType emt = EMT_SYS);

//删除　系统分类信息
int DelSysClassInfos(int64 nID, EModuleType emt = EMT_SYS);

//
int SetSysClassRecomm(int64 nID, int nRecommFlage, EModuleType emt = EMT_SYS);

////////////////////////////系统记录信息///////////////////////////////////////////////////////////
int ExportGetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
int ExportGetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos);

int GetSysRecToClassInfos_DLL(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
int ExportGetSysRecordMaxID(int64& nID, EModuleType emt = EMT_SYS);
int InsertSysRecordInfo_DLL(CRecordItem& record , EModuleType emt = EMT_SYS);
int InsertSysRecToClass_DLL( int64 nRecordID,  int64 nClassID, EModuleType emt = EMT_SYS);
int DelSysRecToClass_DLL( int64 nRecordID, EModuleType emt = EMT_SYS);
int DelSysRecordInfo_DLL(int64 nID, EModuleType emt = EMT_SYS);
int ModSysRecordInfo_DLL(CRecordItem* record, EModuleType emt = EMT_SYS);
///系统附件
int GetSysRecAttachsInfos_DLL(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt = EMT_SYS);
int InsertSysRecordAttachInfo_DLL( CRecordAttachs& AttInfo, EModuleType emt = EMT_SYS );
int DelSysRecordAttachInfo_DLL(int64 nID, EModuleType emt = EMT_SYS);
//////////////公用函数////////////////////////////////////////////////////////////

//功能: Unicode转utf8
static void wstrToUtf8(std::string& dest, const std::wstring& src);

//功能: Unicode转utf8
static std::string wstrToUtf8(const std::wstring& str);

// 功能: 将 wstring 的字符串里的 ' 变成 '' 
static std::wstring ReplaceAllQuote( std::wstring str );

// 重载
// 功能: 将 string 的字符串里的 ' 变成 '' 
static std::string ReplaceAllQuote( std::string str );

//功能: utf8转Unicode
static void utf8toWStr(std::wstring& dest, const std::string& src);

//功能: utf8转Unicode
static std::wstring utf8toWStr(const std::string& str);



//测试用
int ExportClassInfos(std::vector<stSysClassInfoItem>& m_vecClassInfos);