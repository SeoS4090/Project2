#include "Plane.h"
#include "Defines.h"


Plane::Plane(LPDIRECT3DDEVICE9 * _device, const char * _Height, const char * _texture)
{	
	device = *_device;
	// 높이맵 텍스처
	// D3DFMT_X8R8G8B8와 D3DPOOL_MANAGED를 주기위해서 이 함수를 사용했다.
	if (FAILED(D3DXCreateTextureFromFileEx(device, _Height,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &Height)))
	{
		return;
	}

	// 색깔맵
	if (FAILED(D3DXCreateTextureFromFile(device, _texture, &Diffuse)))
		return;


	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;

	Height->GetLevelDesc(0, &ddsd);		// 텍스처의 정보
	cxHeight = ddsd.Width;				// 텍스처의 가로크기
	czHeight = ddsd.Height;				// 텍스처의 세로크기


	return;

}

HRESULT Plane::Init()
{
	D3DXMatrixIdentity(&m_Mat);
	if(SUCCEEDED(InitVB()) && SUCCEEDED(InitIB()))
		return S_OK;
	return E_FAIL;
}

void Plane::DrawPlane(D3DXMATRIXA16* pMat)
{
	// 후면버퍼와 Z버퍼 초기화
	if (pMat == 0)
	{
		pMat = new D3DXMATRIXA16();
		D3DXMatrixIdentity(pMat);
	}

	// 애니메이션 행렬설정
	//Animate();
	// 렌더링 시작
		device->SetTexture(0, Diffuse);							// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0번 텍스처 스테이지의 확대 필터
		device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0번 텍스처 : 0번 텍스처 인덱스 사용

		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		device->SetTransform(D3DTS_WORLD, &(m_Mat * (*pMat)));
		device->SetStreamSource(0, VB, 0, sizeof(CUSTOMEVERTEX));
		device->SetFVF(D3DFVF_CUSTOMEVERTEX);
		device->SetIndices(IB);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, cxHeight*czHeight, 0, (cxHeight - 1)*(czHeight - 1) * 2);

}

void Plane::Update()
{
	D3DXMatrixRotationY(&m_Mat, GetTickCount() / 360.0f);
}

/**-----------------------------------------------------------------------------
* 정점버퍼를 생성하고 정점값을 채워넣는다.
*------------------------------------------------------------------------------
*/
HRESULT Plane::InitVB()
{
	D3DLOCKED_RECT		d3drc;

	if (FAILED(device->CreateVertexBuffer(cxHeight*czHeight * sizeof(CUSTOMEVERTEX),
		0, D3DFVF_CUSTOMEVERTEX,
		D3DPOOL_DEFAULT, &VB, NULL)))
	{
		return E_FAIL;
	}

	// 텍스처 메모리 락!
	Height->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;
	// 정점버퍼 락!
	if (FAILED(VB->Lock(0, cxHeight*czHeight * sizeof(CUSTOMEVERTEX), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	CUSTOMEVERTEX	v;
	CUSTOMEVERTEX*	pV = (CUSTOMEVERTEX*)pVertices;

	for (DWORD z = 0; z < czHeight; z++)
	{
		for (DWORD x = 0; x < cxHeight; x++)
		{
			v.nomal.x = v.position.x = (float)x - cxHeight / 2.0f;		// 정점의 x좌표(메시를 원점에 정렬)
			v.nomal.y = v.position.z = -((float)z - czHeight / 2.0f);	// 정점의 z좌표(메시를 원점에 정렬), z축이 모니터안쪽이므로 -를 곱한다.
			v.nomal.z = v.position.y = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x000000ff)) / 10.0f;	// DWORD이므로 pitch/4
	
			D3DXVec3Normalize(&v.nomal, &v.nomal);
			v.tu = (float)x / (cxHeight - 1);
			v.tv = (float)z / (czHeight - 1);
			*pV++ = v;
		}
	}

	VB->Unlock();
	Height->UnlockRect(0);

	return S_OK;
}

/**-----------------------------------------------------------------------------
* 인덱스 버퍼 초기화
*------------------------------------------------------------------------------
*/
HRESULT Plane::InitIB()
{
	if (FAILED(device->CreateIndexBuffer((cxHeight - 1)*(czHeight - 1) * 2 * sizeof(CUSTOMINDEX),
		0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IB, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMINDEX		i;
	CUSTOMINDEX*	pI;
	if (FAILED(IB->Lock(0, (cxHeight - 1)*(czHeight - 1) * 2 * sizeof(CUSTOMINDEX), (void**)&pI, 0)))
	{
		return E_FAIL;
	}

	for (DWORD z = 0; z < czHeight - 1; z++)
	{
		for (DWORD x = 0; x < cxHeight - 1; x++)
		{
			i._0 = (z*cxHeight + x);
			i._1 = (z*cxHeight + x + 1);
			i._2 = ((z + 1)*cxHeight + x);

			*pI++ = i;

			i._0 = ((z + 1)*cxHeight + x);
			i._1 = (z*cxHeight + x + 1);
			i._2 = ((z + 1)*cxHeight + x + 1);

			*pI++ = i;
		}
	}

	IB->Unlock();

	return S_OK;
}




Plane::~Plane()
{
}
