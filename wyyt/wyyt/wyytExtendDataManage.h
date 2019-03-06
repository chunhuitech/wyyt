#ifndef __WYYT_WYYTEXTENDDATAMANAGE_H__
#define __WYYT_WYYTEXTENDDATAMANAGE_H__

//#include "interface/DB/DataStructDefine.h"
#include "ExportDefine.h" //�������EXPIMP_TEMPLATE template class VECDLL_API std::vector<stSysClassInfoItem>; //�������Ҫ��������exe���ͷ�m_vecʱ�����




#define ERROR_CLASS_ID		-100
#define ROOT_CLASS_COUNTS		99
#define ROOT_CLASS_PARENT_ID		-1

enum ENodeHasLine{
	NHL_NO = 0,
	NHL_YES = 1,
};
//����ͼͼ������
enum ETreeImageIndex{
	TII_NODES = 0,
	TII_NODE = 1,
	TII_NODE_SELECTED = 2,
	TII_NODE_RECOMM = 3,
};


class IsDelClass
{
public:
	IsDelClass( int64& nCurClassID ) : m_nCurClassID(nCurClassID)
	{
	}
	bool operator()(stSysClassInfoItem& stCII)
	{
		return stCII.ID == m_nCurClassID;
	}
private:
	int64 m_nCurClassID;
};
class IsDelRecord
{
public:
	IsDelRecord( int64& nCurRecordID ) : m_nCurRecordID(nCurRecordID)
	{
	}
	bool operator()(CRecordItem& cri)
	{
		return cri.ID == m_nCurRecordID;
	}
private:
	int64 m_nCurRecordID;
};


class wyytExtendDataManage
{
public:
	wyytExtendDataManage(void);
	~wyytExtendDataManage(void);

	static wyytExtendDataManage& Instance();


	/////////������Ϣ/////
public:
	void Init();
	void Unit();
	//-----��ط�����
	void Reload();
	void InitClassInfoDatas();
	void ClearClassInfoDatas();
	int64 GetSysClassMaxID();

	void InsertNewClassInfo( stSysClassInfoItem stNewCII );
	void ModClassInfo(int64 nID, wstring wstrNewClassName);
	void DelClassInfo(int64 nID);
	void SetClassInfoRecomm(int64 nID);
	void CancleClassInfoType(int64 nID);
	bool IfHaseSubClass(int64 nParentID);
	wstring GetClassFullName(int64 nCID);
	bool GetClassAllName(int64 nCID, vector<wstring>& vecAllNames);

	//ϵͳ��¼����
	void InitSysRecordInfoDatas();
	void ClearSysRecordInfoDatas();
	
	void GetSysDataVerInfo();

	void GetSysRecordMaxID(int64& nID);
	void InsertSysRecordInfo( CRecordItem record );
	void ModSysRecordInfo( CRecordItem* record );
	void DelSysRecordInfo(int64 nRID);

	///ϵͳ����
	void InitSysRecAttachsDatas();
	void ClearSysRecAttachsDatas();

	bool InsertSysRecordAttachInfo( CRecordAttachs AttInfo );
	void DelSysRecordAttachInfo(int64 nRID, bool bDelAttachFile = false);

	bool DelSysRecordAttachFile(const WCHAR * srcPathFile);
	bool AddSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath);
	bool MoveSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath);//��������
	//-----��ر�������
public:
	vector<stSysClassInfoItem>  m_vecClassInfos;
	vector<CRecordItem>  m_vecRecordItems;
	map<int64, vector<CRecordAttachs>> m_mapRec2Attachs;
	vector<stSysDataVerInfoItem>  m_vecSysDataVerInfos;
	////////////ϵͳ�ڵ㼯��//////////////////////////////////////////////////////////////
	//-----��ط�����
	void InitSysClassIDs();
	bool IsSysClassID(int64 nID);
	//-----��ر�������
public:
	vector<int64>  m_vecSysClassInfoIDs;
	
};

#endif