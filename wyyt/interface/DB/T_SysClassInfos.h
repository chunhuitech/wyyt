#ifndef __WYYT_T_SYSCLASSINFOS_H__
#define __WYYT_T_SYSCLASSINFOS_H__

//**********************************************************************************
// 
typedef struct _stT_SysClassInfos_
{
public:
	
	int64 ID;
	int64 ParentID;
	short HaseNode;//是否有子节点 1 有 0 没有
	short HaseRecomm;//是否推荐
	short HaseCollect;//是否收藏　1 有 0 没有
	string CName;
	string EName;
	string Note;
	
	_stT_SysClassInfos_()
	{
		ID = 0;
		ParentID = 0;
		HaseNode = 0;
		HaseRecomm  = 0;
	};

}T_SysClassInfos;

#endif