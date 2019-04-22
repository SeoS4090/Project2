/**========================================================================
* 계층 구조
* 계층 구조를 이해하고 계층간의 행렬 실행 순서를 공부한다.
*=========================================================================*/

#include <d3d9.h>
#include <d3dx9.h>

/**========================================================================
* 전역 변수
*=========================================================================*/

LPDIRECT3D9				g_pD3D = NULL;
LPDIRECT3DDEVICE9		g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIB = NULL;



struct planet
{
	D3DXMATRIXA16			m_matScale; //크기
	D3DXMATRIXA16			m_matT; //이동 행렬
	D3DXMATRIXA16			m_matR;	 //자전 행렬
	D3DXMATRIXA16			m_matWR; //공전 행렬
	D3DXMATRIXA16			m_mat;

	planet() {
		D3DXMatrixScaling(&m_matScale, 1, 1, 1);
		D3DXMatrixTranslation(&m_matT, 0, 0, 0);
		D3DXMatrixRotationY(&m_matR, 0);
		D3DXMatrixRotationY(&m_matWR, 0);
	}
	
	D3DXMATRIXA16			calcMatrix(D3DXMATRIXA16& matTMparent) {
		m_mat = calcMatrix() * matTMparent;
		return m_mat;}

	D3DXMATRIXA16			calcMatrix() { 

		m_mat = m_matScale *	m_matR *m_matT *m_matWR;
		return m_mat; }

	D3DXMATRIXA16 getMatrix() { return m_mat; }
};

bool bFullScreen = TRUE;
struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2;
};

/**========================================================================
* Direct3D 초기화
*=========================================================================*/
HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = bFullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

/**========================================================================
* 정점 버퍼
* 정점 버퍼를 생성하고 정점값을 채워넣는다.
*=========================================================================*/

HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] =
	{
		{ -1, 1, 1, 0xffff0000 },
		{ 1, 1, 1, 0xff00ff00 },
		{ 1, 1,-1, 0xff0000ff },
		{ -1, 1,-1, 0xffffff00 },

		{ -1,-1, 1, 0xffff0000 },
		{ 1,-1, 1, 0xffff0000 },
		{ 1,-1,-1, 0xffff0000 },
		{ -1,-1,-1, 0xffff0000 },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	void* pVertices;

	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

/**========================================================================
* 인덱스 버퍼
* 인덱스 버퍼를 생성하고 정점값을 채워넣는다.
*=========================================================================*/

HRESULT InitIB()
{
	MYINDEX indices[] =
	{
		{ 0, 1, 2 },{ 0, 2, 3 },
		{ 4, 6, 5 },{ 4, 7, 6 },
		{ 0, 3, 7 },{ 0, 7, 4 },
		{ 1, 5, 6 },{ 1, 6, 2 },
		{ 3, 2, 6 },{ 3, 6, 7 },
		{ 0, 4, 5 },{ 0, 5, 1 },
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
	{
		return E_FAIL;
	}

	void** pIndices;

	if (FAILED(g_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pIndices, indices, sizeof(indices));

	g_pIB->Unlock();

	return S_OK;
}

/**========================================================================
* 기하 정보 초기화
*=========================================================================*/

HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	return S_OK;
}

/**========================================================================
* 카메라 행렬 설정
*=========================================================================*/
void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(20.0f, 50, -50.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI /4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}



/**========================================================================
* Release - 초기화 객체들 소거
*=========================================================================*/
void Cleanup()
{
	if (g_pIB != NULL)
		g_pIB->Release();

	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)	
		g_pD3D->Release();
}

/**========================================================================
* 메시 그리기
*=========================================================================*/
void DrawMesh(D3DXMATRIXA16* pMat)
{
	
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	
}

/**========================================================================
* 화면 그리기
*=========================================================================*/
void Render()
{
	D3DXMATRIXA16 matWorld;
	

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//스케일 자전 이동 공전 부모

	planet solar[10];

	for (int i = 1; i < 9; i++)
	{
		D3DXMatrixTranslation(&solar[i].m_matT, solar[i-1].m_matT._41 + 3, solar[i - 1].m_matT._42+1, solar[i - 1].m_matT._43 + 2);
		D3DXMatrixRotationY(&solar[i].m_matR, GetTickCount() / 500.0f);
		D3DXMatrixRotationY(&solar[i].m_matWR, GetTickCount() * i / 1024.0f);
	}

	D3DXMatrixScaling(&solar[9].m_matScale, 0.5, 0.5, 0.5);
	D3DXMatrixTranslation(&solar[9].m_matT,3,0,3);
	D3DXMatrixRotationY(&solar[9].m_matWR, GetTickCount() / 1024.0f);
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//그리는 순서가 아닌 행렬 적용 순서가 중요하다.
		for (int i = 0; i < 10; i++)
		{
			if (i == 0)DrawMesh(&solar[i].calcMatrix());
			else if(i == 9) DrawMesh(&solar[i].calcMatrix(solar[3].calcMatrix()));
			else DrawMesh(&solar[i].calcMatrix(solar[0].calcMatrix()));
		}

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**========================================================================
* 윈도우 프로시저
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
* WinMain
*=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc,
		0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Hierarchy", NULL };

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("D3D Hierarchy", "D3D Hierarchy", WS_OVERLAPPEDWINDOW,
		0, 0, 1000, 1000, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitGeometry()))
		{
			SetupCamera();

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
					Render();
				}
			}
		}
	}

	UnregisterClass("D3D Hierarchy", wc.hInstance);
	return 0;
}