// HD_Player_3D_Register.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3D_Register.h"

#include "Network\client.h"
//#include "network.h"

extern Client aClient;
extern CString videolist;


// CHD_Player_3D_Register �Ի���

IMPLEMENT_DYNAMIC(CHD_Player_3D_Register, CDialog)

CHD_Player_3D_Register::CHD_Player_3D_Register(CWnd* pParent /*=NULL*/)
: CDialog(CHD_Player_3D_Register::IDD, pParent)
{

}

CHD_Player_3D_Register::~CHD_Player_3D_Register()
{
}

void CHD_Player_3D_Register::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHD_Player_3D_Register, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFORM, &CHD_Player_3D_Register::OnBnClickedButtonConform)
END_MESSAGE_MAP()


// CHD_Player_3D_Register ��Ϣ�������

extern CString User_Reg;
extern CString Password_Reg;

void CHD_Player_3D_Register::OnBnClickedButtonConform()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//GetDlgItemText(IDC_EDIT_SERVER,Server);
	GetDlgItemText(IDC_EDIT_USER1,User_Reg);

	CString pw2;
	GetDlgItemText(IDC_EDIT_PASSWORD1,Password_Reg);
	GetDlgItemText(IDC_EDIT_PASSWORD1_CONFORM,pw2);

	if(Password_Reg!=pw2)
	{
		::AfxMessageBox(_T("���벻һ�� ����������"));
		//��Ҫ��������ĳ���  GDX
	}

	//CString videolist;
	aClient.regist(User_Reg, Password_Reg, videolist);

	OnOK();
}
