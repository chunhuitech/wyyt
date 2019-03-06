#pragma once
#include "ResizableLib/ResizableDialog.h"
#include "interface/DB/RecordItem.h"
#include "wyytExtendDataManagePri.h"
#include "afxcmn.h"
#include "WyytRichEditCtrl.h"
// CRecordDetailsContentDlg �Ի���

class CRecordDetailsContentDlg : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetailsContentDlg)

public:
	CRecordDetailsContentDlg(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // ��׼���캯��
	virtual ~CRecordDetailsContentDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
