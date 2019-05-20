/**========================================================================
* 텍스쳐
* 메쉬에 텍스쳐를 입히는 공부를 한다.
*=========================================================================*/

#include <Windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>

//SHOW_HOW_TO_USE_TCT가 선언된 것과 선언되지 않은 것의 컴파일 결과를 반드시 비교하자.
//#define SHOW_HOW_TO_USE_TCT

/**========================================================================
* 전역 변수
*=========================================================================*/

LPDIRECT3D9				g_pD3D = NULL;
LPDIRECT3DDEVICE9		g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVB = NULL;
LPDIRECT3DTEXTURE9		g_pTexture = NULL; // 텍스처 인터페이스 선언

struct CUSTOMEVERTEX
{
	D3DXVECTOR3 position;
	D3DCOLOR	color;
	FLOAT		tu, tv; // 텍스처 좌표
};

//사용자 정점 구조체에 관한 정보를 나타내는 FVF 값
#ifdef SHOW_HOW_TO_USE_TCI
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#else
#define D3DFVF_CUSTOMEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#endif

/**========================================================================
* 초기화
*=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

/**========================================================================
* 기하 정보 초기화
* 정점 버퍼와 텍스쳐 생성
*=========================================================================*/
HRESULT InitGeometry()
{
	//D3DX 계열 함수를 사용하여 파일로부터 텍스처 생성
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, "nayeon.jpg", &g_pTexture)))
	{	//현재 폴더에 파일이 없으면 상위 폴더 검색
		if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, "..\\nayeon.jpg", &g_pTexture)))
		{
			MessageBox(NULL, "Could not find nayeon.jpg", "Texture.exe", MB_OK);

			return E_FAIL;
		}
	}

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMEVERTEX), 0,
		D3DFVF_CUSTOMEVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMEVERTEX* pVertices;

	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);

		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].color = 0xffffffff;

#ifndef SHOW_HOW_TO_USE_TCI
		//자동으로 좌표를 생성하지 않고 설정하는 방법.
		pVertices[2 * i + 0].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].tv = 1.0f;

#endif

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].color = 0xff808080;

#ifndef SHOW_HOW_TO_USE_TCI
		//자동으로 좌표를 생성하지 않고 설정하는 방법.
		pVertices[2 * i + 1].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].tv = 0.0f;

#endif
	}

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
* 초기화 객체들 Release
*=========================================================================*/
void Cleanup()
{
	//해제 순서가 중요 인터페이스 생성의 역순으로 해제하자.
	if (g_pTexture != NULL)
		g_pTexture->Release();

	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}

/**========================================================================
* 행렬 설정 : 행렬에는 3가지가 있으며, 각각 월드, 뷰, 프로젝션 행렬이다.
*=========================================================================*/
void SetupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

/**========================================================================
* 화면 그리기
*=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// 후면 버퍼를 파란색(0, 0, 255)으로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	SetupMatrices();

	// 랜더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//생성한 텍스처를 0번 텍스처 스테이지에 올린다.
		//텍스처 스테이지는 여러 장의 텍스처와 색깔 정보를 섞어서 출력할 때 사용한다.
		//여기서는 텍스처의 색깔과 정점의 색깔 정보를 modulate 연산으로 섞어서 출력한다.
		g_pd3dDevice->SetTexture(0, g_pTexture); // 0번 텍스처 스테이지에 텍스처 고정
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); // MODULATE 연산으로 색깔을 섞음
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); // 첫 번째 섞을 색은 텍스처의 색
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); // 두 번째 섞을 색은 정점의 색
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE); // alpha 연산을 사용하지 않음 설정

#ifndef SHOW_HOW_TO_USE_TCI
																			   //D3D의 텍스처 좌표 생성 기능을 사용
																			   //카메라 좌표계에서의 정점 정보를 사용하여 텍스처 좌표를 생성.
																			   //4*4 크기의 텍스처 변환 행렬을 텍스처 좌표 인덱스(TCI=Texture Coord Index)전달 인자를
																			   //사용하여 x, y, z TCI 좌표를 u, v 텍스처 좌표로 변환한다.
																			   //사용한 것은 단순히(-1.0 ~ +1.0) 값을 (0.0 ~ 1.0)사이의 값으로 변환한는 행렬이다.
																			   // 월드, 뷰, 프로젝션 변환을 거친 정점은 (-1.0 ~ +1.0)사이의 값을 갖게 된다.
		D3DXMATRIXA16 mat;
		mat._11 = 0.25f; mat._12 = 0.00f; mat._13 = 0.00f; mat._14 = 0.00f;
		mat._21 = 0.25f; mat._22 = 0.00f; mat._23 = 0.00f; mat._24 = 0.00f;
		mat._31 = 0.25f; mat._32 = 0.00f; mat._33 = 0.00f; mat._34 = 0.00f;
		mat._41 = 0.25f; mat._42 = 0.00f; mat._43 = 0.00f; mat._44 = 0.00f;

		//텍스처 변환행렬
		g_pd3dDevice->SetTransform(D3DTS_TEXTURE0, &mat);
		//2차원 텍스처 사용
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		//카메라 좌표계 변환
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
#endif

		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMEVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMEVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);

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
		"D3D Texture", NULL };

	//winclass 레지스터에 등록
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Texture", "D3D Texture", WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		GetDesktopWindow(), NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitGeometry()))
		{
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
					Render();
			}
		}
	}

	//등록된 레지스트 winclass 릴리즈.
	UnregisterClass("D3D Texture", wc.hInstance);
	return 0;
}