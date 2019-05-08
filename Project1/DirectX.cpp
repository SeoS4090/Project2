#include "DirectX.h"
#include "Object.h"
#include "Plane.h"
#include "Camera.h"
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
	object->setScale(100.0f, 100.0f, 100.0f);

	plane = new Plane(&m_pDevice, "map128.bmp", "tile2.tga");
	plane->Init();


	camera = new Camera();
	camera->setcameraMode(true);
	
	return S_OK;
}


void DirectX::Render()
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(125, 125, 125), 1.0f, 0);
	SetupLights();
	SetupMatrices();
	
	D3DXVECTOR3 vXLine[2];
	D3DXVECTOR3 vYLine[2];
	D3DXVECTOR3 vZLine[2];

	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		
		object->DrawMesh();
		plane->DrawPlane();
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
			Sleep(1000); //1초간 대기
						 //1초간 정지했을 때를 위해 메시지 펌프 루틴을 넣는다.
			PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Result = m_pDevice->TestCooperativeLevel(); // 상태를 다시 조사한다.
		}
		if (FAILED(m_pDevice->Reset(&d3dpp))) Result = D3DERR_DEVICELOST;
		//Reset하고 에러 발생시 에러 코드를 설정한다.
	}
	return S_OK;
}


void DirectX::Clicked(LPARAM lParam)
{
	mouse_Pos = lParam;
}

void DirectX::Update()
{
	if (GetTickCount() - fEilpse <= 0.1f)
		return;
	object->update(GetTickCount() - fEilpse);
	//plane->Update();
	camera->Update(GetTickCount() - fEilpse);
	
}

void DirectX::SetupMatrices()
{
	//월드 행렬
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationX(&matWorld, GetTickCount() / 360.0f);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld); // 생성한 회전 행렬을 월드 행렬로 디바이스에 설정한다.


	//카메라 위치 잡기
														//뷰 행렬(카메라)을 정의하기 위해서는 3가지 값이 필요하다.

										  //뷰 행렬(카메라)
	
	m_pDevice->SetTransform(D3DTS_VIEW, &camera->getCamera()); // 생성한 뷰 행렬을 디바이스에 설정한다.

	//카메라 세부 설정
													// 프로젝션 행렬을 정의하기 위해서는 시야각(FOV = Field Of View)과 종횡비(aspect ratio), 클리핑 평면의 값이 필요하다.
	D3DXMATRIXA16 matProj;
	//첫 번째 : 설정될 행렬
	//두 번째 : 시야각
	//세 번째 : 종횡비
	//네 번째 : 근접 클리핑
	//다섯 번째 : 원거리 클리핑
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 0.5f, 300.0f);


	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj); // 생성한 프로젝션 행렬을 디바이스에 설정.
}

void DirectX::SetupLights()
{
	//재질 설정
	//재질은 디바이스에 단 하나만 설정될 수 있다.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = 1.0f; mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = 1.0f; mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = 1.0f; mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = 1.0f; mtrl.Ambient.a = 1.0f;
	m_pDevice->SetMaterial(&mtrl);

	//광원 설정
	D3DXVECTOR3 vecDir;
	//방향성 광원(directional light)이 향한 빛의 방향
	//광원 구조체
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	//광원의 확산광 색깔의 밝기를 지정한다.
	//광원의 종류를 설정한다(포인트 라이트, 다이렉션 라이트, 스포트 라이트)
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	//광원의 방향 설정
	vecDir = D3DXVECTOR3(0.0f, -1.0f,0.0f);

	//광원의 방향을 단위 벡터로 만든다.
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 1000.0f; // 광원이 다다를 수 있는 최대거리
	m_pDevice->SetLight(0, &light); // 디바이스에 광원 0번을 설치
	m_pDevice->LightEnable(0, TRUE); // 광원 0번을 활성화 한다.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);// 광원 설정을 활성화 한다.

													   //환경 광원의 값 설정
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

