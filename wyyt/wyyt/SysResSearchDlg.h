#pragma once


// CSysResSearchDlg �Ի���

class CSysResSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysResSearchDlg)

public:
	CSysResSearchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysResSearchDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SYSRES_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
