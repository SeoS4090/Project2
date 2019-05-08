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
* ���� �ʱ�ȭ : ���� ���۸� �����ϰ� �������� ä�� �ִ´�.
*=========================================================================*/
HRESULT Object::initVB()
{
	InitTexture("dahyun.jpg");
	// ����(cube)�� �������ϱ� ���� 8���� ���� ����
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

	// ���� ���۸� �����Ѵ�.
	// ���� ����ü 3���� ������ �޸𸮸� �Ҵ��Ѵ�.
	// FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
	if (FAILED((*device)->CreateVertexBuffer(8 * sizeof(CUSTOMEVERTEX), 0, D3DFVF_CUSTOMEVERTEX,
		D3DPOOL_DEFAULT, &VB, NULL)))
	{
		return E_FAIL;
	}

	//������ ���۸� ������ ä���.
	//���� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���´�.
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
		{ 0, 1, 2 },{ 0, 2, 3 }, // ����
		{ 4, 5, 6 },{ 4, 7, 6 }, // �Ʒ���
		{ 0, 3, 7 },{ 0, 7, 4 }, // �޸�
		{ 1, 5, 6 },{ 1, 6, 2 }, // ������
		{ 3, 2, 6 },{ 3, 6, 7 }, // �޸�
		{ 0, 4, 5 },{ 0, 5, 1 }, // �ո�
	};

	//�ε��� ���� ����
	//D3DFMT_INDEX16�� �ε����� ������ 16��Ʈ��� ��.
	//MYINDEX ����ü���� WORD������ �������Ƿ� D3DFMT_INDEX16�� ����Ѵ�.
	if (FAILED((*device)->CreateIndexBuffer(sizeof(indices) * sizeof(CUSTOMINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IB, NULL)))
	{
		return E_FAIL;
	}

	//�ε��� ���۸� ������ ä���.
	void* pIndices;
	//�ε��� ������ Lock() �Լ��� ȣ���Ͽ� �����͸� ���ͼ� ������ ä���.
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


	// TEXTURE�� ���� ������ ��(DIFFUSE)�� ���´�.
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	(*device)->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// alpha ������ ������� �ʴ´�.
	(*device)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	(*device)->SetTexture(0, Texture);

	(*device)->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);


	//ī�޶� ��ǥ�� �������� �Ѹ��� ������ vertext �������� ���� �����ص� ������ �ȵȴ�.
	//ī�޶� ��ǥ�� 0.0f ~ 1.0f �� ���� �ȴ�.
	//(*device)->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);

	//(*device)->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);





	(*device)->SetTransform(D3DTS_WORLD, &(m_Mat * (*pMat)));
	
	(*device)->SetStreamSource(0, VB, 0, sizeof(CUSTOMEVERTEX));

	// D3D���� ���� ���̴� ������ �����Ѵ�. ��κ��� ��쿡�� FVF�� �����Ѵ�.
	(*device)->SetFVF(D3DFVF_CUSTOMEVERTEX);

	// �ε��� ���۸� �����Ѵ�.
	(*device)->SetIndices(IB);

	// ���� ������ ����ϱ� ���� DrawIndexPrimitive() �Լ��� ȣ���Ѵ�.
	// ù ��° : �׸����� �ϴ� �⺻ Ÿ����
	// �� ��° : ���� ������Ʈ�� �ϳ��� ������ ������ �ѹ�.
	// �� ��° : ������ �ּ� �ε��� ��
	// �� ��° : �̹� ȣ�⿡ ������ ���ؽ��� ��
	// �ټ� ��° : �ε��� ���� ������ �б⸦ ������ ��ҷ��� �ε���
	// ���� ��° : �׸����� �ϴ� �⺻���� ��
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
