#pragma once
#include "PPgGeneral.h"
#include "PPgDirSet.h"
#include "PPgSecurity.h"
#include "otherfunctions.h"
#include "TreePropSheet.h"
#include "Neo\ModeLess.h" // NEO: MLD - [ModelesDialogs] <-- Xanatos --
class CPreferencesDlg :	public CModTreePropSheet
{
	DECLARE_DYNAMIC(CPreferencesDlg)

public:
	CPreferencesDlg(void);
	virtual ~CPreferencesDlg(void);

	CPPgGeneral		m_wndGeneral;
	CPPgDirSet	m_wndDirectories;
//#ifndef _PUBLISH_
	CPPgSecurity	m_wndSecurity;
//#endif // _PUBLISH_
	void Localize();
	void SetStartPage(UINT uStartPageID);


protected:
	LPCTSTR m_pPshStartPage;
	bool m_bSaveIniFile;

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

