#ifndef _DIRECT_X_H
#define _DIRECT_X_H
#include "Defines.h"

class Object;
class Plane;
class Camera;

enum MOVEMODE
{
	CAMERA,
	OBJECT,
	END
};

/**
@class DirectX
@brief 다이렉트X 환경 설정
@author 서승현
@date 2019/05/09

*/
class DirectX
{
private:
	static DirectX *		pThis;			///< 싱글턴 포인터
	HWND					hWnd;			///< 윈도우 핸들

	LPDIRECT3DTEXTURE9		g_pTexHeight;	///< Texture 높이맵
	LPDIRECT3DTEXTURE9		g_pTexDiffuse;	///< Texture 색깔맵


	Object	*				object;			///< 객체 오브젝트
	Plane	*				plane;			///< Terrain
	Camera	*				camera;			///< Main Camera

	float					fEilpse;

	bool					isHideFrustum;	///<프러스텀 숨기기
	bool					isLockFrustum;	///<프러스텀 고정

	int						moveMode;
protected:
	LPDIRECT3D9				m_pD3D;			///< directX 포인터
	LPDIRECT3DDEVICE9		m_pDevice;		///< directX 디바이스
	D3DPRESENT_PARAMETERS	d3dpp;			///< directX 설정 환경 변수

	D3DXMATRIXA16			m_matWorld;		///< 월드 좌표
	bool					isFullscreen;	///< DirectX 를 전체화면으로 할 것인가

public:

	/// DirectX 싱글턴 함수
	static DirectX *	GetInstance();		
						DirectX();
						~DirectX();

						/**
						@fn HRESULT		initDirectX(HWND hWnd)
						@return 성공 여부
						@brief DirectX 초기화
						*/
			HRESULT		initDirectX(HWND hWnd); 


						/**
						@brief 기기 찾기
						@return 기기 찾기 성공 여부
						*/
			HRESULT		RestoreDevice();


						/**
						@brief DirectX 업데이트
						*/
			void		Update();

						/**

						*/
			void		SetupMatrices();

						/**
						@brief DirectX 조명 설정
						*/
			void		SetupLights();

						/**
						@brief 화면 그리기
						*/
			void		Render();
			void		ChangeWindow();

};


#endif