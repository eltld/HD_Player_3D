// HD_Player_3D_FileList.cpp : 实现文件
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3D_FileList.h"



extern CString FileList_Downloaded[100];
extern int FileListNum_Downloaded;
extern char filename_264[200];



// CHD_Player_3D_FileList 对话框

IMPLEMENT_DYNAMIC(CHD_Player_3D_FileList, CDialog)

CHD_Player_3D_FileList::CHD_Player_3D_FileList(CWnd* pParent /*=NULL*/)
	: CDialog(CHD_Player_3D_FileList::IDD, pParent)
{

}

CHD_Player_3D_FileList::~CHD_Player_3D_FileList()
{
}

void CHD_Player_3D_FileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_FileList_Play);
}


BEGIN_MESSAGE_MAP(CHD_Player_3D_FileList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LIST_UP, &CHD_Player_3D_FileList::OnBnClickedButtonListUp)
	ON_BN_CLICKED(IDC_BUTTON_LIST_DOWN, &CHD_Player_3D_FileList::OnBnClickedButtonListDown)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CHD_Player_3D_FileList::OnBnClickedButtonPlay)
END_MESSAGE_MAP()


// CHD_Player_3D_FileList 消息处理程序

BOOL CHD_Player_3D_FileList::OnInitDialog()
{	
	CDialog::OnInitDialog();

	ListFile();

	return 0;
}

void CHD_Player_3D_FileList::OnBnClickedButtonListUp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_FileList_Play.SetBkColor((125,125,125));
}

void CHD_Player_3D_FileList::OnBnClickedButtonListDown()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHD_Player_3D_FileList::ListFile(void)
{
	DWORD dwStyle = m_FileList_Play.GetExtendedStyle();
	m_FileList_Play.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_FileList_Play.DeleteAllItems();
	m_FileList_Play.InsertColumn(0,_T("File Name"),LVCFMT_LEFT,235,0);

	for(int i=1;i<=FileListNum_Downloaded;i++)
	{
		m_FileList_Play.InsertItem(i,FileList_Downloaded[i]);
	}		

	//m_FileList_Play.SetBkColor((125,125,125));
}

void CHD_Player_3D_FileList::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码

	for(int i=0;i<=m_FileList_Play.GetItemCount();i++)
	{
		//if(m_FileList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_FileList.GetCheck(i))
		if(m_FileList_Play.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			CString Videoname_Play=FileList_Downloaded[i+1];
			//::wcstombs(filename_264,dlgOpen.GetPathName().GetBuffer(),dlgOpen.GetPathName().GetLength());
			::wcstombs(filename_264,Videoname_Play.GetBuffer(),Videoname_Play.GetLength());
		}
	}
	OnOK();
}
