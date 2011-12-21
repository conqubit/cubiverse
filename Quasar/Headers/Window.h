#pragma once

#include "stdafx.h"

class Window {
public:
    static sf::RenderWindow sfWindow;
    static bool             temp;
    static bool             focus;
    static int posX;
    static int posY;

    static bool Init();
    static void Shutdown();

    static sf::WindowHandle SystemHandle();

    static int Width();
    static int Height();
    static int PosX();
    static int PosY();

    static double AspectRatio();

    static void DoEvents();
    static void DoResize();

    static void Maximize();
    static void ToggleFullscreen();

    static void Focus();
    static void Unfocus();
	
    static bool IsMaximized();
    static bool IsMinimized();

    static bool IsFullScreen();

    static void Display();
};