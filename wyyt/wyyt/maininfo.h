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
 * 1.CwyytApp������CWinApp�࣬����Ĭ�ϵ�CWinAppEx��\r\n
 warning C4520: ��CwyytApp��: ָ���˶��Ĭ�Ϲ��캯��  ��ԭ�����޲ι��캯��ɾ��  //CwyytApp(); \r\n
 * 2.\r\n
 * \section ������---------------
 * 1.InitDEP()�� InitHeapCorruptionDetection()�����ľ�������\r\n
 * 2.VISTA�����ϰ汾��������ĳ���Ŀ¼��\r\n
 *LocalAppData PersonalDownloads PublicDownloads ProgrammData \r\n
 * VISTA���°汾���滷��������Ŀ¼��\r\n
 *AppData ---<user name>\Application Data \r\n
 *Personal --- // My Documents
 * \section ��������---------------
 * 1.CwyytApp���캯��\r\n
 * 2.CwyytApp::InitInstance() \r\n
 * \section ϸ�ں�������---------------
 *-------------wyyt_20120810.rar------
 * 1.	 InitDEP() �� InitHeapCorruptionDetection()������\r\n
 * 2. CPreferences::GetDefaultDirectory(WDefaultDirectory eDirectory, bool bCreate)��һЩ�ض�Ŀ¼�����÷���
 * 3.Debug �� InitThreadLocale();�����ľ������Ӧ��\r\n
 * \section ��̱ʼ�---------------
 *
 *CAtlArray<CString> m_ArrayAllDir;��������������Ա���޸ĳ�Ա�ķ�����\r\n
 * ��ӣ�m_ArrayAllDir.Add(L"22324");\r\n
 * �޸ģ�m_ArrayAllDir.SetAt(0,L"ssfdfsd");\r\n
 * ��������ӣ������ú����SetAt�������Խ��.\r\n
 * \section �������������������⼰���---------------
 * 1.VerQueryValue(  GetFileVersionInfo( GetFileVersionInfoSize(�������������ĵ�����Ҫ����� version.lib
 * etc...
 */
