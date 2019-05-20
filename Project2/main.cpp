//////////////////////////////////////////////////////////////////////////////////////////////////

// File: d3dUtility.h  

//////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef __d3dUtilityH__

#define __d3dUtilityH__



#include <d3dx9.h>

#include <string>



namespace d3d

{

	bool InitD3D(

		HINSTANCE hInstance,       // [in] Application instance.

		int width, int height,    // [in] Backbuffer dimensions.

		bool windowed,            // [in] Windowed (true)or full screen (false).

		D3DDEVTYPE deviceType,    // [in] HAL or REF

		IDirect3DDevice9** device);// [out]The created device.



	int EnterMsgLoop(

		bool(*ptr_display)(float timeDelta));



	LRESULT CALLBACK WndProc(

		HWND hwnd,

		UINT msg,

		WPARAM wParam,

		LPARAM lParam);



	template<class T> void Release(T t)

	{

		if (t)

		{

			t->Release();

			t = 0;

		}

	}



	template<class T> void Delete(T t)

	{

		if (t)

		{

			delete t;

			t = 0;

		}

	}



	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));

	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));

	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));

	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));

	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));

	const D3DXCOLOR    YELLOW(D3DCOLOR_XRGB(255, 255, 0));

	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));

	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));



	//

	// Lights

	//



	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);

	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);

	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);



	//

	// Materials

	//



	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);



	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);

	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);

	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);

	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);

	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

}



#endif // __d3dUtilityH__

//////////////////////////////////////////////////////////////////////////////////////////////////

// File: d3dUtility.cpp       

//////////////////////////////////////////////////////////////////////////////////////////////////







bool d3d::InitD3D(

	HINSTANCE hInstance,

	int width, int height,

	bool windowed,

	D3DDEVTYPE deviceType,

	IDirect3DDevice9** device)

{

	//

	// Create the main application window.

	//



	WNDCLASS wc;



	wc.style = CS_HREDRAW | CS_VREDRAW;

	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;

	wc.cbClsExtra = 0;

	wc.cbWndExtra = 0;

	wc.hInstance = hInstance;

	wc.hIcon = LoadIcon(0, IDI_APPLICATION);

	wc.hCursor = LoadCursor(0, IDC_CROSS);

	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	wc.lpszMenuName = 0;

	wc.lpszClassName = "Direct3D9App";



	if (!RegisterClass(&wc))

	{

		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);

		return false;

	}



	HWND hwnd = 0;

	hwnd = ::CreateWindow("Direct3D9App", "Direct3D9App",

		WS_EX_TOPMOST,

		0, 0, width, height,

		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/);



	if (!hwnd)

	{

		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);

		return false;

	}



	::ShowWindow(hwnd, SW_SHOW);

	::UpdateWindow(hwnd);



	//

	// Init D3D:

	//



	HRESULT hr = 0;



	// Step 1: Create the IDirect3D9 object.



	IDirect3D9* d3d9 = 0;

	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);



	if (!d3d9)

	{

		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);

		return false;

	}



	// Step 2: Check for hardware vp.



	D3DCAPS9 caps;

	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);



	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)

		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else

		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;



	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.



	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth = width;

	d3dpp.BackBufferHeight = height;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hwnd;

	d3dpp.Windowed = windowed;

	d3dpp.EnableAutoDepthStencil = true;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;



	// Step 4: Create the device.



	hr = d3d9->CreateDevice(

		D3DADAPTER_DEFAULT, // primary adapter

		deviceType,        // device type

		hwnd,               // window associated with device

		vp,                // vertex processing

		&d3dpp,            // present parameters

		device);            // return created device



	if (FAILED(hr))

	{

		// try again using a 16-bit depth buffer

		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;



		hr = d3d9->CreateDevice(

			D3DADAPTER_DEFAULT,

			deviceType,

			hwnd,

			vp,

			&d3dpp,

			device);



		if (FAILED(hr))

		{

			d3d9->Release(); // done with d3d9 object

			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);

			return false;

		}

	}



	d3d9->Release(); // done with d3d9 object



	return true;

}



int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))

