// HD_Player_3DDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3DDlg.h"
#include "my_head_file.h"
#include "my_head_file_C.h"

#include "HD_Player_3D_Network.h"
#include "HD_Player_3D_Connect.h"
#include "HD_Player_3D_FileList.h"

//#include "Log.h"
//#include "network.h"

//===========================================================================
#include "Network\client.h"
//===========================================================================


//===========================================================================


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//===========================================================================
int counter_264_1;
int counter_264_2;
int flag_264[408];

//===========================================================================
char filename_264[200];
int counter_264;
int width_264;
int height_264;

BOOL volatile Decoding_264;
unsigned char * buffer_264; //YUV
unsigned char * Dec_Buffer_264; //BGR
//===========================================================================

//=======================����264��Ƶ�Ļ��岥��ģʽ===========================
int Dec_264_No;//100 ���־λһ��
int Dec_264_Decode_flag[100];
int Dec_264_Display_flag[100];
int Dec_264_No_Decode;
int Dec_264_No_Display;
int Dec_264_Offset;
//=======================����264��Ƶ�Ļ��岥��ģʽ===========================

//=======================����264��Ƶ���ŵ�ͬ������===========================
HANDLE h_264_Mutex[100];
int Dec_264_No_Decode_total;//���ڿ��ƽ���Ͳ��ŵ���Խ���
int Dec_264_No_Display_total;//���ڿ��ƽ���Ͳ��ŵ���Խ���
DWORD n_Thread_ID_Dec;//�߳�ID ȫ�ֱ���
DWORD n_Thread_ID_Play;//�߳�ID ȫ�ֱ���
//=======================����264��Ƶ���ŵ�ͬ������===========================


//===========================================================================
Client aClient;
//===========================================================================

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHD_Player_3DDlg �Ի���




CHD_Player_3DDlg::CHD_Player_3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHD_Player_3DDlg::IDD, pParent)
	, buffer(NULL)
	, BMPbuffer(NULL)
	, BMPbuffer_(NULL)
	, Input_File_Type(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHD_Player_3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_POS, m_ctrlSliderPos);
	DDX_Control(pDX, IDC_BUFFER, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CHD_Player_3DDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CHD_Player_3DDlg::OnBnClickedButtonOpen)	
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CHD_Player_3DDlg::OnBnClickedButtonPlay)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_POS, &CHD_Player_3DDlg::OnNMCustomdrawSliderPos)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CHD_Player_3DDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CHD_Player_3DDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_BUFFER, &CHD_Player_3DDlg::OnBnClickedButtonBuffer)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CHD_Player_3DDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_NETWORK, &CHD_Player_3DDlg::OnBnClickedButtonNetwork)
	ON_BN_CLICKED(IDOK, &CHD_Player_3DDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LIST, &CHD_Player_3DDlg::OnBnClickedButtonList)
END_MESSAGE_MAP()


// CHD_Player_3DDlg ��Ϣ�������

BOOL CHD_Player_3DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//=====================================================================================//
	//================================================================================��ʼ������ OnInitDialog �и�ֵ
//	Width=640;
//	Height=3072;
	Width=720;
	Height=2880;
	//Height=360;
	//Height_2d=Height/8;
	Height_2d=360;
	Width_=1920;
	Height_=1080;

	Width_origin=1920;
	Height_origin=1024;

	TotalFrames=0;
	CurrentFrame=0;
	m_bPlaying=FALSE;
	m_bFullScreen=FALSE;
	m_bPlayMode=TRUE;// 1 ���� 0 ƽ��
//	m_bPlayMode=FALSE;// 1 ���� 0 ƽ��

	switch(m_bFullScreen) //����ȫ��״̬��־ ���ò�ͬ��ͼ����ʾ��ʽ
	{
	case 0://��ȫ��״̬

		hdc=::GetDC(m_hWnd);
		::GetClientRect(m_hWnd,&rect);

		rect.left=rect.left+7;
		rect.top=rect.top+6;
		rect.right=rect.left+640;//16
		rect.bottom=rect.top+360;//9		
		break;

	case 1://ȫ��״̬

		hdc=::GetDC(m_hWnd);
		::GetWindowRect(m_hWnd,&rect);//2ѡ1 //::GetClientRect(m_hWnd,&rect);//2ѡ1

		rect.left=0;
		rect.top=0;
		rect.right=::GetSystemMetrics(SM_CXSCREEN);
		rect.bottom=::GetSystemMetrics(SM_CYSCREEN);
		break;
	}

	buffer=new unsigned char[Width*Height*1.5];
    ZeroMemory(buffer,Width*Height*1.5);
	BMPbuffer=new unsigned char[Width*Height*3];	
	BMPbuffer_=new unsigned char[Width_*Height_*3];
	ZeroMemory(BMPbuffer,Width*Height*3);
	ZeroMemory(BMPbuffer_,Width_*Height_*3);

	BMPbuffer_origin=new unsigned char[Width_origin*Height_origin*3];
    ZeroMemory(BMPbuffer_origin,Width_origin*Height_origin*3);
	BMPbuffer_origin_=new unsigned char[Width_origin*Height_origin*3];
    ZeroMemory(BMPbuffer_origin_,Width_origin*Height_origin*3);

	//====================================================================�����洢�Ѿ�����õ�mpeg2��������
	//Mpeg2_Frame_Buffer_Size=30;

	//BMPbuffer_Mpeg2=new unsigned char[1920*1080*3*Mpeg2_Frame_Buffer_Size];
	//ZeroMemory(BMPbuffer_Mpeg2,1920*1080*3*Mpeg2_Frame_Buffer_Size);
	//====================================================================�����洢�Ѿ�����õ�mpeg2��������



	Width_origin_1_8=640;
	Height_origin_1_8=3072;
	/*
	//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�
	buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
	ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

	Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
	ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
	//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�
    */

	//=======================����264��Ƶ�Ļ��岥��ģʽ===========================
	counter_264=0;/////////////////////

	Dec_264_No=100;//100 ���־λһ��
	Dec_264_No_Decode=0;
	Dec_264_No_Display=0;
	Dec_264_No_Decode_total=0;
	Dec_264_No_Display_total=0;
	Dec_264_Offset=0;
	//Dec_264_Decode_flag[100];
	//Dec_264_Display_flag[100];
	for(int i=0;i++;i<100)
	{
		Dec_264_Decode_flag[i]=0;
		Dec_264_Display_flag[i]=0;
	}

	
	//=======================����264��Ƶ�Ļ��岥��ģʽ===========================
	

	//BMPbuffer_all=new unsigned char[Width_*Height_*3];
	//ZeroMemory(BMPbuffer_all,Width_*Height_*3);
	//����OPEN ��Ӧ�������� ��ΪҪ�õ����ļ���õ���֡��

	for(int i=0;i<8;i++)
	{BMPbuffer_2d[i]=NULL;}
	for(int i=0;i<8;i++)
	{
		BMPbuffer_2d[i]=BMPbuffer+i*Width*Height_2d*3; //�Ѿ���֤���� ��������ַ û������ OK		
	}

	m_lpBmpInfo=(LPBITMAPINFO)new BITMAPINFO;
	ZeroMemory(m_lpBmpInfo,sizeof(BITMAPINFO));
	m_lpBmpInfo->bmiHeader.biSize=40;
	m_lpBmpInfo->bmiHeader.biWidth=Width;
	m_lpBmpInfo->bmiHeader.biHeight=Height;
	m_lpBmpInfo->bmiHeader.biPlanes=1;
	m_lpBmpInfo->bmiHeader.biBitCount=24;
	m_lpBmpInfo->bmiHeader.biCompression=BI_RGB;
	m_lpBmpInfo->bmiHeader.biSizeImage=Width*Height*3;

	m_lpBmpInfo_=(LPBITMAPINFO)new BITMAPINFO;
	ZeroMemory(m_lpBmpInfo_,sizeof(BITMAPINFO));
	m_lpBmpInfo_->bmiHeader.biSize=40;
	m_lpBmpInfo_->bmiHeader.biWidth=Width_;
	m_lpBmpInfo_->bmiHeader.biHeight=Height_;
	m_lpBmpInfo_->bmiHeader.biPlanes=1;
	m_lpBmpInfo_->bmiHeader.biBitCount=24;
	m_lpBmpInfo_->bmiHeader.biCompression=BI_RGB;
	m_lpBmpInfo_->bmiHeader.biSizeImage=Width_*Height_*3;



	/*
	CString inf;
	inf.Format(_T("%d"),Width); //=======================ע��Ҫ�ú� _T()
	SetDlgItemText(IDC_EDIT_WIDTH,inf);
	inf.Format(_T("%d"),Height);
	SetDlgItemText(IDC_EDIT_HEIGHT,inf);
	inf.Format(_T("%d"),Width_);
	SetDlgItemText(IDC_EDIT_WIDTH_,inf);
	inf.Format(_T("%d"),Height_);
	SetDlgItemText(IDC_EDIT_HEIGHT_,inf);

	inf.Format(_T("%d"),CurrentFrame); //=======================ע��Ҫ�ú� _T()
	SetDlgItemText(IDC_EDIT_FRAME,inf);
	inf.Format(_T("%d"),TotalFrames);
	SetDlgItemText(IDC_EDIT_FRAMES,inf);
	*/

	//================================================================================��ʼ������ OnInitDialog �и�ֵ
	//=====================================================================================//

	//===================================================================YUV2RGB ���ܵ�׼������
	init_dither_tab();
	//===================================================================YUV2RGB ���ܵ�׼������

	//========================================�ϳ��㷨��Ҫ�õ��Ķ���
	for(int i=0;i<3;i++)
	{
		shuzu[i]=i;//shuzu[3]={0,1,2};
	}

	InitDelta(); //�ϳ��㷨��ʼ��
	//========================================�ϳ��㷨��Ҫ�õ��Ķ���

	::QueryPerformanceFrequency(&m_freq);

	//========================================================�����ǽ��빦����Ҫ�õ��Ķ���
	InputFilePath=NULL;
