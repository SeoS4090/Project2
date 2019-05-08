#include "Camera.h"
#include "Defines.h"
#include "InputManager.h"


void Camera::setPos(float _x, float _y, float _z)
{
	m_vEye = { _x, _y, _z };
	m_vUp = { 0.0f,1.0f,0.0f };
	m_vLookat  = m_vView = { 0.0f,0.0f,1.0f };
	m_vLookat += m_vEye;
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
}

void Camera::Update(float fEiplse)
{
	InputManager::Getinstance()->Update();

	if (InputManager::Getinstance()->isKeyPress(VK_W))			m_vEye += m_vView;
	if (InputManager::Getinstance()->isKeyPress(VK_S))			m_vEye -= m_vView;
	if (InputManager::Getinstance()->isKeyPress(VK_D))			m_vEye -= m_vCross;
	if (InputManager::Getinstance()->isKeyPress(VK_A))			m_vEye += m_vCross;
	if (InputManager::Getinstance()->isKeyPress(VK_SPACE))
	{
		if(InputManager::Getinstance()->isKeyPress(VK_CONTROL))
			m_vEye.y -= 1.0f;
		else
			m_vEye.y += 1.0f;
	}		
	if (InputManager::Getinstance()->isKeyPress(VK_DOWN))
	{
		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationAxis(&matRot, &m_vCross, -D3DX_PI / 180.0f);
		D3DXVECTOR3 newView;
		D3DXVec3TransformCoord(&newView, &m_vView, &matRot);
		D3DXVECTOR3 di(m_vView.x, 0.0f, m_vView.z);

		if (m_bCharacter && fabsf(acos(D3DXVec3Dot(&newView, &di)) * 180.0f / D3DX_PI) > PLAYER_LOOK_UPDOWN_DEGREE)return;


		D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
	}
	if (InputManager::Getinstance()->isKeyPress(VK_UP))
	{
		D3DXMATRIXA16 matRot;
		D3DXVECTOR3 newView;
		D3DXMatrixRotationAxis(&matRot, &m_vCross, D3DX_PI / 180.0f);
		D3DXVec3TransformCoord(&newView, &m_vView, &matRot);
		D3DXVECTOR3 di(m_vView.x, 0.0f, m_vView.z);


		if (m_bCharacter && fabsf(acos(D3DXVec3Dot(&newView, &di)) * 180.0f / D3DX_PI) > PLAYER_LOOK_UPDOWN_DEGREE)return;


		D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
	}
	if (InputManager::Getinstance()->isKeyPress(VK_RIGHT)) {

		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationY(&matRot, D3DX_PI / 180.0f);

		D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);

		D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
	}
	if (InputManager::Getinstance()->isKeyPress(VK_LEFT))
		{
			D3DXMATRIXA16 matRot;
			D3DXMatrixRotationY(&matRot, -D3DX_PI / 180.0f);
			D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
			D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);
			D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
		}
	if (InputManager::Getinstance()->isKeyDown(VK_Q) || InputManager::Getinstance()->isKeyUp(VK_E))
	{
		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationAxis(&matRot, &m_vView, PLAYER_LOOK_LEFTRIGHT_TITLS_DEGREE*-D3DX_PI / 180.0f);
		

		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
		D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
	}
	if (InputManager::Getinstance()->isKeyUp(VK_Q) || InputManager::Getinstance()->isKeyDown(VK_E))
	{
		D3DXMATRIXA16 matRot;
		D3DXMatrixRotationAxis(&matRot, &m_vView, PLAYER_LOOK_LEFTRIGHT_TITLS_DEGREE*D3DX_PI / 180.0f);
		
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
		D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
	}
	
	{
		D3DXMATRIXA16 matRot;
		
		POINT moved = InputManager::Getinstance()->GetMouseMoved();
		float Ytheta = moved.x * D3DX_PI / 180.0f / 10.0f;
		
		float Xtheta = moved.y * D3DX_PI / 180.0f / 10.0f;

		{
			D3DXMATRIXA16 matRot;
			D3DXMatrixRotationY(&matRot, Ytheta);

			D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);
			D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);

			D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
		}

		{

			D3DXMATRIXA16 matRot;
			D3DXVECTOR3 newView;
			D3DXMatrixRotationAxis(&matRot, &m_vCross, Xtheta);
			D3DXVec3TransformCoord(&newView, &m_vView, &matRot);
			D3DXVECTOR3 di(m_vView.x, 0.0f, m_vView.z);


			if (m_bCharacter && fabsf(acos(D3DXVec3Dot(&newView, &di)) * 180.0f / D3DX_PI) > PLAYER_LOOK_UPDOWN_DEGREE)return;


			D3DXVec3TransformCoord(&m_vView, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
			D3DXVec3TransformCoord(&m_vUp, &m_vUp, &matRot);
		}
	}
	D3DXVec3Normalize(&m_vView, &m_vView);
	D3DXVec3Cross(&m_vCross, &m_vView, &m_vUp);

	m_vLookat = m_vEye + m_vView;
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);

}

Camera::Camera()
{
	m_vEye		= D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	m_vLookat	= m_vView  = D3DXVECTOR3(0.0f, 0.0f, 20.0f);
	m_vUp		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vCross	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixIdentity(&m_matView);

	m_vLookat += m_vEye;
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	
	
	InputManager::Getinstance()->RegistKeyCode(VK_A);
	InputManager::Getinstance()->RegistKeyCode(VK_S);
	InputManager::Getinstance()->RegistKeyCode(VK_D);
	InputManager::Getinstance()->RegistKeyCode(VK_W);
	InputManager::Getinstance()->RegistKeyCode(VK_CONTROL);
	InputManager::Getinstance()->RegistKeyCode(VK_SPACE);
	InputManager::Getinstance()->RegistKeyCode(VK_UP);
	InputManager::Getinstance()->RegistKeyCode(VK_DOWN);
	InputManager::Getinstance()->RegistKeyCode(VK_RIGHT);
	InputManager::Getinstance()->RegistKeyCode(VK_LEFT);
	InputManager::Getinstance()->RegistKeyCode(VK_Q);
	InputManager::Getinstance()->RegistKeyCode(VK_E);
	

	m_bCharacter	= true;
	m_btilts		= TILTS_NONE;
	
}


Camera::~Camera()
{
}
