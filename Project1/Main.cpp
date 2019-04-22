#include "DirectX.h"
#include "Main.h"



/**========================================================================
* 윈도우 프로시저
*=========================================================================*/
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

/**========================================================================
* WinMain
*=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Hierarchy", NULL };
	

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Hierarchy", "D3D Hierarchy", WS_OVERLAPPEDWINDOW,
		0, 0, 500, 500, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	DirectX dx3;
	
	if (FAILED(dx3.initDirectX(hWnd)))
		return -1;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dx3.Render();
			if (GetKeyState(VK_RETURN) & 0x8000 && GetKeyState(VK_MENU) & 0x8000)
			{
				dx3.initDirectX(hWnd);
			}
		}
	}

	UnregisterClass("D3D Hierarchy", wc.hInstance);
	return 0;
}