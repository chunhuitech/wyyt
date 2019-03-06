/*! @file info
******************************************************************************** 
<PRE> 
模块名 : <文件所属的模块名称> 
文件名 : <GlobalFunction.h> 
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

#ifndef __GlobalFunction__
#define __GlobalFunction__


#ifdef WIN32
#include <Winsock2.h>  //包含MAX_PATH GetModuleFileName的定义等
#include <direct.h>  //包含_mkdir等函数
#include <time.h>  //包含localtime等
#else // UNIX

#endif

#include <assert.h>  // 包含对 assert(m_hFile != NULL); 宏的定义
//#include <sys/types.h>
#include <errno.h> //包含  EAGAIN 等错误类型定义
#include <sys/stat.h> // 包含对 if (stat(lpszFileName, &filestat) == 0) 中 stat的定义 
#include <stdlib.h>  //包含 NULL定义等
#include <string.h>  //包含strcpy等
#include <stdio.h>  //包含sprintf等

#include "devlib_include.h"


class CGlobalFunction
{
public:
	


	


	

#pragma  region  字符串转换处理 

	static char *itoa(const int value, char * const string, const int radix);

	//字符转换

	/**
	*	多字节字符串转换为宽字符
	*@param pszMultiByte
	*	[in] 多字节字符串
	*@param pwszWideChar
	*	[out] 宽字符串
	*@param iCodePage
	*	[in] 代码页	
	iCodePage 默认为CP_ACP(0)  而一般用到string.c_str()做为pszMultiByte 是采用 CP_UTF8
	*@return int
	*	宽字符数
	*/
	static int MultiByteToWideChar(const char* pszMultiByte, wchar_t* pwszWideChar, int iCodePage = 0);
	static int MultiByteToWideChar(const char* pszMultiByte, wchar_t** pwszWideChar, int iCodePage = 0);

	/**
	*	宽字符字符串转换为多字节
	*@param pwszWideChar
	*	[in] 宽字符字符串
	*@param pszMultiByte
	*	[in] 多字节字符串
	*@param iCodePage
	*	[in] 代码页
	*@return sring
	*	多字节字符串
	*/
	static int WideCharToMultiByte(const wchar_t* pwszWideChar,char* pszMultiByte, int iCodePage = 0);

	/**
	 *	把字符串中的大写字母转化为小写字母
	 *@param strConvert
	 *	[in] 要转化的字符串 调用后转为小写字母
	 *@return bool
	 *	
	 */
	static bool Lower(string& strConvert) ; 
	/**
	 *	把字符串中的小写字母转化为大写字母
	 *@param strConvert
	 *	[in] 要转化的字符串 调用后转为大写字母
	 *@return string
	 *	大写字符串
	 */
	static bool Upper(string& strConvert) ;



	/**
	*	多字节字符串转换为宽字符
	*@param pszMultiByte
	*	[in] 多字节字符串
	*@param iCodePage
	*	[in] 代码页
	*@return wstring
	*	宽字符字符串
	*/
	static wstring MultiByteToWideChar(const char* pszMultiByte, int iCodePage = 0);
	//static CStringW MultiByteToWideChar(const char* pszMultiByte, int iCodePage = 0);

	/**
	*	宽字符字符串转换为多字节
	*@param pwszWideChar
	*	[in] 宽字符字符串
	*@param iCodePage
	*	[in] 代码页
	*@return sring
	*	多字节字符串
	*/
	static string WideCharToMultiByte(const wchar_t* pwszWideChar, int iCodePage = 0);


#pragma  endregion


};

#endif // __GlobalFunction__

