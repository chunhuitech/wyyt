// SQLiteDB.h
// ���ܣ� CSQLiteDB��ɵ�SQLite�����з��������
// ע�⣺ SQLite�ַ�����utf8��ʽ

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

//����
public:
	//���ܣ� �����ݿ�
	//������ pszNameΪ�����ݿ��ļ���·��
	int DBOpen(/*[in]*/ const char *pszName); 	

	//���ܣ� �������Լ���Ӧ�����е����ݿ��
	void CreateTables();

	//���ܣ� ִ��SQL����
	//������ pSQLΪִ��SQL���        
	int DBCommand(/*[in]*/ const char *pSQL);  

	//���ܣ� �ر����ݿ�  
	int DBClose();  

	//���ܣ� ��������ж��������ݵļ�¼
	//������ pSQLΪSQL���
	//       pBufferΪ���������
	//       nBufferSizeΪ�������ݵ��ֽڳ���
	int DBInsertBlob(/*[in]*/ const char *pSQL,        
		             /*[in]*/ const void *pBuffer,     
		             /*[in]*/ const int nBufferSize);  

	//���ܣ� ִ�в�ѯ���
	//������ pSQLΪSQL���
	//       pppResultΪ���صĽ��
	//       pnRowΪ���صļ�¼����
	//       pnColumnΪ���صļ�¼�ֶ���
	int GetRecord(/*[in]*/ const char *pSQL,
		          /*[out]*/  char ***pppResult,
				  /*[out]*/ int *pnRow,
				  /*[out]*/ int *pnColumn);

	// ����: �ͷ� GetRecord ��sqlite3_get_table �з�����ڴ�
	void FreeTable(/*[in]*/ char **ppResult);

	//���ܣ� �õ��������ֶε�����
	//������ pSQLΪSQL���
	//       nColumnΪ��ȡ���ֶ�
	//       ppDataΪ���������ݣ��ɺ����ڲ����䣬����ǵ���FreeBlob�ͷ�
	//       nLengthΪppData�����ݵ�ʵ���ֽڳ���
	int ReadBlob(/*[in]*/ const char *pSQL, 
		         /*[in]*/ const int nColumn,
		         /*[out]*/ void **ppData, 
				 /*[out]*/ int &nLength);

	//���ܣ� �ͷ���ReadBlob���ڲ������ָ��
	//������ pData����ָ��
	void FreeBlob(/*[in]*/ LPVOID &lpData);

	//���ܣ� ��ʼ���ݿ�����
	int BeginTrans();

	//���ܣ� �ύ����
	int CommitTrans();

	//���ܣ� �ع�����
	int RollbackTrans();

private:	
	//���ܣ� ��ȡ���иô����ı�Ľű�
	//������ vctTablesΪ���еĽű�
	void GetDBTables(std::vector<std::string> &vctTables);

protected:
	//
	// ˵���� ��Щ���Ĺ�����ʱ��ʵ�֣���ʱ����Ϊ���Ժ����չ	

	// ��ȡ����
	void AcquireRead(){}

	// �ͷŶ���
	void ReleaseRead(){}

	// ��ȡд��
	void AcquireWrite(){}

	// �ͷ�д��
	void ReleaseWrite(){}

//����
private:
	sqlite3 *m_pSQLiteDB; //���ݿ����
	char *m_pErr;         //���������Ϣ
};

