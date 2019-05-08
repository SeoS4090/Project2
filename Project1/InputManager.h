#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;


class InputManager
{
	struct KEY_INFO
	{
	public:
		int				keyCode;
		bool			KeyPrecce;
		bool			keyUp;
		bool			KeyDown;
	};

	struct MOUSE_INFO
	{
		POINT	m_originpt;
		POINT	m_currentpt;
		bool	Click;
		bool	precce;
	};

private:

	std::vector<InputManager::KEY_INFO>	m_vecKeyRegist;
	static InputManager *				pThis;
	HWND								m_hWnd;
	MOUSE_INFO							m_Mouse;


public:
	static InputManager *	Getinstance() { if (pThis == NULL) pThis = new InputManager(); return pThis; }

	bool					isExistKey(int key);	//키 존재하는지
	void					RegistKeyCode(int key); //키 등록
	void					Init(HWND hWnd);		//윈도우 창 등록 및 초기화
	
	
	
	void					Update();				//키 상태값 업데이트
	void					Clear();				//저장된 키 삭제

	bool					isKeyPress(int keyCode);//키 눌러진 상태인지
	bool					isKeyUp(int keyCode);	//키를 뗀 상태인지
	bool					isKeyDown(int keyCode);	//키를 누른 상태인지
	POINT					GetMousePoint();		//마우스 포인터 위치
	POINT					GetMouseMoved();
	void					InitMousePoint();
	InputManager();
	~InputManager();
};

