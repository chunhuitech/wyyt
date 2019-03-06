#pragma once
#include "ResizableLib/ResizableDialog.h"
#include "afxcmn.h"
#include "DirectoryTreeCtrl.h"
#include "HTRichEditCtrl.h"
// CRecordDetailsClassDlg 对话框

class CRecordDetailsClassDlg : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetailsClassDlg)

public:
	CRecordDetailsClassDlg(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // 标准构造函数
	virtual ~CRecordDetailsClassDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_CLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Localize();
	CDirectoryTreeCtrl m_ClassSelector;
	//void InitClassInfos(void);
	void LoadSelectorClasses(vector<int64>* vecSelClass, int64 nMainClassID);
	bool GetSelectedClasses(vector<int64>* vecSelClass,int64& nMainClassID);
	

	BOOL OnApply();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	EModuleType m_emt;
};
