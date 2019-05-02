#include "DirectX.h"
#include "Main.h"



/**========================================================================
* 윈도우 프로시저
*=========================================================================*/

bool clicked = false;
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto iter = HIWORD(wParam);
	auto iter2s = LOWORD(wParam);
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		clicked = true;
		DirectX::GetInstance()->Clicked(lParam);
		break;
	case WM_MOUSEMOVE:
		if (!clicked)
			break;

		if (wParam == MK_CONTROL)
		{
			DirectX::GetInstance()->EyeMoved(lParam);
		}
		else 	if (wParam == MK_LBUTTON)
			DirectX::GetInstance()->RotateMoved(lParam);

		break;
	case WM_LBUTTONUP:
		
		clicked = false;
		break;
	case WM_MOUSEWHEEL:
		DirectX::GetInstance()->Wheel(GET_WHEEL_DELTA_WPARAM(wParam) / 120);
		
		break ;
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
		1920 - 850, 100, 800, 800, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	DirectX::GetInstance()->initDirectX(hWnd);
	
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
			DirectX::GetInstance()->Render();
			DirectX::GetInstance()->Update();
		}
	}

	UnregisterClass("D3D Hierarchy", wc.hInstance);
	return 0;
}