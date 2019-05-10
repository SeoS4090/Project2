#ifndef _ZFRUSTUM_H_
#define _ZFRUSTUM_H_

#include <d3d9.h>
#include <d3dx9.h>


#define PLANE_EPSILON	5.0f ///< �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��

/*
@brief�������� �ø��� �ϱ����� Ŭ����
*/
class Frustum
{
	static Frustum* pThis;
	D3DXVECTOR3	m_vtx[8];	///< ���������� ������ ���� 8��
	D3DXVECTOR3	m_vPos;		///< ���� ī�޶��� ������ǥ
	D3DXPLANE	m_plane[6];	///< ���������� �����ϴ� 6���� ���

public:
	
	Frustum();
	~Frustum();
	static Frustum* Getinstance();
	/**
	@brief ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	@detail ����ü�� �����.
	*/
	BOOL	Make(D3DXMATRIXA16* pmatViewProj);

	/**
	@brief 3D���Ͱ� �������� �ȿ� �ִ��� �˻�
	@param v �������� �ȿ� �ִ��� �˻��� �� ��
	@return �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	*/
	BOOL	IsIn(D3DXVECTOR3* pv);

	/** 
	@brief ���� �������ҿ� ��ġ���� �ƴ��� Ȯ��
	@param pv ���� ��ǥ
	@param radius ���� ������
	@return �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	*/
	BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);

	/** 
	@brief ���������� ȭ�鿡 �׷��ش�.
	@retun Draw ���н� FALSE ������ TRUE
	*/
	BOOL	Draw(LPDIRECT3DDEVICE9 pDev);

	/** 
	@brief ���� ī�޶��� ������ǥ�� ����ش�.
	@return ī�޶� ��ǥ
	*/
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
};

#endif // _ZFRUSTUM_H_