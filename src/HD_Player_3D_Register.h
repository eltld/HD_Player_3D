#pragma once


// CHD_Player_3D_Register �Ի���

class CHD_Player_3D_Register : public CDialog
{
	DECLARE_DYNAMIC(CHD_Player_3D_Register)

public:
	CHD_Player_3D_Register(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHD_Player_3D_Register();

// �Ի�������
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConform();
};
