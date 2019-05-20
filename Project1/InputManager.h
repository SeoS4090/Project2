#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

/**
@fn InputManager.h
@class InputManager
@brief �Է� ���� Ŭ����
@details Ű���� �Է°� ���콺 �����Ӱ� Ŭ���� ���� �Ѵ�.
@author ������
@date 2019/05/09
*/
class InputManager
{
private:
	/**
	brief Ű���� ����
	*/
	struct KEY_INFO
	{
	public:
		int				keyCode; ///< ���� Ű
		bool			KeyPrecce; ///< Ű�� ������ �ִ°�
		bool			keyUp;	///< Ű�� �����ΰ�
		bool			KeyDown; ///< Ű�� �����°�
	};
	/**
	brief ���콺 ����
	*/
	struct MOUSE_INFO
	{
		POINT	m_originpt; ///< ���� ���콺 ��ġ
		POINT	m_currentpt; ///< ���� ���콺 ��ġ
		bool	Click;		///< Ŭ���� �ߴ°�
		bool	precce;		///< ������ �ִ°�
	};


	std::vector<InputManager::KEY_INFO>	m_vecKeyRegist; ///<�Է��� Ű ���
	static InputManager *				pThis;	///< �̱��� ������
	HWND								m_hWnd; ///< ������ �ڵ� ���
	MOUSE_INFO							m_Mouse; ///< ���콺 ����


public:
	static InputManager *	Getinstance() { if (pThis == NULL) pThis = new InputManager(); return pThis; }

	bool					isExistKey(int key);	///<Ű �����ϴ���
	void					RegistKeyCode(int key); ///<Ű ���
	void					Init(HWND hWnd);		///<������ â ��� �� �ʱ�ȭ
	
	
	
	void					Update();				///<Ű ���°� ������Ʈ
	void					Clear();				///<����� Ű ����

	bool					isKeyPress(int keyCode);///<Ű ������ ��������
	bool					isKeyUp(int keyCode);	///<Ű�� �� ��������
	bool					isKeyDown(int keyCode);	///<Ű�� ���� ��������
	POINT					GetMousePoint();		///<���콺 ������ ��ġ
	/**
	@return ���콺 �巡�������� �̵��� �Ÿ� ��ȯ
	*/
	POINT					GetMouseMoved();		
	void					InitMousePoint();		
	InputManager();
	~InputManager();
};

