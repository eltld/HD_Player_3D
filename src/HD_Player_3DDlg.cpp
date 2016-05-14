// HD_Player_3DDlg.cpp : 实现文件
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

//=======================用于264视频的缓冲播放模式===========================
int Dec_264_No;//100 与标志位一致
int Dec_264_Decode_flag[100];
int Dec_264_Display_flag[100];
int Dec_264_No_Decode;
int Dec_264_No_Display;
int Dec_264_Offset;
//=======================用于264视频的缓冲播放模式===========================

//=======================用于264视频播放的同步功能===========================
HANDLE h_264_Mutex[100];
int Dec_264_No_Decode_total;//用于控制解码和播放的相对进度
int Dec_264_No_Display_total;//用于控制解码和播放的相对进度
DWORD n_Thread_ID_Dec;//线程ID 全局变量
DWORD n_Thread_ID_Play;//线程ID 全局变量
//=======================用于264视频播放的同步功能===========================


//===========================================================================
Client aClient;
//===========================================================================

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CHD_Player_3DDlg 对话框




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


// CHD_Player_3DDlg 消息处理程序

BOOL CHD_Player_3DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//=====================================================================================//
	//================================================================================初始化函数 OnInitDialog 中赋值
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
	m_bPlayMode=TRUE;// 1 立体 0 平面
