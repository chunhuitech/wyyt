#pragma once
#include "ExportDefine.h"

#define MAX_SQL_SIZE    (24 * 1024)  //Ϊ�˱�֤���������д�����ݲ����

//��ʼ�����ͷ� ���ݿ�
int InitSQLiteModule(/*[in]*/const std::wstring &strDBFile, const std::wstring &strDBFile_Pri);
int UnitSQLiteModule();

////////////////////////////ϵͳ����Ϣ///////////////////////////////////////////////////////////
//��ȡϵͳ����Ϣ
int ExportSysClassInfos(std::vector<stSysClassInfoItem>& vecSysClassInfos, EModuleType emt = EMT_SYS);
//��� 
int AddSysClassInfos(stSysClassInfoItem& SysClassInfos, EModuleType emt = EMT_SYS);  //���ﴫ��Ĳ���һ��Ҫ����������
//�޸ġ�ϵͳ������Ϣ
int ModSysClassInfos(int64 nID, const wstring& wstrNewClassName, EModuleType emt = EMT_SYS);

//ɾ����ϵͳ������Ϣ
int DelSysClassInfos(int64 nID, EModuleType emt = EMT_SYS);

//
int SetSysClassRecomm(int64 nID, int nRecommFlage, EModuleType emt = EMT_SYS);

////////////////////////////ϵͳ��¼��Ϣ///////////////////////////////////////////////////////////
int ExportGetSysRecordInfos(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
int ExportGetSysDataVerInfo(std::vector<stSysDataVerInfoItem>& vecSysVerInfos);

int GetSysRecToClassInfos_DLL(std::vector<CRecordItem>& vecSysRecordInfos, EModuleType emt = EMT_SYS);
int ExportGetSysRecordMaxID(int64& nID, EModuleType emt = EMT_SYS);
int InsertSysRecordInfo_DLL(CRecordItem& record , EModuleType emt = EMT_SYS);
int InsertSysRecToClass_DLL( int64 nRecordID,  int64 nClassID, EModuleType emt = EMT_SYS);
int DelSysRecToClass_DLL( int64 nRecordID, EModuleType emt = EMT_SYS);
int DelSysRecordInfo_DLL(int64 nID, EModuleType emt = EMT_SYS);
int ModSysRecordInfo_DLL(CRecordItem* record, EModuleType emt = EMT_SYS);
///ϵͳ����
int GetSysRecAttachsInfos_DLL(std::vector<CRecordAttachs>& vecAllAttachs, EModuleType emt = EMT_SYS);
int InsertSysRecordAttachInfo_DLL( CRecordAttachs& AttInfo, EModuleType emt = EMT_SYS );
int DelSysRecordAttachInfo_DLL(int64 nID, EModuleType emt = EMT_SYS);
//////////////���ú���////////////////////////////////////////////////////////////

//����: Unicodeתutf8
static void wstrToUtf8(std::string& dest, const std::wstring& src);

//����: Unicodeתutf8
static std::string wstrToUtf8(const std::wstring& str);

// ����: �� wstring ���ַ������ ' ��� '' 
static std::wstring ReplaceAllQuote( std::wstring str );

// ����
// ����: �� string ���ַ������ ' ��� '' 
static std::string ReplaceAllQuote( std::string str );

//����: utf8תUnicode
static void utf8toWStr(std::wstring& dest, const std::string& src);

//����: utf8תUnicode
static std::wstring utf8toWStr(const std::string& str);



//������
int ExportClassInfos(std::vector<stSysClassInfoItem>& m_vecClassInfos);