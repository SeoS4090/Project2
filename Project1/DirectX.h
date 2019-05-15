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
@brief ���̷�ƮX ȯ�� ����
@author ������
@date 2019/05/09

*/
class DirectX
{
private:
	static DirectX *		pThis;			///< �̱��� ������
	HWND					hWnd;			///< ������ �ڵ�

	LPDIRECT3DTEXTURE9		g_pTexHeight;	///< Texture ���̸�
	LPDIRECT3DTEXTURE9		g_pTexDiffuse;	///< Texture �����


	Object	*				object;			///< ��ü ������Ʈ
	Plane	*				plane;			///< Terrain
	Camera	*				camera;			///< Main Camera

	float					fEilpse;

	bool					isHideFrustum;	///<�������� �����
	bool					isLockFrustum;	///<�������� ����

	int						moveMode;
protected:
	LPDIRECT3D9				m_pD3D;			///< directX ������
	LPDIRECT3DDEVICE9		m_pDevice;		///< directX ����̽�
	D3DPRESENT_PARAMETERS	d3dpp;			///< directX ���� ȯ�� ����

	D3DXMATRIXA16			m_matWorld;		///< ���� ��ǥ
	bool					isFullscreen;	///< DirectX �� ��üȭ������ �� ���ΰ�

public:

	/// DirectX �̱��� �Լ�
	static DirectX *	GetInstance();		
						DirectX();
						~DirectX();

						/**
						@fn HRESULT		initDirectX(HWND hWnd)
						@return ���� ����
						@brief DirectX �ʱ�ȭ
						*/
			HRESULT		initDirectX(HWND hWnd); 


						/**
						@brief ��� ã��
						@return ��� ã�� ���� ����
						*/
			HRESULT		RestoreDevice();


						/**
						@brief DirectX ������Ʈ
						*/
			void		Update();

						/**

						*/
			void		SetupMatrices();

						/**
						@brief DirectX ���� ����
						*/
			void		SetupLights();

						/**
						@brief ȭ�� �׸���
						*/
			void		Render();
			void		ChangeWindow();

};


#endif