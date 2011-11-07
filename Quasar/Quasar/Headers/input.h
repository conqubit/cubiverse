#pragma once

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {
public:
    Input();
    ~Input();

    bool Init();
    void Shutdown();
    bool ReadInput();
    bool KeyPressed(int key);

    int mx, my, dmx, dmy;

private:
    bool readKeyboard();
    bool readMouse();
    void processInput();

    IDirectInput8W* directInput;
    IDirectInputDevice8W* keyboard;
    IDirectInputDevice8W* mouse;

    unsigned char keyState[256];
    DIMOUSESTATE mouseState;
    int screenWidth, screenHeight;
};