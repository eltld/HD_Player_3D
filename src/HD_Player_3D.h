// HD_Player_3D.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHD_Player_3DApp:
// �йش����ʵ�֣������ HD_Player_3D.cpp
//

class CHD_Player_3DApp : public CWinApp
{
public:
	CHD_Player_3DApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHD_Player_3DApp theApp;