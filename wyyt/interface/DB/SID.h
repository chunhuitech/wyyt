#ifndef _IHAS_SID_H_
#define _IHAS_SID_H_

#include "devlib_include.h"
#include "Types.h"
#include "DETimeValue.h"
//int ��Χ��[-2^31,2^31)����-2147483648~2147483647
//VCʹ��__int64/unsigned __int64����Χ��[-2^63, 2^63)��[0,2^64)����-9223372036854775808~9223372036854775807�� 0~18446744073709551615(Լ1800����)��
//#define SID_FORMAT_RECORD	 "%02d%20d%20d"
#define SID_FLAG_MAXLENG	63
#define SID_FLAG_RECORD	0
//���һ��%30d Ϊ������Ϊ��¼������Ϊ����
#define SID_FORMAT	 L"%02d%06lld%06lld%03lld"
//ϵͳģ�����Ͷ���
enum ESysModelType{
	E_SMT_SYS = 0,
	E_SMT_GROUP = 1,
	E_SMT_PRIVATE = 2,
	E_SMT_UNKNOWN
};


class CSID
{
public:
	int modelId;
	int64 mainClassId;
	int64 recordId;
	int64 attachId;
	
public:
	CSID():
		modelId(0),
		mainClassId(0),
		recordId(0),
		attachId(0)
	{
	}

	CSID
		(const int amodelId,
		const int64 amainClassId,
		const int64 arecordId,
		const int64 aattachId
		):
		modelId(amodelId),
		mainClassId(amainClassId),
		recordId(arecordId),
		attachId(aattachId)
	{
	}

	CSID(const CSID& sid):
		modelId(sid.modelId),
		mainClassId(sid.mainClassId),
		recordId(sid.recordId),
		attachId(sid.attachId)
	{
	}

	~CSID(){}

	CSID& operator=(const CSID& sid)
	{
		copy(sid);
		return *this;
	}

	void Init(const int amodelId, const int64 amainClassId,	const int64 arecordId, const int64 aattachId);	
	void InitByString(const wstring strSID) const;		
	wstring FiniToString() const;	
	
	//TODO: ��ʱ�Եģ�ֻ�Ƚϵ�Сʱ
	bool operator<(const CSID& right);

private:
	void copy(const CSID& sid)
	{
		modelId=sid.modelId;
		mainClassId=sid.mainClassId;
		recordId=sid.recordId;
		attachId=sid.attachId;
	}
};
#endif 
