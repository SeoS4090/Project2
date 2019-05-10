#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

/**
@fn InputManager.h
@class InputManager
@brief 입력 관리 클래스
@details 키보드 입력과 마우스 움직임과 클릭을 관리 한다.
@author 서승현
@date 2019/05/09
*/
class InputManager
{
private:
	/**
	brief 키보드 상태
	*/
	struct KEY_INFO
	{
	public:
		int				keyCode; ///< 눌린 키
		bool			KeyPrecce; ///< 키를 누르고 있는가
		bool			keyUp;	///< 키를 뗀것인가
		bool			KeyDown; ///< 키를 눌렀는가
	};
	/**
	brief 마우스 상태
	*/
	struct MOUSE_INFO
	{
		POINT	m_originpt; ///< 이전 마우스 위치
		POINT	m_currentpt; ///< 현재 마우스 위치
		bool	Click;		///< 클릭을 했는가
		bool	precce;		///< 누르고 있는가
	};


	std::vector<InputManager::KEY_INFO>	m_vecKeyRegist; ///<입력할 키 등록
	static InputManager *				pThis;	///< 싱글턴 포인터
	HWND								m_hWnd; ///< 윈도우 핸들 등록
	MOUSE_INFO							m_Mouse; ///< 마우스 정보


public:
	static InputManager *	Getinstance() { if (pThis == NULL) pThis = new InputManager(); return pThis; }

	bool					isExistKey(int key);	///<키 존재하는지
	void					RegistKeyCode(int key); ///<키 등록
	void					Init(HWND hWnd);		///<윈도우 창 등록 및 초기화
	
	
	
	void					Update();				///<키 상태값 업데이트
	void					Clear();				///<저장된 키 삭제

	bool					isKeyPress(int keyCode);///<키 눌러진 상태인지
	bool					isKeyUp(int keyCode);	///<키를 뗀 상태인지
	bool					isKeyDown(int keyCode);	///<키를 누른 상태인지
	POINT					GetMousePoint();		///<마우스 포인터 위치
	/**
	@return 마우스 드래그했을때 이동된 거리 반환
	*/
	POINT					GetMouseMoved();		
	void					InitMousePoint();		
	InputManager();
	~InputManager();
};

