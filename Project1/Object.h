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


	D3DXVECTOR3				Center;
	D3DXVECTOR3				Side;
	D3DXVECTOR3				Up;
	D3DXVECTOR3				Front;


	D3DXVECTOR3				m_Scale;
	float					m_theta;
	float					m_Xtheta;


	virtual HRESULT			initVB();
	virtual HRESULT			InitIB();
public:
	virtual HRESULT			Init();
	virtual HRESULT			InitTexture(const char * filename);


	LPDIRECT3DVERTEXBUFFER9 getVB() { return VB; }
	LPDIRECT3DINDEXBUFFER9	getIB() { return IB; }

	D3DXMATRIXA16			getMat() {
		D3DXMATRIXA16 rot, mov;
		D3DXMatrixRotationY(&rot, m_theta);
		D3DXMatrixTranslation(&mov, Center.x, Center.y, Center.z);
		return rot * mov;
	}
	float					getrot() { return m_theta; };
	D3DXMATRIXA16			getmovMat();
	D3DXVECTOR3				getPos() { return Center; };
	D3DXVECTOR3				getFront() { return Front; };

	virtual		void		update(float fEilpse);
	void					setPosition(float cx, float cy, float cz);
	void					setScale(float cx, float cy, float cz);
	void					DrawMesh(D3DXMATRIXA16* pMat = 0);

	Object(LPDIRECT3DDEVICE9* _deviece);
							~Object();
};

