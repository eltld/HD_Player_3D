#pragma once
#include "afxcmn.h"


// CHD_Player_3D_Network 对话框

class CHD_Player_3D_Network : public CDialog
{
	DECLARE_DYNAMIC(CHD_Player_3D_Network)

public:
	CHD_Player_3D_Network(CWnd* pParent = NULL);    // 标准构造函数
	virtual ~CHD_Player_3D_Network();

// 对话框数据
	enum { IDD = IDD_DIALOG_NETWORK};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	void ListFile(void);
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonSaveas();

	virtual BOOL OnInitDialog();
	//virtual BOOL CHD_Player_3D_Network::OnInitDialog()
	
	CListCtrl m_FileList;
	
};