//	Dec_Buffer=new BYTE[720*360*3];//�˴���ȫ�ֱ���
//	ZeroMemory(Dec_Buffer,720*360*3);
	//========================================================�����ǽ��빦����Ҫ�õ��Ķ���

	//================================================================================
	//����Ϊ�����µĲ���ģʽ �������ö�ʱ��
	hThread_Display_YUV=NULL;
	//================================================================================
	//����Ϊ�����µĲ���ģʽ �������ö�ʱ��

	//Ϊ���� �����б� ��Ϊ ��ģ̬�Ի���
	FileList.Create(IDD_DIALOG_FILELIST,this);
	FileListShow=FALSE;
	//Ϊ���� �����б� ��Ϊ ��ģ̬�Ի���

	//=================================================���� HOOK
	::g_hKeyBoard=::SetWindowsHookExW(WH_KEYBOARD,KeyBoardProc,NULL,::GetCurrentThreadId());//���̹���
	::g_hMouse=::SetWindowsHookExW(WH_MOUSE,MouseProc,NULL,::GetCurrentThreadId());//��깳��

	//=================================================���� HOOK

	//=================================================���ÿ���̨ʵʱ���������Ϣ
	::AllocConsole();
	::freopen("CONOUT$","r+t",stdout);
	cout<<"�ڳ����п�����ʱ���� cout ��������ִ����Ϣ:"<<endl;
	//=================================================���ÿ���̨ʵʱ���������Ϣ

	//=================================================LOG	
	log_time.Initialize();
	//=================================================LOG

	//=======================����YUV��Ƶ���ŵ�ͬ������===========================
	hThread_YUV_Read=NULL;
	hThread_YUV_Play=NULL;
	//=======================����YUV��Ƶ���ŵ�ͬ������===========================

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CHD_Player_3DDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHD_Player_3DDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHD_Player_3DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHD_Player_3DDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������

	KillTimer(nID_Timer);
	KillTimer(nID_Timer_Mpeg2);
	KillTimer(nID_Timer_Mpeg2_Play);
	//KillTimer(nID_Timer_264_Decode);
	KillTimer(nID_Timer_264_Play);
	//KillTimer(nID_Timer_264_Play_Buffer);

	DWORD ExitCode;
	::TerminateThread(hThread_Mpeg2Dec,ExitCode);
	::TerminateThread(hThread_264Dec,ExitCode);

	if(buffer)
	{
		delete []buffer;
	}	
	if(BMPbuffer)
	{
		delete []BMPbuffer;
	}
	if(BMPbuffer_)
	{
		delete []BMPbuffer_;
	}

	if(BMPbuffer_origin)
	{
		delete []BMPbuffer_origin;
	}
	if(BMPbuffer_origin_)
	{
		delete []BMPbuffer_origin_;
	}
	if(BMPbuffer_Mpeg2)
	{
		delete []BMPbuffer_Mpeg2;
	}

	if(buffer_264)
	{
		delete []buffer_264;
	}
	if(Dec_Buffer_264)
	{
		delete []Dec_Buffer_264;
	}


	if(m_lpBmpInfo)
	{
		delete []m_lpBmpInfo;
	}
	if(m_lpBmpInfo_)
	{
		delete []m_lpBmpInfo_;
	}

	//===================================�����ڴ�ӳ���ļ�
	//::UnmapViewOfFile(p_File_Map);
	//::CloseHandle(m_hFile);
	//::CloseHandle(m_hFile_map);
	//===================================�����ڴ�ӳ���ļ�
	::ReleaseDC(m_hWnd,hdc);

	//=================================================���ÿ���̨ʵʱ���������Ϣ
	::FreeConsole();
	//=================================================���ÿ���̨ʵʱ���������Ϣ
}

void CHD_Player_3DDlg::OnBnClickedButtonOpen()
{
	CString strFile;
	
	strFile="264 File(*.264)|*.264|";
	strFile+="YUV File(*.yuv)|*.yuv|";

	//strFile="YUV File(*.yuv)|*.yuv|";
	//strFile+="264 File(*.264)|*.264|";
	strFile+="All Files(*.*)|*.*|";

	CFileDialog dlgOpen(TRUE,NULL,NULL,OFN_HIDEREADONLY,strFile);

	if(dlgOpen.DoModal()==IDCANCEL)
	{return;}
	if(m_hFile) //������ǵ�һ�δ��ļ� ��Ҫ��ǰһ�δ�ʱ�����ľ�� ���
	{
		::CloseHandle(m_hFile);
		m_hFile=NULL;
	}

	Input_File_Type=dlgOpen.GetFileExt();

	if(Input_File_Type==_T("264"))
	{
		//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�
		buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
		ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

		Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
		ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
		//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�
		::wcstombs(filename_264,dlgOpen.GetPathName().GetBuffer(),dlgOpen.GetPathName().GetLength());

		OnBnClickedButtonBuffer();
	}	

	else if(Input_File_Type==_T("yuv"))
	{
		m_hFile=::CreateFileW(dlgOpen.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		//====================�ļ�·��              ���ʷ�ʽ     ����ʽ      ��ȫ��־   ��������       �ļ���־    another�ļ����
		//If the function [CreateFile] fails, the return value is INVALID_HANDLE_VALUE
		if(m_hFile==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("���ļ�ʧ��"));
			m_hFile=NULL;
			return;
		}

		//�˴������Ƶ�ļ��ܴ� �����ֽ�����ŵ� LARGE_INTEGER�ͽṹ�����length �� �ָ�λ�͵�λ�ֱ�洢
		LARGE_INTEGER length;//struct LARGE_INTEGER{DWORD LowPart;LONG HighPart;}
		::GetFileSizeEx(m_hFile,&length); //�ֽ��� ����ṹ�� length ��

		TotalFrames=length.QuadPart/(Width*Height*1.5); //��֡��
		CurrentFrame=0; //��ǰ֡

		m_ctrlSliderPos.SetRange(0,TotalFrames);
		m_ctrlSliderPos.SetPos(CurrentFrame);

		m_ctrlProgress.SetRange(0,TotalFrames);
		m_ctrlProgress.SetPos(CurrentFrame);

		switch(m_bPlayMode)
		{
		case TRUE:
			DisplayFrame_YUV(CurrentFrame);
			break;
		case FALSE:
			DisplayFrame_YUV_2d(CurrentFrame);
			break;
		}

		//==================================�����洢 YUV��ȡ������ �ŵ��򿪰�ť������ռ�
		buffer_YUV =new unsigned char[Width*Height*1.5*1*100];//YUV
		ZeroMemory(buffer_YUV,Width*Height*1.5*1*100);
		//==================================�����洢 YUV��ȡ������ �ŵ��򿪰�ť������ռ�

		OnBnClickedButtonBuffer();
	}
	/* //�ڴ�ӳ���ļ�
	else if(Input_File_Type==_T("yuv"))
	{
		m_hFile=::CreateFileW(dlgOpen.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		//====================�ļ�·��              ���ʷ�ʽ     ����ʽ      ��ȫ��־   ��������       �ļ���־    another�ļ����
		//If the function [CreateFile] fails, the return value is INVALID_HANDLE_VALUE
		if(m_hFile==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("���ļ�ʧ��"));
			m_hFile=NULL;
			return;
		}

		//============================================================�����ڴ�ӳ���ļ��ں˶���
		m_hFile_map=::CreateFileMapping(m_hFile,NULL,PAGE_READONLY,0,0,NULL);
		if(m_hFile_map==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("[����] �ڴ�ӳ���ļ� ʧ��"));
			m_hFile_map=NULL;
			return;
		}
		//============================================================�����ڴ�ӳ���ļ��ں˶���
		//============================================================���ڴ�ӳ���ļ�ӳ�䵽���̵ĵ�ַ�ռ�
		//============================================================������������н���
		
		p_File_Map=::MapViewOfFile(m_hFile_map,FILE_MAP_READ,0,0,0);
		if(p_File_Map==NULL)
		{
			::AfxMessageBox(_T("[ӳ��] �ڴ�ӳ���ļ� ʧ��"));
			p_File_Map=NULL;
			return;
		}
		//============================================================���ڴ�ӳ���ļ�ӳ�䵽���̵ĵ�ַ�ռ�


		//�˴������Ƶ�ļ��ܴ� �����ֽ�����ŵ� LARGE_INTEGER�ͽṹ�����length �� �ָ�λ�͵�λ�ֱ�洢
		LARGE_INTEGER length;//struct LARGE_INTEGER{DWORD LowPart;LONG HighPart;}
		::GetFileSizeEx(m_hFile,&length); //�ֽ��� ����ṹ�� length ��

		TotalFrames=length.QuadPart/(Width*Height*1.5); //��֡��
		CurrentFrame=0; //��ǰ֡

		m_ctrlSliderPos.SetRange(0,TotalFrames);
		m_ctrlSliderPos.SetPos(CurrentFrame);

		m_ctrlProgress.SetRange(0,TotalFrames);
		m_ctrlProgress.SetPos(CurrentFrame);

		switch(m_bPlayMode)
		{
		case TRUE:
			//DisplayFrame_YUV(CurrentFrame);
			DisplayFrame(CurrentFrame);
			break;
		case FALSE:
			//DisplayFrame_YUV_2d(CurrentFrame);
			DisplayFrame_2d(CurrentFrame);
			break;
		}
	}
	*/ //�ڴ�ӳ���ļ�

}


void CHD_Player_3DDlg::OnBnClickedButtonBuffer()
{

	if(Input_File_Type==_T("264"))
	{
		/*
		::hThread_264Dec=::CreateThread(NULL,0,H264_Dec_Shell,this,CREATE_SUSPENDED,NULL);
		::Decoding_264=1;
		::ResumeThread(hThread_264Dec);
		*/

		//����һ����ʱ�� ����ʱ�̼�������߳��벥���̵߳Ľ���
		/*SetTimer(nID_Timer_264_Decode,USER_TIMER_MINIMUM,NULL);*/
		//����һ����ʱ�� ����ʱ�̼�������߳��벥���̵߳Ľ���

		//����һ����ʱ�� ������ʾ�����̵߳Ľ���
		SetTimer(nID_Timer_264_Decode,USER_TIMER_MINIMUM,NULL);
		//����һ����ʱ�� ������ʾ�����̵߳Ľ���

		//=========================================================================�����߳�
		//LPDWORD p_Thread_ID_Dec;//�߳�ID ȫ�ֱ���
		::hThread_264Dec=::CreateThread(NULL,0,H264_Dec_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_Dec);
		//::CloseHandle(::hThread_264Dec);//==*** CloseHandle ��������� �����˾��޷��þ�������߳��� ***==//
		::Decoding_264=1;
		::ResumeThread(hThread_264Dec);
		//=========================================================================�����߳�

		//=========================================================================�����߳�
		//LPDWORD p_Thread_ID_Play;//�߳�ID ȫ�ֱ���
		::hThread_264_Play=::CreateThread(NULL,0,H264_Play_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_Play);
		//::CloseHandle(hThread_264_Play);//==*** CloseHandle ��������� �����˾��޷��þ�������߳��� ***==//

		//::ResumeThread(hThread_264_Play);//�ŵ� PLAY ��������
		OnBnClickedButtonPlay();
		//=========================================================================�����߳�
	}

	else if(Input_File_Type==_T("yuv"))
	{
		//hThread_Buffer_YUV=::CreateThread(NULL,0,YUV_Buffer_Shell,this,0,NULL);//�����߳�

		//for(int frame=0;frame<TotalFrames;frame++)
		//{
		//	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

		//	DWORD dwRead;
		//	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);
		//	// Buffer ��������Ҫ�� ��ȡ���ݵĹ��� �������� ��ʡ�� ���ɺ���
		//	//Convert(buffer,BMPbuffer);
		//	//Combine(BMPbuffer,BMPbuffer_);
		//	m_ctrlProgress.SetPos(frame+1);
		//}

		//����һ����ʱ�� ���� ��ȡYUV �̵߳Ľ���
		SetTimer(nID_Timer_YUV_Read,USER_TIMER_MINIMUM,NULL);
		//����һ����ʱ�� ���� ��ȡYUV �̵߳Ľ���

		//=========================================================================YUV �������߳�
		hThread_YUV_Read=::CreateThread(NULL,0,YUV_Read_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_YUV_Read);		
		::ResumeThread(hThread_YUV_Read);
		//=========================================================================YUV �������߳�

		//=========================================================================YUV �����߳�
		hThread_YUV_Play=::CreateThread(NULL,0,YUV_Play_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_YUV_Play);//�����߳�
		//::ResumeThread(hThread_YUV_Play);//�ŵ� PLAY ��������
		OnBnClickedButtonPlay();
		//=========================================================================YUV �����߳�
	}

	
}

