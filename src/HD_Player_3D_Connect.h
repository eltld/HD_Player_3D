#pragma once


// CHD_Player_3D_Connect �Ի���

class CHD_Player_3D_Connect : public CDialog
{
	DECLARE_DYNAMIC(CHD_Player_3D_Connect)

public:
	CHD_Player_3D_Connect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHD_Player_3D_Connect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();

	virtual BOOL OnInitDialog();
};
