#ifndef __WYYT_DATASTRUCTDEFINE_H__
#define __WYYT_DATASTRUCTDEFINE_H__
#include "devlib_include.h"
#include "Types.h"
#include "DbDefine.h"
#include "RecordItem.h"

enum ESpecialClassInfoItems{
	SCII_NO = 0,
	SCII_NOTES = SCII_NO,	// ��������Ϣ�ڵ�
	SCII_WYYT = 1,
	SCII_ALL_OTHER = 100	//"��������Ϣ" node			
	//....//����Ԥ��100��������Ϣ��������������Ϣ�Ǵ�101��ʼ��ŵ�
};
//�����ڵ�ID����
enum ESpecialClassInfoIDs
{
	SCII_NO_ID = -1,
	SCII_NOTES_ID = 0,	// ��������Ϣ�ڵ�
	SCII_WYYT_ID = 1,
	SCII_ALL_OTHER_ID = 100	//"��������Ϣ" node		
	//....//����Ԥ��100��������Ϣ��������������Ϣ�Ǵ�101��ʼ��ŵ�
};
//ESpecialClassInfoItems ESpecialClassInfoIDs������������:
//ǰ���������ִ��ࣨ�������ݿ��в�������Ǵ���д����
//������Ϊ���ݿ���ж�Ӧ��ID��ֵ��
//��ID�е�ǰ��Ԥ��100�������������Ӧ

enum EHaseSubNode{
	HSN_NO = 0,
	HSN_YES
};

enum EHaseSelected{
	HS_NO = 0,
	HS_YES
};

enum EHaseFlag{
	HF_NO = 0,
	HF_YES
};
//**********************************************************************************
// ClassInfoItem
typedef struct _stSysClassInfoItem_
{
public:
	
	//��Ŵ˽ṹ�ļ�¼��,����Ҫ��֤���Ȱ�ParentID����ͬһID�ڲ��ٰ�ID����
	int64 ID;
	int64 ParentID;
	short sHaseNode; //�Ƿ����ӽڵ� 1 �� 0 û��
	short sHaseSel; //�Ƿ�ѡ��  1��ѡ�� 0 û��ѡ��
	short sHaseRecomm; //�Ƿ��Ƽ�
	short sHaseCollect;//�Ƿ��ղء�1 �� 0 û��
	//short sIfSelected; //�Ƿ��Ѿ���ѡ�����ڲ�ѯ 1 �� 0 û��
	//short sHideNode; //�Ƿ����ش˽ڵ� 1 ���� 0 ������
	wstring cname;
	wstring ename;
	//CString name;
	//wstring name_w;
	wstring Note;//��ע��������������
	_stSysClassInfoItem_()
	{
		ID = 0;
		ParentID = 0;
		sHaseNode = HSN_NO;
		sHaseSel = HS_NO;
		sHaseRecomm  = HF_NO;
		sHaseCollect = HF_NO;
	};

} stSysClassInfoItem;

// ClassInfoItem
typedef struct _stSysDataVerInfoItem_
{
public:

	//��Ŵ˽ṹ�ļ�¼��,����Ҫ��֤���Ȱ�ParentID����ͬһID�ڲ��ٰ�ID����
	int64 ID;
	wstring Version;
	wstring VerUpDate;
	wstring Notes;//��ע��������������
	wstring InputTime;
	_stSysDataVerInfoItem_()
	{
		ID = 0;
	};

} stSysDataVerInfoItem;

#endif