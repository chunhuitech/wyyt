#ifndef __WYYT_DATADEFINE_H__
#define __WYYT_DATADEFINE_H__
#include "interface/DB/DataStructDefine.h"
#define ContentMaxLength    (24 * 1024/2 - 1000)

////这里用投机的方法，动态用非法类ID把nAllClassID容器增加到SUBCLASS_MAXCOUNT,以容纳用户的设置，否则若用reserve(10)的方法，则会在释放时报错
#define SUBCLASS_MAXCOUNT		10 //一条记录所属于的最多分类数量
//#ifdef DB_EXPORTS
//#define VECDLL_API_C extern "C" __declspec(dllexport)
//#define VECDLL_API __declspec(dllexport)
//#define EXPIMP_TEMPLATE
//#else
//#define VECDLL_API_C extern "C"  __declspec(dllimport)
//#define VECDLL_API __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
//#endif


//EXPIMP_TEMPLATE template class VECDLL_API std::vector<stSysClassInfoItem>; //这个很重要，否则在exe中释放m_vec时会出错
//EXPIMP_TEMPLATE template class VECDLL_API CRecordAttachs;
//EXPIMP_TEMPLATE VECDLL_API CRecordItem; //这个很重要，否则在exe中释放m_vec时会出错
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordAttachs>; //这个很重要，否则在exe中释放m_vec时会出错
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordItem>; //这个很重要，否则在exe中释放m_vec时会出错
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordItem*>; //这个很重要，否则在exe中释放m_vec时会出错
#endif