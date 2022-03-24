#pragma once

#include "Base.h"

BEGIN(Engine)

#define INPUT_MAX 256

class CInput_Device :
	public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
	enum MOUSEBUTTONSTATE { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_END };
private:
	CInput_Device();
	virtual ~CInput_Device() = default;
public:
	_bool Get_DIKState(_ubyte byKeyID) const {
		if (m_KeyBoardState[byKeyID] & 0x80) {
			return true;
		}
		else {
			return false;
		}
	}

	_bool Get_DIKDown(_ubyte _byKeyID) const {
		return m_prevKeyBoardState[_byKeyID] > m_KeyBoardState[_byKeyID] ? true : false;
	}

	_bool Get_DIKUp(_ubyte _byKeyID) const {
		return m_prevKeyBoardState[_byKeyID] < m_KeyBoardState[_byKeyID] ? true : false;
	}

	_long Get_MouseMoveState(MOUSEMOVESTATE eMoveState) const {
		return ((_long*)&m_MouseState)[eMoveState];
	}

	_byte Get_MouseButtonState(MOUSEBUTTONSTATE eButtonState) const {
		return m_MouseState.rgbButtons[eButtonState];
	}

	_byte Get_MouseButtonStateUp(MOUSEBUTTONSTATE eButtonState) const {
		return m_MouseState.rgbButtons[eButtonState] > m_prevMouseState.rgbButtons[eButtonState] ? true : false;
	}
	_byte Get_MouseButtonStateDown(MOUSEBUTTONSTATE eButtonState) const {
		return m_MouseState.rgbButtons[eButtonState] < m_prevMouseState.rgbButtons[eButtonState] ? true : false;
	}
public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);

	/* 입력장치의 현재 상태를 얻어온다. */
	HRESULT SetUp_InputDeviceState();

private:
	IDirectInput8*			m_pInputSDK = nullptr;
	IDirectInputDevice8*	m_pKeyBoard = nullptr;
	IDirectInputDevice8*	m_pMouse = nullptr;

private:
	_byte				m_prevKeyBoardState[INPUT_MAX];
	_byte				m_KeyBoardState[INPUT_MAX];
	DIMOUSESTATE		m_prevMouseState;
	DIMOUSESTATE		m_MouseState;
public:
	virtual void Free() override;
};

END