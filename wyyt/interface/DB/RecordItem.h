#pragma once
#include "RecordAttachs.h"

class CRecordItem
{
public:
	CRecordItem(){
		ID = 0;
		HaseAttach = 0;
		AttachSpaceSize = 0;
		HaseRecomm = 0;
		MainClassID = ERROR_CLASSID;
		Reference = L"";
		DETimeValue tm_currenttime;
		InputTime = ModTime = tm_currenttime.timetostring();
	};
	CRecordItem(int64 id);

	virtual ~CRecordItem(void);

public:
	int64 ID;
	wstring SID;
	wstring Titlel;
	wstring KeyValues;
	int HaseAttach;
	uint64 AttachSpaceSize;
	int HaseRecomm;
	wstring Content;
	wstring Reference;
	wstring ModTime;
	wstring InputTime;
	

	int64 MainClassID;
	vector<int64> nAllClassID;
	//vector<CRecordAttachs> vecRec2Attachs;

	CRecordItem & operator=(const CRecordItem &RI1);
	/*{
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
	}*/
};

