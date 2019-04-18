/**========================================================================
* 디바이스 생성
* D3D 디바이스를 생성하고, 이를 사용해서 화면을 지우는 방법에 대해서 공부하게 된다.
*=========================================================================*/

// Direct3D9을 사용하기 위한 헤더
#include <d3d9.h>
#include <d3dx9.h>
/**========================================================================
* 전역 변수
*=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;			// D3D 디바이스를 생성할 D3D 객체 변수
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;	// 랜더링에 사용될 D3D 디바이스
IDirect3DVertexBuffer9 * g_pVB = NULL;
D3DDISPLAYMODE		displaymode;
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; //정점의 변환된 좌표(rhw 값이 있으면 변환이 완료된 정점이다.)
	DWORD color; // 정점의 색깔
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

											/**========================================================================
											* Direct3D 초기화
											*=========================================================================*/


HRESULT InitVB()
{

	D3DXMATRIX rotation;
	D3DXMatrixRotationZ(&rotation,60 * 3.141592 / 180);
	D3DXVECTOR3 ref;
	
	CUSTOMVERTEX vertices[8];

	vertices[0] = { 0.0f, 0.0f, 0.5f, 1.0f, RGB(255,0,255), };
	vertices[1] = { 50.0f, 0.0f, 0.5f, 1.0f, RGB(255,0,255), };

	for (int i = 2; i < 8; i++)
	{
		D3DXVECTOR3 vec3 = { vertices[i - 1].x,vertices[i - 1].y,vertices[i - 1].z };

		D3DXVec3TransformNormal(&ref, &vec3, &rotation);
		vertices[i] = { ref.x, ref.y,ref.z,RGB(255,0,255) };
	}



	for (int i = 0; i < 8; i++)
	{
		vertices[i].x += 500;
		vertices[i].y += 500;
	}
	
	// 정점 버퍼를 생성한다.
	// 정점 구조체 3개를 저장할 메모리를 할당한다.
	// FVF를 지정하여 보관할 데이터의 형식을 지정한다.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(vertices),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}
	//정점의 버퍼를 값으로 채운다.
	//정점 버퍼의 Lock() 함수를 호출하여 포인터를 얻어온다.
	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();
	return S_OK;

}

HRESULT InitD3D(HWND hWnd)
{
	// 디바이스를 생성하기 위한 D3D 객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;				// 디바이스 생성을 위한 구조체
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// 반드시 ZeroMemory() 함수로 미리 구조체를 깨끗이 지워야 한다.

	d3dpp.Windowed = TRUE;						// 창 모드로 생성
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 가장 효율적인 SWAP 효과
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// 현재 바탕화면 모드에 맞춰서 후면 버퍼를 생성.
	d3dpp.BackBufferHeight = displaymode.Height;
	d3dpp.BackBufferWidth = displaymode.Width;
	

												// 디바이스를 설정해서 생성
												// 디폴트 비디오카드를 사용하고, HAL 디바이스를 생성한다.
												// 정점 처리는 모든 카드에서 지원하는 SW처리로 생성한다.
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// 디바이스 상태 정보를 처리할 경우 여기에서 한다.
	if (SUCCEEDED(InitVB()))
		return S_OK;
	else
		return E_FAIL;
}

/**========================================================================
* 초기화 객체들 Release
*=========================================================================*/
void Cleanup()
{
	//해제 순서가 중요 인터페이스 생성의 역순으로 해제하자.

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if(g_pVB != NULL)
		g_pVB->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();


}

/**========================================================================
* 화면 그리기
*=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// 후면 버퍼를 파란색(0, 0, 255)으로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//실제 렌더링 명령들이 나열될 곳
		// 정점 버퍼로 삼각형을 그린다.
		// 정점 정보가 담겨 있는 정점 버퍼를 출력 스트림으로 할당한다.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		// D3D에게 정점 셰이터 정보를 지정한다. 대부분의 경우 FVF만 지정한다.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		// 기하 정보를 출력하기 위한 DrawPrimitive() 함수 호출
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 6);

		//렌더링 종료
		g_pd3dDevice->EndScene();
	}

	// 후면 버퍼를 보이는 화면으로 전환.
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**========================================================================
* WinProc
*=========================================================================*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState(VK_RETURN) & 0x8000)
			int a = 0;
		return 0;
	case WM_PAINT:
		Render();
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**========================================================================
* Window 생성
*=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	//윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Init", NULL };

	//winclass 레지스터에 등록
	RegisterClassEx(&wc);
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displaymode);

	HWND hWnd = CreateWindow("D3D Init", "D3D Init", WS_OVERLAPPEDWINDOW, 0, 0, displaymode.Width, displaymode.Height,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}
