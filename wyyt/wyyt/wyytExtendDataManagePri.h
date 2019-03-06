#ifndef __WYYT_WYYTEXTENDDATAMANAGEPRI_H__
#define __WYYT_WYYTEXTENDDATAMANAGEPRI_H__

#include "wyytExtendDataManage.h" 



//
//#define ERROR_CLASS_ID		-100
//#define ROOT_CLASS_COUNTS		5
//#define ROOT_CLASS_PARENT_ID		-1
//
//enum ENodeHasLine{
//	NHL_NO = 0,
//	NHL_YES = 1,
//};
////树视图图标索引
//enum ETreeImageIndex{
//	TII_NODES = 0,
//	TII_NODE = 1,
//	TII_NODE_SELECTED = 2,
//	TII_NODE_RECOMM = 3,
//};




class wyytExtendDataManagePri
{
public:
	wyytExtendDataManagePri(void);
	~wyytExtendDataManagePri(void);

	static wyytExtendDataManagePri& Instance();


	/////////分类信息/////
public:
	//void Init();
	//void Unit();
	//-----相关法定义
	//void Reload();
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

	//系统记录数据
	void InitSysRecordInfoDatas();
	void ClearSysRecordInfoDatas();

	void GetSysRecordMaxID(int64& nID);
	void InsertSysRecordInfo( CRecordItem record );
	void ModSysRecordInfo( CRecordItem* record );
	void DelSysRecordInfo(int64 nRID);

	///系统附件
	void InitSysRecAttachsDatas();
	void ClearSysRecAttachsDatas();

	bool InsertSysRecordAttachInfo( CRecordAttachs AttInfo );
	void DelSysRecordAttachInfo(int64 nRID, bool bDelAttachFile = false);

	bool DelSysRecordAttachFile(const WCHAR * srcPathFile);
	bool AddSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath);
	bool MoveSysRecordAttachFile(const WCHAR * srcPath, const WCHAR * destPath);//即重命名
	//-----相关变量定义
public:
	vector<stSysClassInfoItem>  m_vecClassInfos;
	vector<CRecordItem>  m_vecRecordItems;
	map<int64, vector<CRecordAttachs>> m_mapRec2Attachs;

	////////////系统节点集合//////////////////////////////////////////////////////////////
	//-----相关法定义
	void InitSysClassIDs();
	bool IsSysClassID(int64 nID);
	//-----相关变量定义
public:
	vector<int64>  m_vecSysClassInfoIDs;
	
};

#endif