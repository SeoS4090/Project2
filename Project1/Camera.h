#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Object.h"

/**
@file Camera.h
@brief ī�޶� Ŭ���� ��� ����
*/


/**
@class Camera
@author ������
@date 2019/05/09
@brief ī�޶� Ŭ����
*/

/**
@brief ����� ���� 
*/
enum tilts
{
	TILTS_NONE, ///< ����� ����
	TILTS_LEFT, ///< �������� �����
	TILTS_RIGHT ///< ���������� �����
};


class Camera
{
private:
	static	Camera * pThis;
	D3DXVECTOR3		m_vEye;			///< ī�޶��� ���� ��ġ
	D3DXVECTOR3		m_vLookat;		///< ī�޶��� �ü� ��ġ
	D3DXVECTOR3		m_vUp;			///< ī�޶��� ��溤��

	D3DXVECTOR3		m_vView;		///< ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		///< ī������ ���麤�� cross( view, up )

	D3DXMATRIXA16	m_matView;		///< ī�޶� ���
	D3DXMATRIXA16	matProj;		///< �������� ���

	int				m_btilts;		///< ����� Ȱ��ȭ
	Object *		m_child;
public:
	static Camera * Getinstance();
	/**
	@fn D3DXMATRIXA16 getCamera()
	@return ī�޶� ���
	*/
	D3DXMATRIXA16 getCamera() { return m_matView; }
	/**
	@fn D3DXMATRIXA16 getProjection()
	@return �������� ���
	*/
	D3DXMATRIXA16 getProjection() { return matProj; }

	/**
	@fn virtual void setPos(float _x, float _y, float _z)
	@brief ī�޶� ��ġ ����
	@param x X �� ��ġ
	@param y Y �� ��ġ
	@param z Z �� ��ġ
	*/
	virtual void setPos(float _x, float _y, float _z);

	/**
	@fn virtual void Update(float fEiplse);
	@param fEiplse �Լ��� �ٽ� ȣ��Ǳ������ �ð�
	@brief
	Ű���� ����Ű�� ���� ȸ��
	���콺 �̵��� ���� ȸ��
	*/
	virtual void Update(float fEiplse);

	void addObject(Object* child);

	Camera();
	~Camera();
};

