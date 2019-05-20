#include "Object.h"
#include "Defines.h"
#include "Camera.h"
#include "InputManager.h"
#include "Frustum.h"


HRESULT Object::Init()
{
	if (SUCCEEDED(initVB()) && SUCCEEDED(InitIB()))
	{
		D3DXMatrixIdentity(&m_Mat);
		Center = { 0.0f,0.0f,0.0f };
		m_Scale = { 1.0f, 1.0f, 1.0f };
		Front = { 0.0f, 0.0f, 1.0f };
		Up = { 0.0f, 1.0f, 0.0f };
		Side = { 1.0f, 0.0f, 0.0f };

		return S_OK;
	}

	return E_FAIL;
}


/**========================================================================
* 정점 초기화 : 정점 버퍼를 생성하고 정점값을 채워 넣는다.
*=========================================================================*/
HRESULT Object::initVB()
{
	InitTexture("dahyun.jpg");
	// 상자(cube)를 랜더링하기 위해 8개의 정점 선언
	CUSTOMEVERTEX vertices[8];
	vertices[0].position = vertices[0].nomal = { -0.5f,0.5f,-0.5f };
	vertices[0].tu = 0.0f; vertices[0].tv = 0.0f;

	vertices[1].position = vertices[1].nomal = { 0.5f,0.5f,-0.5f };
	vertices[1].tu = 1.0f; vertices[1].tv = 0.0f;

	vertices[2].position = vertices[2].nomal = { 0.5f,0.5f,0.5f };
	vertices[2].tu = 0.0f; vertices[2].tv = 0.0f;

	vertices[3].position = vertices[3].nomal = { -0.5f,0.5f,0.5f };
	vertices[3].tu = 1.0f; vertices[3].tv = 0.0f;

	vertices[4].position = vertices[4].nomal = { -0.5f,-0.5f,-0.5f };
	vertices[4].tu = 0.0f; vertices[4].tv = 1.0f;

	vertices[5].position = vertices[5].nomal = { 0.5f,-0.5f,-0.5f };
	vertices[5].tu = 1.0f; vertices[5].tv = 1.0f;

	vertices[6].position = vertices[6].nomal = { 0.5f,-0.5f,0.5f };
	vertices[6].tu = 0.0f; vertices[6].tv = 1.0f;

	vertices[7].position = vertices[7].nomal = { -0.5f,-0.5f,0.5f };
	vertices[7].tu = 1.0f; vertices[7].tv = 1.0f;
	
	
	for (int i = 0; i < 8; i++)
	{
		vertices[i].Color = D3DCOLOR_ARGB(0, 125,125,125);
		D3DXVec3Normalize(&vertices[i].nomal, &vertices[i].nomal);
	}

	// 정점 버퍼를 생성한다.
	// 정점 구조체 3개를 저장할 메모리를 할당한다.
	// FVF를 지정하여 보관할 데이터의 형식을 지정한다.
	if (FAILED((*device)->CreateVertexBuffer(8 * sizeof(CUSTOMEVERTEX), 0, D3DFVF_CUSTOMEVERTEX,
		D3DPOOL_DEFAULT, &VB, NULL)))
	{
		return E_FAIL;
	}

	//정점의 버퍼를 값으로 채운다.
	//정점 버퍼의 Lock() 함수를 호출하여 포인터를 얻어온다.
	void* pVertices;

	if (FAILED(VB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	VB->Unlock();

	Center = { 0.0f, 1.0f, 0.0f };
	
	return S_OK;

}

HRESULT Object::InitIB()
{
	CUSTOMINDEX indices[] =
	{
		{ 0, 1, 2 },{ 0, 2, 3 }, // 윗면
		{ 4, 5, 6 },{ 4, 7, 6 }, // 아랫면
		{ 0, 3, 7 },{ 0, 7, 4 }, // 왼면
		{ 1, 5, 6 },{ 1, 6, 2 }, // 오른면
		{ 3, 2, 6 },{ 3, 6, 7 }, // 뒷면
		{ 0, 4, 5 },{ 0, 5, 1 }, // 앞면
	};

	//인덱스 버퍼 생성
	//D3DFMT_INDEX16은 인덱스의 단위가 16비트라는 것.
	//MYINDEX 구조체에서 WORD형으로 선언으므로 D3DFMT_INDEX16을 사용한다.
	if (FAILED((*device)->CreateIndexBuffer(sizeof(indices) * sizeof(CUSTOMINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IB, NULL)))
	{
		return E_FAIL;
	}

	//인덱스 버퍼를 값으로 채운다.
	void* pIndices;
	//인덱스 버퍼의 Lock() 함수를 호출하여 포인터를 얻어와서 정보를 채운다.
	if (FAILED(IB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
	{
		return E_FAIL;
	}

	memcpy(pIndices, indices, sizeof(indices));

	IB->Unlock();

	return S_OK;
}

HRESULT Object::InitTexture(const char * filename)
{
	if (FAILED(D3DXCreateTextureFromFile(*device, filename, &Texture)))
	{	
		return E_FAIL;
	}

	return S_OK;
}

D3DXMATRIXA16 Object::getmovMat()
{
	D3DXMATRIXA16 mov; 		
	D3DXMatrixTranslation(&mov, Center.x, Center.y, Center.z); 
	return mov;
}

void Object::update(float fEilpse)
{
	D3DXVECTOR3 temp = Front;
	temp.y = 0;
	D3DXVec3Normalize(&temp, &temp);
	if (InputManager::Getinstance()->isKeyPress(VK_W))
	{	
		Center += Front;/*
		Center.z += temp.z;
		Center.x += temp.x;*/
	}
		
	if (InputManager::Getinstance()->isKeyPress(VK_S)) {
		Center -= Front;
		/*Center.z -= temp.z;
		Center.x -= temp.x;*/
	}

	temp = Side;
	temp.y = 0;
	D3DXVec3Normalize(&temp, &temp);

	if (InputManager::Getinstance()->isKeyPress(VK_D)) {
		Center.z += temp.z;
		Center.x += temp.x;
	}
	if (InputManager::Getinstance()->isKeyPress(VK_A)) {
		Center.z -= temp.z;
		Center.x -= temp.x;
	}
	
	if (InputManager::Getinstance()->isKeyPress(VK_RIGHT))
	{
		m_theta += D3DX_PI / 180.0f;
		D3DXMATRIXA16 rot;
		D3DXMatrixRotationY(&rot, D3DX_PI / 180.0f);
		D3DXVec3TransformCoord(&Front, &Front, &rot);
		D3DXVec3Cross(&Side, &Up, &Front);
	}
	if (InputManager::Getinstance()->isKeyPress(VK_LEFT)) {
		m_theta -= D3DX_PI / 180.0f;
		D3DXMATRIXA16 rot;
		D3DXMatrixRotationY(&rot, -D3DX_PI / 180.0f);
		D3DXVec3TransformCoord(&Front, &Front, &rot);
		D3DXVec3Cross(&Side, &Up, &Front);
	}
/*
	if (InputManager::Getinstance()->isKeyPress(VK_UP))
	{
		m_theta += D3DX_PI / 180.0f;
		D3DXMATRIXA16 rot;
		D3DXMatrixRotationY(&rot, D3DX_PI / 180.0f);
		D3DXVec3TransformCoord(&Front, &Front, &rot);
		D3DXVec3Cross(&Side, &Up, &Front);
	}
	if (InputManager::Getinstance()->isKeyPress(VK_DOWN)) {
		m_theta -= D3DX_PI / 180.0f;
		D3DXMATRIXA16 rot;
		D3DXMatrixRotationY(&rot, -D3DX_PI / 180.0f);
		D3DXVec3TransformCoord(&Front, &Front, &rot);
		D3DXVec3Cross(&Side, &Up, &Front);
	}*/


	if (InputManager::Getinstance()->isKeyPress(VK_SPACE))
	{
		if (InputManager::Getinstance()->isKeyPress(VK_CONTROL))
			Center.y -= 1.0f;
		else
			Center.y += 1.0f;
	}



// 마우스 회전
	{
		D3DXMATRIXA16 matRot;

		POINT moved = InputManager::Getinstance()->GetMouseMoved();
		float Ytheta = moved.x * D3DX_PI / 180.0f / 10.0f;

		float Xtheta = moved.y * D3DX_PI / 180.0f / 10.0f;

		{
			D3DXMATRIXA16 matRot;
			D3DXMatrixRotationY(&matRot, Ytheta);
			m_theta += Ytheta;
			D3DXVec3TransformCoord(&Front, &Front, &matRot);
			D3DXVec3TransformCoord(&Up, &Up, &matRot);
			D3DXVec3Cross(&Side, &Up, &Front);
		}

		{

			D3DXMATRIXA16 matRot;
			D3DXVECTOR3 newView;
			D3DXMatrixRotationAxis(&matRot, &Side, Xtheta);
			D3DXVec3TransformCoord(&newView, &Front, &matRot);

			D3DXVECTOR3 di(Front.x, 0.0f, Front.z);

			if (fabsf(acos(D3DXVec3Dot(&newView, &di)) * 180.0f / D3DX_PI) > PLAYER_LOOK_UPDOWN_DEGREE)
				return;
			m_Xtheta += Xtheta;

			D3DXVec3TransformCoord(&Front, &Front, &matRot);
			D3DXVec3TransformCoord(&Up, &Up, &matRot);
		}
	}
}

void Object::setPosition(float cx, float cy, float cz)
{
	Center = { cx,cy+1.0f,cz };
}

void Object::setScale(float cx, float cy, float cz) {

	m_Scale.x = cx;
	m_Scale.y = cy;
	m_Scale.z = cz;

}


void Object::DrawMesh(D3DXMATRIXA16 * pMat)
{
	if (pMat == 0)
	{
		pMat = new D3DXMATRIXA16();
		D3DXMatrixIdentity(pMat);
	}
	
	//*스케일링자전이동공전부모*/
	D3DXMATRIXA16 temp;
	D3DXMatrixScaling(&temp, m_Scale.x, m_Scale.y, m_Scale.z);
	m_Mat = temp;	
	D3DXMatrixRotationY(&temp, m_theta);
	m_Mat *= temp;
	D3DXMatrixRotationAxis(&temp, &Side, m_Xtheta);
	m_Mat *= temp;
	

	D3DXMatrixTranslation(&temp, Center.x, Center.y, Center.z);
	m_Mat *= temp;
	m_Mat *= *pMat;

	(*device)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*device)->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

	// TEXTURE의 색과 정점의 색(DIFFUSE)을 섞는다.
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	// alpha 연산을 사용하지 않는다.
	(*device)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	(*device)->SetTexture(0, Texture);

	(*device)->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	(*device)->SetTransform(D3DTS_WORLD, &m_Mat);
	
	(*device)->SetStreamSource(0, VB, 0, sizeof(CUSTOMEVERTEX));

	// D3D에게 정점 셰이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
	(*device)->SetFVF(D3DFVF_CUSTOMEVERTEX);

	// 인덱스 버퍼를 지정한다.
	(*device)->SetIndices(IB);

	// 기하 정보를 출력하기 위한 DrawIndexPrimitive() 함수를 호출한다.
	// 첫 번째 : 그리고자 하는 기본 타입형
	// 두 번째 : 여러 오브젝트를 하나로 묶을때 더해질 넘버.
	// 세 번째 : 참조할 최소 인덱스 값
	// 네 번째 : 이번 호출에 참조될 버텍스의 수
	// 다섯 번째 : 인덱스 버퍼 내에서 읽기를 시작할 요소로의 인덱스
	// 여섯 번째 : 그리고자 하는 기본형의 수
	(*device)->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

Object::Object(LPDIRECT3DDEVICE9* _deviece)
{
	device = _deviece;
	VB = NULL;
	IB = NULL;
	
}



Object::~Object()
{
	if (Texture != NULL)
		Texture->Release();

	if (IB != NULL)
		IB->Release();
	if (VB != NULL)
		VB->Release();

	if ((*device) != NULL)
		(*device)->Release();
}
