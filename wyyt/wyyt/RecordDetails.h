#pragma once
#include "ResizableLib/ResizableDialog.h"
#include "afxcmn.h"
#include "RecordDetailsClassDlg.h"
#include "RecordDetailsContentDlg.h"
#include "RecordDetailsAttachDlg.h"
// CRecordDetails 对话框

class CRecordDetails : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetails)

public:
	CRecordDetails(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // 标准构造函数
	virtual ~CRecordDetails();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD_DETAILS };


	CImageList m_ImageList;
	void InitListCtrl( CListCtrl& listCtrl );
	void InitOperItems();
	void ReleaseOperItems();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listOperOption;
	void Localize();
	CRecordDetailsClassDlg* m_pRecDetClaDlg;
	CRecordDetailsContentDlg* m_pRecDetContDlg;
	CRecordDetailsAttachDlg* m_pRecDetAttachDlg;
	CWnd*			activewnd;
	void SetActiveDialog(CWnd* dlg);
	afx_msg void OnNMClickListOperOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void SetCurOperOption(int nOpt);
	void InitCurRecordInfo(CRecordItem* pRI);
	int GetCurOperOption();
	int m_nCurOperOPtion;

	void ReallySave_Attachs(int nOperOption);
	void ReallyCancel_Attach();
	CRecordItem* m_pRI;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	EModuleType m_emt;
};
