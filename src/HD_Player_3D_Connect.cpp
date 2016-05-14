// HD_Player_3D_Connect.cpp : 实现文件
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3D_Connect.h"

#include "HD_Player_3D_Network.h"

#include "Network\client.h"

// CHD_Player_3D_Connect 对话框

IMPLEMENT_DYNAMIC(CHD_Player_3D_Connect, CDialog)

CHD_Player_3D_Connect::CHD_Player_3D_Connect(CWnd* pParent /*=NULL*/)
	: CDialog(CHD_Player_3D_Connect::IDD, pParent)
{

}

CHD_Player_3D_Connect::~CHD_Player_3D_Connect()
{
}

void CHD_Player_3D_Connect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHD_Player_3D_Connect, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CHD_Player_3D_Connect::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CHD_Player_3D_Connect 消息处理程序

extern CString Server;
extern Client aClient;

void CHD_Player_3D_Connect::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_EDIT_SERVERADDRESS,Server);

	
	//====================================================
	int length = Server.GetLength();
	char *ip_addr = new char[length+1];
	::wcstombs(ip_addr,Server.GetBuffer(),Server.GetLength());
	ip_addr[length] = '\0';
	aClient.tcpinitial(ip_addr);
	//====================================================
	//====================================================
	delete []ip_addr;
	ip_addr = NULL;
	//====================================================
	

	OnOK();//关闭此对话框

	CHD_Player_3D_Network NetworkDlg;
	NetworkDlg.DoModal();
	
}

BOOL CHD_Player_3D_Connect::OnInitDialog()
{	
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT_SERVERADDRESS,_T("192.168.1.102"));
	return 0;
}
