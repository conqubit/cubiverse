#include "stdafx.h"
#include "system.h"

Input::Input() {
	directInput = NULL;
	keyboard = NULL;
	mouse = NULL;

	mx = 0;
	my = 0;
	dmx = 0;
	dmy = 0;

	screenWidth = 0;
	screenHeight = 0;
}

Input::~Input() {
}

bool Input::init() {
	HRESULT r;
	screenWidth = 800;
	screenHeight = 600;

	r = DirectInput8Create(System::hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(r)) {
		return false;
	}

	r = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(r)) {
		return false;
	}

	r = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(r)) {
		return false;
	}

	r = keyboard->SetCooperativeLevel(System::hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(r)) {
		return false;
	}

	r = keyboard->Acquire();
	if (FAILED(r)) {
		return false;
	}

	r = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(r)) {
		return false;
	}

	r = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(r)) {
		return false;
	}

	r = mouse->SetCooperativeLevel(System::hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(r)) {
		return false;
	}

	r = mouse->Acquire();
	if (FAILED(r)) {
		return false;
	}

	return true;
}

bool Input::keyPressed(int key) {
	return (keyState[key] & 0x80) == 0x80;
}

bool Input::readInput() {
	bool r;

	r = readKeyboard();
	if (!r) {
		return false;
	}

	r = readMouse();
	if (!r) {
		return false;
	}

	processInput();

	return true;
}

bool Input::readKeyboard() {
	HRESULT r;

	r = keyboard->GetDeviceState(sizeof(keyState), (LPVOID)&keyState);
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

bool Input::readMouse() {
	HRESULT r;

	r = mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
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

	return true;
}

void Input::processInput() {
	dmx = mouseState.lX;
	dmy = mouseState.lY;
	mx += dmx;
	my += dmy;

	if (mx < 0) {
		mx = 0;
	}
	if (my < 0) {
		my = 0;
	}

	if (mx > screenWidth) {
		mx = screenWidth;
	}
	if (my > screenHeight) {
		my = screenHeight;
	}
}

void Input::shutdown() {
	if (mouse) {
		mouse->Unacquire();
		mouse->Release();
		mouse = NULL;
	}
	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = NULL;
	}
	if (directInput) {
		directInput->Release();
		directInput = NULL;
	}
}