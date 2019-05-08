#include "Object.h"
#include "Defines.h"



HRESULT Object::Init()
{
	if (SUCCEEDED(initVB()) && SUCCEEDED(InitIB()))
	{
		D3DXMatrixIdentity(&m_Mat);

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
		vertices[i].Color = D3DCOLOR_ARGB(0, 255,200,200);
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


void Object::update(float fEilpse)
{
	//D3DXMatrixRotationY(&m_Mat, GetTickCount() / 360.0f);
}

void Object::setPosition(float cx, float cy, float cz)
{
	D3DXMatrixIdentity(&m_Mat);
	D3DXMatrixTranslation(&m_Mat, cx, cy, cz);
}

void Object::setScale(float cx, float cy, float cz) {

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&mat, cx, cy, cz);
	mat *= m_Mat;
	m_Mat = mat;
}


void Object::DrawMesh(D3DXMATRIXA16 * pMat)
{
	if (pMat == 0)
	{
		pMat = new D3DXMATRIXA16();
		D3DXMatrixIdentity(pMat);
	}
	(*device)->SetRenderState(D3DRS_LIGHTING, FALSE);

	(*device)->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);


	// TEXTURE의 색과 정점의 색(DIFFUSE)을 섞는다.
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// alpha 연산을 사용하지 않는다.
	(*device)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	(*device)->SetTexture(0, Texture);

	(*device)->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);


	//카메라 좌표계 설정으로 뿌리기 때문에 vertext 설정에서 각각 설정해도 적용이 안된다.
	//카메라 좌표료 0.0f ~ 1.0f 로 설정 된다.
	//(*device)->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);

	//(*device)->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);





	(*device)->SetTransform(D3DTS_WORLD, &(m_Mat * (*pMat)));
	
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
