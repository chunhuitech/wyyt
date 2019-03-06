#pragma once
#include ".\interface\db\RecordAttachs.h"
typedef enum eAttachOperType
{
	AOT_ADD = 0,
	AOT_MOD,
	AOT_SHOW,
}AttachOperType;
// CAttachAddDlg �Ի���

class CAttachAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttachAddDlg)

public:
	CAttachAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttachAddDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECORD_DETAILS_ATTACH_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void Localize();
	void SetRecAttInitInfo(CRecordAttachs* pAttInfo,AttachOperType aot = AOT_ADD);
	afx_msg void OnBnClickedButtonBrowsev();

	CRecordAttachs* m_pRA;
	afx_msg void OnBnClickedOk();

	AttachOperType m_aot;
};
