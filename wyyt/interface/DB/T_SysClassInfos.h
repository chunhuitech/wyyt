#ifndef __WYYT_T_SYSCLASSINFOS_H__
#define __WYYT_T_SYSCLASSINFOS_H__

//**********************************************************************************
// 
typedef struct _stT_SysClassInfos_
{
public:
	
	int64 ID;
	int64 ParentID;
	short HaseNode;//�Ƿ����ӽڵ� 1 �� 0 û��
	short HaseRecomm;//�Ƿ��Ƽ�
	short HaseCollect;//�Ƿ��ղء�1 �� 0 û��
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