/*! @file info
******************************************************************************** 
<PRE> 
ģ���� : <�ļ�������ģ������> 
�ļ��� : <GlobalFunction.cpp> 
����ļ� : <����ļ���ص������ļ�> 
�ļ�ʵ�ֹ��� :
 * 1- 
 * 2- 
���� : <IHAS> 
�汾 : <1.0> 
��Ȩ : Copyright (c) 2010 IHAS
�������� : <2010/10/29>
-------------------------------------------------------------------------------- 
��ע : <����˵��> 
-------------------------------------------------------------------------------- 
�޸ļ�¼ : 
�� ��         �汾           �޸���           �޸����� 
YYYY/MM/DD    X.Y        <IHAS>     <�޸�����> 
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
		//ÿһ�ε��� MultiByteToWideChar ��ȡ���ַ� �ռ��С iWideCharSize �Դ�������
		int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);

		if (0 == iWideCharSize)
		{
			throw -1;
		}

		pwszWideCharTemp = new wchar_t[iWideCharSize];
		memset(pwszWideCharTemp, 0, iWideCharSize*sizeof(wchar_t));

		//ÿ���ε��� MultiByteToWideChar ����������ת��
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
		//ÿһ�ε��� MultiByteToWideChar ��ȡ���ַ� �ռ��С iWideCharSize �Դ�������
		int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);

		if (0 == iWideCharSize)
		{
			throw -1;
		}

		(*pwszWideChar) = new wchar_t[iWideCharSize];
		memset(*pwszWideChar, 0, iWideCharSize*sizeof(wchar_t));

		//ÿ���ε��� MultiByteToWideChar ����������ת��
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

	////����1 ������ �����С�������ֶ��ͷ�  ��ʹ�������
	//wchar_t wszPath[_MAX_PATH]={0};
	//CStdCommond::MultiByteToWideChar(szpa,wszPath,CP_ACP);
	//csPath = wszPath;
	////����2  ���������� ָ���С ��Ҫ��������ֶ��ͷ�
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
