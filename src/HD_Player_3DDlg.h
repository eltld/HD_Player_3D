// HD_Player_3DDlg.h : 头文件
//

#pragma once

//需要包含头文件:vfw.h 引入库文件:vfw32.lib
#include "vfw.h"
#include "afxcmn.h"

#include "HD_Player_3D_FileList.h"
//为了让 播放列表 成为 非模态对话框

#include "Log.h"


// CHD_Player_3DDlg 对话框
class CHD_Player_3DDlg : public CDialog
{
// 构造
public:
	CHD_Player_3DDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HD_PLAYER_3D_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//================================构造函数中初始化
	BYTE *buffer;
	BYTE *BMPbuffer;
	BYTE *BMPbuffer_;
	BYTE *BMPbuffer_origin;
	BYTE *BMPbuffer_origin_;
//	BYTE *BMPbuffer_all;
	BYTE *BMPbuffer_Mpeg2;
	//================================构造函数中初始化

	//======================================================初始化函数 OnInitDialog 中赋值
	BYTE *BMPbuffer_2d[8];
	int Width;
	int Height;
	int Height_2d;
	int Width_;
	int Height_;

	int Width_origin;
	int Height_origin;

	int Width_origin_1_8;
	int Height_origin_1_8;

	int TotalFrames;
	int CurrentFrame;

	CSliderCtrl m_ctrlSliderPos;
	CProgressCtrl m_ctrlProgress;

	BOOL m_bPlaying;
	BOOL m_bFullScreen;
	BOOL m_bPlayMode;// 1 立体 0 平面

	LPBITMAPINFO m_lpBmpInfo; //这个信息结构 显示BMP时 用作DrawDibDraw函数中的参数
	LPBITMAPINFO m_lpBmpInfo_; //这个信息结构 显示BMP时 用作DrawDibDraw函数中的参数
	//======================================================初始化函数 OnInitDialog 中赋值

	//================================其他地方用到的东西

	//================================其他地方用到的东西

	//================================函数声明
	BOOL DisplayFrame(int frame);
	BOOL DisplayFrame_2d(int frame);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonFullscreen();
	//================================函数声明

	//========================================合成算法需要用到的东西
	void InitDelta(void);	
	void InvertPixel(PBYTE BmpBuf);
	void InvertImage(PBYTE pDataIn, PBYTE pDataOut,int type);//type表示颠倒图像的类型 1 按原始图颠倒 2按高清图颠倒 两者宽高不同
	void InvertImage_origin(PBYTE pDataIn, PBYTE pDataOut,int type);

	void Reorder(PBYTE pDataIn, PBYTE pDataOut);
	int delta[1080];
	int shuzu[3];
	//========================================合成算法需要用到的东西

	//======================================================================YUV2RGB 多线程实现
	void Convert(PBYTE pDataIn, PBYTE pDataOut);
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象
	static DWORD WINAPI Convert_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_4_Shell(LPVOID lpParameter);
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象

	DWORD __fastcall Convert_1(LPVOID lpParameter);
	DWORD __fastcall Convert_2(LPVOID lpParameter);
	DWORD __fastcall Convert_3(LPVOID lpParameter);
	DWORD __fastcall Convert_4(LPVOID lpParameter);
	//======================================================================YUV2RGB 多线程实现


	//================================================================================原来的 合成算法
	int Combine(PBYTE pDataIn,PBYTE pDataOut);
	//================================================================================原来的 合成算法
	//================================================================================原来的 合成算法的 4个线程
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象
	static DWORD WINAPI Combine_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_4_Shell(LPVOID lpParameter);
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象

	DWORD __fastcall Combine_1(LPVOID lpParameter);
	DWORD __fastcall Combine_2(LPVOID lpParameter);
	DWORD __fastcall Combine_3(LPVOID lpParameter);
	DWORD __fastcall Combine_4(LPVOID lpParameter);
	//================================================================================原来的 合成算法的 4个线程

	
	//================================================================================我自己的 合成算法
	void Combine_GDX(PBYTE pDataIn, PBYTE pDataOut);
    //================================================================================我自己的 合成算法
	//================================================================================我自己的 合成算法的 4个线程
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象
	static DWORD WINAPI Combine_GDX_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_4_Shell(LPVOID lpParameter);
	//静态成员函数 不依赖于任何对象就可以直接调用
	//非静态成员函数 必学依赖于一个对象

