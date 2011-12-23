#include "stdafx.h"

#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

int Input::mx;
int Input::my;
int Input::dmx;
int Input::dmy;

IDirectInput8W* Input::directInput;
IDirectInputDevice8W* Input::keyboard;
IDirectInputDevice8W* Input::mouse;

bool Input::locked = false;
bool Input::directInputInitialized = false;

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

void Input::Lock() {
    locked = true;
}

void Input::Unlock() {
    locked = false;
}

void Input::SetMousePosition(int x, int y) {
    sf::Mouse::SetPosition(sf::Vector2i(x, y), Window::sfWindow);
}

bool Input::MouseLeft() {
    if (locked) return false;
    return sf::Mouse::IsButtonPressed(sf::Mouse::Button::Left);
}

bool Input::MouseRight() {
    return sf::Mouse::IsButtonPressed(sf::Mouse::Button::Right);
}

int Input::Mx() {
    return mx;
}

int Input::My() {
    return my;
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
    //return sf::Keyboard::IsKeyPressed(key);
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