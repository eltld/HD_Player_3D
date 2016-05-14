// HD_Player_3D_Network.cpp : 实现文件
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3D_Network.h"

#include "HD_Player_3DDlg.h"
#include "network.h"

#include "HD_Player_3D_Register.h"

#include "Network\client.h"



// CHD_Player_3D_Network 对话框

IMPLEMENT_DYNAMIC(CHD_Player_3D_Network, CDialog)

CHD_Player_3D_Network::CHD_Player_3D_Network(CWnd* pParent /*=NULL*/)
	: CDialog(CHD_Player_3D_Network::IDD, pParent)
{
}

CHD_Player_3D_Network::~CHD_Player_3D_Network()
{
}

void CHD_Player_3D_Network::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
}


BEGIN_MESSAGE_MAP(CHD_Player_3D_Network, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CHD_Player_3D_Network::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CHD_Player_3D_Network::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CHD_Player_3D_Network::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CHD_Player_3D_Network::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CHD_Player_3D_Network::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// CHD_Player_3D_Network 消息处理程序

extern Client aClient;


BOOL CHD_Player_3D_Network::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_SERVER,Server);//显示服务器地址
	SetDlgItemText(IDC_EDIT_USER,_T("jinjin"));
	SetDlgItemText(IDC_EDIT_PASSWORD,_T("123456"));
	SetDlgItemText(IDC_EDIT_SAVEPATH,_T("F:\\"));

	FileListNum_Downloaded=0;////////////////////

	return 0;
}


void CHD_Player_3D_Network::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_EDIT_USER,User);
	GetDlgItemText(IDC_EDIT_PASSWORD,Password);

	//CString videolist;
	//CString videolist;
	aClient.login(User,Password,videolist);

	if(videolist==_T(""))
	{
		//::AfxMessageBox(_T("获取服务器文件信息失败 列表由本地创建"));
		//videolist=_T("hehua.264kroger.264ballroom.264");
		::AfxMessageBox(_T("获取服务器文件信息失败"));
	}
	else if(videolist!=_T(""))
	{
		ListFile();
	}	
}

void CHD_Player_3D_Network::ListFile(void)
{
	//=====================此处需要加上把返回的文件名信息转换成 CString 数组的操作

	//=====================将服务器返回的文件信息 转换为需要用的 FileList FileListNum

	int l=videolist.GetLength();
	int num=0;
	int start=0;
	for(int i=0;i<l-3;i++)
	{
		if(videolist.Mid(i,4)==_T(".264"))
		{
			FileList[num]=videolist.Mid(start,i-start+4);
			start=i+4;
			num=num+1;
			//::AfxMessageBox(FileList[num]);
		}
	}

	CString inf;
	for(int i=0;i<num;i++)
	{
		//::AfxMessageBox(FileList[i]);
		inf=inf+FileList[i]+_T(" ");
	}
	::AfxMessageBox(_T("获取服务器文件列表: ")+inf);

	FileListNum=num;

	//=================================================================
	//此处解决了从第二个文件名开始每个字符串对象前面多了个" "的问题
	for(int i=1;i<=FileListNum;i++)
	{
		FileList[i]=FileList[i].Mid(1,FileList[i].GetLength()-1);
	}
	//=================================================================
	//此处解决了从第二个文件名开始每个字符串对象前面多了个" "的问题

	
	//=====================将服务器返回的文件信息 转换为需要用的 FileList FileListNum


	//=================================================================
	//显示可下载文件列表

	DWORD dwStyle = m_FileList.GetExtendedStyle();
	//m_FileList.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_FileList.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_FileList.DeleteAllItems();
	m_FileList.InsertColumn(0,_T("File Name"),LVCFMT_LEFT,340,0);

	for(int i=0;i<FileListNum;i++)
	{
		m_FileList.InsertItem(i,FileList[i]);
	}
	//=================================================================
	//显示可下载文件列表
}

void CHD_Player_3D_Network::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码	

	GetDlgItemText(IDC_EDIT_SAVEPATH,SavePath);

	//===================================================================== 
	CString str;

	for(int i=0;i<m_FileList.GetItemCount();i++)
	{
		//if(m_FileList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_FileList.GetCheck(i))
		if(m_FileList.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{ 
			str.Format(_T("下载文件: ")+SavePath+FileList[i]);

			//::AfxMessageBox(str,MB_OKCANCEL);
			if(::AfxMessageBox(str,MB_OKCANCEL)==IDOK)
			{
				//=========================在此处加 下载文件代码
				CString Videoname=FileList[i];				
				aClient.getvideo(Videoname,SavePath);
				//=========================增加下载列表信息
				FileListNum_Downloaded=FileListNum_Downloaded+1;
				FileList_Downloaded[FileListNum_Downloaded]=SavePath+FileList[i];
				//=========================增加下载列表信息
			}
		} 
	}

}

void CHD_Player_3D_Network::OnBnClickedButtonRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CHD_Player_3D_Register RegisterDlg;
	RegisterDlg.DoModal();
}

void CHD_Player_3D_Network::OnBnClickedButtonSaveas()
{
	// TODO: 在此添加控件通知处理程序代码
	//CFileDialog dlgOpen(TRUE,NULL,NULL,OFN_HIDEREADONLY,strFile);

	CFileDialog dlgSaveAs(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT);
	dlgSaveAs.DoModal();
}

void CHD_Player_3D_Network::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	aClient.quit();
	OnOK();
}





