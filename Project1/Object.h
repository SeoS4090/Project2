#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Object
{
private:
	LPDIRECT3DDEVICE9		*device;
	D3DXMATRIXA16			m_Mat;
	LPDIRECT3DVERTEXBUFFER9 VB;
	LPDIRECT3DINDEXBUFFER9	IB;
	LPDIRECT3DTEXTURE9		Texture;
public:
	virtual HRESULT			Init();
	virtual HRESULT			initVB();
	virtual HRESULT			InitIB();
	virtual HRESULT			InitTexture(const char * filename);


	LPDIRECT3DVERTEXBUFFER9 getVB() { return VB; }
	LPDIRECT3DINDEXBUFFER9	getIB() { return IB; }

	virtual		void		update(float fEilpse);
	void					DrawMesh(D3DXMATRIXA16* pMat = 0);
							Object(LPDIRECT3DDEVICE9* _deviece);
							~Object();
};

