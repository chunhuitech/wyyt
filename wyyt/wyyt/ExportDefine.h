#ifndef __WYYT_DATADEFINE_H__
#define __WYYT_DATADEFINE_H__
#include "interface/DB/DataStructDefine.h"
#define ContentMaxLength    (24 * 1024/2 - 1000)

////������Ͷ���ķ�������̬�÷Ƿ���ID��nAllClassID�������ӵ�SUBCLASS_MAXCOUNT,�������û������ã���������reserve(10)�ķ�����������ͷ�ʱ����
#define SUBCLASS_MAXCOUNT		10 //һ����¼�����ڵ�����������
//#ifdef DB_EXPORTS
//#define VECDLL_API_C extern "C" __declspec(dllexport)
//#define VECDLL_API __declspec(dllexport)
//#define EXPIMP_TEMPLATE
//#else
//#define VECDLL_API_C extern "C"  __declspec(dllimport)
//#define VECDLL_API __declspec(dllimport)
//#define EXPIMP_TEMPLATE extern
//#endif


//EXPIMP_TEMPLATE template class VECDLL_API std::vector<stSysClassInfoItem>; //�������Ҫ��������exe���ͷ�m_vecʱ�����
//EXPIMP_TEMPLATE template class VECDLL_API CRecordAttachs;
//EXPIMP_TEMPLATE VECDLL_API CRecordItem; //�������Ҫ��������exe���ͷ�m_vecʱ�����
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordAttachs>; //�������Ҫ��������exe���ͷ�m_vecʱ�����
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordItem>; //�������Ҫ��������exe���ͷ�m_vecʱ�����
//EXPIMP_TEMPLATE template class VECDLL_API std::vector<CRecordItem*>; //�������Ҫ��������exe���ͷ�m_vecʱ�����
#endif