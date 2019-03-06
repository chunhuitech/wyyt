#ifndef _DETIMEVALUE_H
#define _DETIMEVALUE_H

#include <Winsock2.h>
typedef unsigned __int64 __uint64;
#include <string>
using namespace std;


#define DE_INT64_LITERAL(n) n ## i64

#define  DE_TIME_FORMAT L"%4d-%02d-%02d %02d:%02d:%02d"
typedef struct time_spec
{
	/// Seconds
	time_t tv_sec;
	long tv_nsec;
} time_spec_t;
typedef struct time_spec time_spec_t;


// Define some helpful constants.
// Not type-safe, and signed.  For backward compatibility.
#define DE_ONE_SECOND_IN_MSECS 1000L
#define DE_ONE_SECOND_IN_USECS 1000000L
#define DE_ONE_SECOND_IN_NSECS 1000000000L

class DETimeValue
{
public:
	DETimeValue(void);
	DETimeValue(wstring strSetTime);
	~DETimeValue(void);

#pragma region FILETIME相关

	/// Construct the DETimeValue object from a Win32 FILETIME
	DETimeValue (const FILETIME &ft);

	///  Initializes the DETimeValue object from a Win32 FILETIME.
	void set (const FILETIME &ft);

	/// Returns the value of the object as a Win32 FILETIME.
	//强制类型转换
	operator FILETIME () const;

#pragma endregion

#pragma region timeval相关

	/// Construct the DETimeValue from a timeval.
	DETimeValue (const struct timeval &t);
	/// Initializes the DETimeValue from a timeval.
	void set (const timeval &t);

#pragma endregion



#pragma region time_spec_t相关

	/// Construct the DETimeValue object from a timespec_t.
	DETimeValue (const time_spec_t &t);

	/// Initializes the DETimeValue object from a timespec_t.
	void set (const time_spec_t &t);

#pragma endregion

#pragma region sec usec设置相关
	/// Constructor.
	DETimeValue(long sec, long usec = 0);
	/// Initializes the DETimeValue from two longs.
	void set (long sec, long usec);
#pragma endregion


#pragma region double设置相关

	/// Initializes the DETimeValue from a double, which is assumed to be
	/// in second format, with any remainder treated as microseconds.
	void set (double d);
	
#pragma endregion

#pragma region 获取设置秒数 sec
	 /// Get seconds.
  /**
   * @return The second field/component of this PPM_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  long sec (void) const;

  /// Set seconds.
  void sec (long sec);

#pragma endregion

  #pragma region 获取设置毫秒数 msec

   /// Converts from DETimeValue format into milli-seconds format.
  /**  毫秒数
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds).
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an DETimeValue.
   */
  __int64 msec (void) const;

  /// Converts from milli-seconds format into DETimeValue format.
  /**
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an DETimeValue.
   */
  void msec (__int64);

#pragma endregion

  #pragma region 获取设置微秒数 usec
   /// Get microseconds.
  /**
   * @return The microsecond field/component of this PPM_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  long usec (void) const;

  /// Set microseconds.
  void usec (long usec);
#pragma endregion

	wstring timetostring() const;

	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	int GetMilliSecond();

	//通过设置年月日时分秒毫秒设置时间
	BOOL SetTime(WORD wyear,BYTE bmonth,BYTE bday,BYTE bhour,BYTE bminute,BYTE bsecond,WORD wms);
	//计算两个时间之间的天数差
	int DayDiff(const DETimeValue&tBegin);

private:
	/// Put the timevalue into a canonical form.
	void normalize (void);
	/// Store the values as a timeval.
	timeval tv_;
};


inline DETimeValue::DETimeValue (const struct timeval &tv)
// : tv_ ()
{
	
	this->set (tv);
}

inline void DETimeValue::set (const timeval &tv)
{
	this->tv_.tv_sec = tv.tv_sec;
	this->tv_.tv_usec = tv.tv_usec;

	this->normalize ();
}

//个位以上是秒,小数*1百万,即微妙
inline void	 DETimeValue::set (double d)
{
	long l = (long) d;
	this->tv_.tv_sec = l;
	this->tv_.tv_usec = (long) ((d - (double) l) * DE_ONE_SECOND_IN_USECS);
	this->normalize ();
}

// Initializes the DETimeValue object from a timespec_t.

inline DETimeValue::DETimeValue (const time_spec_t &tv)
// : tv_ ()
{
	this->set (tv);
}

// Initializes a timespec_t.  Note that this approach loses precision
// since it converts the nano-seconds into micro-seconds.  But then
// again, do any real systems have nano-second timer precision?!

inline void	 DETimeValue::set (const time_spec_t &tv)
{
	this->tv_.tv_sec = (long)tv.tv_sec;
	// Convert nanoseconds into microseconds.
	this->tv_.tv_usec = tv.tv_nsec / 1000;

	this->normalize ();
}

inline DETimeValue::DETimeValue (long sec, long usec)
{
	this->set (sec, usec);
}

//设置时间并正则化
inline void DETimeValue::set (long sec, long usec)
{
	this->tv_.tv_sec = sec;
	this->tv_.tv_usec = usec;
	this->normalize ();
}


// Returns number of seconds.
//返回秒数
inline long DETimeValue::sec (void) const
{
	return this->tv_.tv_sec;
}

// Sets the number of seconds.
//设置秒数
inline void DETimeValue::sec (long sec)
{
	this->tv_.tv_sec = sec;
}


// Converts from Time_Value format into milli-seconds format.
//返回毫秒
inline __int64 DETimeValue::msec (void) const
{
	return ((__int64)this->tv_.tv_sec) * 1000 + ((__int64)this->tv_.tv_usec) / 1000;
}

// Converts from milli-seconds format into Time_Value format.
//将毫秒数设置为DETimeValue表达方式
inline void DETimeValue::msec (__int64 milliseconds)
{
	// Convert millisecond units to seconds;
	this->tv_.tv_sec = (long)(milliseconds / 1000);
	// Convert remainder to microseconds;
	this->tv_.tv_usec = (long)((milliseconds - (this->tv_.tv_sec * 1000)) * 1000);
}


// Returns number of micro-seconds.
//返回微妙
inline long DETimeValue::usec (void) const
{
	return this->tv_.tv_usec;
}

// Sets the number of micro-seconds.
//设置微妙
inline void DETimeValue::usec (long usec)
{
	this->tv_.tv_usec = usec;
}

//通过设置年月日时分秒毫秒设置时间
inline BOOL DETimeValue::SetTime(WORD wyear,BYTE bmonth,BYTE bday,BYTE bhour,BYTE bminute,BYTE bsecond,WORD wms)
{
	SYSTEMTIME Systemtime;
	::memset(&Systemtime,0,sizeof(Systemtime));
	Systemtime.wYear = wyear;
	Systemtime.wMonth = bmonth;
	Systemtime.wDay = bday;
	Systemtime.wHour = bhour;
	Systemtime.wMinute = bminute;
	Systemtime.wSecond = bsecond;
	Systemtime.wMilliseconds = wms;

	FILETIME tm_filetime;
	FILETIME tm_localfiletime;
	if(0 == ::SystemTimeToFileTime(&Systemtime,&tm_localfiletime))
	{
		this->msec(0);
		return FALSE;
	}
	if(0 == ::LocalFileTimeToFileTime(&tm_localfiletime,&tm_filetime))
	{
		this->msec(0);
		return FALSE;
	}
	this->set(tm_filetime);
	return TRUE;


}
#endif /* _DETIMEVALUE_H */