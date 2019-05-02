#ifndef _DIRECT_X_H
#define _DIRECT_X_H
#include "Defines.h"

class Object;
class DirectX
{
private:
	static DirectX *		pThis;
	HWND					hWnd;

	//Ä«¸Þ¶ó º¯¼ö
	D3DXVECTOR3				vEyePt;
	D3DXVECTOR3				vForword;
	D3DXVECTOR3				vUp;
	D3DXVECTOR3				vSide;

	float					Ztheta; //¥È
	float					XYtheta; // ¥Õ
	float					distance;




	LPDIRECT3DTEXTURE9		g_pTexHeight; // Texture ³ôÀÌ¸Ê
	LPDIRECT3DTEXTURE9		g_pTexDiffuse; // Texture »ö±ò¸Ê

	Object *				object;

	float					fEilpse;
protected:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DPRESENT_PARAMETERS	d3dpp;

	D3DXMATRIXA16			m_matWorld;
	bool					isFullscreen;
	LPARAM					mouse_Pos;

public:

	static DirectX *	GetInstance();
						DirectX();
						~DirectX();

			HRESULT		initDirectX(HWND hWnd);
			HRESULT		RestoreDevice();


			void		Clicked(LPARAM lParam);
			void		EyeMoved(LPARAM lParam);
			void		RotateMoved(LPARAM lParam);
			void		Wheel(int scroll);
			void		Update();
			void		SetupMatrices();
			void		SetupLights();
			void		Render();
			void		ChangeWindow();

};


#endif