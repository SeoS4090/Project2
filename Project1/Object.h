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



	virtual HRESULT			initVB();
	virtual HRESULT			InitIB();
public:
	virtual HRESULT			Init();
	virtual HRESULT			InitTexture(const char * filename);


	LPDIRECT3DVERTEXBUFFER9 getVB() { return VB; }
	LPDIRECT3DINDEXBUFFER9	getIB() { return IB; }

	virtual		void		update(float fEilpse);
	void					setPosition(float cx, float cy, float cz);
	void					setScale(float cx, float cy, float cz);
	void					DrawMesh(D3DXMATRIXA16* pMat = 0);
							Object(LPDIRECT3DDEVICE9* _deviece);
							~Object();
};