//	m_bPlayMode=FALSE;// 1 立体 0 平面

	switch(m_bFullScreen) //根据全屏状态标志 采用不同的图像显示方式
	{
	case 0://非全屏状态

		hdc=::GetDC(m_hWnd);
		::GetClientRect(m_hWnd,&rect);

		rect.left=rect.left+7;
		rect.top=rect.top+6;
		rect.right=rect.left+640;//16
		rect.bottom=rect.top+360;//9		
		break;

	case 1://全屏状态

		hdc=::GetDC(m_hWnd);
		::GetWindowRect(m_hWnd,&rect);//2选1 //::GetClientRect(m_hWnd,&rect);//2选1

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

	//====================================================================用来存储已经处理好的mpeg2解码数据
	//Mpeg2_Frame_Buffer_Size=30;

	//BMPbuffer_Mpeg2=new unsigned char[1920*1080*3*Mpeg2_Frame_Buffer_Size];
	//ZeroMemory(BMPbuffer_Mpeg2,1920*1080*3*Mpeg2_Frame_Buffer_Size);
	//====================================================================用来存储已经处理好的mpeg2解码数据



	Width_origin_1_8=640;
	Height_origin_1_8=3072;
	/*
	//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间
	buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
	ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

	Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
	ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
	//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间
    */

	//=======================用于264视频的缓冲播放模式===========================
	counter_264=0;/////////////////////

	Dec_264_No=100;//100 与标志位一致
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

	
	//=======================用于264视频的缓冲播放模式===========================
	

	//BMPbuffer_all=new unsigned char[Width_*Height_*3];
	//ZeroMemory(BMPbuffer_all,Width_*Height_*3);
	//放在OPEN 相应函数里面 因为要用到打开文件后得到的帧数

	for(int i=0;i<8;i++)
	{BMPbuffer_2d[i]=NULL;}
	for(int i=0;i<8;i++)
	{
		BMPbuffer_2d[i]=BMPbuffer+i*Width*Height_2d*3; //已经验证过了 这样赋地址 没有问题 OK		
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
	inf.Format(_T("%d"),Width); //=======================注意要用宏 _T()
	SetDlgItemText(IDC_EDIT_WIDTH,inf);
	inf.Format(_T("%d"),Height);
	SetDlgItemText(IDC_EDIT_HEIGHT,inf);
	inf.Format(_T("%d"),Width_);
	SetDlgItemText(IDC_EDIT_WIDTH_,inf);
	inf.Format(_T("%d"),Height_);
	SetDlgItemText(IDC_EDIT_HEIGHT_,inf);

	inf.Format(_T("%d"),CurrentFrame); //=======================注意要用宏 _T()
	SetDlgItemText(IDC_EDIT_FRAME,inf);
	inf.Format(_T("%d"),TotalFrames);
	SetDlgItemText(IDC_EDIT_FRAMES,inf);
	*/

	//================================================================================初始化函数 OnInitDialog 中赋值
	//=====================================================================================//

	//===================================================================YUV2RGB 功能的准备部分
	init_dither_tab();
	//===================================================================YUV2RGB 功能的准备部分

	//========================================合成算法需要用到的东西
	for(int i=0;i<3;i++)
	{
		shuzu[i]=i;//shuzu[3]={0,1,2};
	}

	InitDelta(); //合成算法初始化
	//========================================合成算法需要用到的东西

	::QueryPerformanceFrequency(&m_freq);

	//========================================================以下是解码功能需要用到的东西
	InputFilePath=NULL;
//	Dec_Buffer=new BYTE[720*360*3];//此处是全局变量
//	ZeroMemory(Dec_Buffer,720*360*3);
	//========================================================以上是解码功能需要用到的东西

	//================================================================================
	//以下为采用新的播放模式 而不是用定时器
	hThread_Display_YUV=NULL;
	//================================================================================
	//以上为采用新的播放模式 而不是用定时器

	//为了让 播放列表 成为 非模态对话框
	FileList.Create(IDD_DIALOG_FILELIST,this);
	FileListShow=FALSE;
	//为了让 播放列表 成为 非模态对话框

	//=================================================钩子 HOOK
	::g_hKeyBoard=::SetWindowsHookExW(WH_KEYBOARD,KeyBoardProc,NULL,::GetCurrentThreadId());//键盘钩子
	::g_hMouse=::SetWindowsHookExW(WH_MOUSE,MouseProc,NULL,::GetCurrentThreadId());//鼠标钩子

	//=================================================钩子 HOOK

	//=================================================利用控制台实时输出程序信息
	::AllocConsole();
	::freopen("CONOUT$","r+t",stdout);
	cout<<"在程序中可以随时利用 cout 输出程序的执行信息:"<<endl;
	//=================================================利用控制台实时输出程序信息

	//=================================================LOG	
	log_time.Initialize();
	//=================================================LOG

	//=======================用于YUV视频播放的同步功能===========================
	hThread_YUV_Read=NULL;
	hThread_YUV_Play=NULL;
	//=======================用于YUV视频播放的同步功能===========================

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHD_Player_3DDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHD_Player_3DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHD_Player_3DDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码

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

	//===================================清理内存映射文件
	//::UnmapViewOfFile(p_File_Map);
	//::CloseHandle(m_hFile);
	//::CloseHandle(m_hFile_map);
	//===================================清理内存映射文件
	::ReleaseDC(m_hWnd,hdc);

	//=================================================利用控制台实时输出程序信息
	::FreeConsole();
	//=================================================利用控制台实时输出程序信息
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
	if(m_hFile) //如果不是第一次打开文件 需要把前一次打开时创建的句柄 清空
	{
		::CloseHandle(m_hFile);
		m_hFile=NULL;
	}

	Input_File_Type=dlgOpen.GetFileExt();

	if(Input_File_Type==_T("264"))
	{
		//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间
		buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
		ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

		Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
		ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
		//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间
		::wcstombs(filename_264,dlgOpen.GetPathName().GetBuffer(),dlgOpen.GetPathName().GetLength());

		OnBnClickedButtonBuffer();
	}	

	else if(Input_File_Type==_T("yuv"))
	{
		m_hFile=::CreateFileW(dlgOpen.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		//====================文件路径              访问方式     共享方式      安全标志   创建处理       文件标志    another文件句柄
		//If the function [CreateFile] fails, the return value is INVALID_HANDLE_VALUE
		if(m_hFile==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("打开文件失败"));
			m_hFile=NULL;
			return;
		}

		//此处如果视频文件很大 则其字节数存放到 LARGE_INTEGER型结构体变量length 中 分高位和低位分别存储
		LARGE_INTEGER length;//struct LARGE_INTEGER{DWORD LowPart;LONG HighPart;}
		::GetFileSizeEx(m_hFile,&length); //字节数 存入结构体 length 中

		TotalFrames=length.QuadPart/(Width*Height*1.5); //总帧数
		CurrentFrame=0; //当前帧

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

		//==================================用来存储 YUV读取的数据 放到打开按钮处申请空间
		buffer_YUV =new unsigned char[Width*Height*1.5*1*100];//YUV
		ZeroMemory(buffer_YUV,Width*Height*1.5*1*100);
		//==================================用来存储 YUV读取的数据 放到打开按钮处申请空间

		OnBnClickedButtonBuffer();
	}
	/* //内存映射文件
	else if(Input_File_Type==_T("yuv"))
	{
		m_hFile=::CreateFileW(dlgOpen.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		//====================文件路径              访问方式     共享方式      安全标志   创建处理       文件标志    another文件句柄
		//If the function [CreateFile] fails, the return value is INVALID_HANDLE_VALUE
		if(m_hFile==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("打开文件失败"));
			m_hFile=NULL;
			return;
		}

		//============================================================创建内存映射文件内核对象
		m_hFile_map=::CreateFileMapping(m_hFile,NULL,PAGE_READONLY,0,0,NULL);
		if(m_hFile_map==INVALID_HANDLE_VALUE)
		{
			::AfxMessageBox(_T("[创建] 内存映射文件 失败"));
			m_hFile_map=NULL;
			return;
		}
		//============================================================创建内存映射文件内核对象
		//============================================================将内存映射文件映射到进程的地址空间
		//============================================================后边三个参数有讲究
		
		p_File_Map=::MapViewOfFile(m_hFile_map,FILE_MAP_READ,0,0,0);
		if(p_File_Map==NULL)
		{
			::AfxMessageBox(_T("[映射] 内存映射文件 失败"));
			p_File_Map=NULL;
			return;
		}
		//============================================================将内存映射文件映射到进程的地址空间


		//此处如果视频文件很大 则其字节数存放到 LARGE_INTEGER型结构体变量length 中 分高位和低位分别存储
		LARGE_INTEGER length;//struct LARGE_INTEGER{DWORD LowPart;LONG HighPart;}
		::GetFileSizeEx(m_hFile,&length); //字节数 存入结构体 length 中

		TotalFrames=length.QuadPart/(Width*Height*1.5); //总帧数
		CurrentFrame=0; //当前帧

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
	*/ //内存映射文件

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

		//启动一个定时器 用以时刻检验解码线程与播放线程的进度
		/*SetTimer(nID_Timer_264_Decode,USER_TIMER_MINIMUM,NULL);*/
		//启动一个定时器 用以时刻检验解码线程与播放线程的进度

		//启动一个定时器 用以显示解码线程的进度
		SetTimer(nID_Timer_264_Decode,USER_TIMER_MINIMUM,NULL);
		//启动一个定时器 用以显示解码线程的进度

		//=========================================================================解码线程
		//LPDWORD p_Thread_ID_Dec;//线程ID 全局变量
		::hThread_264Dec=::CreateThread(NULL,0,H264_Dec_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_Dec);
		//::CloseHandle(::hThread_264Dec);//==*** CloseHandle 不能随便用 用完了就无法用句柄控制线程了 ***==//
		::Decoding_264=1;
		::ResumeThread(hThread_264Dec);
		//=========================================================================解码线程

		//=========================================================================播放线程
		//LPDWORD p_Thread_ID_Play;//线程ID 全局变量
		::hThread_264_Play=::CreateThread(NULL,0,H264_Play_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_Play);
		//::CloseHandle(hThread_264_Play);//==*** CloseHandle 不能随便用 用完了就无法用句柄控制线程了 ***==//

		//::ResumeThread(hThread_264_Play);//放到 PLAY 里面启动
		OnBnClickedButtonPlay();
		//=========================================================================播放线程
	}

	else if(Input_File_Type==_T("yuv"))
	{
		//hThread_Buffer_YUV=::CreateThread(NULL,0,YUV_Buffer_Shell,this,0,NULL);//创建线程

		//for(int frame=0;frame<TotalFrames;frame++)
		//{
		//	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

		//	DWORD dwRead;
		//	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);
		//	// Buffer 的作用主要在 读取数据的过程 已下两步 可省略 差别可忽略
		//	//Convert(buffer,BMPbuffer);
		//	//Combine(BMPbuffer,BMPbuffer_);
		//	m_ctrlProgress.SetPos(frame+1);
		//}

		//启动一个定时器 用以 读取YUV 线程的进度
		SetTimer(nID_Timer_YUV_Read,USER_TIMER_MINIMUM,NULL);
		//启动一个定时器 用以 读取YUV 线程的进度

		//=========================================================================YUV 读数据线程
		hThread_YUV_Read=::CreateThread(NULL,0,YUV_Read_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_YUV_Read);		
		::ResumeThread(hThread_YUV_Read);
		//=========================================================================YUV 读数据线程

		//=========================================================================YUV 播放线程
		hThread_YUV_Play=::CreateThread(NULL,0,YUV_Play_Shell,this,CREATE_SUSPENDED,&n_Thread_ID_YUV_Play);//创建线程
		//::ResumeThread(hThread_YUV_Play);//放到 PLAY 里面启动
		OnBnClickedButtonPlay();
		//=========================================================================YUV 播放线程
	}

	
}

