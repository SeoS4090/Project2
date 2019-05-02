#include "DirectX.h"
#include "Object.h"
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
	vEyePt = { 0.0f, 0.0f, -3.0f }; //눈의 위치(0.0f, 3.0f, -5.0f)
	vForword = { 0.0f, 0.0f, 1.0f }; // 눈이 바라보는 위치(0.0f, 0.0f, 0.0f)
	vUp = { 0.0f, 1.0f, 0.0f }; // 윗 방향을 나타내는 상방 벡터(0.0f, 1.0f, 0.0f)

	float r = sqrtf(pow(vForword.x, 2) + pow(vForword.y, 2) + pow(vForword.z, 2));

	Ztheta = acosf(vForword.z / r);

	if (vForword.x == 0) XYtheta = D3DX_PI / 2;
	else XYtheta = atanf(vForword.y/vForword.x);

	vForword = {r*sinf(Ztheta)*cosf(XYtheta),r*sinf(Ztheta)*sinf(XYtheta)  ,r*cosf(Ztheta)};

	D3DXVec3Cross(&vSide, &vForword, &vUp);//외적

	D3DXVec3Normalize(&vForword, &vForword); //정규화 크기를 1로 만들어줌
	D3DXVec3Normalize(&vSide, &vSide); //정규화 크기를 1로 만들어줌
	D3DXVec3Normalize(&vUp, &vUp); //정규화 크기를 1로 만들어줌



	
	




	distance = 1.0f;
	fEilpse = GetTickCount();
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

void DirectX::EyeMoved(LPARAM lParam)
{
	float moved_pos_y = HIWORD(lParam);
	float moved_pos_x = LOWORD(lParam);

	float origin_pos_y = HIWORD(mouse_Pos);
	float origin_pos_x = LOWORD(mouse_Pos);

	D3DXVECTOR3 move;
	D3DXVECTOR3 forword = vForword - vEyePt;
	D3DXVECTOR3 up = vUp + vEyePt;

	if (abs((int)(origin_pos_x - moved_pos_x)) > abs((int)(origin_pos_y - moved_pos_y)))
	{
		move.x = forword.y * up.z - forword.z * up.y;
		move.y = forword.z * up.x - forword.x * up.z;
		move.z = forword.x * up.y - forword.y * up.x;

		move /= (500 / (origin_pos_x - moved_pos_x));

		vEyePt -= move;
		vForword -= move;
	}

	else
	{
		float direct = 0.1f;
		
		if(origin_pos_y - moved_pos_y > 0)
			direct *= -1.0f;
		

		vEyePt.y += direct;
		vForword.y += direct;
		vUp.y += vEyePt.y + 1;

	}

	mouse_Pos = lParam;
}

void DirectX::RotateMoved(LPARAM lParam)
{
	float moved_pos_y = HIWORD(lParam);
	float moved_pos_x = LOWORD(lParam);

	float origin_pos_y = HIWORD(mouse_Pos);
	float origin_pos_x = LOWORD(mouse_Pos);

	mouse_Pos = lParam;
}

void DirectX::Wheel(int scroll)
{
	D3DXVECTOR3 forword = vForword - vEyePt;
	
	
	vEyePt += forword * scroll / 2.0f;
	vForword += forword * scroll / 2.0f;
}

void DirectX::Update()
{
	if (GetTickCount() - fEilpse <= 0.1f)
		return;
	object->update(GetTickCount() - fEilpse);
	
	////vLookatPt = { 0.0f, 0.0f, 0.0f };
	//if (GetKeyState(0x57) & 0x8000) //w
	//{
	//	vEyePt.z += 0.1f;
	//}
	//if (GetKeyState(0x53) & 0x8000) //s
	//	vEyePt.z -= 0.1f;

	//if (GetKeyState(0x41) & 0x8000) //a
	//{
	//	vEyePt.x += 0.1f;
	//}
	//if (GetKeyState(0x44) & 0x8000) //d
	//{
	//	vEyePt.x -= 0.1f;
	//}

	//if (GetKeyState(VK_SPACE) & 0x8000) //d
	//{
	//	if (GetKeyState(VK_CONTROL) & 0x8000) //d
	//		vEyePt.y -= 0.1f;
	//	else
	//		vEyePt.y += 0.1f;
	//}


	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		vEyePt = { 0.0f, 0.0f, 0.0f }; //눈의 위치(0.0f, 3.0f, -5.0f)
		vForword = { 0.0f,0.0f,1.0f };
	}
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
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vForword, &vUp);// 3가지 Point를 이용해 뷰 행렬을 생성한다.

	m_pDevice->SetTransform(D3DTS_VIEW, &matView); // 생성한 뷰 행렬을 디바이스에 설정한다.

	//카메라 세부 설정
													  // 프로젝션 행렬을 정의하기 위해서는 시야각(FOV = Field Of View)과 종횡비(aspect ratio), 클리핑 평면의 값이 필요하다.
	D3DXMATRIXA16 matProj;
	//첫 번째 : 설정될 행렬
	//두 번째 : 시야각
	//세 번째 : 종횡비
	//네 번째 : 근접 클리핑
	//다섯 번째 : 원거리 클리핑
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 0.5f, 100.0f);


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
	vecDir = D3DXVECTOR3(0, -1.0f, 0);

	//광원의 방향을 단위 벡터로 만든다.
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 1000.0f; // 광원이 다다를 수 있는 최대거리
	m_pDevice->SetLight(0, &light); // 디바이스에 광원 0번을 설치
	m_pDevice->LightEnable(0, TRUE); // 광원 0번을 활성화 한다.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);// 광원 설정을 활성화 한다.

													   //환경 광원의 값 설정
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

