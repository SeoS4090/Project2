#ifndef _ZFRUSTUM_H_
#define _ZFRUSTUM_H_

#include <d3d9.h>
#include <d3dx9.h>


#define PLANE_EPSILON	5.0f ///< 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값

/*
@brief프러스텀 컬링을 하기위한 클래스
*/
class Frustum
{
	static Frustum* pThis;
	D3DXVECTOR3	m_vtx[8];	///< 프러스텀을 구성할 정점 8개
	D3DXVECTOR3	m_vPos;		///< 현재 카메라의 월드좌표
	D3DXPLANE	m_plane[6];	///< 프러스텀을 구성하는 6개의 평면

public:
	
	Frustum();
	~Frustum();
	static Frustum* Getinstance();
	/**
	@brief 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
	@detail 절두체를 만든다.
	*/
	BOOL	Make(D3DXMATRIXA16* pmatViewProj);

	/**
	@brief 3D벡터가 프러스텀 안에 있는지 검사
	@param v 프러스텀 안에 있는지 검사할 한 점
	@return 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	*/
	BOOL	IsIn(D3DXVECTOR3* pv);

	/** 
	@brief 구가 프러스텀에 겹치는지 아닌지 확인
	@param pv 중점 좌표
	@param radius 구의 반지름
	@return 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	*/
	BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);

	/** 
	@brief 프러스텀을 화면에 그려준다.
	@retun Draw 실패시 FALSE 성공시 TRUE
	*/
	BOOL	Draw(LPDIRECT3DDEVICE9 pDev);

	/** 
	@brief 현재 카메라의 월드좌표를 얻어준다.
	@return 카메라 좌표
	*/
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
};

#endif // _ZFRUSTUM_H_