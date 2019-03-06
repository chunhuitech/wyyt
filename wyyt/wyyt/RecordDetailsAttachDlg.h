#pragma once
#include "afxcmn.h"
#include "ResizableLib/ResizableDialog.h"
#include "wyytExtendDataManagePri.h"
// CRecordDetailsAttachDlg 对话框

class CRecordDetailsAttachDlg : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetailsAttachDlg)

public:
	CRecordDetailsAttachDlg(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // 标准构造函数
	virtual ~CRecordDetailsAttachDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_ATTACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void InitCurRecordInfo(int64 nCurRecordID, int64 nMainClassID);
	void Localize();
	CListCtrl m_listAttachs;
	void FillAttachsList();
	void LoadAttach(size_t index);
	void ReallySave(int nOperOption, uint64& getFileAllSize);
	void ReallyCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	int64 m_nCurRecordID;
	int64 m_nMainClassID;
	int GetMaxIndexID();
	int m_nExistAttCount;//已经存在的最大索引编号
	int m_nCurAttCount;//当前新增加到的索引编号
	vector<CString> m_vecModOldFiles;
	vector<CString> m_vecMulUpFiles;//存放多上传送的文件 即如果进行取消操作，则原来上传的文件要删除掉
	vector<CRecordAttachs> m_vecTempCurAttachs;
	EModuleType m_emt;
public:
	afx_msg void OnNMRClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
