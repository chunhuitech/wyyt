#pragma once
#include "ResizableLib/ResizablePage.h"
#include "RichEditCtrlX.h"
#include "afxcmn.h"
#include "interface/DB/RecordItem.h"
#include "wyytExtendDataManagePri.h"
#include "WyytRichEditCtrl.h"
// CArchivePreviewDlg 对话框

class CArchivePreviewDlg : public CResizablePage
{
	DECLARE_DYNAMIC(CArchivePreviewDlg)

public:
	CArchivePreviewDlg();   // 标准构造函数
	virtual ~CArchivePreviewDlg();
	void SetModuleType(EModuleType emt)
	{
		m_emt = emt;
	}
	void SetFiles(const CSimpleArray<void*>* paFiles) 
	{ 
		m_paFiles = paFiles; 
		if ( paFiles && m_paFiles->GetSize() >= 1 )
		{
			m_pRI = (const CRecordItem*)((*m_paFiles)[0]);
		}
		m_bDataChanged = true;	
	}
	//void SetRecord(const CRecordItem* pRecord) { m_pRI = pRecord; m_bDataChanged = true;	}
	void SetReducedDialog()								 { m_bReducedDlg = true; }
	void Localize();
protected:
	const CSimpleArray<void*>* m_paFiles;
	//CSimpleArray<const CRecordItem*> m_aFiles;
	const CRecordItem* m_pRI;
	bool		m_bDataChanged;
	bool		m_bReducedDlg;
// 对话框数据
	enum { IDD = IDD_DIALOG_ARCHPREV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CWyytRichEditCtrl m_ctlRENr; //CRichEditCtrl 
	afx_msg void OnDestroy();
	virtual BOOL OnSetActive();

	EModuleType m_emt;
};
