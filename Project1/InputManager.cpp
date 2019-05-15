#include "InputManager.h"
#include <algorithm>

InputManager * InputManager::pThis = NULL;



InputManager::InputManager()
{
	m_vecKeyRegist.clear();
	m_Mouse.Click = false;
	m_Mouse.precce = false;
}


InputManager::~InputManager()
{

}


bool InputManager::isExistKey(int key)
{
	for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
	{
		if ((*iter).keyCode == key)
			return true;
	}

	return false;
}


void InputManager::RegistKeyCode(int key)
{
	if (isExistKey(key))
	{
		return;
	}
	else
	{
		KEY_INFO code;
		memset(&code, 0, sizeof(KEY_INFO));
		code.keyCode = key;
		m_vecKeyRegist.push_back(code);
	}
}

void InputManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	InitMousePoint();
}
POINT InputManager::GetMousePoint()
{
	return m_Mouse.m_currentpt;
}
POINT InputManager::GetMouseMoved()
{
	return POINT{ m_Mouse.m_currentpt.x - m_Mouse.m_originpt.x, m_Mouse.m_currentpt.y - m_Mouse.m_originpt.y };
}

void InputManager::InitMousePoint()
{
	POINT pt;
	RECT rec;
	GetClientRect(m_hWnd, &rec);

	pt.x = fabsf(rec.right - rec.left) / 2 + rec.left;
	pt.y = fabsf(rec.top - rec.bottom) / 2 + rec.top;

	ClientToScreen(m_hWnd, &pt);
	

	
	if (fabsf(pt.x - m_Mouse.m_currentpt.x) >fabsf(rec.right - rec.left) / 2 - 10 || 
		fabsf(pt.y - m_Mouse.m_currentpt.y) > fabsf(rec.top - rec.bottom) / 2 - 10)/*(GetMouseMoved().x == 0 && GetMouseMoved().y == 0)*/
	{
		SetCursorPos(pt.x, pt.y);
		m_Mouse.m_originpt = m_Mouse.m_currentpt = pt;
	}
}


void InputManager::Update()
{
	POINT pt;
	GetCursorPos(&pt);
	m_Mouse.m_originpt = m_Mouse.m_currentpt;
	m_Mouse.m_currentpt = pt;

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		m_Mouse.Click = true;
		
	}
	else if (m_Mouse.Click)
	{
		m_Mouse.Click = false;
	}

	
	InitMousePoint();

	for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
	{
		if (GetKeyState((*iter).keyCode) & 0x8000)
		{
			if ((*iter).KeyPrecce == false)//키 다운
			{
				(*iter).KeyDown = true;
			}
			else
			{
				(*iter).KeyDown = false;
			}

			(*iter).KeyPrecce = true;
			(*iter).keyUp = false;
		}
		else
		{
			if ((*iter).KeyPrecce)//키 업
			{
				(*iter).keyUp = true;
			}
			else
			{
				(*iter).keyUp = false;
			}

			(*iter).KeyDown = false;
			(*iter).KeyPrecce = false;
		}
	}
	
}

void InputManager::Clear()
{
	m_vecKeyRegist.clear();
}

bool InputManager::isKeyPress(int keyCode)
{
	auto iter = m_vecKeyRegist.begin();
	for (; iter != m_vecKeyRegist.end(); iter++)
	{
		if ((*iter).keyCode == keyCode)
		{
			return (*iter).KeyPrecce;
		}
	}

	if (iter == m_vecKeyRegist.end())
		RegistKeyCode(keyCode);
	return false;
}

bool InputManager::isKeyUp(int keyCode)
{
	for (auto iter = m_vecKeyRegist.begin(); iter != m_vecKeyRegist.end(); iter++)
	{
		if ((*iter).keyCode == keyCode)
		{
			return (*iter).keyUp;
		}
	}

	return false;
}

bool InputManager::isKeyDown(int keyCode)
{
	auto iter = m_vecKeyRegist.begin();
	for (; iter != m_vecKeyRegist.end(); iter++)
	{
		if ((*iter).keyCode == keyCode)
		{
			return (*iter).KeyDown;
		}
	}
	if (iter == m_vecKeyRegist.end())
		RegistKeyCode(keyCode);

	return false;
}
