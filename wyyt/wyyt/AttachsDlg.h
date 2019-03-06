#pragma once
#include "ResizableLib/ResizablePage.h"
#include "afxcmn.h"
#include "interface/DB/RecordItem.h"
#include "wyytExtendDataManagePri.h"
// CAttachsDlg 对话框

class CAttachsDlg : public CResizablePage
{
	DECLARE_DYNAMIC(CAttachsDlg)

public:
	CAttachsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAttachsDlg();
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
	void SetReducedDialog()								 
	{ 
		m_bReducedDlg = true; 
	}
	void Localize();
protected:
	const CSimpleArray<void*>* m_paFiles;
	const CRecordItem* m_pRI;
	bool		m_bDataChanged;
	bool		m_bReducedDlg;
// 对话框数据
	enum { IDD = IDD_DIALOG_ATTACHS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	vector<CRecordAttachs> m_vecTempCurAttachs;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listAttachs;
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	void FillAttachsList();
	void LoadAttach(size_t index);
	afx_msg void OnNMClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	EModuleType m_emt;
	afx_msg void OnLvnItemchangedListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
};
