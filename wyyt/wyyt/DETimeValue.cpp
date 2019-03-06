#include "DETimeValue.h"
#include <tchar.h>
DETimeValue::DETimeValue(void)
{
	FILETIME   tfile;
	::GetSystemTimeAsFileTime (&tfile);
	this->set (tfile);

}

DETimeValue::DETimeValue(wstring strSetTime)
{
	SYSTEMTIME Systemtime;
	::memset(&Systemtime,0,sizeof(Systemtime));
	_stscanf_s(strSetTime.c_str(), DE_TIME_FORMAT, &Systemtime.wYear,&Systemtime.wMonth,&Systemtime.wDay,
		&Systemtime.wHour,&Systemtime.wMinute,&Systemtime.wSecond);

	FILETIME tm_filetime;
	FILETIME tm_localfiletime;
	if(0 == ::SystemTimeToFileTime(&Systemtime,&tm_localfiletime))
	{
		this->msec(0);
	}
	if(0 == ::LocalFileTimeToFileTime(&tm_localfiletime,&tm_filetime))
	{
		this->msec(0);
	}
	this->set(tm_filetime);
	
}
DETimeValue::~DETimeValue(void)
{
}

void DETimeValue::normalize (void)
{
	
	// From Hans Rohnert...

	if (this->tv_.tv_usec >= DE_ONE_SECOND_IN_USECS)
	{
		do
		{
			this->tv_.tv_sec++;
			this->tv_.tv_usec -= DE_ONE_SECOND_IN_USECS;
		}
		while (this->tv_.tv_usec >= DE_ONE_SECOND_IN_USECS);
	}
	else if (this->tv_.tv_usec <= -DE_ONE_SECOND_IN_USECS)
	{
		do
		{
			this->tv_.tv_sec--;
			this->tv_.tv_usec += DE_ONE_SECOND_IN_USECS;
		}
		while (this->tv_.tv_usec <= -DE_ONE_SECOND_IN_USECS);
	}

	if (this->tv_.tv_sec >= 1 && this->tv_.tv_usec < 0)
	{
		this->tv_.tv_sec--;
		this->tv_.tv_usec += DE_ONE_SECOND_IN_USECS;
	}
	else if (this->tv_.tv_sec < 0 && this->tv_.tv_usec > 0)
	{
		this->tv_.tv_sec++;
		this->tv_.tv_usec -= DE_ONE_SECOND_IN_USECS;
	}
}

#pragma region FILETIME相关

const DWORDLONG FILETIME_to_timval_skew =
DE_INT64_LITERAL (0x19db1ded53e8000);
//  Initializes the DETimeValue object from a Win32 FILETIME

DETimeValue::DETimeValue (const FILETIME &file_time)
{
	
	this->set (file_time);
}

void DETimeValue::set (const FILETIME &file_time)
{
	//  Initializes the DETimeValue object from a Win32 FILETIME
	// Don't use a struct initializer, gcc don't like it.
	ULARGE_INTEGER _100ns;
	_100ns.LowPart = file_time.dwLowDateTime;
	_100ns.HighPart = file_time.dwHighDateTime;
	//消除两种表达方式的起止时间差
	_100ns.QuadPart -= FILETIME_to_timval_skew; 

	// Convert 100ns units to seconds;
	this->tv_.tv_sec = (long) (_100ns.QuadPart / (10000 * 1000));
	// Convert remainder to microseconds;
	this->tv_.tv_usec = (long) ((_100ns.QuadPart % (10000 * 1000)) / 10);
	this->normalize ();
}

// Returns the value of the object as a Win32 FILETIME.
//强制类型转换
DETimeValue::operator FILETIME () const
{
	FILETIME file_time;
	

	ULARGE_INTEGER _100ns;
	_100ns.QuadPart = (((DWORDLONG) this->tv_.tv_sec * (10000 * 1000) +
		this->tv_.tv_usec * 10) +
		FILETIME_to_timval_skew);

	file_time.dwLowDateTime = _100ns.LowPart;
	file_time.dwHighDateTime = _100ns.HighPart;

	return file_time;
}

#pragma endregion



wstring DETimeValue::timetostring() const
{
	WCHAR strtime[50] = {0};
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);

	_snwprintf(strtime,50,DE_TIME_FORMAT,
		Systemtime.wYear,Systemtime.wMonth,Systemtime.wDay,
		Systemtime.wHour,Systemtime.wMinute,Systemtime.wSecond);
	/*sprintf(strtime,"%4d-%02d-%02d %02d:%02d:%02d.%06d",
		Systemtime.wYear,Systemtime.wMonth,Systemtime.wDay,
		Systemtime.wHour,Systemtime.wMinute,Systemtime.wSecond,usec());*/
	//		Systemtime.wHour,Systemtime.wMinute,Systemtime.wSecond,Systemtime.wMilliseconds);
	wstring wtm_str(strtime);
	return wtm_str;
}
int DETimeValue::GetYear()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wYear;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return (tmCurr.tm_year+1900);

#endif
}


int DETimeValue::GetMonth()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wMonth;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return tmCurr.tm_mon + 1;
#endif
}
int DETimeValue::GetDay()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wDay;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return tmCurr.tm_mday ;
#endif
}

int DETimeValue::GetHour()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wHour;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return tmCurr.tm_hour ;
#endif
}
int DETimeValue::GetMinute()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wMinute;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return tmCurr.tm_min ;
#endif
}
int DETimeValue::GetSecond()
{
#ifdef WIN32
	SYSTEMTIME Systemtime;
	FILETIME tm_filetime = *this;
	FILETIME tm_plocalfiletime;
	::FileTimeToLocalFileTime(&tm_filetime,&tm_plocalfiletime);
	::FileTimeToSystemTime(&tm_plocalfiletime,&Systemtime);
	return Systemtime.wSecond;
#else
	struct tm tmCurr;
	time_t secs = sec ();
	tmCurr = *localtime_r(&secs,&tmCurr);
	return tmCurr.tm_sec ;
#endif
}

int DETimeValue::GetMilliSecond()
{
	return tv_.tv_usec/1000;
}



//计算两个时间之间的天数差
int DETimeValue::DayDiff(const DETimeValue& tBegin)
{
	DETimeValue tm1,tm2=tBegin;
	tm1.SetTime(GetYear(), GetMonth(), GetDay(),0,0,0,0);
	tm2.SetTime(tm2.GetYear(), tm2.GetMonth(), tm2.GetDay(),0,0,0,0);
	int secDiff = tm1.tv_.tv_sec - tm2.tv_.tv_sec;
	return secDiff/(3600*24);
}