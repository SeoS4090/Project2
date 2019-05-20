#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "Object.h"

/**
@file Camera.h
@brief 카메라 클래스 헤더 파일
*/


/**
@class Camera
@author 서승현
@date 2019/05/09
@brief 카메라 클래스
*/

/**
@brief 기울임 상태 
*/
enum tilts
{
	TILTS_NONE, ///< 기울임 안함
	TILTS_LEFT, ///< 왼쪽으로 기울임
	TILTS_RIGHT ///< 오른쪽으로 기울임
};


class Camera
{
private:
	static	Camera * pThis;
	D3DXVECTOR3		m_vEye;			///< 카메라의 현재 위치
	D3DXVECTOR3		m_vLookat;		///< 카메라의 시선 위치
	D3DXVECTOR3		m_vUp;			///< 카메라의 상방벡터

	D3DXVECTOR3		m_vView;		///< 카메라가 향하는 단위방향벡터
	D3DXVECTOR3		m_vCross;		///< 카마레의 측면벡터 cross( view, up )

	D3DXMATRIXA16	m_matView;		///< 카메라 행렬
	D3DXMATRIXA16	matProj;		///< 프로젝션 행렬

	int				m_btilts;		///< 기울임 활성화
	Object *		m_child;
public:
	static Camera * Getinstance();
	/**
	@fn D3DXMATRIXA16 getCamera()
	@return 카메라 행렬
	*/
	D3DXMATRIXA16 getCamera() { return m_matView; }
	/**
	@fn D3DXMATRIXA16 getProjection()
	@return 프로젝션 행렬
	*/
	D3DXMATRIXA16 getProjection() { return matProj; }

	/**
	@fn virtual void setPos(float _x, float _y, float _z)
	@brief 카메라 위치 설정
	@param x X 축 위치
	@param y Y 축 위치
	@param z Z 축 위치
	*/
	virtual void setPos(float _x, float _y, float _z);

	/**
	@fn virtual void Update(float fEiplse);
	@param fEiplse 함수가 다시 호출되기까지의 시간
	@brief
	키보드 방향키에 따라 회전
	마우스 이동에 따라 회전
	*/
	virtual void Update(float fEiplse);

	void addObject(Object* child);

	Camera();
	~Camera();
};

