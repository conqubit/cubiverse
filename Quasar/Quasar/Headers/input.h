#pragma once

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {
public:
    Input();
    ~Input();

    bool init();
    void shutdown();
    bool readInput();
    bool keyPressed(int key);

    int mx, my, dmx, dmy;

private:
    bool readKeyboard();
    bool readMouse();
    void processInput();

    LPDIRECTINPUT8 directInput;
    LPDIRECTINPUTDEVICE8 keyboard;
    LPDIRECTINPUTDEVICE8 mouse;

    unsigned char keyState[256];
    DIMOUSESTATE mouseState;
    int screenWidth, screenHeight;
};