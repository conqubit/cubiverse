#pragma once

#include "stdafx.h"
#include "graphics/graphics.h"
#include "graphics/direct3d.h"
#include "input.h"

class System {
public:
	static bool Init(HINSTANCE hInst, HWND hWnd);
	static void Run();
	static void Tick();
	static void Shutdown();
	static LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static HINSTANCE hInstance;
	static HWND hWindow;
	static bool running;
	static Graphics* graphics;
	static Input* input;
};