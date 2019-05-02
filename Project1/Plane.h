#ifndef _PLANE_H_
#define _PLANE_H_

#include <d3d9.h>
#include <d3dx9.h>


class Plane
{
private:
	LPDIRECT3DTEXTURE9		Height; // Texture ³ôÀÌ¸Ê
	LPDIRECT3DTEXTURE9		Diffuse; // Texture »ö±ò¸Ê
	DWORD					g_cxHeight = 0;
	DWORD					g_czHeight = 0;
public:
	Plane(const char * Height);
	~Plane();
};


#endif