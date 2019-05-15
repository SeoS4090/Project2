#include "Camera.h"
#include "Defines.h"
#include "InputManager.h"
Camera * Camera::pThis = NULL;

Camera * Camera::Getinstance()
{
	if (pThis == NULL)
		pThis = new Camera();
	return pThis;
}

void Camera::setPos(float _x, float _y, float _z)
{
	m_vEye = { _x, _y, _z };
	m_vLookat  = m_vView = {0.0f,0.0f,20.0f};
	m_vUp = { 0.0f,1.0f,0.0f };
	D3DXVec3Normalize(&m_vView, &m_vView);
	
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
}

void Camera::Update(float fEiplse)
{
	
	/*///< 카메라 이동
	if (InputManager::Getinstance()->isKeyPress(VK_W))			m_vEye += {m_vView.x, 0, m_vView.x};
	if (InputManager::Getinstance()->isKeyPress(VK_S))			m_vEye -= {m_vView.x, 0, m_vView.x};
	if (InputManager::Getinstance()->isKeyPress(VK_D))			m_vEye -= m_vCross;
	if (InputManager::Getinstance()->isKeyPress(VK_A))			m_vEye += m_vCross;
	*/
	if (InputManager::Getinstance()->isKeyPress(VK_SPACE))
	{
		if (InputManager::Getinstance()->isKeyPress(VK_CONTROL))
			m_vEye.y -= 1.0f;
		else
			m_vEye.y += 1.0f;
	}
	


	//기울임
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
	


	D3DXVec3Normalize(&m_vView, &m_vView);
	D3DXVec3Cross(&m_vCross, &m_vView, &m_vUp);

	m_vLookat = m_vEye + m_vView;
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);

	if (m_child != NULL)
	{
		m_vEye = { 0.0f, 10.0f, -30.0f };
		m_vEye -= m_child->getFront();
		D3DXVec3TransformCoord(&m_vEye, &m_vEye, &m_child->getMat());
		
		m_vView = m_vLookat = m_child->getFront()*100 + m_child->getPos();
		D3DXVec3Normalize(&m_vView, &m_vView);

		D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	}
	
}


void Camera::addObject(Object * child)
{
	m_child = child;
	
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
	D3DXVec3Normalize(&m_vView, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	

	//프로젝션 행렬 설정
	D3DXMatrixPerspectiveFovLH(&matProj, 105 / 2 * D3DX_PI / 180.0f, 1.0f, 0.5f, 300.0f);
	
	m_btilts		= TILTS_NONE;
	m_child = NULL;
}


Camera::~Camera()
{
}
