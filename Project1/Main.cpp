#include "DirectX.h"
#include "InputManager.h"

/**
@file Main.cpp
@data 2019/09/08
@author 서승현
@version 0.0.1
@brief 메인 프로그램
*/

/*
@fn LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
@data 2019/09/08
@author 서승현
@brief 윈도우 메시지 처리기
@param hwnd 윈도우 핸들
@param msg 윈도우 핸들에 들어오는 메시지 값
@param wParam WPARAM 값
@param lParam LPARAM 값
@return 윈도우 처리 결과
*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**
@fn INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)

@brief 윈도우 API 메인 함수
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Hierarchy", NULL };
	
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Hierarchy", "D3D Hierarchy", WS_OVERLAPPEDWINDOW,
		1920 - 850, 100, 800, 800, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	
	DirectX::GetInstance()->initDirectX(hWnd);
	
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	//ShowCursor(false);
	InputManager::Getinstance()->Init(hWnd);
	InputManager::Getinstance()->RegistKeyCode(VK_CONTROL);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InputManager::Getinstance()->Update();
			DirectX::GetInstance()->Update();

			DirectX::GetInstance()->Render();
			
		}
	}

	UnregisterClass("D3D Hierarchy", wc.hInstance);
	return 0;
}