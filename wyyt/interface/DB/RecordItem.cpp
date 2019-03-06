#include "StdAfx.h"
#include "RecordItem.h"


CRecordItem::CRecordItem(int64 id)
{
	ID = id;
}


CRecordItem::~CRecordItem(void)
{
}
CRecordItem & CRecordItem::operator=(const CRecordItem &RI1)
{
	ID = RI1.ID;
	SID = RI1.SID;
	Titlel = RI1.Titlel;
	KeyValues = RI1.KeyValues;
	HaseAttach = RI1.HaseAttach;
	AttachSpaceSize = RI1.AttachSpaceSize;
	HaseRecomm = RI1.HaseRecomm;
	Content = RI1.Content;
	Reference = RI1.Reference;
	ModTime = RI1.ModTime;
	InputTime = RI1.InputTime;
	MainClassID = RI1.MainClassID;
	for (vector<int64>::const_iterator it = RI1.nAllClassID.begin(); it != RI1.nAllClassID.end(); it++ )
	{
		nAllClassID.push_back(*it);
	}

	return *this;
}