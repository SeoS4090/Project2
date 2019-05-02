#ifndef _DEFINES_H
#define _DEFINES_H
#include <d3d9.h>
#include <d3dx9.h>


// ����� ���� ����ü�� ���� ������ ��Ÿ���� FVF��
#define D3DFVF_CUSTOMEVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_DIFFUSE | D3DFVF_TEX1)	
#define D3DFVF_PLANEVERTEX	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
struct CUSTOMEVERTEX
{
	D3DXVECTOR3		position;	//������ 3���� ��ǥ
	D3DXVECTOR3		nomal;
	D3DCOLOR		Color;		//������ ���� ����
	float			tu, tv;		//�ؽ��� ��ǥ (x ,y)
};


struct CUSTOMINDEX
{
	WORD _0, _1, _2;	
};

#endif // !_DEFINES_H
