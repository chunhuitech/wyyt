#ifndef __WYYT_DATASTRUCTDEFINE_H__
#define __WYYT_DATASTRUCTDEFINE_H__
#include "devlib_include.h"
#include "Types.h"
#include "DbDefine.h"
#include "RecordItem.h"

enum ESpecialClassInfoItems{
	SCII_NO = 0,
	SCII_NOTES = SCII_NO,	// 公告类信息节点
	SCII_WYYT = 1,
	SCII_ALL_OTHER = 100	//"所有类信息" node			
	//....//这里预留100个大类信息，即所有子类信息是从101开始编号的
};
//索引节点ID定义
enum ESpecialClassInfoIDs
{
	SCII_NO_ID = -1,
	SCII_NOTES_ID = 0,	// 公告类信息节点
	SCII_WYYT_ID = 1,
	SCII_ALL_OTHER_ID = 100	//"所有类信息" node		
	//....//这里预留100个大类信息，即所有子类信息是从101开始编号的
};
//ESpecialClassInfoItems ESpecialClassInfoIDs这两个的区别:
//前者用来区分大类（即非数据库中查出，而是代码写死）
//后者则为数据库表中对应的ID列值，
//且ID列的前面预留100个编号与大类相对应

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
	
	//存放此结构的记录集,内容要保证按先按ParentID排序，同一ID内部再按ID排序
	int64 ID;
	int64 ParentID;
	short sHaseNode; //是否有子节点 1 有 0 没有
	short sHaseSel; //是否被选中  1　选中 0 没有选中
	short sHaseRecomm; //是否推荐
	short sHaseCollect;//是否收藏　1 有 0 没有
	//short sIfSelected; //是否已经被选中用于查询 1 有 0 没有
	//short sHideNode; //是否隐藏此节点 1 隐藏 0 不隐藏
	wstring cname;
	wstring ename;
	//CString name;
	//wstring name_w;
	wstring Note;//备注，用于索引查找
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

	//存放此结构的记录集,内容要保证按先按ParentID排序，同一ID内部再按ID排序
	int64 ID;
	wstring Version;
	wstring VerUpDate;
	wstring Notes;//备注，用于索引查找
	wstring InputTime;
	_stSysDataVerInfoItem_()
	{
		ID = 0;
	};

} stSysDataVerInfoItem;

#endif