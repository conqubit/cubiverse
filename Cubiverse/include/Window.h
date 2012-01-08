#pragma once

#include "stdafx.h"

class Window {
public:
	static bool focusBeforeEvents;
	static int posX;
	static int posY;

	static bool Init();
	static void Shutdown();

	static void SetIcon(string file);

	static HWND SystemHandle();

	static int Width();
	static int Height();
	static int X();
	static int Y();

	static double AspectRatio();

	static void DoEvents();

	static void Maximize();
	static void ToggleFullscreen();

	static bool HasFocus();

	static bool IsMaximized();
	static bool IsMinimized();

	static bool IsFullScreen();

	static void Display();
	static void SetCallbacks();
	static void ClearCallbacks();

private:
	static void GLFWCALL KeyCallback(int key, int action);
	static int GLFWCALL CloseCallback();
	static void GLFWCALL ResizeCallback(int width, int height);
	static void GLFWCALL MouseButtonCallback(int button, int action);
	static void GLFWCALL CharCallback(int character, int action);
	static void GLFWCALL MousePositionCallback(int x, int y);
};