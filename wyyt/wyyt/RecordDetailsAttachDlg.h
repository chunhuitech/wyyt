#pragma once
#include "afxcmn.h"
#include "ResizableLib/ResizableDialog.h"
#include "wyytExtendDataManagePri.h"
// CRecordDetailsAttachDlg �Ի���

class CRecordDetailsAttachDlg : public CResizableDialog
{
	DECLARE_DYNAMIC(CRecordDetailsAttachDlg)

public:
	CRecordDetailsAttachDlg(CWnd* pParent = NULL,EModuleType emt = EMT_SYS);   // ��׼���캯��
	virtual ~CRecordDetailsAttachDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_ATTACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	int m_nExistAttCount;//�Ѿ����ڵ�����������
	int m_nCurAttCount;//��ǰ�����ӵ����������
	vector<CString> m_vecModOldFiles;
	vector<CString> m_vecMulUpFiles;//��Ŷ��ϴ��͵��ļ� ���������ȡ����������ԭ���ϴ����ļ�Ҫɾ����
	vector<CRecordAttachs> m_vecTempCurAttachs;
	EModuleType m_emt;
public:
	afx_msg void OnNMRClickListAttachs(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