void CHD_Player_3DDlg::OnBnClickedButtonPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_bPlaying)
	{
	case FALSE:
		if(Input_File_Type==_T("yuv"))
		{
			//SetTimer(nID_Timer,USER_TIMER_MINIMUM,NULL);
			//HANDLE hThread_Display_YUV=::CreateThread(NULL,0,DisplayFrame_YUV_Thread_Shell,this,0,NULL);
			//::ResumeThread(hThread_Display_YUV);
			/*
			if(hThread_Display_YUV!=NULL)
			{
				::ResumeThread(hThread_Display_YUV);//�ָ��߳�
			}
			else
			{
				hThread_Display_YUV=::CreateThread(NULL,0,DisplayFrame_YUV_Thread_Shell,this,0,NULL);//�����߳�
				//::CloseHandle(hThread_Display_YUV);
			}
			*/

			//if(hThread_YUV_Play!=NULL)
			//{
			//	::ResumeThread(hThread_YUV_Play);//�ָ��߳�
			//}
			//else
			//{
			//	hThread_YUV_Play=::CreateThread(NULL,0,YUV_Play_Shell,this,0,&n_Thread_ID_YUV_Play);//�����߳�
			//}

			::ResumeThread(hThread_YUV_Play);//�ָ��߳�
		}
		else if(Input_File_Type==_T("264"))
		{
			//SetTimer(nID_Timer_264_Play,USER_TIMER_MINIMUM,NULL);
			//SetTimer(nID_Timer_264_Play_Buffer,USER_TIMER_MINIMUM,NULL);
			//=========================���ڼ�ʱ��ʱ϶����Ӧ�����֡��===============================
			/*SetTimer(nID_Timer_264_Play_Buffer,50,NULL);*/
			//=========================���ڼ�ʱ��ʱ϶����Ӧ�����֡��===============================

			//::ResumeThread(hThread_264Dec);

			//=========================================================================�����߳�
			//�ָ�ʱ �ָ������߳�
			::ResumeThread(hThread_264_Play);
			//=========================================================================�����߳�
		}

		m_bPlaying=TRUE;
		SetDlgItemText(IDC_BUTTON_PLAY,_T("��ͣ"));
		break;

	case TRUE:
		if(Input_File_Type==_T("yuv"))
		{
			::SuspendThread(hThread_YUV_Play);			
		}
		else if(Input_File_Type==_T("264"))
		{
			//KillTimer(nID_Timer_264_Play);
			/*KillTimer(nID_Timer_264_Play_Buffer);*/

			//::SuspendThread(hThread_264Dec);
			//============================================================
			//��ͣʱ ���𲥷��߳� ����������߳�
			//::SuspendThread(hThread_264Dec);
			::SuspendThread(hThread_264_Play);
			//============================================================
		}

		m_bPlaying=FALSE;
		SetDlgItemText(IDC_BUTTON_PLAY,_T("����"));
		break;
	}
}


void CHD_Player_3DDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	if(nIDEvent==nID_Timer_264_Decode)
	{
		m_ctrlProgress.SetRange(0,Dec_264_No);
		m_ctrlProgress.SetPos(Dec_264_No_Decode);

		//if(Dec_264_Decode_flag[Dec_264_No_Decode]>Dec_264_Display_flag[Dec_264_No_Decode])
		//{
		//	::SuspendThread(hThread_264Dec);
		//	::Decoding_264=0;
		//}

		//if(Dec_264_Decode_flag[Dec_264_No_Decode]<=Dec_264_Display_flag[Dec_264_No_Decode])
		//{
		//	::ResumeThread(hThread_264Dec);
		//	::Decoding_264=1;
		//}
	}

	//if(nIDEvent==nID_Timer_YUV_Read)
	//{
	//	m_ctrlSliderPos.SetPos(YUV_No_Display_total);//�������ò��Ž����� ��Ļ��
	//}


	/*
	if(nIDEvent==nID_Timer_264_Play_Buffer)//���岥��ģʽ
	{
		m_ctrlSliderPos.SetRange(0,Dec_264_No);

		if(Dec_264_No_Display==0)
		{
			::QueryPerformanceCounter(&m_count_start_);
		}

		if(Dec_264_Decode_flag[Dec_264_No_Display]>Dec_264_Display_flag[Dec_264_No_Display])
		{
			::QueryPerformanceCounter(&m_count_start);
			switch(m_bPlayMode)
			{
			case TRUE:
				DisplayFrame_264_Buffer(Dec_264_No_Display);
				break;
			case FALSE:
				DisplayFrame_264_Buffer_2d(Dec_264_No_Display);
				break;
			}
			::QueryPerformanceCounter(&m_count_stop);
			time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
			TIME.Format(_T("��֡ʱ��: %lld ms     "),time);
			::QueryPerformanceCounter(&m_count_stop_);
			time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
			TIME_.Format(_T("�ۼ�ʱ��: %lld ms     "),time_);

			if(Dec_264_No_Display!=0)
			{
				fps=1000/(time);// f/s
				FPS.Format(_T("����֡��: %lld f/s     "),fps);

				fps_=1000/(time_/Dec_264_No_Display);// f/s
				FPS_.Format(_T("ʵ��֡��: %lld f/s     "),fps_);
			}

			if(!m_bFullScreen)
			{
				CString inf;
				inf.Format(_T("���Ž���:%d     "),Dec_264_No_Display);
				SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
			}
			//================================================================================

			Dec_264_Display_flag[Dec_264_No_Display]++;
			Dec_264_No_Display++;
			if(Dec_264_No_Display>=Dec_264_No)
			{
				Dec_264_No_Display=Dec_264_No_Display%Dec_264_No;
			}
		}
	}
	*/


	if(nIDEvent==nID_Timer_264_Play)//��֡����ģʽ
	{		
		if(Decoding_264==0)
		{
			::SuspendThread(hThread_264Dec);
			//==================Display====================
			switch(m_bPlayMode)
			{
			case TRUE:
				DisplayFrame_264(0);
				break;
			case FALSE:
				DisplayFrame_264_2d(0);
				break;
			}
			//==================Display====================
			Decoding_264=1;
			::ResumeThread(hThread_264Dec);
		}
		else if(Decoding_264==1)
		{
			SetDlgItemText(IDC_EDIT_INFO,_T("264 Decoding now"));
		}
	}

	
	//================================================================================================================
	if(nIDEvent==nID_Timer)
	{
		//==========================================���㲢��ʾ ֡��

		if(CurrentFrame==0)
		{
			::QueryPerformanceCounter(&m_count_start_);
		}

		//::QueryPerformanceFrequency(&m_freq);

		::QueryPerformanceCounter(&m_count_start);
		switch(m_bPlayMode)
		{
		case TRUE:
			//DisplayFrame_YUV(CurrentFrame);
			DisplayFrame_YUV(CurrentFrame);
			break;
		case FALSE:
			//DisplayFrame_YUV_2d(CurrentFrame);
			DisplayFrame_YUV_2d(CurrentFrame);
			break;
		}
		//DisplayFrame_YUV(CurrentFrame);//=============//�˴�ʵ�ֲ���
		::QueryPerformanceCounter(&m_count_stop);

		time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
		TIME.Format(_T("��֡ʱ��: %lld ms     "),time);

		::QueryPerformanceCounter(&m_count_stop_);

		time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
		TIME_.Format(_T("�ۼ�ʱ��: %lld ms     "),time_);

		if(CurrentFrame!=0)
		{
			fps=1000/(time);// f/s
			FPS.Format(_T("����֡��: %lld f/s     "),fps);

			fps_=1000/(time_/CurrentFrame);// f/s
			FPS_.Format(_T("ʵ��֡��: %lld f/s     "),fps_);
		}		
		//==========================================���㲢��ʾ ֡��

		//==========================================��ʾ ��ǰ֡ ����
		if(!m_bFullScreen)
		{
			CString inf;
			inf.Format(_T("���Ž���:%d/%d     "),CurrentFrame,TotalFrames);

			//SetDlgItemText(IDC_EDIT_INFO,inf+TIME+FPS+TIME_+FPS_);
			//���ô�����Ĳ���ģʽ�� ����Ҫ TIME FPS ��2����Ϣ��:
			SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);


			//m_ctrlSliderPos.SetPos(CurrentFrame);
			//���������ڴ˴������� ȫ��״̬����ʱ ���ȷ��ط�ȫ��ǰ�ϵ� ������
			//Ӧ�ü�������һ��
		}
		//==========================================��ʾ ��ǰ֡ ����

		m_ctrlSliderPos.SetPos(CurrentFrame);//Ӧ�ü��ڴ˴� ��������ȫ������ʱ ���ȴ���

		//m_ctrlProgress.SetPos(CurrentFrame);

		CurrentFrame=CurrentFrame+1;//֡������ ׼��������һ֡

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames ʱ ���Ѿ�������Ƶ�ļ������ݷ�Χ��
		{
			CurrentFrame=0;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CHD_Player_3DDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(Input_File_Type==_T("264"))
	{
		KillTimer(nID_Timer_264_Play);
		//KillTimer(nID_Timer_264_Decode);
		//KillTimer(nID_Timer_264_Play_Buffer);

		::SuspendThread(hThread_264Dec);
		::SuspendThread(hThread_264_Play);
	}

	else if(Input_File_Type==_T("yuv"))
	{
		CurrentFrame=0;
		switch(m_bPlayMode)
		{
		case TRUE:
			//DisplayFrame_YUV(CurrentFrame);
			DisplayFrame_YUV(CurrentFrame);
			break;
		case FALSE:
			//DisplayFrame_YUV_2d(CurrentFrame);
			DisplayFrame_YUV_2d(CurrentFrame);
			break;
		}
		//DisplayFrame_YUV(CurrentFrame);
		m_ctrlSliderPos.SetPos(CurrentFrame);
		m_bPlaying=FALSE;

		//KillTimer(nID_Timer);//�����ڲ��� �����ö�ʱ�� �ı䲥�ű�־ //ʵ����ͣ
		DWORD ExitCode;
		::TerminateThread(hThread_YUV_Play,ExitCode);//��ֹ�߳�

		SetDlgItemText(IDC_BUTTON_PLAY,_T("����"));
		//��ϣ���رյ�ǰ���ŵ���Ƶ�ļ� ����ʾ����ͼƬ ����Ҫ��ӹر�DC�Ĵ��� Ŀǰδʵ��
	}

		
}



void CHD_Player_3DDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bPlayMode=!m_bPlayMode;

	//////////////////////////////////////////////////////////////////////////////////////////
}