{

	MSG msg;

	::ZeroMemory(&msg, sizeof(MSG));



	static float lastTime = (float)timeGetTime();



	while (msg.message != WM_QUIT)

	{

		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))

		{

			::TranslateMessage(&msg);

			::DispatchMessage(&msg);

		}

		else

		{

			float currTime = (float)timeGetTime();

			float timeDelta = (currTime - lastTime)*0.001f;



			ptr_display(timeDelta);



			lastTime = currTime;

		}

	}

	return msg.wParam;

}



D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)

{

	D3DLIGHT9 light;

	::ZeroMemory(&light, sizeof(light));



	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Ambient = *color * 0.4f;

	light.Diffuse = *color;

	light.Specular = *color * 0.6f;

	light.Direction = *direction;



	return light;

}



D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)

{

	D3DLIGHT9 light;

	::ZeroMemory(&light, sizeof(light));



	light.Type = D3DLIGHT_POINT;

	light.Ambient = *color * 0.4f;

	light.Diffuse = *color;

	light.Specular = *color * 0.6f;

	light.Position = *position;

	light.Range = 1000.0f;

	light.Falloff = 1.0f;

	light.Attenuation0 = 1.0f;

	light.Attenuation1 = 0.0f;

	light.Attenuation2 = 0.0f;



	return light;

}



D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)

{

	D3DLIGHT9 light;

	::ZeroMemory(&light, sizeof(light));



	light.Type = D3DLIGHT_SPOT;

	light.Ambient = *color * 0.4f;

	light.Diffuse = *color;

	light.Specular = *color * 0.6f;

	light.Position = *position;

	light.Direction = *direction;

	light.Range = 1000.0f;

	light.Falloff = 1.0f;

	light.Attenuation0 = 1.0f;

	light.Attenuation1 = 0.0f;

	light.Attenuation2 = 0.0f;

	light.Theta = 0.5f;

	light.Phi = 0.7f;



	return light;

}



D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)

{

	D3DMATERIAL9 mtrl;

	mtrl.Ambient = a;

	mtrl.Diffuse = d;

	mtrl.Specular = s;

	mtrl.Emissive = e;

	mtrl.Power = p;

	return mtrl;

}

//////////////////////////////////////////////////////////////////////////////////////////////////

// File: d3dxcreatetext.cpp      

//////////////////////////////////////////////////////////////////////////////////////////////////







//

// Globals

//



IDirect3DDevice9* Device = 0;



const int Width = 640;

const int Height = 480;



ID3DXMesh* Text = 0;



//

// Framework functions

//

bool Setup()

