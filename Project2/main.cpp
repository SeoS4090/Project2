/**========================================================================
* ����̽� ����
* D3D ����̽��� �����ϰ�, �̸� ����ؼ� ȭ���� ����� ����� ���ؼ� �����ϰ� �ȴ�.
*=========================================================================*/

// Direct3D9�� ����ϱ� ���� ���
#include <d3d9.h>
#include <d3dx9.h>
/**========================================================================
* ���� ����
*=========================================================================*/

LPDIRECT3D9			g_pD3D = NULL;			// D3D ����̽��� ������ D3D ��ü ����
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;	// �������� ���� D3D ����̽�
IDirect3DVertexBuffer9 * g_pVB = NULL;
D3DDISPLAYMODE		displaymode;
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; //������ ��ȯ�� ��ǥ(rhw ���� ������ ��ȯ�� �Ϸ�� �����̴�.)
	DWORD color; // ������ ����
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

											/**========================================================================
											* Direct3D �ʱ�ȭ
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
	
	// ���� ���۸� �����Ѵ�.
	// ���� ����ü 3���� ������ �޸𸮸� �Ҵ��Ѵ�.
	// FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(vertices),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}
	//������ ���۸� ������ ä���.
	//���� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���´�.
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
	// ����̽��� �����ϱ� ���� D3D ��ü ����
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS	d3dpp;				// ����̽� ������ ���� ����ü
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �ݵ�� ZeroMemory() �Լ��� �̸� ����ü�� ������ ������ �Ѵ�.

	d3dpp.Windowed = TRUE;						// â ���� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ���� ȿ������ SWAP ȿ��
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// ���� ����ȭ�� ��忡 ���缭 �ĸ� ���۸� ����.
	d3dpp.BackBufferHeight = displaymode.Height;
	d3dpp.BackBufferWidth = displaymode.Width;
	

												// ����̽��� �����ؼ� ����
												// ����Ʈ ����ī�带 ����ϰ�, HAL ����̽��� �����Ѵ�.
												// ���� ó���� ��� ī�忡�� �����ϴ� SWó���� �����Ѵ�.
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// ����̽� ���� ������ ó���� ��� ���⿡�� �Ѵ�.
	if (SUCCEEDED(InitVB()))
		return S_OK;
	else
		return E_FAIL;
}

/**========================================================================
* �ʱ�ȭ ��ü�� Release
*=========================================================================*/
void Cleanup()
{
	//���� ������ �߿� �������̽� ������ �������� ��������.

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if(g_pVB != NULL)
		g_pVB->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();


}

/**========================================================================
* ȭ�� �׸���
*=========================================================================*/
void Render()
{
	if (NULL == g_pd3dDevice)
		return;

	// �ĸ� ���۸� �Ķ���(0, 0, 255)���� �����.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//���� ������ ��ɵ��� ������ ��
		// ���� ���۷� �ﰢ���� �׸���.
		// ���� ������ ��� �ִ� ���� ���۸� ��� ��Ʈ������ �Ҵ��Ѵ�.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		// D3D���� ���� ������ ������ �����Ѵ�. ��κ��� ��� FVF�� �����Ѵ�.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		// ���� ������ ����ϱ� ���� DrawPrimitive() �Լ� ȣ��
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 6);

		//������ ����
		g_pd3dDevice->EndScene();
	}

	// �ĸ� ���۸� ���̴� ȭ������ ��ȯ.
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
* Window ����
*=========================================================================*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	//������ Ŭ���� ���
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Init", NULL };

	//winclass �������Ϳ� ���
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

	//��ϵ� ������Ʈ winclass ������.
	UnregisterClass("D3D Init", wc.hInstance);
	return 0;
}
