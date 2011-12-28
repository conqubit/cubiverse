#pragma once

#include <dinput.h>

class Input {
public:
	static bool Init();
	static void Shutdown();
	static void SetMousePosition(int x, int y);
	static bool KeyPressed(int key);
	static bool MouseLeft();
	static bool MouseRight();
	static int Mx();
	static int My();
	static int DeltaMx();
	static int DeltaMy();
	static bool ReadKeyboard();
	static bool ReadMouse();
	static void Lock();
	static void Unlock();

	static int mx, my;
	static int dmx, dmy;
	static bool locked;

	static bool directInputInitialized;

	static IDirectInput8W* directInput;
	static IDirectInputDevice8W* keyboard;
	static IDirectInputDevice8W* mouse;
};