#pragma once


// CCheckPwdDlg �Ի���

class CCheckPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckPwdDlg)

public:
	CCheckPwdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckPwdDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN_PRI };
	void Localize(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
