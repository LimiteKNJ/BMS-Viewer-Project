#include "stdafx.h"
#include "inputClass.h"


InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}



bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	if (FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_directInput, NULL))) {
		return false;
	}

	if (FAILED(m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL))) {
		return false;
	}

	// Keyboard Data형식 지정
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard))) {
		return false;
	}

	if (FAILED(m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
		return false;
	}

	if (FAILED(m_keyboard->Acquire())) {
		return false;
	}

	if (FAILED(m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL))) {
		return false;
	}

	// Mouse Data형식 지정
	if (FAILED(m_mouse->SetDataFormat(&c_dfDIMouse))) {
		return false;
	}

	if (FAILED(m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		return false;
	}

	if (FAILED(m_mouse->Acquire())) {
		return false;
	}

	return true;
}



void InputClass::Shutdown() {

	if (m_mouse) {

		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard) {

		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}
	
	if (m_directInput) {

		m_directInput->Release();
		m_directInput = 0;
	}
}


// 현재 상태를 읽어 변경상태를 처리
bool InputClass::Frame() {

	if (!ReadKeyboard()) {
		return false;
	}

	if (!ReadMouse()) {
		return false;
	}

	ProcessInput();
	return true;
}




bool InputClass::ReadKeyboard()
{
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire(); // 포커스를 읽었거나 획득되지 않았을 경우
		}
		else {
			return false;
		}
	} return true;
}



bool InputClass::ReadMouse() {

	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_mouse->Acquire(); // 포커스를 읽었거나 획득되지 않았을 경우
		}
		else {
			return false;
		}
	} return true;
}


void InputClass::ProcessInput() {

	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}


bool InputClass::IsEscapePressed() {
	
	// 0x80 = ESC
	if (m_keyboardState[DIK_ESCAPE] & 0x80) {
		return true;
	}

	return false;
}


void InputClass::GetMouseLocation(int& mouseX, int& mouseY) {

	mouseX = m_mouseX;
	mouseY = m_mouseY;
}