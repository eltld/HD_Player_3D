// HD_Player_3D.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CHD_Player_3DApp:
// 有关此类的实现，请参阅 HD_Player_3D.cpp
//

class CHD_Player_3DApp : public CWinApp
{
public:
	CHD_Player_3DApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CHD_Player_3DApp theApp;