void CHD_Player_3DDlg::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
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
				::ResumeThread(hThread_Display_YUV);//恢复线程
			}
			else
			{
				hThread_Display_YUV=::CreateThread(NULL,0,DisplayFrame_YUV_Thread_Shell,this,0,NULL);//创建线程
				//::CloseHandle(hThread_Display_YUV);
			}
			*/

			//if(hThread_YUV_Play!=NULL)
			//{
			//	::ResumeThread(hThread_YUV_Play);//恢复线程
			//}
			//else
			//{
			//	hThread_YUV_Play=::CreateThread(NULL,0,YUV_Play_Shell,this,0,&n_Thread_ID_YUV_Play);//创建线程
			//}

			::ResumeThread(hThread_YUV_Play);//恢复线程
		}
		else if(Input_File_Type==_T("264"))
		{
			//SetTimer(nID_Timer_264_Play,USER_TIMER_MINIMUM,NULL);
			//SetTimer(nID_Timer_264_Play_Buffer,USER_TIMER_MINIMUM,NULL);
			//=========================调节计时器时隙以适应解码的帧率===============================
			/*SetTimer(nID_Timer_264_Play_Buffer,50,NULL);*/
			//=========================调节计时器时隙以适应解码的帧率===============================

			//::ResumeThread(hThread_264Dec);

			//=========================================================================播放线程
			//恢复时 恢复播放线程
			::ResumeThread(hThread_264_Play);
			//=========================================================================播放线程
		}

		m_bPlaying=TRUE;
		SetDlgItemText(IDC_BUTTON_PLAY,_T("暂停"));
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
			//暂停时 挂起播放线程 不挂起解码线程
			//::SuspendThread(hThread_264Dec);
			::SuspendThread(hThread_264_Play);
			//============================================================
		}

		m_bPlaying=FALSE;
		SetDlgItemText(IDC_BUTTON_PLAY,_T("播放"));
		break;
	}
}


