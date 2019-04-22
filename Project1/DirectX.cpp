#include "DirectX.h"



DirectX::DirectX()
{
}

HRESULT DirectX::initDirectX(HWND hWnd)
{

	//D3Dinit
	{
		if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		{
			return E_FAIL;
		}

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.BackBufferCount = 1;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	

		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pd3dDevice)))
		{
			return E_FAIL;
		}

		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
		
	return S_OK;
}

void DirectX::Render()
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
DirectX::~DirectX()
{
	if (m_pd3dDevice != NULL)
		m_pd3dDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();
}
