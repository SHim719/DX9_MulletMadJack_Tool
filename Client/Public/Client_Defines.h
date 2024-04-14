#pragma once

#include "../Default/stdafx.h"
#include <process.h>

#include "Client_Components.h"

/* Ŭ���̾�Ʈ���� �����ϴ� ��� Ŭ�������� ���������� ���� ����ϴ� ���ǵ��� ��Ƶд�. */
namespace Client
{
	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, 
		LEVEL_LOADING, 
		LEVEL_LOGO, 
		LEVEL_GAMEPLAY, 
		LEVEL_TOOL,
		LEVEL_END };
}



extern HWND				g_hWnd;





using namespace Client;