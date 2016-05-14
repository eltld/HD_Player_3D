#pragma once
#include "afxcmn.h"


// CHD_Player_3D_FileList 对话框

class CHD_Player_3D_FileList : public CDialog
{
	DECLARE_DYNAMIC(CHD_Player_3D_FileList)

public:
	CHD_Player_3D_FileList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHD_Player_3D_FileList();

	// 对话框数据
	enum { IDD = IDD_DIALOG_FILELIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CListCtrl m_FileList_Play;
	afx_msg void OnBnClickedButtonListUp();
	afx_msg void OnBnClickedButtonListDown();
	void ListFile(void);
	afx_msg void OnBnClickedButtonPlay();
};
