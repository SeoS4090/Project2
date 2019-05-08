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
	D3DXVECTOR3		m_vEye;			// ī�޶��� ���� ��ġ
	D3DXVECTOR3		m_vLookat;		// ī�޶��� �ü� ��ġ
	D3DXVECTOR3		m_vUp;			// ī�޶��� ��溤��

	D3DXVECTOR3		m_vView;		// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		// ī������ ���麤�� cross( view, up )

	D3DXMATRIXA16	m_matView;		// ī�޶� ���

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

