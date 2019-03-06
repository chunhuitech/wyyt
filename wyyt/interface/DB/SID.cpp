#include "StdAfx.h"
#include "SID.h"

void CSID::Init(const int amodelId, const int64 amainClassId,	const int64 arecordId, const int64 aattachId)
{
	modelId = amodelId;
	mainClassId = amainClassId;
	recordId = arecordId;
	attachId = aattachId;
}

void CSID::InitByString(const wstring strSID) const
{
	//sscanf
	swscanf(strSID.c_str(), SID_FORMAT, 
		&modelId, &mainClassId, &recordId, &attachId);
	
}
wstring CSID::FiniToString() const
{
	WCHAR dest[SID_FLAG_MAXLENG] = {0};
	//_snprintf
	_snwprintf(dest, SID_FLAG_MAXLENG, SID_FORMAT,
		modelId, mainClassId, recordId, attachId);
	dest[SID_FLAG_MAXLENG - 1] = 0;
	wstring sidString(dest);
	return sidString;
	
}
bool CSID::operator<(const CSID& right)
{
	return this->modelId < right.modelId
		|| (this->modelId == right.modelId && this->mainClassId < right.mainClassId)
		|| (this->modelId == right.modelId && this->mainClassId == right.mainClassId && this->recordId < right.recordId)
		|| (this->modelId == right.modelId && this->mainClassId == right.mainClassId && this->recordId < right.recordId && this->attachId < right.attachId);
}
