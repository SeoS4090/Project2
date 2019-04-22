#pragma once
#include <d3d9.h>

class DirectX
{
protected:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	D3DPRESENT_PARAMETERS	d3dpp;
	
public:
				DirectX();
				HRESULT initDirectX(HWND hWnd);
				~DirectX();
	void		Render();
};