BOOL CHD_Player_3DDlg::DisplayFrame(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{
	//=================================================LOG
	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);
	//=================================================LOG

	//////////////////////////////////////////////////////////////////////////////////////////
	//====================���л��幦�ܵİ汾�� ���˲��Ż���֮�� ȫ���ƶ��� BUFFER ����Ӧ������

	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	//======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||

	//=================================================================YUV2RGB: buffer->BMPbuffer
	//��ʵ�����������û���� ָ���Ѿ����̺߳����������ú��� ===========//Convert(NULL,NULL);
	Convert(buffer,BMPbuffer);
	//=================================================================YUV2RGB: buffer->BMPbuffer

	//=================================================================�ϳ��㷨: BMPbuffer->BMPbuffer_
	//InitDelta(); �ϳ��㷨��ʼ�� ���ڳ�ʼ����������
	//��ʵ�����������û���� ָ���Ѿ����̺߳����������ú��� ===========//Combine(NULL,NULL);
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ����	
//	Combine_GDX(BMPbuffer,BMPbuffer_); //�ϳ��㷨(2) ���Լ��ĺϳ��㷨
	//=================================================================�ϳ��㷨: BMPbuffer->BMPbuffer_
	
	//////////////////////////////////////////////////////////////////////////////////////////
	//====================���л��幦�ܵİ汾�� ���˲��Ż���֮�� ȫ���ƶ��� BUFFER ����Ӧ������


	//===========================================================================================���DC�Ĳ��� ʵ����ʾһ֡ͼ����
	/* //�Ѿ�����Ϊ��ĳ�Ա
	HBITMAP m_hBmp;
	BITMAP m_Bmp;
	RECT rect;
	HDC hdc;
	HDC hdcMem;
	*/ //�Ѿ�����Ϊ��ĳ�Ա

//	m_hBmp=::CreateBitmap(Width,Height,1,24,BMPbuffer);	//��ô�֡ͼ��λͼ���ľ��

	

	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}


BOOL CHD_Player_3DDlg::DisplayFrame_2d(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{
	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	Convert(buffer,BMPbuffer);

	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}


void CHD_Player_3DDlg::OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(Input_File_Type==_T("yuv"))
	{
		CurrentFrame=m_ctrlSliderPos.GetPos();
		if(CurrentFrame>=1)
		{
			switch(m_bPlayMode)
			{
			case TRUE:
				//DisplayFrame_YUV(CurrentFrame);
				DisplayFrame_YUV(CurrentFrame);
				break;
			case FALSE:
				//DisplayFrame_YUV_2d(CurrentFrame);
				DisplayFrame_YUV_2d(CurrentFrame);
				break;
			}
			//DisplayFrame_YUV(CurrentFrame);
		}
	}
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}



void CHD_Player_3DDlg::OnBnClickedButtonFullscreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	switch(m_bFullScreen)
	{
	case FALSE:
		this->Invalidate(TRUE);//�������������ڵı�����DC��ͼ���µģ�

		//����� �����ر�����
		::ShowWindow(m_hWnd,SW_MAXIMIZE); //��󻯴��ڣ��͵���������ϵ���󻯰�ťЧ��һ���� �ɹ�
		::SetWindowLongW(m_hWnd,GWL_STYLE,::GetWindowLongW(m_hWnd,GWL_STYLE)-WS_CAPTION); //���ر����� �ɹ�
		m_bFullScreen=TRUE;
		//==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����
		//HWND h_;
		/*
		#define IDC_BUTTON_OPEN                 1000
		#define IDC_BUTTON_PLAY                 1001
		#define IDC_BUTTON_STOP                 1002
		#define IDC_BUTTON_FULLSCREEN           1003
		#define IDC_BUTTON_SET                  1004
		#define IDC_SLIDER_POS                  1005
		#define IDC_STATIC_POS                  1006
		#define IDC_STATIC_INFO                 1007
		#define IDC_EDIT_WIDTH                  1008
		#define IDC_EDIT_HEIGHT                 1009
		#define IDC_EDIT_FRAME                  1010
		#define IDC_EDIT_FRAMES                 1011
		#define IDC_EDIT_WIDTH_                 1012
		#define IDC_EDIT_HEIGHT_                1013
		*/
		for(int nID=1000-1;nID<=1017;nID++)//��resource.h�ж����IDֵ���Ӧ
		{
			h_=::GetDlgItem(m_hWnd,nID);
			::ShowWindow(h_,SW_HIDE);
		}
		h_=::GetDlgItem(m_hWnd,IDOK);
		::ShowWindow(h_,SW_HIDE);
		//==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����
		break;
	case TRUE:
		this->Invalidate(TRUE);//�������������ڵı�����DC��ͼ���µģ�

		//ע�� ˳������ Ҫ������Ĳ��� ��Ӧ
		//����ʾ������ ��ԭʼ��С
		::SetWindowLongW(m_hWnd,GWL_STYLE,::GetWindowLongW(m_hWnd,GWL_STYLE)+WS_CAPTION); //��ʾ������ �ɹ�		
		::ShowWindow(m_hWnd,SW_SHOWNORMAL);				
		m_bFullScreen=FALSE;
		//==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����
		//HWND h_;
		/*
		#define IDC_BUTTON_OPEN                 1000
		#define IDC_BUTTON_PLAY                 1001
		#define IDC_BUTTON_STOP                 1002
		#define IDC_BUTTON_FULLSCREEN           1003
		#define IDC_BUTTON_SET                  1004
		#define IDC_SLIDER_POS                  1005
		#define IDC_STATIC_POS                  1006
		#define IDC_STATIC_INFO                 1007
		#define IDC_EDIT_WIDTH                  1008
		#define IDC_EDIT_HEIGHT                 1009
		#define IDC_EDIT_FRAME                  1010
		#define IDC_EDIT_FRAMES                 1011
		#define IDC_EDIT_WIDTH_                 1012
		#define IDC_EDIT_HEIGHT_                1013
		*/
		for(int nID=1000-1;nID<=1017;nID++)//��resource.h�ж����IDֵ���Ӧ
		{
			h_=::GetDlgItem(m_hWnd,nID);
			::ShowWindow(h_,SW_SHOW);
		}
		h_=::GetDlgItem(m_hWnd,IDOK);
		::ShowWindow(h_,SW_SHOW);
		//==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����
		break;
	}

	switch(m_bFullScreen) //����ȫ��״̬��־ ���ò�ͬ��ͼ����ʾ��ʽ
	{
	case 0://��ȫ��״̬

		hdc=::GetDC(m_hWnd);
		::GetClientRect(m_hWnd,&rect);

		rect.left=rect.left+7;
		rect.top=rect.top+6;
		rect.right=rect.left+640;//16
		rect.bottom=rect.top+360;//9		
		break;

	case 1://ȫ��״̬

		hdc=::GetDC(m_hWnd);
		::GetWindowRect(m_hWnd,&rect);//2ѡ1 //::GetClientRect(m_hWnd,&rect);//2ѡ1

		rect.left=0;
		rect.top=0;
		rect.right=::GetSystemMetrics(SM_CXSCREEN);
		rect.bottom=::GetSystemMetrics(SM_CYSCREEN);
		break;
	}
}


//================================================================================�ϳ��㷨 ׼������
void CHD_Player_3DDlg::InitDelta(void)
{
	int i,j=0;
	for(i=0;i<=Height_-3;i=i+3)
	{
		delta[i]=j;
		delta[i]%=8;
		delta[i+1]=delta[i]+1;
		delta[i+2]=delta[i]+2;
		j+=2;
	}//delta=8 7 6 6 5 4 4 3 2 2 1 0
}

void CHD_Player_3DDlg::InvertPixel(PBYTE BmpBuf)
{
	BYTE swap;
	for(int i=0;i<Width_*Height_;i++)
	{
		//BmpBuf+3*i BmpBuf+3*i+2 ����
		swap=*(BmpBuf+3*i+0);
		*(BmpBuf+3*i+0)=*(BmpBuf+3*i+2);
		*(BmpBuf+3*i+2)=swap;
	}
}

//type��ʾ�ߵ�ͼ������� 1 ��ԭʼͼ�ߵ� 2������ͼ�ߵ� ���߿�߲�ͬ
void CHD_Player_3DDlg::InvertImage(PBYTE pDataIn, PBYTE pDataOut,int type)
{
	switch(type)
	{
	case 1:
		for(int i=0;i<Height;i++)
		{
			memcpy(pDataOut+i*Width*3,pDataIn+(Height-1-i)*Width*3,Width*3);
		}
		break;

	case 2:
		for(int i=0;i<Height_;i++)
		{
			memcpy(pDataOut+i*Width_*3,pDataIn+(Height_-1-i)*Width_*3,Width_*3);
		}
		break;
	}
}

void CHD_Player_3DDlg::InvertImage_origin(PBYTE pDataIn, PBYTE pDataOut,int type)
{
	for(int i=0;i<Height_origin;i++)
	{
		memcpy(pDataOut+i*Width_origin*3,pDataIn+(Height_origin-1-i)*Width_origin*3,Width_origin*3);
	}
}








//================================================================================�ϳ��㷨 ׼������

//================================================================8��ͼ�� ˳��ߵ�һ��
void CHD_Player_3DDlg::Reorder(PBYTE pDataIn, PBYTE pDataOut)
{
	for(int i=0;i<8;i++)
	{
		memcpy(pDataOut+i*Width*Height_2d*3,pDataIn+(8-1-i)*Width*Height_2d*3,Width*Height_2d*3);
	}
}
//================================================================8��ͼ�� ˳��ߵ�һ��

//================================================================================ԭ���� �ϳ��㷨
/*
int CHD_Player_3DDlg::Combine(PBYTE pDataIn,PBYTE pDataOut)
{
	int m,t,n,y;
	int pm = 0,

		b_0 = Width*Height_2d*0*3,
		b_1 = Width*Height_2d*1*3,
		b_2 = Width*Height_2d*2*3,
		b_3 = Width*Height_2d*3*3,
		b_4 = Width*Height_2d*4*3,
		b_5 = Width*Height_2d*5*3,
		b_6 = Width*Height_2d*6*3,
		b_7 = Width*Height_2d*7*3;

	for(m=0; m<Height_; m++)
	{		
		y=m/3;
		b_0 = y*(Width*3);
		//for(n=0; n<Width; n++)
		for(n=0; n<Width-1; n++)
		{	
			//delta=8 7 6 6 5 4 4 3 2 2 1 0
			t=n*8+delta[m];
			pDataOut[pm+t+0] = pDataIn[b_0    +3*n+shuzu[(t+0)%3]];
			pDataOut[pm+t+1] = pDataIn[b_0+b_1+3*n+shuzu[(t+1)%3]];
			pDataOut[pm+t+2] = pDataIn[b_0+b_2+3*n+shuzu[(t+2)%3]];
			pDataOut[pm+t+3] = pDataIn[b_0+b_3+3*n+shuzu[(t+3)%3]];
			pDataOut[pm+t+4] = pDataIn[b_0+b_4+3*n+shuzu[(t+4)%3]];
			pDataOut[pm+t+5] = pDataIn[b_0+b_5+3*n+shuzu[(t+5)%3]];
			pDataOut[pm+t+6] = pDataIn[b_0+b_6+3*n+shuzu[(t+6)%3]];
			pDataOut[pm+t+7] = pDataIn[b_0+b_7+3*n+shuzu[(t+7)%3]];
		}
		pm +=Width_*3;
	}
	return 0;
}
*/ //ʹ�ö��̺߳ϳ��㷨֮ǰ ����
//================================================================================ԭ���� �ϳ��㷨

//================================================================================���Լ��� �ϳ��㷨
/*
void CHD_Player_3DDlg::Combine_GDX(PBYTE pDataIn, PBYTE pDataOut)
{
	for(int line=0;line<Height_/3;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
}
*/ //ʹ�ö��̺߳ϳ��㷨֮ǰ ����
//================================================================================���Լ��� �ϳ��㷨


