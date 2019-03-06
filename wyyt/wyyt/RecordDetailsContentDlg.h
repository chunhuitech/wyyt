#pragma once
#include "ResizableLib/ResizableDialog.h"
#include "interface/DB/RecordItem.h"
#include "wyytExtendDataManagePri.h"
#include "afxcmn.h"
#include "WyytRichEditCtrl.h"
// CRecordDetailsContentDlg 对话框

class CRecordDetailsContentDlg : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetailsContentDlg)

public:
	CRecordDetailsContentDlg(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // 标准构造函数
	virtual ~CRecordDetailsContentDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Localize();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditContent();
	void BliandData();

	void InitCurRecordInfo(CRecordItem* pRI);
	void GetRecordInfo();

	CRecordItem* m_pRI;
	CRichEditCtrl/*CHTRichEditCtrl*/ m_RECSelClaInfo;
	void ShowSelectedClasses(vector<int64>& vecSelClass,int64 nMainClassID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWyytRichEditCtrl m_REContent;

	EModuleType m_emt;
};
