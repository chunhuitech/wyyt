#pragma once
#include "RecPPClassDlg.h"
#include "otherfunctions.h"
#include "TreePropSheet.h"
#include "Neo\ModeLess.h" // NEO: MLD - [ModelesDialogs] <-- Xanatos --
class CRecordInfoSheetDlg :	public CModTreePropSheet
{
	DECLARE_DYNAMIC(CRecordInfoSheetDlg)

public:
	CRecordInfoSheetDlg(void);
	virtual ~CRecordInfoSheetDlg(void);

	CRecPPClassDlg		m_wndClass;

	void Localize();
	void SetStartPage(UINT uStartPageID);
	void SetCurOperOption(int nOpt);
	int GetCurOperOption();

protected:
	LPCTSTR m_pPshStartPage;
	bool m_bSaveIniFile;
	int m_nCurOperOPtion;

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