//==========================================================================================YUV2RGB ���߳�ʵ��
void CHD_Player_3DDlg::Convert(PBYTE pDataIn, PBYTE pDataOut)
{

	HANDLE hThread1=::CreateThread(NULL,0,Convert_1_Shell,this,0,NULL);
	HANDLE hThread2=::CreateThread(NULL,0,Convert_2_Shell,this,0,NULL);
	HANDLE hThread3=::CreateThread(NULL,0,Convert_3_Shell,this,0,NULL);
	HANDLE hThread4=::CreateThread(NULL,0,Convert_4_Shell,this,0,NULL);

	::SetThreadPriority(hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread2,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread3,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread4,THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hThread[4];
	hThread[0]=hThread1;
	hThread[1]=hThread2;
	hThread[2]=hThread3;
	hThread[3]=hThread4;
	::WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	::CloseHandle(hThread1);
	::CloseHandle(hThread2);
	::CloseHandle(hThread3);
	::CloseHandle(hThread4);
}


DWORD WINAPI CHD_Player_3DDlg::Convert_1_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_1(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_2_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_2(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_3_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_3(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_4_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_4(NULL);
}

DWORD __fastcall CHD_Player_3DDlg::Convert_1(LPVOID lpParameter)
{
	PBYTE pDataIn=buffer;
	PBYTE pDataOut=BMPbuffer;

	ConvertYUVtoRGB((unsigned char*)pDataIn+0*Width*Height_2d*2,
		(unsigned char*)(pDataIn+(Width*Height)+0*Width*Height_2d*2*1/4),
		(unsigned char*)(pDataIn+(int)(Width*Height*1.25+0*Width*Height_2d*2*1/4)),
		pDataOut+0*Width*Height_2d*2*3,
		Width,
		Height_2d*2);

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_2(LPVOID lpParameter)
{
	PBYTE pDataIn=buffer;
	PBYTE pDataOut=BMPbuffer;

	ConvertYUVtoRGB((unsigned char*)pDataIn+1*Width*Height_2d*2,
		(unsigned char*)(pDataIn+(Width*Height)+1*Width*Height_2d*2*1/4),
		(unsigned char*)(pDataIn+(int)(Width*Height*1.25+1*Width*Height_2d*2*1/4)),
		pDataOut+1*Width*Height_2d*2*3,
		Width,
		Height_2d*2);

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_3(LPVOID lpParameter)
{
	PBYTE pDataIn=buffer;
	PBYTE pDataOut=BMPbuffer;

	ConvertYUVtoRGB((unsigned char*)pDataIn+2*Width*Height_2d*2,
		(unsigned char*)(pDataIn+(Width*Height)+2*Width*Height_2d*2*1/4),
		(unsigned char*)(pDataIn+(int)(Width*Height*1.25+2*Width*Height_2d*2*1/4)),
		pDataOut+2*Width*Height_2d*2*3,
		Width,
		Height_2d*2);

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_4(LPVOID lpParameter)
{
	PBYTE pDataIn=buffer;
	PBYTE pDataOut=BMPbuffer;

	ConvertYUVtoRGB((unsigned char*)pDataIn+3*Width*Height_2d*2,
		(unsigned char*)(pDataIn+(Width*Height)+3*Width*Height_2d*2*1/4),
		(unsigned char*)(pDataIn+(int)(Width*Height*1.25+3*Width*Height_2d*2*1/4)),
		pDataOut+3*Width*Height_2d*2*3,
		Width,
		Height_2d*2);

	return 0;
}

//==========================================================================================YUV2RGB ���߳�ʵ��

//====================================================================================ԭ���� �ϳ��㷨�� 4���߳�
int CHD_Player_3DDlg::Combine(PBYTE pDataIn,PBYTE pDataOut)
{
	//��CreateThread��������Combine_1_Shellʱ��������� ԭ��
	//����û����������ǰ������ǲ����ڵģ����ԣ�
	//������ʹ�� [��Ǿ�̬��Ա����] ��Ϊ[�̺߳���]
	//���������
	//(1)���� Combine_1_Shell �����ɾ�̬��Ա����
	//(2)���� Combine_1_Shell ������������ ���ǽ�������Ϊȫ�ֺ���
	//�˴�����(1)

	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������

	HANDLE hThread1=::CreateThread(NULL,0,Combine_1_Shell,this,0,NULL);
	HANDLE hThread2=::CreateThread(NULL,0,Combine_2_Shell,this,0,NULL);
	HANDLE hThread3=::CreateThread(NULL,0,Combine_3_Shell,this,0,NULL);
	HANDLE hThread4=::CreateThread(NULL,0,Combine_4_Shell,this,0,NULL);

	::SetThreadPriority(hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread2,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread3,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread4,THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hThread[4];
	hThread[0]=hThread1;
	hThread[1]=hThread2;
	hThread[2]=hThread3;
	hThread[3]=hThread4;
	::WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	::CloseHandle(hThread1);
	::CloseHandle(hThread2);
	::CloseHandle(hThread3);
	::CloseHandle(hThread4);

	return 0;
}
DWORD WINAPI CHD_Player_3DDlg::Combine_1_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_1(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_2_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_2(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_3_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_3(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_4_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_4(NULL);
}



DWORD __fastcall CHD_Player_3DDlg::Combine_1(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_;

	int m,t,n,y;
	int pm = 0,
		b_0 = Width*Height_2d*0*3,
		b_1 = Width*Height_2d*1*3,
		b_2 = Width*Height_2d*2*3,
		b_3 = Width*Height_2d*3*3,
		b_4 = Width*Height_2d*4*3,
		b_5 = Width*Height_2d*5*3,
		b_6 = Width*Height_2d*6*3,
		b_7 = Width*Height_2d*7*3;

	for(m=0; m<Height_/4; m++)
	{		
		y=m/3;
		b_0 = y*(Width*3);
		for(n=0; n<Width-1; n++)
		{	
			t=n*8+delta[m];
			pDataOut[pm+t+0] = pDataIn[b_0    +3*n+shuzu[(t+0)%3]];
			pDataOut[pm+t+1] = pDataIn[b_0+b_1+3*n+shuzu[(t+1)%3]];
			pDataOut[pm+t+2] = pDataIn[b_0+b_2+3*n+shuzu[(t+2)%3]];
			pDataOut[pm+t+3] = pDataIn[b_0+b_3+3*n+shuzu[(t+3)%3]];
			pDataOut[pm+t+4] = pDataIn[b_0+b_4+3*n+shuzu[(t+4)%3]];
			pDataOut[pm+t+5] = pDataIn[b_0+b_5+3*n+shuzu[(t+5)%3]];
			pDataOut[pm+t+6] = pDataIn[b_0+b_6+3*n+shuzu[(t+6)%3]];
			pDataOut[pm+t+7] = pDataIn[b_0+b_7+3*n+shuzu[(t+7)%3]];
		}
		pm +=Width_*3;
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_2(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_+(Width_*Height_*3)*1/4;

	int m,t,n,y;
	int pm = 0,
		b_0 = Width*Height_2d*0*3,
		b_1 = Width*Height_2d*1*3,
		b_2 = Width*Height_2d*2*3,
		b_3 = Width*Height_2d*3*3,
		b_4 = Width*Height_2d*4*3,
		b_5 = Width*Height_2d*5*3,
		b_6 = Width*Height_2d*6*3,
		b_7 = Width*Height_2d*7*3;

	for(m=Height_/4; m<Height_*2/4; m++)
	{		
		y=m/3;
		b_0 = y*(Width*3);
		for(n=0; n<Width-1; n++)
		{	
			t=n*8+delta[m];
			pDataOut[pm+t+0] = pDataIn[b_0    +3*n+shuzu[(t+0)%3]];
			pDataOut[pm+t+1] = pDataIn[b_0+b_1+3*n+shuzu[(t+1)%3]];
			pDataOut[pm+t+2] = pDataIn[b_0+b_2+3*n+shuzu[(t+2)%3]];
			pDataOut[pm+t+3] = pDataIn[b_0+b_3+3*n+shuzu[(t+3)%3]];
			pDataOut[pm+t+4] = pDataIn[b_0+b_4+3*n+shuzu[(t+4)%3]];
			pDataOut[pm+t+5] = pDataIn[b_0+b_5+3*n+shuzu[(t+5)%3]];
			pDataOut[pm+t+6] = pDataIn[b_0+b_6+3*n+shuzu[(t+6)%3]];
			pDataOut[pm+t+7] = pDataIn[b_0+b_7+3*n+shuzu[(t+7)%3]];
		}
		pm +=Width_*3;
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_3(LPVOID lpParameter)
{

	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_+(Width_*Height_*3)*2/4;

	int m,t,n,y;
	int pm = 0,
		b_0 = Width*Height_2d*0*3,
		b_1 = Width*Height_2d*1*3,
		b_2 = Width*Height_2d*2*3,
		b_3 = Width*Height_2d*3*3,
		b_4 = Width*Height_2d*4*3,
		b_5 = Width*Height_2d*5*3,
		b_6 = Width*Height_2d*6*3,
		b_7 = Width*Height_2d*7*3;

	for(m=Height_*2/4;m<Height_*3/4; m++)
	{		
		y=m/3;
		b_0 = y*(Width*3);
		for(n=0; n<Width-1; n++)
		{	
			t=n*8+delta[m];
			pDataOut[pm+t+0] = pDataIn[b_0    +3*n+shuzu[(t+0)%3]];
			pDataOut[pm+t+1] = pDataIn[b_0+b_1+3*n+shuzu[(t+1)%3]];
			pDataOut[pm+t+2] = pDataIn[b_0+b_2+3*n+shuzu[(t+2)%3]];
			pDataOut[pm+t+3] = pDataIn[b_0+b_3+3*n+shuzu[(t+3)%3]];
			pDataOut[pm+t+4] = pDataIn[b_0+b_4+3*n+shuzu[(t+4)%3]];
			pDataOut[pm+t+5] = pDataIn[b_0+b_5+3*n+shuzu[(t+5)%3]];
			pDataOut[pm+t+6] = pDataIn[b_0+b_6+3*n+shuzu[(t+6)%3]];
			pDataOut[pm+t+7] = pDataIn[b_0+b_7+3*n+shuzu[(t+7)%3]];
		}
		pm +=Width_*3;
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_4(LPVOID lpParameter)
{

	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_+(Width_*Height_*3)*3/4;

	int m,t,n,y;
	int pm = 0,
		b_0 = Width*Height_2d*0*3,
		b_1 = Width*Height_2d*1*3,
		b_2 = Width*Height_2d*2*3,
		b_3 = Width*Height_2d*3*3,
		b_4 = Width*Height_2d*4*3,
		b_5 = Width*Height_2d*5*3,
		b_6 = Width*Height_2d*6*3,
		b_7 = Width*Height_2d*7*3;

	for(m=Height_*3/4;m<Height_*4/4; m++)
	{		
		y=m/3;
		b_0 = y*(Width*3);
		for(n=0; n<Width-1; n++)
		{	
			t=n*8+delta[m];
			pDataOut[pm+t+0] = pDataIn[b_0    +3*n+shuzu[(t+0)%3]];
			pDataOut[pm+t+1] = pDataIn[b_0+b_1+3*n+shuzu[(t+1)%3]];
			pDataOut[pm+t+2] = pDataIn[b_0+b_2+3*n+shuzu[(t+2)%3]];
			pDataOut[pm+t+3] = pDataIn[b_0+b_3+3*n+shuzu[(t+3)%3]];
			pDataOut[pm+t+4] = pDataIn[b_0+b_4+3*n+shuzu[(t+4)%3]];
			pDataOut[pm+t+5] = pDataIn[b_0+b_5+3*n+shuzu[(t+5)%3]];
			pDataOut[pm+t+6] = pDataIn[b_0+b_6+3*n+shuzu[(t+6)%3]];
			pDataOut[pm+t+7] = pDataIn[b_0+b_7+3*n+shuzu[(t+7)%3]];
		}
		pm +=Width_*3;
	}
	return 0;
}

//====================================================================================ԭ���� �ϳ��㷨�� 4���߳�

//====================================================================================���Լ��� �ϳ��㷨�� 4���߳�
void CHD_Player_3DDlg::Combine_GDX(PBYTE pDataIn, PBYTE pDataOut)
{
	HANDLE hThread1=::CreateThread(NULL,0,Combine_GDX_1_Shell,this,0,NULL);
	HANDLE hThread2=::CreateThread(NULL,0,Combine_GDX_2_Shell,this,0,NULL);
	HANDLE hThread3=::CreateThread(NULL,0,Combine_GDX_3_Shell,this,0,NULL);
	HANDLE hThread4=::CreateThread(NULL,0,Combine_GDX_4_Shell,this,0,NULL);

	::SetThreadPriority(hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread2,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread3,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread4,THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hThread[4];
	hThread[0]=hThread1;
	hThread[1]=hThread2;
	hThread[2]=hThread3;
	hThread[3]=hThread4;
	::WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	::CloseHandle(hThread1);
	::CloseHandle(hThread2);
	::CloseHandle(hThread3);
	::CloseHandle(hThread4);

	/*
	for(int line=0;line<Height_/3;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
	*/
}
DWORD WINAPI CHD_Player_3DDlg::Combine_GDX_1_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_GDX_1(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_GDX_2_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_GDX_2(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_GDX_3_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_GDX_3(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Combine_GDX_4_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Combine_GDX_4(NULL);
}



DWORD __fastcall CHD_Player_3DDlg::Combine_GDX_1(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_;

	for(int line=0;line<(Height_/3)*1/4;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_GDX_2(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_;

	for(int line=(Height_/3)*1/4;line<(Height_/3)*2/4;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_GDX_3(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_;

	for(int line=(Height_/3)*2/4;line<(Height_/3)*3/4;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Combine_GDX_4(LPVOID lpParameter)
{
	PBYTE pDataIn=BMPbuffer;
	PBYTE pDataOut=BMPbuffer_;

	for(int line=(Height_/3)*3/4;line<(Height_/3)*4/4;line++)//��Ҫ�ϳɵ�����Ϊ 3*360=1080 ÿ��ѭ���ϳ�3��
	{
		//ÿ3�зֳ� 720 �����ݵ�Ԫ 1920*3=5760=720*8
		//�����Ǵ�λ������ ����ʵ��ȡֵ�� 0 �� Width-2 һ�� Width-1 ����Ԫ
		for(int column=0;column<Width-1;column++)
		{
			for(int i=0;i<8;i++)
			{
				*(pDataOut+line*Width_*3*3+Width_*3*0+column*8+delta[3*line+0]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+0])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*1+column*8+delta[3*line+1]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+1])%3);
				*(pDataOut+line*Width_*3*3+Width_*3*2+column*8+delta[3*line+2]+(i))=*(pDataIn+i*Width*Height_2d*3+line*Width*3+column*3+(column*8+i+delta[3*line+2])%3);
			}
		}
	}
	return 0;
}


//====================================================================================���Լ��� �ϳ��㷨�� 4���߳�


//�Ի�����Ĵ��ں����� �޷�ֱ�ӽ��ܼ�����Ϣ ��Ҫ����Ӧ����д�� PreTranslateMessage ������
//[����] CDialog::PreTranslateMessage() ����
BOOL CHD_Player_3DDlg::PreTranslateMessage(MSG * pMsg)
{
	//��ִ�л���� PreTranslateMessage ����ǰ ���������Ϣ��������Ӧ����
	if(pMsg->message==WM_KEYDOWN)
	{
		//û���� ESC SPACE ��Ϊ���ǱȽ����� ��Ĭ��״̬�������⹦��
		//�� [�����] ������ ȫ�� �� ��ȫ�� ֮���л�
		if(pMsg->wParam)
		{
			OnBnClickedButtonFullscreen();
		}
	}

	CDialog::PreTranslateMessage(pMsg);
	return 0;//ע�⣺�˴�һ��Ҫ����0 ����������Ϣ�Ĵ������ܵ�Ӱ��!
}
//�Ի�����Ĵ��ں����� �޷�ֱ�ӽ��ܼ�����Ϣ ��Ҫ����Ӧ����д�� PreTranslateMessage ������
//[����] CDialog::PreTranslateMessage() ����


static DWORD WINAPI H264_Dec_Shell(LPVOID lpParameter);
DWORD __fastcall H264_Dec(LPVOID lpParameter);

DWORD WINAPI CHD_Player_3DDlg::H264_Dec_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->H264_Dec(NULL);
}


extern "C" int decoder_264();
DWORD __fastcall CHD_Player_3DDlg::H264_Dec(LPVOID lpParameter)
{
	//HANDLE h_264_Mutex[100];
	for(int i=0;i<100;i++)
	{
		//h_264_Mutex[i]=::CreateMutexW(NULL,TRUE,NULL);
		h_264_Mutex[i]=::CreateMutexW(NULL,FALSE,NULL);
	}

	decoder_264();
	return 0;
}


//static DWORD WINAPI H264_Play_Shell(LPVOID lpParameter);
//DWORD __fastcall H264_Play(LPVOID lpParameter);

DWORD WINAPI CHD_Player_3DDlg::H264_Play_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->H264_Play(NULL);
}


#define WM_READY_TO_PLAY WM_USER+1

DWORD __fastcall CHD_Player_3DDlg::H264_Play(LPVOID lpParameter)
{
	//=======================================�߳�ͨ��
	MSG msg;
	while(1)
	{
		if(::GetMessage(&msg,0,0,0))
		{
			if(msg.message==WM_READY_TO_PLAY)
			{
				//::AfxMessageBox(_T("WM_READY_TO_PLAY"));
				break;
			}
		}
	}	
	//=======================================�߳�ͨ��
	//=========================================//=========================================
	while(1)
	{
		if(Dec_264_No_Display_total<Dec_264_No_Decode_total)
		{
			::WaitForSingleObject(h_264_Mutex[Dec_264_No_Display],INFINITE);

			switch(m_bPlayMode)
			{
			case TRUE:
				DisplayFrame_264_Buffer(Dec_264_No_Display);
				break;
			case FALSE:
				DisplayFrame_264_Buffer_2d(Dec_264_No_Display);
				break;
			}

			m_ctrlSliderPos.SetPos(Dec_264_No_Display);//�������ò��Ž����� ��Ļ������

			::ReleaseMutex(h_264_Mutex[Dec_264_No_Display]);

			Dec_264_No_Display++;
			Dec_264_No_Display_total++;
			if(Dec_264_No_Display>=Dec_264_No)
			{
				Dec_264_No_Display=Dec_264_No_Display%Dec_264_No;
			}
		}

		/*
		::WaitForSingleObject(h_264_Mutex[Dec_264_No_Display],INFINITE);

		switch(m_bPlayMode)
		{
		case TRUE:
			DisplayFrame_264_Buffer(Dec_264_No_Display);
			break;
		case FALSE:
			DisplayFrame_264_Buffer_2d(Dec_264_No_Display);
			break;
		}

		m_ctrlSliderPos.SetPos(Dec_264_No_Display);//�������ò��Ž����� ��Ļ������

		::ReleaseMutex(h_264_Mutex[Dec_264_No_Display]);

		Dec_264_No_Display++;
		Dec_264_No_Display_total++;
		if(Dec_264_No_Display>=Dec_264_No)
		{
			Dec_264_No_Display=Dec_264_No_Display%Dec_264_No;
		}
		*/
	}
	//=========================================//=========================================


	////=========================================//=========================================
	//m_ctrlSliderPos.SetRange(0,Dec_264_No);

	//if(Dec_264_No_Display==0)
	//{
	//	::QueryPerformanceCounter(&m_count_start_);
	//}

	//if(Dec_264_Decode_flag[Dec_264_No_Display]>Dec_264_Display_flag[Dec_264_No_Display])
	//{
	//	::QueryPerformanceCounter(&m_count_start);
	//	switch(m_bPlayMode)
	//	{
	//	case TRUE:
	//		DisplayFrame_264_Buffer(Dec_264_No_Display);
	//		break;
	//	case FALSE:
	//		DisplayFrame_264_Buffer_2d(Dec_264_No_Display);
	//		break;
	//	}
	//	::QueryPerformanceCounter(&m_count_stop);
	//	time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
	//	TIME.Format(_T("��֡ʱ��: %lld ms     "),time);
	//	::QueryPerformanceCounter(&m_count_stop_);
	//	time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
	//	TIME_.Format(_T("�ۼ�ʱ��: %lld ms     "),time_);

	//	if(Dec_264_No_Display!=0)
	//	{
	//		fps=1000/(time);// f/s
	//		FPS.Format(_T("����֡��: %lld f/s     "),fps);

	//		fps_=1000/(time_/Dec_264_No_Display);// f/s
	//		FPS_.Format(_T("ʵ��֡��: %lld f/s     "),fps_);
	//	}

	//	if(!m_bFullScreen)
	//	{
	//		CString inf;
	//		inf.Format(_T("���Ž���:%d     "),Dec_264_No_Display);
	//		SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
	//	}
	//	//================================================================================

	//	Dec_264_Display_flag[Dec_264_No_Display]++;
	//	Dec_264_No_Display++;
	//	if(Dec_264_No_Display>=Dec_264_No)
	//	{
	//		Dec_264_No_Display=Dec_264_No_Display%Dec_264_No;
	//	}
	//}
	////=========================================//=========================================
	return 0;
}






//��������� 10 �� ԭʼ��Ƶ �ֽ������ 640384 * 8 ��ֱ���еĳ�����״
void CHD_Player_3DDlg::Sample_19201024_640384_8(PBYTE pDataIn, PBYTE pDataOut)
{
	//::memcpy(pDataOut,pDataIn,Width_origin*Height_origin*3);

	PBYTE pOut=pDataOut+640*384*7*3;
	PBYTE pIn=pDataIn+1920*640*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(1) OK

	pOut=pDataOut+640*384*6*3;
	pIn=pDataIn+(1920*640+640)*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(2) OK

	pOut=pDataOut+640*384*5*3;
	pIn=pDataIn+(1920*640+640+640)*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(3) OK

	pOut=pDataOut+640*384*4*3;
	pIn=pDataIn+(1920*256)*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(4) OK

	pOut=pDataOut+640*384*3*3;
	pIn=pDataIn+(1920*256+640)*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(5) OK

	pOut=pDataOut+640*384*2*3;
	pIn=pDataIn+(1920*256+640+640)*3;
	for(int i=0;i<384;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(6) OK

	pOut=pDataOut+640*384*1*3;
	pIn=pDataIn;
	for(int i=0;i<256;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	pIn=pDataIn+(1920*128+640)*3;
	for(int i=0;i<128;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	//(7) OK

	pOut=pDataOut;
	pIn=pDataIn+640*3;
	for(int i=0;i<128;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}
	pIn=pDataIn+(640+640)*3;
	for(int i=0;i<256;i++)
	{
		::memcpy(pOut,pIn,640*3);
		pOut=pOut+640*3;
		pIn=pIn+1920*3;
	}	
	//(8) OK


	//::saveBmp("D:\\My Documents\\Visual Studio 2008\\Projects_201006_1\\hehe_test.bmp",pDataOut,640,3072,24,0);
}






//=============================================================================================
//======================================������264���Ź���======================================

BOOL CHD_Player_3DDlg::DisplayFrame_264(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //���߳�ʵ��

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //���߳�ʵ��
	
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��



	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}

BOOL CHD_Player_3DDlg::DisplayFrame_264_2d(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //���߳�ʵ��

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //���߳�ʵ��
	
	//Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��


	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//������ֵ���Զ����ó�-1����ʾ��ԭʼλͼ�Ŀ�Ⱥ͸߶Ȼ�ͼ
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}


BOOL CHD_Player_3DDlg::DisplayFrame_264_Buffer(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	//Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //���߳�ʵ��
	for(int i=0;i<8;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //���߳�ʵ��
	
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��


	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}



BOOL CHD_Player_3DDlg::DisplayFrame_264_Buffer_2d(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	//Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //���߳�ʵ��
	for(int i=0;i<8;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264
			+frame*width_264*height_264*8*3/2
			+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //���߳�ʵ��
	
	//Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//������ֵ���Զ����ó�-1����ʾ��ԭʼλͼ�Ŀ�Ⱥ͸߶Ȼ�ͼ
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}

//====================================================================================================

BOOL CHD_Player_3DDlg::DisplayFrame_YUV_Buffer(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	frame=frame%100;

	::ConvertYUVtoRGB(
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2,
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2+Width*Height,
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2+Width*Height*5/4,
			BMPbuffer,
			Width,
			Height);
	
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��


	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}



BOOL CHD_Player_3DDlg::DisplayFrame_YUV_Buffer_2d(int frame)//frame=0 ��һ֡ frame=frame-1 ��frame֡
{	
	frame=frame%100;

	::ConvertYUVtoRGB(
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2,
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2+Width*Height,
			(unsigned char*)buffer_YUV
			+frame*Width*Height*3/2+Width*Height*5/4,
			BMPbuffer,
			Width,
			Height);
	
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ���� //���߳�ʵ��

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//������ֵ���Զ����ó�-1����ʾ��ԭʼλͼ�Ŀ�Ⱥ͸߶Ȼ�ͼ
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}

//====================================================================================================


int CHD_Player_3DDlg::Sample_6403072_7202880(PBYTE pDataIn, PBYTE pDataOut)
{
	/*
	for(int n=0;n<=7;n++)
	{
		PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		PBYTE pOut=pDataOut+n*Width*Height_2d*3;

		for(int i=0;i<360;i++)
		{
			//::memcpy(pOut,pIn,640*3);
			for(int p=0;p<80;p++)// 640/8
			{
				::memcpy(pOut+p*9*3,pIn+p*8*3,8*3);
				::memcpy(pOut+p*9*3+8*3,pIn+p*8*3+7*3,3);
			}						

			pIn=pIn+640*3;
			pOut=pOut+720*3;
		}
	}
	//::saveBmp("D:\\My Documents\\Visual Studio 2008\\Projects_201006_1\\hehe_test_.bmp",pDataOut,720,2880,24,0);
	*/ //���Ϲ��̸��� 4�߳� ʵ�� ��߹���Ч��

	HANDLE hThread1=::CreateThread(NULL,0,Sample_6403072_7202880_1_Shell,this,0,NULL);
	HANDLE hThread2=::CreateThread(NULL,0,Sample_6403072_7202880_2_Shell,this,0,NULL);
	HANDLE hThread3=::CreateThread(NULL,0,Sample_6403072_7202880_3_Shell,this,0,NULL);
	HANDLE hThread4=::CreateThread(NULL,0,Sample_6403072_7202880_4_Shell,this,0,NULL);

	::SetThreadPriority(hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread2,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread3,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread4,THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hThread[4];
	hThread[0]=hThread1;
	hThread[1]=hThread2;
	hThread[2]=hThread3;
	hThread[3]=hThread4;
	::WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	::CloseHandle(hThread1);
	::CloseHandle(hThread2);
	::CloseHandle(hThread3);
	::CloseHandle(hThread4);

	return 0;
}

DWORD WINAPI CHD_Player_3DDlg::Sample_6403072_7202880_1_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Sample_6403072_7202880_1(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Sample_6403072_7202880_2_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Sample_6403072_7202880_2(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Sample_6403072_7202880_3_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Sample_6403072_7202880_3(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Sample_6403072_7202880_4_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Sample_6403072_7202880_4(NULL);
}


DWORD __fastcall CHD_Player_3DDlg::Sample_6403072_7202880_1(LPVOID lpParameter)
{
	for(int n=0;n<2;n++)
	{
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		PBYTE pOut=BMPbuffer+n*Width*Height_2d*3;

		for(int i=0;i<360;i++)
		{
			//::memcpy(pOut,pIn,640*3);
			for(int p=0;p<80;p++)// 640/8
			{
				::memcpy(pOut+p*9*3,pIn+p*8*3,8*3);
				::memcpy(pOut+p*9*3+8*3,pIn+p*8*3+7*3,3);
			}						

			pIn=pIn+640*3;
			pOut=pOut+720*3;
		}
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Sample_6403072_7202880_2(LPVOID lpParameter)
{
	for(int n=2;n<4;n++)
	{
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		PBYTE pOut=BMPbuffer+n*Width*Height_2d*3;

		for(int i=0;i<360;i++)
		{
			//::memcpy(pOut,pIn,640*3);
			for(int p=0;p<80;p++)// 640/8
			{
				::memcpy(pOut+p*9*3,pIn+p*8*3,8*3);
				::memcpy(pOut+p*9*3+8*3,pIn+p*8*3+7*3,3);
			}						

			pIn=pIn+640*3;
			pOut=pOut+720*3;
		}
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Sample_6403072_7202880_3(LPVOID lpParameter)
{
	for(int n=4;n<6;n++)
	{
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		PBYTE pOut=BMPbuffer+n*Width*Height_2d*3;

		for(int i=0;i<360;i++)
		{
			//::memcpy(pOut,pIn,640*3);
			for(int p=0;p<80;p++)// 640/8
			{
				::memcpy(pOut+p*9*3,pIn+p*8*3,8*3);
				::memcpy(pOut+p*9*3+8*3,pIn+p*8*3+7*3,3);
			}						

			pIn=pIn+640*3;
			pOut=pOut+720*3;
		}
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Sample_6403072_7202880_4(LPVOID lpParameter)
{
	for(int n=6;n<8;n++)
	{
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//ȡ�м��360�� ���ϱߺ����±ߵ�12��ȥ��
		PBYTE pOut=BMPbuffer+n*Width*Height_2d*3;

		for(int i=0;i<360;i++)
		{
			//::memcpy(pOut,pIn,640*3);
			for(int p=0;p<80;p++)// 640/8
			{
				::memcpy(pOut+p*9*3,pIn+p*8*3,8*3);
				::memcpy(pOut+p*9*3+8*3,pIn+p*8*3+7*3,3);
			}						

			pIn=pIn+640*3;
			pOut=pOut+720*3;
		}
	}

	return 0;
}


//==========================================================================================

int CHD_Player_3DDlg::Convert_264YUV_BGR(PBYTE pDataIn, PBYTE pDataOut)
{
	/*
	for(int i=0;i<8;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}
	*/
	

	HANDLE hThread1=::CreateThread(NULL,0,Convert_264YUV_BGR_1_Shell,this,0,NULL);
	HANDLE hThread2=::CreateThread(NULL,0,Convert_264YUV_BGR_2_Shell,this,0,NULL);
	HANDLE hThread3=::CreateThread(NULL,0,Convert_264YUV_BGR_3_Shell,this,0,NULL);
	HANDLE hThread4=::CreateThread(NULL,0,Convert_264YUV_BGR_4_Shell,this,0,NULL);

	::SetThreadPriority(hThread1,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread2,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread3,THREAD_PRIORITY_TIME_CRITICAL);
	::SetThreadPriority(hThread4,THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hThread[4];
	hThread[0]=hThread1;
	hThread[1]=hThread2;
	hThread[2]=hThread3;
	hThread[3]=hThread4;
	::WaitForMultipleObjects(4,hThread,TRUE,INFINITE);

	::CloseHandle(hThread1);
	::CloseHandle(hThread2);
	::CloseHandle(hThread3);
	::CloseHandle(hThread4);

	return 0;
}

DWORD WINAPI CHD_Player_3DDlg::Convert_264YUV_BGR_1_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_264YUV_BGR_1(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_264YUV_BGR_2_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_264YUV_BGR_2(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_264YUV_BGR_3_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_264YUV_BGR_3(NULL);
}
DWORD WINAPI CHD_Player_3DDlg::Convert_264YUV_BGR_4_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->Convert_264YUV_BGR_4(NULL);
}


DWORD __fastcall CHD_Player_3DDlg::Convert_264YUV_BGR_1(LPVOID lpParameter)
{
	for(int i=0;i<2;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_264YUV_BGR_2(LPVOID lpParameter)
{
	for(int i=2;i<4;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_264YUV_BGR_3(LPVOID lpParameter)
{
	for(int i=4;i<6;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	return 0;
}

DWORD __fastcall CHD_Player_3DDlg::Convert_264YUV_BGR_4(LPVOID lpParameter)
{
	for(int i=6;i<8;i++)
	{
		::ConvertYUVtoRGB(
			(unsigned char*)buffer_264+i*width_264*height_264*3/2,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264,
			(unsigned char*)buffer_264+i*width_264*height_264*3/2+width_264*height_264*5/4,
			Dec_Buffer_264+i*width_264*height_264*3,
			width_264,
			height_264);
	}

	return 0;
}

//========================================================================================

void CHD_Player_3DDlg::OnBnClickedButtonNetwork()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CHD_Player_3D_Network NetworkDlg;
	//NetworkDlg.DoModal();

	CHD_Player_3D_Connect ConnectDlg;
	ConnectDlg.DoModal();


	
}

void CHD_Player_3DDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnDestroy();
	
	OnOK();

	//CHD_Player_3DDlg newdlg;
	//newdlg.DoModal();
	
}

void CHD_Player_3DDlg::OnBnClickedButtonList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CHD_Player_3D_FileList FileList;	


	//=====================================================================��ģ̬�Ի���
	//FileList.Create(IDD_DIALOG_FILELIST,this);
	switch(FileListShow) //����ȫ��״̬��־ ���ò�ͬ��ͼ����ʾ��ʽ
	{
	case FALSE:
		FileList.ShowWindow(SW_SHOW);
		FileListShow=TRUE;
		break;
	case TRUE:
		FileList.ShowWindow(SW_HIDE);
		FileListShow=FALSE;
		break;
	}
	//=====================================================================��ģ̬�Ի���


	/*
	//=====================================================================ģ̬�Ի���
	if(FileList.DoModal()==IDOK)
	{
		Input_File_Type=_T("264");
		//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�
		buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
		ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

		Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
		ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
		//==================================�����洢�Ѿ�����õ�264�������� �ŵ��򿪰�ť������ռ�

		OnBnClickedButtonBuffer();
	}
	//=====================================================================ģ̬�Ի���
	*/
}


BOOL CHD_Player_3DDlg::DisplayFrame_YUV(int frame)//�� ����ʱ�Ĺ���
{
	////=================================================LOG
	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time_1();
	////=================================================LOG

	log_time.Query_Time_1();
	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);
	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);
	log_time.Query_Time_2(_T("Read"));

	log_time.Query_Time_1();
	Convert(buffer,BMPbuffer);
	log_time.Query_Time_2(_T("Convert"));

	log_time.Query_Time_1();
	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ����	
	log_time.Query_Time_2(_T("Combine"));
	
	log_time.Query_Time_1();
	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);
	log_time.Query_Time_2(_T("Display"));

	////=================================================LOG
	//log_time.Get_Time_2(inf_time);
	////=================================================LOG

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}


/*
BOOL CHD_Player_3DDlg::DisplayFrame_YUV(int frame)//���� ����ʱ�Ĺ��� ������ĺ���һ��ֻ��ȥ���˲�ʱ�Ĺ���
{
	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);
	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	Convert(buffer,BMPbuffer);

	Combine(BMPbuffer,BMPbuffer_); //�ϳ��㷨(1) ԭ����	

	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//�˴�ΪDIB���ݵ���ʼָ��
			//          BMPbuffer_all+frame*Width_*Height_*3,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}
*/



BOOL CHD_Player_3DDlg::DisplayFrame_YUV_2d(int frame)
{
	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	Convert(buffer,BMPbuffer);

	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//�˴�ΪDIB���ݵ���ʼָ��
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //����1��ʾ�������� ����0��ʾδ��������
}

/*
DWORD WINAPI CHD_Player_3DDlg::DisplayFrame_YUV_Thread(LPVOID lpParameter)
{
	if(CurrentFrame==0)
	{
		::QueryPerformanceCounter(&m_count_start_);
	}

	::QueryPerformanceCounter(&m_count_start);
	switch(m_bPlayMode)
	{
	case TRUE:
		DisplayFrame_YUV(CurrentFrame);
		break;
	case FALSE:
		DisplayFrame_YUV_2d(CurrentFrame);
		break;
	}
	::QueryPerformanceCounter(&m_count_stop);

	time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
	TIME.Format(_T("��֡ʱ��: %lld ms     "),time);

	::QueryPerformanceCounter(&m_count_stop_);

	time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
	TIME_.Format(_T("�ۼ�ʱ��: %lld ms     "),time_);

	if(CurrentFrame!=0)
	{
		fps=1000/(time);// f/s
		FPS.Format(_T("����֡��: %lld f/s     "),fps);

		fps_=1000/(time_/CurrentFrame);// f/s
		FPS_.Format(_T("ʵ��֡��: %lld f/s     "),fps_);
	}		
	//==========================================���㲢��ʾ ֡��

	//==========================================��ʾ ��ǰ֡ ����
	if(!m_bFullScreen)
	{
		CString inf;
		inf.Format(_T("���Ž���:%d/%d     "),CurrentFrame,TotalFrames);
		SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
	}
	//==========================================��ʾ ��ǰ֡ ����

	m_ctrlSliderPos.SetPos(CurrentFrame);//Ӧ�ü��ڴ˴� ��������ȫ������ʱ ���ȴ���

	CurrentFrame=CurrentFrame+1;//֡������ ׼��������һ֡

	if(CurrentFrame>=TotalFrames)//frame=TotalFrames ʱ ���Ѿ�������Ƶ�ļ������ݷ�Χ��
	{
		CurrentFrame=0;
	}

	return 0;
}
*/


DWORD WINAPI CHD_Player_3DDlg::DisplayFrame_YUV_Thread_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->DisplayFrame_YUV_Thread(NULL);
}

/*
DWORD __fastcall CHD_Player_3DDlg::DisplayFrame_YUV_Thread(LPVOID lpParameter)
{
	for(int CurrentFrame=1;CurrentFrame<TotalFrames;CurrentFrame++)
	{
		switch(m_bPlayMode)
		{
		case TRUE:
			DisplayFrame_YUV(CurrentFrame);
			break;
		case FALSE:
			DisplayFrame_YUV_2d(CurrentFrame);
			break;
		}

		CurrentFrame=CurrentFrame+1;//֡������ ׼��������һ֡

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames ʱ ���Ѿ�������Ƶ�ļ������ݷ�Χ��
		{
			CurrentFrame=0;
		}
	}
	

	return 0;
}
*/



DWORD __fastcall CHD_Player_3DDlg::DisplayFrame_YUV_Thread(LPVOID lpParameter)
{
	while(1)
	{

		if(CurrentFrame==0)
		{
			::QueryPerformanceCounter(&m_count_start_);
		}

		::QueryPerformanceCounter(&m_count_start);

		switch(m_bPlayMode)
		{
		case TRUE:
			DisplayFrame_YUV(CurrentFrame);
			//DisplayFrame(CurrentFrame);
			break;
		case FALSE:
			DisplayFrame_YUV_2d(CurrentFrame);
			//DisplayFrame_2d(CurrentFrame);
			break;
		}
		::QueryPerformanceCounter(&m_count_stop);

		time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
		TIME.Format(_T("��֡ʱ��: %lld ms     "),time);

		::QueryPerformanceCounter(&m_count_stop_);

		time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
		TIME_.Format(_T("�ۼ�ʱ��: %lld ms     "),time_);

		if(CurrentFrame!=0)
		{
			fps=1000/(time);// f/s
			FPS.Format(_T("����֡��: %lld f/s     "),fps);

			fps_=1000/(time_/CurrentFrame);// f/s
			FPS_.Format(_T("ʵ��֡��: %lld f/s     "),fps_);
		}		
		//==========================================���㲢��ʾ ֡��

		//==========================================��ʾ ��ǰ֡ ����
		if(!m_bFullScreen)
		{
			CString inf;
			inf.Format(_T("���Ž���:%d/%d     "),CurrentFrame,TotalFrames);
			SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
		}
		//==========================================��ʾ ��ǰ֡ ����

		//=====================//********************//�˾䵼��YUVÿ֡�������� ԭ��δ֪ ��ʱ��ע����
		m_ctrlSliderPos.SetPos(CurrentFrame);//Ӧ�ü��ڴ˴� ��������ȫ������ʱ ���ȴ���
		//=====================//********************//�˾䵼��YUVÿ֡�������� ԭ��δ֪ ��ʱ��ע����

		//CurrentFrame=CurrentFrame+1;//֡������ ׼��������һ֡
		CurrentFrame++;

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames ʱ ���Ѿ�������Ƶ�ļ������ݷ�Χ��
		{
			CurrentFrame=0;
		}

		//switch(m_bPlayMode)
		//{
		//case TRUE:
		//	DisplayFrame_YUV(CurrentFrame);
		//	//DisplayFrame(CurrentFrame);
		//	break;
		//case FALSE:
		//	DisplayFrame_YUV_2d(CurrentFrame);
		//	//DisplayFrame_2d(CurrentFrame);
		//	break;
		//}
		//CurrentFrame=CurrentFrame+1;
	}

	return 0;
}

//======================================================================================


DWORD WINAPI CHD_Player_3DDlg::YUV_Read_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->YUV_Read(NULL);
}



DWORD __fastcall CHD_Player_3DDlg::YUV_Read(LPVOID lpParameter)
{
	//HANDLE yuv_Mutex[100];
	for(int i=0;i<100;i++)
	{
		yuv_Mutex[i]=::CreateMutexW(NULL,FALSE,NULL);
	}
	//============================================================

	YUV_No_Read_total=0;//���ڿ��ƽ���Ͳ��ŵ���Խ���
	YUV_No_Display_total=0;//���ڿ��ƽ���Ͳ��ŵ���Խ���

	DWORD dwRead;

	while(YUV_No_Read_total<TotalFrames)
	{
		while(YUV_No_Read_total-YUV_No_Display_total<99)//������99 ����100
		{
			//=============================================================���� Mutex
			::WaitForSingleObject(yuv_Mutex[YUV_No_Read_total%100],INFINITE);//////
			//=============================================================���� Mutex

			::SetFilePointer(m_hFile,YUV_No_Read_total*(Width*Height*1.5),NULL,FILE_BEGIN);

			/*::ReadFile(m_hFile,
			buffer_YUV+YUV_No_Read_total*(Width*Height*1.5),
			Width*Height*1.5,&dwRead,NULL);*/
			::ReadFile(m_hFile,
				buffer_YUV+(YUV_No_Read_total%100)*(Width*Height*3/2),
				Width*Height*1.5,&dwRead,NULL);

			cout<<"Read YUV frame: "<<YUV_No_Read_total<<endl;

			//=============================================================�ͷ� Mutex
			::ReleaseMutex(yuv_Mutex[YUV_No_Read_total%100]);//////
			//=============================================================�ͷ� Mutex

			m_ctrlProgress.SetPos(YUV_No_Read_total);

			YUV_No_Read_total++;
		}

		//=======================================�߳�ͨ��
		if(YUV_No_Read_total==99)
		{
			::PostThreadMessage(n_Thread_ID_YUV_Play,WM_READY_TO_PLAY,0,0);
		}
		//=======================================�߳�ͨ��


	}

	return 0;
}




DWORD WINAPI CHD_Player_3DDlg::YUV_Play_Shell(LPVOID lpParameter)
{
	CHD_Player_3DDlg * ThisThread=(CHD_Player_3DDlg *)lpParameter;
	return ThisThread->YUV_Play(NULL);
}


DWORD __fastcall CHD_Player_3DDlg::YUV_Play(LPVOID lpParameter)
{
	//=======================================�߳�ͨ��
	MSG msg;
	while(1)
	{
		if(::GetMessage(&msg,0,0,0))
		{
			if(msg.message==WM_READY_TO_PLAY)
			{
				//::AfxMessageBox(_T("WM_READY_TO_PLAY"));
				break;
			}
		}
	}	
	//=======================================�߳�ͨ��
	//=========================================//=========================================
	while(1)
	{
		if(YUV_No_Display_total<YUV_No_Read_total)
		{
			::WaitForSingleObject(yuv_Mutex[YUV_No_Display_total%100],INFINITE);

			switch(m_bPlayMode)
			{
			case TRUE:
				DisplayFrame_YUV_Buffer(YUV_No_Display_total);
				break;
			case FALSE:
				DisplayFrame_YUV_Buffer_2d(YUV_No_Display_total);
				break;
			}

			//m_ctrlSliderPos.SetPos(YUV_No_Display_total);//�������ò��Ž����� ��Ļ������

			::ReleaseMutex(yuv_Mutex[YUV_No_Display_total%100]);

			YUV_No_Display_total++;
		}
	}
	return 0;
}
