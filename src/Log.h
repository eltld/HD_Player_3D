#include <afx.h>
#pragma once
//|||||||||||||||||||||||||||||||||||||||||||==========================================LOG
class Log
{
public:

	_SYSTEMTIME time;
	_SYSTEMTIME time_1;
	_SYSTEMTIME time_2;

	HANDLE hFile;
	CString inf_cstring;
	char * inf_char;

	Log();
	~Log();

	int Initialize();
	int Get_Time(CString inf);
	int Get_Time_1();
	int Get_Time_2(CString inf);

	//=========================================================
	LARGE_INTEGER m_freq;
	LARGE_INTEGER m_count_start;	
	LARGE_INTEGER m_count_stop;
	LONGLONG time_Consumed;

	int Query_Time_1();
	int Query_Time_2(CString inf);
	

};

//|||||||||||||||||||||||||||||||||||||||||||==========================================LOG