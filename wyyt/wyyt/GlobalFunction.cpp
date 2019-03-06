/*! @file info
******************************************************************************** 
<PRE> 
模块名 : <文件所属的模块名称> 
文件名 : <GlobalFunction.cpp> 
相关文件 : <与此文件相关的其它文件> 
文件实现功能 :
 * 1- 
 * 2- 
作者 : <IHAS> 
版本 : <1.0> 
版权 : Copyright (c) 2010 IHAS
创建日期 : <2010/10/29>
-------------------------------------------------------------------------------- 
备注 : <其它说明> 
-------------------------------------------------------------------------------- 
修改记录 : 
日 期         版本           修改人           修改内容 
YYYY/MM/DD    X.Y        <IHAS>     <修改内容> 
</PRE> 
*******************************************************************************/



//#include <Windows.h>
//#include <assert.h>
#include <tchar.h>
#include "GlobalFunction.h"

char *CGlobalFunction::itoa(const int value, char * const string, const int radix)
{
#ifdef WIN32
	_itoa_s(value, string, radix,radix);
	return string;

#else
	if(radix == 10)
	{
		sprintf(string , "%d" , value);
	}
	else if(radix == 16)
	{
		sprintf(string , "%x" , value);
	}
	return string;
#endif
}


int CGlobalFunction::MultiByteToWideChar(const char* pszMultiByte, wchar_t* pwszWideChar, int iCodePage)
{
	wchar_t* pwszWideCharTemp = NULL;
	try
	{
		//每一次调用 MultiByteToWideChar 获取宽字符 空间大小 iWideCharSize 以创建缓存
		int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);

		if (0 == iWideCharSize)
		{
			throw -1;
		}

		pwszWideCharTemp = new wchar_t[iWideCharSize];
		memset(pwszWideCharTemp, 0, iWideCharSize*sizeof(wchar_t));

		//每二次调用 MultiByteToWideChar 进行真正的转换
		iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, pwszWideCharTemp, iWideCharSize);

		if (0 == iWideCharSize)
		{
			throw -1;
		}

		wcscpy_s(pwszWideChar,iWideCharSize+10,pwszWideCharTemp);


		delete[] pwszWideCharTemp;
		pwszWideCharTemp = NULL;

		return iWideCharSize;
	}
	catch (...)
	{
		if (NULL != pwszWideCharTemp)
		{
			delete[] pwszWideCharTemp;
			pwszWideCharTemp = NULL;
		}

		return 0;
	}
}


int CGlobalFunction::WideCharToMultiByte(const wchar_t* pwszWideChar,char* pszMultiByte, int iCodePage)
{
	char* pszMultiByteTemp = NULL;
	try
	{
		int iMultiByteSize = ::WideCharToMultiByte(iCodePage, 0, pwszWideChar, -1, NULL, 0, NULL, NULL);

		if (0 == iMultiByteSize)
		{
			throw -1;
		}

		pszMultiByteTemp = new char[iMultiByteSize];
		memset(pszMultiByteTemp, 0, iMultiByteSize*sizeof(char));

		iMultiByteSize = ::WideCharToMultiByte(iCodePage, 0, pwszWideChar, -1, pszMultiByteTemp, iMultiByteSize, NULL, NULL);

		if (0 == iMultiByteSize)
		{
			throw -1;
		}

		strcpy_s(pszMultiByte,iMultiByteSize+10,pszMultiByteTemp);

		delete[] pszMultiByteTemp;
		pszMultiByteTemp = NULL;

		return iMultiByteSize;
	}
	catch (...)
	{
		if (NULL != pszMultiByteTemp)
		{
			delete[] pszMultiByteTemp;
			pszMultiByteTemp = NULL;
		}

		return 0;
	}
}

int CGlobalFunction::MultiByteToWideChar(const char* pszMultiByte, wchar_t** pwszWideChar, int iCodePage)
{

	try
	{
		//每一次调用 MultiByteToWideChar 获取宽字符 空间大小 iWideCharSize 以创建缓存
		int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);

		if (0 == iWideCharSize)
		{
			throw -1;
		}

		(*pwszWideChar) = new wchar_t[iWideCharSize];
		memset(*pwszWideChar, 0, iWideCharSize*sizeof(wchar_t));

		//每二次调用 MultiByteToWideChar 进行真正的转换
		iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, *pwszWideChar, iWideCharSize);

		if (0 == iWideCharSize)
		{
			throw -1;
		}


		return iWideCharSize;
	}
	catch (...)
	{

		return 0;
	}
}
wstring CGlobalFunction::MultiByteToWideChar(const char* pszMultiByte, int iCodePage)
{

	////方法1 需设置 数组大小，不用手动释放  就使用这个了
	//wchar_t wszPath[_MAX_PATH]={0};
	//CStdCommond::MultiByteToWideChar(szpa,wszPath,CP_ACP);
	//csPath = wszPath;
	////方法2  不用先设置 指针大小 但要在用完后手动释放
	//wchar_t *pwszPath=NULL;
	//CStdCommond::MultiByteToWideChar(szpa,&pwszPath,CP_ACP);
	//wstrPath = pwszPath;
	//delete pwszPath;


	wchar_t wszWideChar[_MAX_PATH]={0};
	MultiByteToWideChar(pszMultiByte,wszWideChar,iCodePage);
	wstring wstrWideChar = wszWideChar;
	return wstrWideChar;
}

//CStringW CGlobalFunction::MultiByteToWideChar(const char* pszMultiByte, int iCodePage)
//{
//	wchar_t wszWideChar[_MAX_PATH]={0};
//	MultiByteToWideChar(pszMultiByte,wszWideChar,iCodePage);
//	CStringW wcsWideChar = wszWideChar;
//	return wcsWideChar;
//}


string CGlobalFunction::WideCharToMultiByte(const wchar_t* pwszWideChar, int iCodePage)
{
	char szMultiByte[_MAX_PATH]={0};
	WideCharToMultiByte(pwszWideChar,szMultiByte,iCodePage);
	string strMultiByte = szMultiByte;

	return strMultiByte;

}

bool CGlobalFunction::Lower(string& strConvert)
{
	try
	{
		string::iterator it ;
		for (it=strConvert.begin(); it!=strConvert.end(); ++it)
		{
			if (isascii(*it) && isupper(*it))
			{
				*it = tolower(*it) ;
			}
		}

		return true ;
	}
	catch (...)
	{
		return false ;
	}
}

bool CGlobalFunction::Upper(string& strConvert)
{
	try
	{
		string::iterator it ;
		for (it=strConvert.begin(); it!=strConvert.end(); ++it)
		{
			if (isascii(*it) && islower(*it))
			{
				*it = toupper(*it) ;
			}
		}

		return true ;
	}
	catch (...)
	{
		return false ;
	}
}