	DWORD __fastcall Combine_GDX_1(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_2(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_3(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_4(LPVOID lpParameter);
	//================================================================================我自己的 合成算法的 4个线程

	//================================================显示图像 用到的东西
	HANDLE m_hFile;//普通文件句柄
	HANDLE m_hFile_map;//内存映射文件句柄
	LPVOID p_File_Map;//映射的内存地址

//	HBITMAP m_hBmp;
	BITMAP m_Bmp;
	RECT rect;
	HDC hdc;
//	HDC hdcMem;

	//DrawDibDraw 用到的东西
	HDRAWDIB m_hDIB;
	//DrawDibDraw 用到的东西
	//================================================显示图像 用到的东西

	//==============================全屏状态时需要隐藏所有控件 防止播放时出现
	HWND h_;
    //==============================全屏状态时需要隐藏所有控件 防止播放时出现

	//对话框类的窗口很特殊 无法直接接受键盘消息 需要把相应操作写在 PreTranslateMessage 函数中
    //[重载] CDialog::PreTranslateMessage() 函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//对话框类的窗口很特殊 无法直接接受键盘消息 需要把相应操作写在 PreTranslateMessage 函数中
    //[重载] CDialog::PreTranslateMessage() 函数

	//==========================================计算帧率的模块
	LARGE_INTEGER m_count_start;
	LARGE_INTEGER m_count_start_;
	LARGE_INTEGER m_count_stop;
	LARGE_INTEGER m_count_stop_;
	LARGE_INTEGER m_freq;

	LONGLONG time;//单帧时长
	LONGLONG time_;//总时长
	LONGLONG fps;//理论帧率
	LONGLONG fps_;//实际帧率

	CString TIME;
	CString TIME_;
	CString FPS;
	CString FPS_;

	LONGLONG t1;
	LONGLONG t2;
	LONGLONG t3;
	LONGLONG t4;
	LONGLONG t5;
	//==========================================计算帧率的模块

	afx_msg void OnBnClickedButtonBuffer();
	afx_msg void OnBnClickedButtonSet();

	//========================================================以下是解码功能需要用到的东西
	char * InputFilePath;


	CString Input_File_Type;

	//=========================================================
	void Sample_19201024_640384_8(PBYTE pDataIn, PBYTE pDataOut);
	//void Sample_6403072_7202880(PBYTE pDataIn, PBYTE pDataOut);


	//=========================================================播放mpeg2 用到的帧号码
	int Mpeg2_Frame_Buffer_Size;
	int play_n;


	//=============================================================================================
    //=========================================264播放功能=========================================

	static DWORD WINAPI H264_Dec_Shell(LPVOID lpParameter);
	DWORD __fastcall H264_Dec(LPVOID lpParameter);

	static DWORD WINAPI H264_Play_Shell(LPVOID lpParameter);
	DWORD __fastcall H264_Play(LPVOID lpParameter);

	BOOL DisplayFrame_264(int frame);
	BOOL DisplayFrame_264_2d(int frame);

	BOOL DisplayFrame_264_Buffer(int frame);
	BOOL DisplayFrame_264_Buffer_2d(int frame);

	//=============================================================================================
    //=========================================264播放功能=========================================

	//=============================================================================================
    //=========================================YUV播放功能=========================================
	static DWORD WINAPI YUV_Read_Shell(LPVOID lpParameter);
	DWORD __fastcall YUV_Read(LPVOID lpParameter);

	static DWORD WINAPI YUV_Play_Shell(LPVOID lpParameter);
	DWORD __fastcall YUV_Play(LPVOID lpParameter);

	BOOL DisplayFrame_YUV_Buffer(int frame);
	BOOL DisplayFrame_YUV_Buffer_2d(int frame);

	//=============================================================================================
    //=========================================YUV播放功能=========================================

	//=============================================================================================
    //=========================================YUV Buffer==========================================
	/*HANDLE hThread_Buffer_YUV;
	static DWORD WINAPI YUV_Buffer_Shell(LPVOID lpParameter);
	DWORD __fastcall YUV_Buffer(LPVOID lpParameter);*/
	//=============================================================================================
    //=========================================YUV Buffer==========================================


	//================================================================================
	int Sample_6403072_7202880(PBYTE pDataIn, PBYTE pDataOut);

	static DWORD WINAPI Sample_6403072_7202880_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Sample_6403072_7202880_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Sample_6403072_7202880_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Sample_6403072_7202880_4_Shell(LPVOID lpParameter);

	DWORD __fastcall Sample_6403072_7202880_1(LPVOID lpParameter);
	DWORD __fastcall Sample_6403072_7202880_2(LPVOID lpParameter);
	DWORD __fastcall Sample_6403072_7202880_3(LPVOID lpParameter);
	DWORD __fastcall Sample_6403072_7202880_4(LPVOID lpParameter);
	//================================================================================


	//================================================================================
	int Convert_264YUV_BGR(PBYTE pDataIn, PBYTE pDataOut);
	
	static DWORD WINAPI Convert_264YUV_BGR_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_264YUV_BGR_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_264YUV_BGR_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_264YUV_BGR_4_Shell(LPVOID lpParameter);

	DWORD __fastcall Convert_264YUV_BGR_1(LPVOID lpParameter);
	DWORD __fastcall Convert_264YUV_BGR_2(LPVOID lpParameter);
	DWORD __fastcall Convert_264YUV_BGR_3(LPVOID lpParameter);
	DWORD __fastcall Convert_264YUV_BGR_4(LPVOID lpParameter);
	//================================================================================

	afx_msg void OnBnClickedButtonNetwork();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonList();
	BOOL DisplayFrame_YUV(int frame);
	BOOL DisplayFrame_YUV_2d(int frame);

	//================================================================================
	//以下为采用新的播放模式 而不是用定时器

	static DWORD WINAPI DisplayFrame_YUV_Thread_Shell(LPVOID lpParameter);
	DWORD __fastcall DisplayFrame_YUV_Thread(LPVOID lpParameter);

	HANDLE hThread_Display_YUV;

	//为了让 播放列表 成为 非模态对话框
	CHD_Player_3D_FileList FileList;
	bool FileListShow;
	//为了让 播放列表 成为 非模态对话框

	//=================================================LOG
	Log log_time;
	CString inf_time;
	//=================================================LOG

	//=======////=======////=======////=======////=======////=======////=======//
	unsigned char * buffer_YUV;
	//=======================用于YUV视频播放的同步功能===========================
	HANDLE yuv_Mutex[100];

	int YUV_No;//100 与标志位一致
	int YUV_No_Read;
	int YUV_No_Play;

	int YUV_No_Read_total;//用于控制解码和播放的相对进度
	int YUV_No_Display_total;//用于控制解码和播放的相对进度
	DWORD n_Thread_ID_YUV_Read;//线程ID
	DWORD n_Thread_ID_YUV_Play;//线程ID

	HANDLE hThread_YUV_Read;
	HANDLE hThread_YUV_Play;
	//=======================用于YUV视频播放的同步功能===========================
};

