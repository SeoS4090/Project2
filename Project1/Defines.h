#ifndef _DEFINES_H
#define _DEFINES_H
#include <d3d9.h>
#include <d3dx9.h>


// 사용자 정점 구조체에 관한 정보를 나타내는 FVF값
#define D3DFVF_CUSTOMEVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_DIFFUSE | D3DFVF_TEX1)	
#define D3DFVF_PLANEVERTEX	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#define PLAYER_LOOK_UPDOWN_DEGREE 89
#define PLAYER_LOOK_LEFTRIGHT_TITLS_DEGREE 30


//KEY CODE
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Q 0x51
#define VK_E 0x45


struct CUSTOMEVERTEX
{
	D3DXVECTOR3		position;	//정점의 3차원 좌표
	D3DXVECTOR3		nomal;
	D3DCOLOR		Color;		//정점의 법선 벡터
	float			tu, tv;		//텍스쳐 좌표 (x ,y)
};


struct CUSTOMINDEX
{
	WORD _0, _1, _2;	
};

#endif // !_DEFINES_H
