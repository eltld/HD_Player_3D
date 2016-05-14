#pragma once


// CHD_Player_3D_Register 对话框

class CHD_Player_3D_Register : public CDialog
{
	DECLARE_DYNAMIC(CHD_Player_3D_Register)

public:
	CHD_Player_3D_Register(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHD_Player_3D_Register();

// 对话框数据
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConform();
};
