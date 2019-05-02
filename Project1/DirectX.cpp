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
	vEyePt = { 0.0f, 0.0f, -3.0f }; //���� ��ġ(0.0f, 3.0f, -5.0f)
	vForword = { 0.0f, 0.0f, 1.0f }; // ���� �ٶ󺸴� ��ġ(0.0f, 0.0f, 0.0f)
	vUp = { 0.0f, 1.0f, 0.0f }; // �� ������ ��Ÿ���� ��� ����(0.0f, 1.0f, 0.0f)

	float r = sqrtf(pow(vForword.x, 2) + pow(vForword.y, 2) + pow(vForword.z, 2));

	Ztheta = acosf(vForword.z / r);

	if (vForword.x == 0) XYtheta = D3DX_PI / 2;
	else XYtheta = atanf(vForword.y/vForword.x);

	vForword = {r*sinf(Ztheta)*cosf(XYtheta),r*sinf(Ztheta)*sinf(XYtheta)  ,r*cosf(Ztheta)};

	D3DXVec3Cross(&vSide, &vForword, &vUp);//����

	D3DXVec3Normalize(&vForword, &vForword); //����ȭ ũ�⸦ 1�� �������
	D3DXVec3Normalize(&vSide, &vSide); //����ȭ ũ�⸦ 1�� �������
	D3DXVec3Normalize(&vUp, &vUp); //����ȭ ũ�⸦ 1�� �������



	
	




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
		vEyePt = { 0.0f, 0.0f, 0.0f }; //���� ��ġ(0.0f, 3.0f, -5.0f)
		vForword = { 0.0f,0.0f,1.0f };
	}
}

void DirectX::SetupMatrices()
{
	//���� ���
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationX(&matWorld, GetTickCount() / 360.0f);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld); // ������ ȸ�� ����� ���� ��ķ� ����̽��� �����Ѵ�.


	//ī�޶� ��ġ ���
														//�� ���(ī�޶�)�� �����ϱ� ���ؼ��� 3���� ���� �ʿ��ϴ�.

										  //�� ���(ī�޶�)
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vForword, &vUp);// 3���� Point�� �̿��� �� ����� �����Ѵ�.

	m_pDevice->SetTransform(D3DTS_VIEW, &matView); // ������ �� ����� ����̽��� �����Ѵ�.

	//ī�޶� ���� ����
													  // �������� ����� �����ϱ� ���ؼ��� �þ߰�(FOV = Field Of View)�� ��Ⱦ��(aspect ratio), Ŭ���� ����� ���� �ʿ��ϴ�.
	D3DXMATRIXA16 matProj;
	//ù ��° : ������ ���
	//�� ��° : �þ߰�
	//�� ��° : ��Ⱦ��
	//�� ��° : ���� Ŭ����
	//�ټ� ��° : ���Ÿ� Ŭ����
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 0.5f, 100.0f);


	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj); // ������ �������� ����� ����̽��� ����.
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
	vecDir = D3DXVECTOR3(0, -1.0f, 0);

	//������ ������ ���� ���ͷ� �����.
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 1000.0f; // ������ �ٴٸ� �� �ִ� �ִ�Ÿ�
	m_pDevice->SetLight(0, &light); // ����̽��� ���� 0���� ��ġ
	m_pDevice->LightEnable(0, TRUE); // ���� 0���� Ȱ��ȭ �Ѵ�.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);// ���� ������ Ȱ��ȭ �Ѵ�.

													   //ȯ�� ������ �� ����
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

