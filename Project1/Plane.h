#ifndef _PLANE_H_
#define _PLANE_H_

#include <d3d9.h>
#include <d3dx9.h>


class Plane
{
private:


	LPDIRECT3DDEVICE9       device;
	D3DXMATRIXA16			m_Mat;
	LPDIRECT3DVERTEXBUFFER9 VB;
	LPDIRECT3DINDEXBUFFER9	IB;
	LPDIRECT3DTEXTURE9		Texture;



	LPDIRECT3DTEXTURE9		Height; // Texture ³ôÀÌ¸Ê
	LPDIRECT3DTEXTURE9		Diffuse; // Texture »ö±ò¸Ê
	DWORD					cxHeight = 0;
	DWORD					czHeight = 0;


	HRESULT InitVB();
	HRESULT InitIB();

	BOOL					isWireFrame;
public:
	Plane(LPDIRECT3DDEVICE9 * device , const char * Height , const char * texture);

	HRESULT Init();
	void DrawPlane(D3DXMATRIXA16* pMat = 0);
	void ChangeDrawMode() { isWireFrame = !isWireFrame; }
	void Update();
	~Plane();
};


#endif