{

	//

	// Get a handle to a device context.

	//

	HDC hdc = CreateCompatibleDC(0);

	// �μ��� �־���(0) hdc�� ȣȯ�Ǵ� �޸� DC�� ����.

	HFONT hFont;

	//GDI Object HFONT, HPEN, HBITMAP�� GDI Object���� �� H�� ����.

	HFONT hFontOld;



	//

	// Describe the font we want.

	//



	/*

	D3DXFONT_DESCA ����ü�� LOGFONT ����ü:

	- D3DXFONT_DESCA�� d3dx9core.h�� ����.

	- LOGFONT�� WinGDI.h�� ����.

	- GDI�� ����ϸ� �ٱ�� �� ����.

	- D3DXFONT�� �ѱ��� �� �ܱ�� �� ����.

	- LOGFONT�� ����� �� CreateFontIndirect�� ���.

	- D3DXFONT�� ����� �� D3DXCreateFontIndirect�� ���.

	*/

	LOGFONT lf;

	ZeroMemory(&lf, sizeof(LOGFONT));



	// CreateFont�� �����ϴ� �� ����, ����ü�� ����ϸ�

	// ����� �Ϻκи� �����ؼ� �����ϰų�, ���� ��� ������ �ִ�.

	lf.lfHeight = 0;

	lf.lfWidth = 0;

	lf.lfEscapement = 0;

	lf.lfOrientation = 0;

	lf.lfWeight = 0;   // boldness, range 0(light) - 1000(bold)

	lf.lfItalic = 0;

	lf.lfUnderline = 0;

	lf.lfStrikeOut = 0;

	lf.lfCharSet = DEFAULT_CHARSET;

	lf.lfOutPrecision = 0;

	lf.lfClipPrecision = 0;

	lf.lfQuality = 0;

	lf.lfPitchAndFamily = 0;

	strcpy((char*)lf.lfFaceName, "Times New Roman"); // font style

	hFont = CreateFontIndirect(&lf);

	// ��Ʈ�� �����ϴ� �Լ�(LOGFONT ����ü�� ���).

	// ����� �޽��� �Ἥ ����ϱ� ������ ����ü�� �׳� �ʱ�ȭ�뵵.

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	// ���� ��Ʈ�� ����. hFontOld���� ������ Font�� ���Ϲ޾Ƽ� ����.



	//

	// Create the text mesh based on the selected font in the HDC.

	//

	D3DXCreateText(Device, hdc, "�ȳ��ϼ���",

		0.001f, 0.4f, &Text, 0, 0);

	// ����̽�, DC, ���ڿ�, �ڵ� ����, z���� ���� �������� �о� ������ �ؽ�Ʈ�� ��,

	// ���Ϲ��� �޽� ������, ���� ���� ����Ʈ, LPGLYPHMETRICSFLOAT ����ü�� �迭�� ������.

	// �ѱ��� �� �� D3DXCreateTextW�� L�߰�

	// ����� �� �� D3DXCreateTextA�� L��� ��.

	// �ؽ�Ʈ ���ڿ��� 3D �޽��� ���� �� D3DXCreateText�� ���.



	// ������ ��Ʈ�� �ǵ�����, ���� ��Ʈ ��ü�� DC�� ����.

	SelectObject(hdc, hFontOld);

	DeleteObject(hFont);

	DeleteDC(hdc);





	// ������ ����, ����, ���� ����.

	D3DXVECTOR3 dir(0.0f, -0.5f, 1.0f);

	D3DXCOLOR col = d3d::WHITE;

	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);



	Device->SetLight(0, &light);

	Device->LightEnable(0, true);



	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	Device->SetRenderState(D3DRS_SPECULARENABLE, true);





	// ī�޶��� ����, ���, ������.

	D3DXVECTOR3 pos(0.0f, 1.5f, -3.3f);

	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);



	D3DXMATRIX V;

	D3DXMatrixLookAtLH(

		&V,

		&pos,

		&target,

		&up);



	Device->SetTransform(D3DTS_VIEW, &V);



	//

	// Set projection matrix.

	//



	D3DXMATRIX proj;

	D3DXMatrixPerspectiveFovLH(

		&proj,

		D3DX_PI * 0.25f, // 45 - degree

		(float)Width / (float)Height,

		0.01f,

		1000.0f);

	Device->SetTransform(D3DTS_PROJECTION, &proj);



	return true;

}



void Cleanup()

{

	d3d::Release<ID3DXMesh*>(Text);

}



bool Display(float timeDelta)

{

	if (Device)

	{

		//

		// Update: Spin the 3D text.

		//



		D3DXMATRIX yRot, T;



		static float y = 0.0f;



		D3DXMatrixRotationY(&yRot, y);

		y += timeDelta;



		if (y >= 6.28f)

			y = 0.0f;



		D3DXMatrixTranslation(&T, -1.6f, 0.0f, 0.0f);

		T = T * yRot;



		Device->SetTransform(D3DTS_WORLD, &T);



		//

		// Render

		//



		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

		Device->BeginScene();



		Device->SetMaterial(&d3d::RED_MTRL);

		Text->DrawSubset(0);

		// �ؽ�Ʈ �޽��� �׸���.



		Device->EndScene();

		Device->Present(0, 0, 0, 0);

	}

	return true;

}



//

// WndProc

//

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)

{

	switch (msg)

	{

	case WM_DESTROY:

		::PostQuitMessage(0);

		break;



	case WM_KEYDOWN:

		if (wParam == VK_ESCAPE)

			::DestroyWindow(hwnd);

		break;

	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);

}



//

// WinMain

//

int WINAPI WinMain(HINSTANCE hinstance,

	HINSTANCE prevInstance,

	PSTR cmdLine,

	int showCmd)

{

	if (!d3d::InitD3D(hinstance,

		Width, Height, true, D3DDEVTYPE_HAL, &Device))

	{

		::MessageBox(0, "InitD3D() - FAILED", 0, 0);

		return 0;

	}



	if (!Setup())

	{

		::MessageBox(0, "Setup() - FAILED", 0, 0);

		return 0;

	}



	d3d::EnterMsgLoop(Display);



	Cleanup();



	Device->Release();



	return 0;

}