void CHD_Player_3DDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
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
	//	m_ctrlSliderPos.SetPos(YUV_No_Display_total);//这里设置播放进度条 屏幕闪
	//}


	/*
	if(nIDEvent==nID_Timer_264_Play_Buffer)//缓冲播放模式
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
			TIME.Format(_T("单帧时长: %lld ms     "),time);
			::QueryPerformanceCounter(&m_count_stop_);
			time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
			TIME_.Format(_T("累计时长: %lld ms     "),time_);

			if(Dec_264_No_Display!=0)
			{
				fps=1000/(time);// f/s
				FPS.Format(_T("理论帧率: %lld f/s     "),fps);

				fps_=1000/(time_/Dec_264_No_Display);// f/s
				FPS_.Format(_T("实际帧率: %lld f/s     "),fps_);
			}

			if(!m_bFullScreen)
			{
				CString inf;
				inf.Format(_T("播放进度:%d     "),Dec_264_No_Display);
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


	if(nIDEvent==nID_Timer_264_Play)//单帧播放模式
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
		//==========================================计算并显示 帧率

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
		//DisplayFrame_YUV(CurrentFrame);//=============//此处实现播放
		::QueryPerformanceCounter(&m_count_stop);

		time=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms
		TIME.Format(_T("单帧时长: %lld ms     "),time);

		::QueryPerformanceCounter(&m_count_stop_);

		time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
		TIME_.Format(_T("累计时长: %lld ms     "),time_);

		if(CurrentFrame!=0)
		{
			fps=1000/(time);// f/s
			FPS.Format(_T("理论帧率: %lld f/s     "),fps);

			fps_=1000/(time_/CurrentFrame);// f/s
			FPS_.Format(_T("实际帧率: %lld f/s     "),fps_);
		}		
		//==========================================计算并显示 帧率

		//==========================================显示 当前帧 进度
		if(!m_bFullScreen)
		{
			CString inf;
			inf.Format(_T("播放进度:%d/%d     "),CurrentFrame,TotalFrames);

			//SetDlgItemText(IDC_EDIT_INFO,inf+TIME+FPS+TIME_+FPS_);
			//采用带缓冲的播放模式后 不需要 TIME FPS 这2个信息了:
			SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);


			//m_ctrlSliderPos.SetPos(CurrentFrame);
			//以上这句加在此处会引起 全屏状态返回时 进度返回非全屏前断点 的问题
			//应该加在外面一层
		}
		//==========================================显示 当前帧 进度

		m_ctrlSliderPos.SetPos(CurrentFrame);//应该加在此处 否则引起全屏返回时 进度错误

		//m_ctrlProgress.SetPos(CurrentFrame);

		CurrentFrame=CurrentFrame+1;//帧数增加 准备播放下一帧

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames 时 就已经超出视频文件的数据范围了
		{
			CurrentFrame=0;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CHD_Player_3DDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

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

		//KillTimer(nID_Timer);//若正在播放 则设置定时器 改变播放标志 //实现暂停
		DWORD ExitCode;
		::TerminateThread(hThread_YUV_Play,ExitCode);//终止线程

		SetDlgItemText(IDC_BUTTON_PLAY,_T("播放"));
		//若希望关闭当前播放的视频文件 并显示背景图片 则还需要添加关闭DC的代码 目前未实现
	}

		
}



void CHD_Player_3DDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPlayMode=!m_bPlayMode;

	//////////////////////////////////////////////////////////////////////////////////////////
}


BOOL CHD_Player_3DDlg::DisplayFrame(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
{
	//=================================================LOG
	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);
	//=================================================LOG

	//////////////////////////////////////////////////////////////////////////////////////////
	//====================带有缓冲功能的版本中 除了播放环节之外 全部移动到 BUFFER 的响应函数中

	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);

	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	//======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||======||||||

	//=================================================================YUV2RGB: buffer->BMPbuffer
	//其实这里输入参数没有用 指针已经在线程函数里面设置好了 ===========//Convert(NULL,NULL);
	Convert(buffer,BMPbuffer);
	//=================================================================YUV2RGB: buffer->BMPbuffer

	//=================================================================合成算法: BMPbuffer->BMPbuffer_
	//InitDelta(); 合成算法初始化 放在初始化函数里了
	//其实这里输入参数没有用 指针已经在线程函数里面设置好了 ===========//Combine(NULL,NULL);
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的	
//	Combine_GDX(BMPbuffer,BMPbuffer_); //合成算法(2) 我自己的合成算法
	//=================================================================合成算法: BMPbuffer->BMPbuffer_
	
	//////////////////////////////////////////////////////////////////////////////////////////
	//====================带有缓冲功能的版本中 除了播放环节之外 全部移动到 BUFFER 的响应函数中


	//===========================================================================================针对DC的操作 实现显示一帧图像功能
	/* //已经声明为类的成员
	HBITMAP m_hBmp;
	BITMAP m_Bmp;
	RECT rect;
	HDC hdc;
	HDC hdcMem;
	*/ //已经声明为类的成员

