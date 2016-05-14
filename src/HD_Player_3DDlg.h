// HD_Player_3DDlg.h : ͷ�ļ�
//

#pragma once

//��Ҫ����ͷ�ļ�:vfw.h ������ļ�:vfw32.lib
#include "vfw.h"
#include "afxcmn.h"

#include "HD_Player_3D_FileList.h"
//Ϊ���� �����б� ��Ϊ ��ģ̬�Ի���

#include "Log.h"


// CHD_Player_3DDlg �Ի���
class CHD_Player_3DDlg : public CDialog
{
// ����
public:
	CHD_Player_3DDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HD_PLAYER_3D_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//================================���캯���г�ʼ��
	BYTE *buffer;
	BYTE *BMPbuffer;
	BYTE *BMPbuffer_;
	BYTE *BMPbuffer_origin;
	BYTE *BMPbuffer_origin_;
//	BYTE *BMPbuffer_all;
	BYTE *BMPbuffer_Mpeg2;
	//================================���캯���г�ʼ��

	//======================================================��ʼ������ OnInitDialog �и�ֵ
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
	BOOL m_bPlayMode;// 1 ���� 0 ƽ��

	LPBITMAPINFO m_lpBmpInfo; //�����Ϣ�ṹ ��ʾBMPʱ ����DrawDibDraw�����еĲ���
	LPBITMAPINFO m_lpBmpInfo_; //�����Ϣ�ṹ ��ʾBMPʱ ����DrawDibDraw�����еĲ���
	//======================================================��ʼ������ OnInitDialog �и�ֵ

	//================================�����ط��õ��Ķ���

	//================================�����ط��õ��Ķ���

	//================================��������
	BOOL DisplayFrame(int frame);
	BOOL DisplayFrame_2d(int frame);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnNMCustomdrawSliderPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonFullscreen();
	//================================��������

	//========================================�ϳ��㷨��Ҫ�õ��Ķ���
	void InitDelta(void);	
	void InvertPixel(PBYTE BmpBuf);
	void InvertImage(PBYTE pDataIn, PBYTE pDataOut,int type);//type��ʾ�ߵ�ͼ������� 1 ��ԭʼͼ�ߵ� 2������ͼ�ߵ� ���߿�߲�ͬ
	void InvertImage_origin(PBYTE pDataIn, PBYTE pDataOut,int type);

	void Reorder(PBYTE pDataIn, PBYTE pDataOut);
	int delta[1080];
	int shuzu[3];
	//========================================�ϳ��㷨��Ҫ�õ��Ķ���

	//======================================================================YUV2RGB ���߳�ʵ��
	void Convert(PBYTE pDataIn, PBYTE pDataOut);
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������
	static DWORD WINAPI Convert_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Convert_4_Shell(LPVOID lpParameter);
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������

	DWORD __fastcall Convert_1(LPVOID lpParameter);
	DWORD __fastcall Convert_2(LPVOID lpParameter);
	DWORD __fastcall Convert_3(LPVOID lpParameter);
	DWORD __fastcall Convert_4(LPVOID lpParameter);
	//======================================================================YUV2RGB ���߳�ʵ��


	//================================================================================ԭ���� �ϳ��㷨
	int Combine(PBYTE pDataIn,PBYTE pDataOut);
	//================================================================================ԭ���� �ϳ��㷨
	//================================================================================ԭ���� �ϳ��㷨�� 4���߳�
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������
	static DWORD WINAPI Combine_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_4_Shell(LPVOID lpParameter);
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������

	DWORD __fastcall Combine_1(LPVOID lpParameter);
	DWORD __fastcall Combine_2(LPVOID lpParameter);
	DWORD __fastcall Combine_3(LPVOID lpParameter);
	DWORD __fastcall Combine_4(LPVOID lpParameter);
	//================================================================================ԭ���� �ϳ��㷨�� 4���߳�

	
	//================================================================================���Լ��� �ϳ��㷨
	void Combine_GDX(PBYTE pDataIn, PBYTE pDataOut);
    //================================================================================���Լ��� �ϳ��㷨
	//================================================================================���Լ��� �ϳ��㷨�� 4���߳�
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������
	static DWORD WINAPI Combine_GDX_1_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_2_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_3_Shell(LPVOID lpParameter);
	static DWORD WINAPI Combine_GDX_4_Shell(LPVOID lpParameter);
	//��̬��Ա���� ���������κζ���Ϳ���ֱ�ӵ���
	//�Ǿ�̬��Ա���� ��ѧ������һ������

