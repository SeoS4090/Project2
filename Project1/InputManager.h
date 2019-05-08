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

	bool					isExistKey(int key);	//Ű �����ϴ���
	void					RegistKeyCode(int key); //Ű ���
	void					Init(HWND hWnd);		//������ â ��� �� �ʱ�ȭ
	
	
	
	void					Update();				//Ű ���°� ������Ʈ
	void					Clear();				//����� Ű ����

	bool					isKeyPress(int keyCode);//Ű ������ ��������
	bool					isKeyUp(int keyCode);	//Ű�� �� ��������
	bool					isKeyDown(int keyCode);	//Ű�� ���� ��������
	POINT					GetMousePoint();		//���콺 ������ ��ġ
	POINT					GetMouseMoved();
	void					InitMousePoint();
	InputManager();
	~InputManager();
};

