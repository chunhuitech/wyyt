/*! \mainpage WYYT Index Page
 *
 * \section intro_sec Introduction----------
 *
 * This is the introduction.
 *
 * \section install_sec Installation------------
 *
 * \subsection step1 Step 1: Opening the box
 *  
 * \section install_his History--------------
 * 1.CwyytApp派生于CWinApp类，而非默认的CWinAppEx类\r\n
 warning C4520: “CwyytApp”: 指定了多个默认构造函数  把原来的无参构造函数删除  //CwyytApp(); \r\n
 * 2.\r\n
 * \section 待测试---------------
 * 1.InitDEP()和 InitHeapCorruptionDetection()函数的具体作用\r\n
 * 2.VISTA及以上版本下面变量的程序目录：\r\n
 *LocalAppData PersonalDownloads PublicDownloads ProgrammData \r\n
 * VISTA以下版本下面环境变量的目录：\r\n
 *AppData ---<user name>\Application Data \r\n
 *Personal --- // My Documents
 * \section 加载流程---------------
 * 1.CwyytApp构造函数\r\n
 * 2.CwyytApp::InitInstance() \r\n
 * \section 细节后续梳理---------------
 *-------------wyyt_20120810.rar------
 * 1.	 InitDEP() 和 InitHeapCorruptionDetection()　函数\r\n
 * 2. CPreferences::GetDefaultDirectory(WDefaultDirectory eDirectory, bool bCreate)　一些特定目录的设置方法
 * 3.Debug 下 InitThreadLocale();函数的具体测试应用\r\n
 * \section 编程笔记---------------
 *
 *CAtlArray<CString> m_ArrayAllDir;　类型添加数组成员及修改成员的方法：\r\n
 * 添加：m_ArrayAllDir.Add(L"22324");\r\n
 * 修改：m_ArrayAllDir.SetAt(0,L"ssfdfsd");\r\n
 * 必须先添加，才能用后面的SetAt，否则会越界.\r\n
 * \section 开发过程中遇到的问题及解决---------------
 * 1.VerQueryValue(  GetFileVersionInfo( GetFileVersionInfoSize(　这三个函数的调用需要导入库 version.lib
 * etc...
 */
