#include "stdafx.h"

#include "Input.h"

int Input::mx;
int Input::my;
int Input::dmx;
int Input::dmy;

IDirectInput8W* Input::directInput;
IDirectInputDevice8W* Input::mouse;

bool Input::Init() {
    HRESULT r;

    r = DirectInput8Create((HINSTANCE)GetWindowLong(System::WindowHandle(), GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
    if (FAILED(r)) return false;

    r = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
    if (FAILED(r)) return false;

    r = mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(r)) return false;

    r = mouse->SetCooperativeLevel(System::WindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(r)) return false;

    r = mouse->Acquire();
    if (FAILED(r)) return false;

    return true;
}

void Input::SetMousePosition(int x, int y) {
    sf::Mouse::SetPosition(sf::Vector2i(x, y), System::window);
}

bool Input::KeyPressed(Key key) {
    return sf::Keyboard::IsKeyPressed(key);
}

bool Input::MouseLeft() {
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
    return dmx;
}

int Input::DeltaMy() {
    return dmy;
}

DIMOUSESTATE mouseState;

bool Input::ReadMouse() {
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