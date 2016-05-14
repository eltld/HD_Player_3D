#include "stdafx.h"
#include <afx.h>
#include "Log.h"

Log::Log()
{
}
Log::~Log()
{
}

int Log::Initialize()
{
	hFile=::CreateFileW(
		_T("D:\\My Documents\\Visual Studio 2008\\Projects_201010_2\\Log.txt"),
		GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile==INVALID_HANDLE_VALUE)
	{
		::printf("File Handle Invalid\n");
		hFile=NULL;
		return 1;
	}

	::QueryPerformanceFrequency(&m_freq);

	return 0;
}

int Log::Get_Time(CString inf)
{
	_SYSTEMTIME time;
	::GetLocalTime(&time);//本地时间

	inf_cstring.Format(_T("%d-%d-%d  %d:%d:%d:%d"),
		time.wYear,
		time.wMonth,
		time.wDay,
		time.wHour,
		time.wMinute,
		time.wSecond,
		time.wMilliseconds);

	inf_cstring=inf_cstring+_T(" :")+inf;

	inf_char=new char[inf_cstring.GetLength()+1+2];
	for(int i=0;i<=inf_cstring.GetLength();i++)
	{
		*(inf_char+i)=inf_cstring.GetAt(i);
	}

	*(inf_char+inf_cstring.GetLength()+1)=13;//回车
	*(inf_char+inf_cstring.GetLength()+2)=10;//换行

	DWORD Writen_Bytes;
	::WriteFile(hFile,inf_char,inf_cstring.GetLength()+2,&Writen_Bytes,NULL);
	::WriteFile(hFile,"\n",1,&Writen_Bytes,NULL);

	return 0;
}

int Log::Get_Time_1()
{
	::GetLocalTime(&time_1);//本地时间
	return 0;
}

int Log::Get_Time_2(CString inf)
{
	::GetLocalTime(&time_2);//本地时间

	//========================================================
	WORD time_d=(time_2.wMinute*60*1000+time_2.wSecond*1000+time_2.wMilliseconds)-
		        (time_1.wMinute*60*1000+time_1.wSecond*1000+time_1.wMilliseconds);

	inf_cstring.Format(_T("%d"),time_d);
	//========================================================

	inf_cstring=inf_cstring+_T(" ms :")+inf;

	inf_char=new char[inf_cstring.GetLength()+1+2];
	for(int i=0;i<=inf_cstring.GetLength();i++)
	{
		*(inf_char+i)=inf_cstring.GetAt(i);
	}
	*(inf_char+inf_cstring.GetLength()+1)=13;//回车
	*(inf_char+inf_cstring.GetLength()+2)=10;//换行

	DWORD Writen_Bytes;
	::WriteFile(hFile,inf_char,inf_cstring.GetLength()+2,&Writen_Bytes,NULL);
	::WriteFile(hFile,"\n",1,&Writen_Bytes,NULL);

	return 0;
}

int Log::Query_Time_1()
{
	::QueryPerformanceCounter(&m_count_start);
	return 0;
}

int Log::Query_Time_2(CString inf)
{
	::QueryPerformanceCounter(&m_count_stop);

	time_Consumed=1000*(m_count_stop.QuadPart-m_count_start.QuadPart)/(m_freq.QuadPart);// ms

	inf_cstring.Format(_T("%lld"),time_Consumed);


	//=============================================================
	inf_cstring=inf_cstring+_T(" ms :")+inf;

	inf_char=new char[inf_cstring.GetLength()+1+2];
	for(int i=0;i<=inf_cstring.GetLength();i++)
	{
		*(inf_char+i)=inf_cstring.GetAt(i);
	}
	*(inf_char+inf_cstring.GetLength()+1)=13;//回车
	*(inf_char+inf_cstring.GetLength()+2)=10;//换行

	DWORD Writen_Bytes;
	::WriteFile(hFile,inf_char,inf_cstring.GetLength()+2,&Writen_Bytes,NULL);
	::WriteFile(hFile,"\n",1,&Writen_Bytes,NULL);


	return 0;
}