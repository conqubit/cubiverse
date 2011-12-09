#pragma once

#include "System.h"

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input {
public:
    static bool InitDirectInput();
    static void Shutdown();
    static void SetMousePosition(int x, int y);
    static bool KeyPressed(Key key);
    static bool MouseLeft();
    static bool MouseRight();
    static int Mx();
    static int My();
    static int DeltaMx();
    static int DeltaMy();
	static bool ReadMouse();
    static void Lock();
    static void Unlock();

    static int mx, my;
    static int dmx, dmy;
    static bool locked;

    static bool directInputInitialized;

    static IDirectInput8W* directInput;
    static IDirectInputDevice8W* mouse;
};