//	m_hBmp=::CreateBitmap(Width,Height,1,24,BMPbuffer);	//获得此帧图像（位图）的句柄

	

	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}


BOOL CHD_Player_3DDlg::DisplayFrame_2d(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
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
			BMPbuffer,//此处为DIB数据的起始指针
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}


void CHD_Player_3DDlg::OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

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
	
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}



void CHD_Player_3DDlg::OnBnClickedButtonFullscreen()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_bFullScreen)
	{
	case FALSE:
		this->Invalidate(TRUE);//擦除更新区域内的背景（DC绘图留下的）

		//先最大化 再隐藏标题栏
		::ShowWindow(m_hWnd,SW_MAXIMIZE); //最大化窗口（和点击标题栏上的最大化按钮效果一样） 成功
		::SetWindowLongW(m_hWnd,GWL_STYLE,::GetWindowLongW(m_hWnd,GWL_STYLE)-WS_CAPTION); //隐藏标题栏 成功
		m_bFullScreen=TRUE;
		//==============================全屏状态时需要隐藏所有控件 防止播放时出现
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
		for(int nID=1000-1;nID<=1017;nID++)//与resource.h中定义的ID值相对应
		{
			h_=::GetDlgItem(m_hWnd,nID);
			::ShowWindow(h_,SW_HIDE);
		}
		h_=::GetDlgItem(m_hWnd,IDOK);
		::ShowWindow(h_,SW_HIDE);
		//==============================全屏状态时需要隐藏所有控件 防止播放时出现
		break;
	case TRUE:
		this->Invalidate(TRUE);//擦除更新区域内的背景（DC绘图留下的）

		//注意 顺序不能乱 要与上面的操作 对应
		//先显示标题栏 再原始大小
		::SetWindowLongW(m_hWnd,GWL_STYLE,::GetWindowLongW(m_hWnd,GWL_STYLE)+WS_CAPTION); //显示标题栏 成功		
		::ShowWindow(m_hWnd,SW_SHOWNORMAL);				
		m_bFullScreen=FALSE;
		//==============================全屏状态时需要隐藏所有控件 防止播放时出现
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
		for(int nID=1000-1;nID<=1017;nID++)//与resource.h中定义的ID值相对应
		{
			h_=::GetDlgItem(m_hWnd,nID);
			::ShowWindow(h_,SW_SHOW);
		}
		h_=::GetDlgItem(m_hWnd,IDOK);
		::ShowWindow(h_,SW_SHOW);
		//==============================全屏状态时需要隐藏所有控件 防止播放时出现
		break;
	}

	switch(m_bFullScreen) //根据全屏状态标志 采用不同的图像显示方式
	{
	case 0://非全屏状态

		hdc=::GetDC(m_hWnd);
		::GetClientRect(m_hWnd,&rect);

		rect.left=rect.left+7;
		rect.top=rect.top+6;
		rect.right=rect.left+640;//16
		rect.bottom=rect.top+360;//9		
		break;

	case 1://全屏状态

		hdc=::GetDC(m_hWnd);
		::GetWindowRect(m_hWnd,&rect);//2选1 //::GetClientRect(m_hWnd,&rect);//2选1

		rect.left=0;
		rect.top=0;
		rect.right=::GetSystemMetrics(SM_CXSCREEN);
		rect.bottom=::GetSystemMetrics(SM_CYSCREEN);
		break;
	}
}


//================================================================================合成算法 准备工作
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
		//BmpBuf+3*i BmpBuf+3*i+2 互换
		swap=*(BmpBuf+3*i+0);
		*(BmpBuf+3*i+0)=*(BmpBuf+3*i+2);
		*(BmpBuf+3*i+2)=swap;
	}
}

//type表示颠倒图像的类型 1 按原始图颠倒 2按高清图颠倒 两者宽高不同
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








//================================================================================合成算法 准备工作

//================================================================8幅图像 顺序颠倒一下
void CHD_Player_3DDlg::Reorder(PBYTE pDataIn, PBYTE pDataOut)
{
	for(int i=0;i<8;i++)
	{
		memcpy(pDataOut+i*Width*Height_2d*3,pDataIn+(8-1-i)*Width*Height_2d*3,Width*Height_2d*3);
	}
}
//================================================================8幅图像 顺序颠倒一下

