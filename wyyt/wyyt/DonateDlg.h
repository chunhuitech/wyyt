#pragma once


// CDonateDlg �Ի���

class CDonateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDonateDlg)

public:
	CDonateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDonateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DONATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
