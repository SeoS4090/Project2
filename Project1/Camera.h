#pragma once
#include <d3dx9.h>
#include <d3d9.h>

enum tilts
{
	TILTS_NONE,
	TILTS_LEFT,
	TILTS_RIGHT
};

class Camera
{
private:
	D3DXVECTOR3		m_vEye;			// 카메라의 현재 위치
	D3DXVECTOR3		m_vLookat;		// 카메라의 시선 위치
	D3DXVECTOR3		m_vUp;			// 카메라의 상방벡터

	D3DXVECTOR3		m_vView;		// 카메라가 향하는 단위방향벡터
	D3DXVECTOR3		m_vCross;		// 카마레의 측면벡터 cross( view, up )

	D3DXMATRIXA16	m_matView;		// 카메라 행렬

	bool			m_bCharacter;
	int				m_btilts;
public:
	D3DXMATRIXA16 getCamera() { return m_matView; }
	const inline void setcameraMode(bool isCharacter)	{ m_bCharacter = isCharacter; }
	virtual void setPos(float _x, float _y, float _z);
	virtual void Update(float fEiplse);
	Camera();
	~Camera();
};