//================================================================================原来的 合成算法
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
*/ //使用多线程合成算法之前 保存
//================================================================================原来的 合成算法

//================================================================================我自己的 合成算法
/*
void CHD_Player_3DDlg::Combine_GDX(PBYTE pDataIn, PBYTE pDataOut)
{
	for(int line=0;line<Height_/3;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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
*/ //使用多线程合成算法之前 保存
//================================================================================我自己的 合成算法


//==========================================================================================YUV2RGB 多线程实现
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

//==========================================================================================YUV2RGB 多线程实现

//====================================================================================原来的 合成算法的 4个线程
int CHD_Player_3DDlg::Combine(PBYTE pDataIn,PBYTE pDataOut)
{
	//向CreateThread函数传递Combine_1_Shell时会出现问题 原因：
	//在类没有声明对象前这个类是不存在的，所以：
	//不可以使用 [类非静态成员函数] 作为[线程函数]
	//解决方案：
	//(1)函数 Combine_1_Shell 声明成静态成员函数
	//(2)函数 Combine_1_Shell 不在类中声明 而是将其声明为全局函数
	//此处采用(1)

	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象

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

//====================================================================================原来的 合成算法的 4个线程

//====================================================================================我自己的 合成算法的 4个线程
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
	for(int line=0;line<Height_/3;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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

	for(int line=0;line<(Height_/3)*1/4;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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

	for(int line=(Height_/3)*1/4;line<(Height_/3)*2/4;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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

	for(int line=(Height_/3)*2/4;line<(Height_/3)*3/4;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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

	for(int line=(Height_/3)*3/4;line<(Height_/3)*4/4;line++)//需要合成的行数为 3*360=1080 每个循环合成3行
	{
		//每3行分成 720 个数据单元 1920*3=5760=720*8
		//由于是错位填数据 所以实际取值从 0 到 Width-2 一共 Width-1 个单元
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


//====================================================================================我自己的 合成算法的 4个线程


//对话框类的窗口很特殊 无法直接接受键盘消息 需要把相应操作写在 PreTranslateMessage 函数中
//[重载] CDialog::PreTranslateMessage() 函数
BOOL CHD_Player_3DDlg::PreTranslateMessage(MSG * pMsg)
{
	//在执行基类的 PreTranslateMessage 函数前 捕获键盘消息并作出相应处理
	if(pMsg->message==WM_KEYDOWN)
	{
		//没有用 ESC SPACE 因为他们比较特殊 在默认状态下有特殊功能
		//用 [任意键] 可以在 全屏 与 非全屏 之间切换
		if(pMsg->wParam)
		{
			OnBnClickedButtonFullscreen();
		}
	}

	CDialog::PreTranslateMessage(pMsg);
	return 0;//注意：此处一定要返回0 否则所有消息的处理都会受到影响!
}
//对话框类的窗口很特殊 无法直接接受键盘消息 需要把相应操作写在 PreTranslateMessage 函数中
//[重载] CDialog::PreTranslateMessage() 函数


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
	//=======================================线程通信
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
	//=======================================线程通信
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

			m_ctrlSliderPos.SetPos(Dec_264_No_Display);//这里设置播放进度条 屏幕不闪了

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

		m_ctrlSliderPos.SetPos(Dec_264_No_Display);//这里设置播放进度条 屏幕不闪了

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
	//	TIME.Format(_T("单帧时长: %lld ms     "),time);
	//	::QueryPerformanceCounter(&m_count_stop_);
	//	time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
	//	TIME_.Format(_T("累计时长: %lld ms     "),time_);

	//	if(Dec_264_No_Display!=0)
	//	{
	//		fps=1000/(time);// f/s
	//		FPS.Format(_T("理论帧率: %lld f/s     "),fps);

	//		fps_=1000/(time_/Dec_264_No_Display);// f/s
	//		FPS_.Format(_T("实际帧率: %lld f/s     "),fps_);
	//	}

	//	if(!m_bFullScreen)
	//	{
	//		CString inf;
	//		inf.Format(_T("播放进度:%d     "),Dec_264_No_Display);
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






//这个函数将 10 块 原始视频 分解抽样成 640384 * 8 垂直排列的长条形状
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
//======================================以下是264播放功能======================================

BOOL CHD_Player_3DDlg::DisplayFrame_264(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
{	
	Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //四线程实现

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //四线程实现
	
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现



	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}

BOOL CHD_Player_3DDlg::DisplayFrame_264_2d(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
{	
	Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //四线程实现

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //四线程实现
	
	//Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现


	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//这两个值可以都设置成-1，表示按原始位图的宽度和高度绘图
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//此处为DIB数据的起始指针
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}


BOOL CHD_Player_3DDlg::DisplayFrame_264_Buffer(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
{	
	//Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //四线程实现
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

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //四线程实现
	
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现


	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}



BOOL CHD_Player_3DDlg::DisplayFrame_264_Buffer_2d(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
{	
	//Convert_264YUV_BGR(buffer_264,Dec_Buffer_264); //四线程实现
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

	Sample_6403072_7202880(Dec_Buffer_264,BMPbuffer); //四线程实现
	
	//Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//这两个值可以都设置成-1，表示按原始位图的宽度和高度绘图
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//此处为DIB数据的起始指针
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}

//====================================================================================================

BOOL CHD_Player_3DDlg::DisplayFrame_YUV_Buffer(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
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
	
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现


	//inf_time.Format(_T("Frame %d "),frame);
	//log_time.Get_Time(inf_time);

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}



BOOL CHD_Player_3DDlg::DisplayFrame_YUV_Buffer_2d(int frame)//frame=0 第一帧 frame=frame-1 第frame帧
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
	
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的 //四线程实现

	
	m_hDIB=DrawDibOpen();
	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo->bmiHeader,Width,Height,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,
			rect.right,rect.bottom,//这两个值可以都设置成-1，表示按原始位图的宽度和高度绘图
			&m_lpBmpInfo->bmiHeader,
			BMPbuffer,//此处为DIB数据的起始指针
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}

	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
}

//====================================================================================================


int CHD_Player_3DDlg::Sample_6403072_7202880(PBYTE pDataIn, PBYTE pDataOut)
{
	/*
	for(int n=0;n<=7;n++)
	{
		PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
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
	*/ //以上过程改用 4线程 实现 提高工作效率

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
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
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
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
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
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
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
		//PBYTE pIn=pDataIn+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
		//PBYTE pOut=pDataOut+n*Width*Height_2d*3;
		PBYTE pIn=Dec_Buffer_264+n*640*384*3+640*3*12;//取中间的360行 最上边和最下边的12行去掉
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
	// TODO: 在此添加控件通知处理程序代码
	//CHD_Player_3D_Network NetworkDlg;
	//NetworkDlg.DoModal();

	CHD_Player_3D_Connect ConnectDlg;
	ConnectDlg.DoModal();


	
}

void CHD_Player_3DDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnDestroy();
	
	OnOK();

	//CHD_Player_3DDlg newdlg;
	//newdlg.DoModal();
	
}

void CHD_Player_3DDlg::OnBnClickedButtonList()
{
	// TODO: 在此添加控件通知处理程序代码
	//CHD_Player_3D_FileList FileList;	


	//=====================================================================非模态对话框
	//FileList.Create(IDD_DIALOG_FILELIST,this);
	switch(FileListShow) //根据全屏状态标志 采用不同的图像显示方式
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
	//=====================================================================非模态对话框


	/*
	//=====================================================================模态对话框
	if(FileList.DoModal()==IDOK)
	{
		Input_File_Type=_T("264");
		//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间
		buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*1.5*1*100];//YUV
		ZeroMemory(buffer_264,Width_origin_1_8*Height_origin_1_8*1.5*1*100);

		Dec_Buffer_264 =new unsigned char[Width_origin_1_8*Height_origin_1_8*3*1];//BGR
		ZeroMemory(Dec_Buffer_264,Width_origin_1_8*Height_origin_1_8*3*1);
		//==================================用来存储已经处理好的264解码数据 放到打开按钮处申请空间

		OnBnClickedButtonBuffer();
	}
	//=====================================================================模态对话框
	*/
}


BOOL CHD_Player_3DDlg::DisplayFrame_YUV(int frame)//带 测量时耗功能
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
	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的	
	log_time.Query_Time_2(_T("Combine"));
	
	log_time.Query_Time_1();
	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
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

	return 1; //返回1表示正常播放 返回0表示未正常播放
}


/*
BOOL CHD_Player_3DDlg::DisplayFrame_YUV(int frame)//不带 测量时耗功能 与上面的函数一样只是去掉了测时耗功能
{
	::SetFilePointer(m_hFile,frame*(Width*Height*1.5),NULL,FILE_BEGIN);
	DWORD dwRead;
	::ReadFile(m_hFile,buffer,Width*Height*1.5,&dwRead,NULL);

	Convert(buffer,BMPbuffer);

	Combine(BMPbuffer,BMPbuffer_); //合成算法(1) 原来的	

	m_hDIB=DrawDibOpen();

	if(m_hDIB!=NULL)
	{		
		DrawDibBegin(m_hDIB,hdc,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,Width_,Height_,0);

		DrawDibDraw(m_hDIB,hdc,rect.left,rect.top,rect.right,rect.bottom,&m_lpBmpInfo_->bmiHeader,
			BMPbuffer_,//此处为DIB数据的起始指针
			//          BMPbuffer_all+frame*Width_*Height_*3,//此处为DIB数据的起始指针
			0,0,
			Width_,				 // src : width
			Height_,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
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
			BMPbuffer,//此处为DIB数据的起始指针
			0,0,
			Width,				 // src : width
			Height_2d,             // src : height
			DDF_SAME_DRAW);

		DrawDibEnd(m_hDIB);
	}
	//::ReleaseDC(m_hWnd,hdc);

	return 1; //返回1表示正常播放 返回0表示未正常播放
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
	TIME.Format(_T("单帧时长: %lld ms     "),time);

	::QueryPerformanceCounter(&m_count_stop_);

	time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
	TIME_.Format(_T("累计时长: %lld ms     "),time_);

	if(CurrentFrame!=0)
	{
		fps=1000/(time);// f/s
		FPS.Format(_T("理论帧率: %lld f/s     "),fps);

		fps_=1000/(time_/CurrentFrame);// f/s
		FPS_.Format(_T("实际帧率: %lld f/s     "),fps_);
	}		
	//==========================================计算并显示 帧率

	//==========================================显示 当前帧 进度
	if(!m_bFullScreen)
	{
		CString inf;
		inf.Format(_T("播放进度:%d/%d     "),CurrentFrame,TotalFrames);
		SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
	}
	//==========================================显示 当前帧 进度

	m_ctrlSliderPos.SetPos(CurrentFrame);//应该加在此处 否则引起全屏返回时 进度错误

	CurrentFrame=CurrentFrame+1;//帧数增加 准备播放下一帧

	if(CurrentFrame>=TotalFrames)//frame=TotalFrames 时 就已经超出视频文件的数据范围了
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

		CurrentFrame=CurrentFrame+1;//帧数增加 准备播放下一帧

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames 时 就已经超出视频文件的数据范围了
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
		TIME.Format(_T("单帧时长: %lld ms     "),time);

		::QueryPerformanceCounter(&m_count_stop_);

		time_=1000*(m_count_stop_.QuadPart-m_count_start_.QuadPart)/(m_freq.QuadPart);// ms
		TIME_.Format(_T("累计时长: %lld ms     "),time_);

		if(CurrentFrame!=0)
		{
			fps=1000/(time);// f/s
			FPS.Format(_T("理论帧率: %lld f/s     "),fps);

			fps_=1000/(time_/CurrentFrame);// f/s
			FPS_.Format(_T("实际帧率: %lld f/s     "),fps_);
		}		
		//==========================================计算并显示 帧率

		//==========================================显示 当前帧 进度
		if(!m_bFullScreen)
		{
			CString inf;
			inf.Format(_T("播放进度:%d/%d     "),CurrentFrame,TotalFrames);
			SetDlgItemText(IDC_EDIT_INFO,inf+TIME_+FPS_);
		}
		//==========================================显示 当前帧 进度

		//=====================//********************//此句导致YUV每帧播放两遍 原因未知 暂时不注释了
		m_ctrlSliderPos.SetPos(CurrentFrame);//应该加在此处 否则引起全屏返回时 进度错误
		//=====================//********************//此句导致YUV每帧播放两遍 原因未知 暂时不注释了

		//CurrentFrame=CurrentFrame+1;//帧数增加 准备播放下一帧
		CurrentFrame++;

		if(CurrentFrame>=TotalFrames)//frame=TotalFrames 时 就已经超出视频文件的数据范围了
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

	YUV_No_Read_total=0;//用于控制解码和播放的相对进度
	YUV_No_Display_total=0;//用于控制解码和播放的相对进度

	DWORD dwRead;

	while(YUV_No_Read_total<TotalFrames)
	{
		while(YUV_No_Read_total-YUV_No_Display_total<99)//这里是99 不是100
		{
			//=============================================================请求 Mutex
			::WaitForSingleObject(yuv_Mutex[YUV_No_Read_total%100],INFINITE);//////
			//=============================================================请求 Mutex

			::SetFilePointer(m_hFile,YUV_No_Read_total*(Width*Height*1.5),NULL,FILE_BEGIN);

			/*::ReadFile(m_hFile,
			buffer_YUV+YUV_No_Read_total*(Width*Height*1.5),
			Width*Height*1.5,&dwRead,NULL);*/
			::ReadFile(m_hFile,
				buffer_YUV+(YUV_No_Read_total%100)*(Width*Height*3/2),
				Width*Height*1.5,&dwRead,NULL);

			cout<<"Read YUV frame: "<<YUV_No_Read_total<<endl;

			//=============================================================释放 Mutex
			::ReleaseMutex(yuv_Mutex[YUV_No_Read_total%100]);//////
			//=============================================================释放 Mutex

			m_ctrlProgress.SetPos(YUV_No_Read_total);

			YUV_No_Read_total++;
		}

		//=======================================线程通信
		if(YUV_No_Read_total==99)
		{
			::PostThreadMessage(n_Thread_ID_YUV_Play,WM_READY_TO_PLAY,0,0);
		}
		//=======================================线程通信


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
	//=======================================线程通信
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
	//=======================================线程通信
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

			//m_ctrlSliderPos.SetPos(YUV_No_Display_total);//这里设置播放进度条 屏幕不闪了

			::ReleaseMutex(yuv_Mutex[YUV_No_Display_total%100]);

			YUV_No_Display_total++;
		}
	}
	return 0;
}
