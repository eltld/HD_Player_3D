// HD_Player_3D_Network.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3D_Network.h"

#include "HD_Player_3DDlg.h"
#include "network.h"

#include "HD_Player_3D_Register.h"

#include "Network\client.h"



// CHD_Player_3D_Network �Ի���

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


// CHD_Player_3D_Network ��Ϣ�������

extern Client aClient;


BOOL CHD_Player_3D_Network::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_SERVER,Server);//��ʾ��������ַ
	SetDlgItemText(IDC_EDIT_USER,_T("jinjin"));
	SetDlgItemText(IDC_EDIT_PASSWORD,_T("123456"));
	SetDlgItemText(IDC_EDIT_SAVEPATH,_T("F:\\"));

	FileListNum_Downloaded=0;////////////////////

	return 0;
}


void CHD_Player_3D_Network::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItemText(IDC_EDIT_USER,User);
	GetDlgItemText(IDC_EDIT_PASSWORD,Password);

	//CString videolist;
	//CString videolist;
	aClient.login(User,Password,videolist);

	if(videolist==_T(""))
	{
		//::AfxMessageBox(_T("��ȡ�������ļ���Ϣʧ�� �б��ɱ��ش���"));
		//videolist=_T("hehua.264kroger.264ballroom.264");
		::AfxMessageBox(_T("��ȡ�������ļ���Ϣʧ��"));
	}
	else if(videolist!=_T(""))
	{
		ListFile();
	}	
}

void CHD_Player_3D_Network::ListFile(void)
{
	//=====================�˴���Ҫ���ϰѷ��ص��ļ�����Ϣת���� CString ����Ĳ���

	//=====================�����������ص��ļ���Ϣ ת��Ϊ��Ҫ�õ� FileList FileListNum

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
	::AfxMessageBox(_T("��ȡ�������ļ��б�: ")+inf);

	FileListNum=num;

	//=================================================================
	//�˴�����˴ӵڶ����ļ�����ʼÿ���ַ�������ǰ����˸�" "������
	for(int i=1;i<=FileListNum;i++)
	{
		FileList[i]=FileList[i].Mid(1,FileList[i].GetLength()-1);
	}
	//=================================================================
	//�˴�����˴ӵڶ����ļ�����ʼÿ���ַ�������ǰ����˸�" "������

	
	//=====================�����������ص��ļ���Ϣ ת��Ϊ��Ҫ�õ� FileList FileListNum


	//=================================================================
	//��ʾ�������ļ��б�

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
	//��ʾ�������ļ��б�
}

void CHD_Player_3D_Network::OnBnClickedButtonDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	GetDlgItemText(IDC_EDIT_SAVEPATH,SavePath);

	//===================================================================== 
	CString str;

	for(int i=0;i<m_FileList.GetItemCount();i++)
	{
		//if(m_FileList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_FileList.GetCheck(i))
		if(m_FileList.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{ 
			str.Format(_T("�����ļ�: ")+SavePath+FileList[i]);

			//::AfxMessageBox(str,MB_OKCANCEL);
			if(::AfxMessageBox(str,MB_OKCANCEL)==IDOK)
			{
				//=========================�ڴ˴��� �����ļ�����
				CString Videoname=FileList[i];				
				aClient.getvideo(Videoname,SavePath);
				//=========================���������б���Ϣ
				FileListNum_Downloaded=FileListNum_Downloaded+1;
				FileList_Downloaded[FileListNum_Downloaded]=SavePath+FileList[i];
				//=========================���������б���Ϣ
			}
		} 
	}

}

void CHD_Player_3D_Network::OnBnClickedButtonRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHD_Player_3D_Register RegisterDlg;
	RegisterDlg.DoModal();
}

void CHD_Player_3D_Network::OnBnClickedButtonSaveas()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CFileDialog dlgOpen(TRUE,NULL,NULL,OFN_HIDEREADONLY,strFile);

	CFileDialog dlgSaveAs(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT);
	dlgSaveAs.DoModal();
}

void CHD_Player_3D_Network::OnBnClickedButtonLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	aClient.quit();
	OnOK();
}





