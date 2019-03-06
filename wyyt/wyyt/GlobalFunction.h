/*! @file info
******************************************************************************** 
<PRE> 
ģ���� : <�ļ�������ģ������> 
�ļ��� : <GlobalFunction.h> 
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

#ifndef __GlobalFunction__
#define __GlobalFunction__


#ifdef WIN32
#include <Winsock2.h>  //����MAX_PATH GetModuleFileName�Ķ����
#include <direct.h>  //����_mkdir�Ⱥ���
#include <time.h>  //����localtime��
#else // UNIX

#endif

#include <assert.h>  // ������ assert(m_hFile != NULL); ��Ķ���
//#include <sys/types.h>
#include <errno.h> //����  EAGAIN �ȴ������Ͷ���
#include <sys/stat.h> // ������ if (stat(lpszFileName, &filestat) == 0) �� stat�Ķ��� 
#include <stdlib.h>  //���� NULL�����
#include <string.h>  //����strcpy��
#include <stdio.h>  //����sprintf��

#include "devlib_include.h"


class CGlobalFunction
{
public:
	


	


	

#pragma  region  �ַ���ת������ 

	static char *itoa(const int value, char * const string, const int radix);

	//�ַ�ת��

	/**
	*	���ֽ��ַ���ת��Ϊ���ַ�
	*@param pszMultiByte
	*	[in] ���ֽ��ַ���
	*@param pwszWideChar
	*	[out] ���ַ���
	*@param iCodePage
	*	[in] ����ҳ	
	iCodePage Ĭ��ΪCP_ACP(0)  ��һ���õ�string.c_str()��ΪpszMultiByte �ǲ��� CP_UTF8
	*@return int
	*	���ַ���
	*/
	static int MultiByteToWideChar(const char* pszMultiByte, wchar_t* pwszWideChar, int iCodePage = 0);
	static int MultiByteToWideChar(const char* pszMultiByte, wchar_t** pwszWideChar, int iCodePage = 0);

	/**
	*	���ַ��ַ���ת��Ϊ���ֽ�
	*@param pwszWideChar
	*	[in] ���ַ��ַ���
	*@param pszMultiByte
	*	[in] ���ֽ��ַ���
	*@param iCodePage
	*	[in] ����ҳ
	*@return sring
	*	���ֽ��ַ���
	*/
	static int WideCharToMultiByte(const wchar_t* pwszWideChar,char* pszMultiByte, int iCodePage = 0);

	/**
	 *	���ַ����еĴ�д��ĸת��ΪСд��ĸ
	 *@param strConvert
	 *	[in] Ҫת�����ַ��� ���ú�תΪСд��ĸ
	 *@return bool
	 *	
	 */
	static bool Lower(string& strConvert) ; 
	/**
	 *	���ַ����е�Сд��ĸת��Ϊ��д��ĸ
	 *@param strConvert
	 *	[in] Ҫת�����ַ��� ���ú�תΪ��д��ĸ
	 *@return string
	 *	��д�ַ���
	 */
	static bool Upper(string& strConvert) ;



	/**
	*	���ֽ��ַ���ת��Ϊ���ַ�
	*@param pszMultiByte
	*	[in] ���ֽ��ַ���
	*@param iCodePage
	*	[in] ����ҳ
	*@return wstring
	*	���ַ��ַ���
	*/
	static wstring MultiByteToWideChar(const char* pszMultiByte, int iCodePage = 0);
	//static CStringW MultiByteToWideChar(const char* pszMultiByte, int iCodePage = 0);

	/**
	*	���ַ��ַ���ת��Ϊ���ֽ�
	*@param pwszWideChar
	*	[in] ���ַ��ַ���
	*@param iCodePage
	*	[in] ����ҳ
	*@return sring
	*	���ֽ��ַ���
	*/
	static string WideCharToMultiByte(const wchar_t* pwszWideChar, int iCodePage = 0);


#pragma  endregion


};

#endif // __GlobalFunction__

