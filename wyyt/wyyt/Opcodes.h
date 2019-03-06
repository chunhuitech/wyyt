#pragma once

#ifdef _DEBUG
#define WYYT_GUID				_T("WYYT-{4ECDC6FC-516F-222c-9066-97D123456770}-DEBUG")
#else
#define WYYT_GUID				_T("WYYT-{4ECDC6FC-516F-222c-9066-97D123456770}")
#endif

#define CONFIGFOLDER			_T("config\\")

#define PREFFILE_VERSION				0x14	//<<-- last change: reduced .dat, by using .ini