	DWORD __fastcall Combine_GDX_1(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_2(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_3(LPVOID lpParameter);
	DWORD __fastcall Combine_GDX_4(LPVOID lpParameter);
	//================================================================================���Լ��� �ϳ��㷨�� 4���߳�

	//================================================��ʾͼ�� �õ��Ķ���
	HANDLE m_hFile;//��ͨ�ļ����
	HANDLE m_hFile_map;//�ڴ�ӳ���ļ����
	LPVOID p_File_Map;//ӳ����ڴ��ַ

//	HBITMAP m_hBmp;
	BITMAP m_Bmp;
	RECT rect;
	HDC hdc;
//	HDC hdcMem;

	//DrawDibDraw �õ��Ķ���
	HDRAWDIB m_hDIB;
	//DrawDibDraw �õ��Ķ���
	//================================================��ʾͼ�� �õ��Ķ���

	//==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����
	HWND h_;
    //==============================ȫ��״̬ʱ��Ҫ�������пؼ� ��ֹ����ʱ����

	//�Ի�����Ĵ��ں����� �޷�ֱ�ӽ��ܼ�����Ϣ ��Ҫ����Ӧ����д�� PreTranslateMessage ������
    //[����] CDialog::PreTranslateMessage() ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�Ի�����Ĵ��ں����� �޷�ֱ�ӽ��ܼ�����Ϣ ��Ҫ����Ӧ����д�� PreTranslateMessage ������
    //[����] CDialog::PreTranslateMessage() ����

	//==========================================����֡�ʵ�ģ��
	LARGE_INTEGER m_count_start;
	LARGE_INTEGER m_count_start_;
	LARGE_INTEGER m_count_stop;
	LARGE_INTEGER m_count_stop_;
	LARGE_INTEGER m_freq;

	LONGLONG time;//��֡ʱ��
	LONGLONG time_;//��ʱ��
	LONGLONG fps;//����֡��
	LONGLONG fps_;//ʵ��֡��

	CString TIME;
	CString TIME_;
	CString FPS;
	CString FPS_;

	LONGLONG t1;
	LONGLONG t2;
	LONGLONG t3;
	LONGLONG t4;
	LONGLONG t5;
	//==========================================����֡�ʵ�ģ��

	afx_msg void OnBnClickedButtonBuffer();
	afx_msg void OnBnClickedButtonSet();

	//========================================================�����ǽ��빦����Ҫ�õ��Ķ���
	char * InputFilePath;


	CString Input_File_Type;

	//=========================================================
	void Sample_19201024_640384_8(PBYTE pDataIn, PBYTE pDataOut);
	//void Sample_6403072_7202880(PBYTE pDataIn, PBYTE pDataOut);


	//=========================================================����mpeg2 �õ���֡����
	int Mpeg2_Frame_Buffer_Size;
	int play_n;


	//=============================================================================================
    //=========================================264���Ź���=========================================

	static DWORD WINAPI H264_Dec_Shell(LPVOID lpParameter);
	DWORD __fastcall H264_Dec(LPVOID lpParameter);

	static DWORD WINAPI H264_Play_Shell(LPVOID lpParameter);
	DWORD __fastcall H264_Play(LPVOID lpParameter);

	BOOL DisplayFrame_264(int frame);
	BOOL DisplayFrame_264_2d(int frame);

	BOOL DisplayFrame_264_Buffer(int frame);
	BOOL DisplayFrame_264_Buffer_2d(int frame);

	//=============================================================================================
    //=========================================264���Ź���=========================================

	//=============================================================================================
    //=========================================YUV���Ź���=========================================
	static DWORD WINAPI YUV_Read_Shell(LPVOID lpParameter);
	DWORD __fastcall YUV_Read(LPVOID lpParameter);

	static DWORD WINAPI YUV_Play_Shell(LPVOID lpParameter);
	DWORD __fastcall YUV_Play(LPVOID lpParameter);

	BOOL DisplayFrame_YUV_Buffer(int frame);
	BOOL DisplayFrame_YUV_Buffer_2d(int frame);

	//=============================================================================================
    //=========================================YUV���Ź���=========================================

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
	//����Ϊ�����µĲ���ģʽ �������ö�ʱ��

	static DWORD WINAPI DisplayFrame_YUV_Thread_Shell(LPVOID lpParameter);
	DWORD __fastcall DisplayFrame_YUV_Thread(LPVOID lpParameter);

	HANDLE hThread_Display_YUV;

	//Ϊ���� �����б� ��Ϊ ��ģ̬�Ի���
	CHD_Player_3D_FileList FileList;
	bool FileListShow;
	//Ϊ���� �����б� ��Ϊ ��ģ̬�Ի���

	//=================================================LOG
	Log log_time;
	CString inf_time;
	//=================================================LOG

	//=======////=======////=======////=======////=======////=======////=======//
	unsigned char * buffer_YUV;
	//=======================����YUV��Ƶ���ŵ�ͬ������===========================
	HANDLE yuv_Mutex[100];

	int YUV_No;//100 ���־λһ��
	int YUV_No_Read;
	int YUV_No_Play;

	int YUV_No_Read_total;//���ڿ��ƽ���Ͳ��ŵ���Խ���
	int YUV_No_Display_total;//���ڿ��ƽ���Ͳ��ŵ���Խ���
	DWORD n_Thread_ID_YUV_Read;//�߳�ID
	DWORD n_Thread_ID_YUV_Play;//�߳�ID

	HANDLE hThread_YUV_Read;
	HANDLE hThread_YUV_Play;
	//=======================����YUV��Ƶ���ŵ�ͬ������===========================
};

