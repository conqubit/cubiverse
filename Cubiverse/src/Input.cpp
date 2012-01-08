#include "stdafx.h"

#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

IDirectInput8W* Input::directInput;
IDirectInputDevice8W* Input::keyboard;
IDirectInputDevice8W* Input::mouse;

bool Input::directInputInitialized = false;

int Input::dmx;
int Input::dmy;

bool Input::locked = false;

bool Input::Init() {
	directInputInitialized = false;
	HRESULT r;

	r = DirectInput8Create((HINSTANCE)GetWindowLongPtr(Window::SystemHandle(), GWLP_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(r)) return false;


	r = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(r)) return false;

	r = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(r)) return false;

	r = keyboard->SetCooperativeLevel(Window::SystemHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(r)) return false;

	r = keyboard->Acquire();
	if (FAILED(r)) return false;


	r = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(r)) return false;

	r = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(r)) return false;

	r = mouse->SetCooperativeLevel(Window::SystemHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(r)) return false;

	r = mouse->Acquire();
	if (FAILED(r)) return false;


	directInputInitialized = true;
	return true;
}

void Input::Shutdown() {
	mouse->Unacquire();
	mouse->Release();
	directInput->Release();

	directInputInitialized = false;
}

bool Input::IsLocked() {
	return locked || !Window::HasFocus();
}

void Input::Lock() {
	glfwEnable(GLFW_MOUSE_CURSOR);
	locked = true;
}

void Input::Unlock() {
	glfwDisable(GLFW_MOUSE_CURSOR);
	locked = false;
}

void Input::SetMousePosition(int x, int y) {
	glfwSetMousePos(x, y);
}

bool Input::MouseLeft() {
	if (locked) return false;
	switch (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
	case GLFW_PRESS:
		return true;
	case GLFW_RELEASE:
		return false;
	}
}

bool Input::MouseRight() {
	if (locked) return false;
	switch (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
	case GLFW_PRESS:
		return true;
	case GLFW_RELEASE:
		return false;
	}
}

int Input::Mx() {
	int x;
	glfwGetMousePos(&x, nullptr);
	return x;
}

int Input::My() {
	int y;
	glfwGetMousePos(nullptr, &y);
	return y;
}

int Input::DeltaMx() {
	if (locked) return 0;
	return dmx;
}

int Input::DeltaMy() {
	if (locked) return 0;
	return dmy;
}

byte keyState[256];

bool Input::KeyPressed(int key) {
	if (locked) return false;
	return (keyState[key] & 0x80) == 0x80;
}

bool Input::ReadKeyboard() {
	if (!directInputInitialized && !Init()) {
		return false;
	}

	HRESULT r;

	r = keyboard->GetDeviceState(sizeof(keyState), (void*)&keyState);
	if (FAILED(r)) {
		if (r == DIERR_INPUTLOST || r == DIERR_NOTACQUIRED) {
			r = keyboard->Acquire();
			if (FAILED(r)) {
				return false;
			}
		} else {
			return false;
		}
	}

	return true;
}

DIMOUSESTATE mouseState;

bool Input::ReadMouse() {
	if (!directInputInitialized && !Init()) {
		return false;
	}

	HRESULT r;

	r = mouse->GetDeviceState(sizeof(mouseState), (void*)&mouseState);
	if (FAILED(r)) {
		if (r == DIERR_INPUTLOST || r == DIERR_NOTACQUIRED) {
			r = mouse->Acquire();
			if (FAILED(r)) {
				return false;
			}
		} else {
			return false;
		}
	}

	dmx = mouseState.lX;
	dmy = mouseState.lY;

	return true;
}