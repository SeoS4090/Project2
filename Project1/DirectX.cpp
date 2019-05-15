#include "DirectX.h"
#include "Object.h"
#include "Plane.h"
#include "Camera.h"
#include "InputManager.h"
#include "Frustum.h"
#include <math.h>
DirectX * DirectX::pThis = NULL;

DirectX * DirectX::GetInstance()
{
	if (pThis == NULL)
		pThis = new DirectX();
	return pThis;
}

DirectX::DirectX()
{

}

HRESULT DirectX::initDirectX(HWND _hWnd)
{
	hWnd = _hWnd;
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}
	isFullscreen = FALSE;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice)))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	object = new Object(&m_pDevice);
	object->Init();
	object->setPosition(0.0f, 10.0f, 30.0f);
	object->setScale(10.0f, 10.0f, 10.0f);

	//Camera::Getinstance()->addObject(object);

	plane = new Plane(&m_pDevice, "map128.bmp", "tile2.tga");
	plane->Init();

	moveMode = 0;
	
	isHideFrustum = TRUE;
	isLockFrustum = FALSE;

	InputManager::Getinstance()->RegistKeyCode(VK_F1);
	return S_OK;
}


void DirectX::Render()
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(125, 125, 125), 1.0f, 0);
	SetupLights();
	SetupMatrices();
	if (!isLockFrustum) Frustum::Getinstance()->Make(&D3DXMATRIXA16(camera->Getinstance()->getCamera() * camera->Getinstance()->getProjection()));

	D3DXVECTOR3 vXLine[2];
	D3DXVECTOR3 vYLine[2];
	D3DXVECTOR3 vZLine[2];

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		
		object->DrawMesh();
		plane->DrawPlane();
		if (!isHideFrustum) Frustum::Getinstance()->Draw(m_pDevice);


		m_pDevice->EndScene();
	}
	

	if (m_pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
		RestoreDevice();
}

void DirectX::ChangeWindow()
{
	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.BackBufferWidth = 1920;
		d3dpp.BackBufferHeight = 1080;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.hDeviceWindow = hWnd;
	}
	else
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		SetWindowPos(hWnd, NULL, 0, 0, 600, 600, WS_EX_OVERLAPPEDWINDOW);
	}

	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
}

DirectX::~DirectX()
{

	if (m_pDevice != NULL)
		m_pDevice->Release();

	if (m_pD3D != NULL)
		m_pD3D->Release();
}

HRESULT DirectX::RestoreDevice()
{
	HRESULT Result = m_pDevice->TestCooperativeLevel();

	while (Result == D3DERR_DEVICELOST)
	{
		while (Result != D3DERR_DEVICENOTRESET)
		{
			MSG msg;
			Sleep(1000); //1�ʰ� ���
						 //1�ʰ� �������� ���� ���� �޽��� ���� ��ƾ�� �ִ´�.
			PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Result = m_pDevice->TestCooperativeLevel(); // ���¸� �ٽ� �����Ѵ�.
		}
		if (FAILED(m_pDevice->Reset(&d3dpp))) Result = D3DERR_DEVICELOST;
		//Reset�ϰ� ���� �߻��� ���� �ڵ带 �����Ѵ�.
	}
	return S_OK;
}



void DirectX::Update()
{
	if (GetTickCount() - fEilpse <= 0.1f)
		return;


	switch (moveMode)
	{
	case OBJECT:
		object->update(GetTickCount() - fEilpse);
		break;
	case CAMERA:
		camera->Getinstance()->Update(GetTickCount() - fEilpse);
		break;
	}
	
	if (InputManager::Getinstance()->isKeyDown(VK_F1))
	{
		isLockFrustum = !isLockFrustum;
		isHideFrustum = !isLockFrustum;
	}

	if (InputManager::Getinstance()->isKeyDown(VK_F2))
		plane->ChangeDrawMode();
	if (InputManager::Getinstance()->isKeyDown(VK_F3))
	{
		moveMode++;
		if (moveMode == MOVEMODE::END)
			moveMode = 0;
	}


}

void DirectX::SetupMatrices()
{
	//���� ��ǥ ����
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld); // ������ ȸ�� ����� ���� ��ķ� ����̽��� �����Ѵ�.

	//ī�޶� ����
	m_pDevice->SetTransform(D3DTS_VIEW, &camera->Getinstance()->getCamera()); // ������ �� ����� ����̽��� �����Ѵ�.
	m_pDevice->SetTransform(D3DTS_PROJECTION, &camera->Getinstance()->getProjection()); // ������ �������� ����� ����̽��� ����.
}

void DirectX::SetupLights()
{
	//���� ����
	//������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = 1.0f; mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = 1.0f; mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = 1.0f; mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = 1.0f; mtrl.Ambient.a = 1.0f;
	m_pDevice->SetMaterial(&mtrl);

	//���� ����
	D3DXVECTOR3 vecDir;
	//���⼺ ����(directional light)�� ���� ���� ����
	//���� ����ü
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	//������ Ȯ�걤 ������ ��⸦ �����Ѵ�.
	//������ ������ �����Ѵ�(����Ʈ ����Ʈ, ���̷��� ����Ʈ, ����Ʈ ����Ʈ)
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	//������ ���� ����
	vecDir = D3DXVECTOR3(0.0f, -1.0f,0.0f);

	//������ ������ ���� ���ͷ� �����.
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 1000.0f; // ������ �ٴٸ� �� �ִ� �ִ�Ÿ�
	m_pDevice->SetLight(0, &light); // ����̽��� ���� 0���� ��ġ
	m_pDevice->LightEnable(0, TRUE); // ���� 0���� Ȱ��ȭ �Ѵ�.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);// ���� ������ Ȱ��ȭ �Ѵ�.

													   //ȯ�� ������ �� ����
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

