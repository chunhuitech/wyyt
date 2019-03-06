#pragma once
#define ERROR_CLASSID  -1
#include "SID.h"

class CRecordAttachs
{
public:
	CRecordAttachs(){
		RecordID = ERROR_CLASSID;
		IndexID = 0;
		PathIndex = 0;
		FileSize = 0;
		FileState = 0;
		DETimeValue tm_currenttime;
		InputTime = tm_currenttime.timetostring();
	};
	
	virtual ~CRecordAttachs(void);

public:
	int64 RecordID;
	int IndexID;
	wstring CodeID;
	int PathIndex;
	wstring SrcPath;
	wstring FileName;
	wstring HashValue;
	int64 FileSize;
	int FileState;
	wstring FileSuffix;
	wstring DownloadAddress;
	wstring Note;
	wstring InputTime;

};

