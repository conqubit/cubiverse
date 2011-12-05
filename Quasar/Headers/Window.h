#pragma once

#include "stdafx.h"

class Window {
public:
    static sf::RenderWindow sfWindow;
    static bool             temp;
    static bool             focus;

    static bool Init();
    static void Shutdown();

    static sf::WindowHandle SystemHandle();

    static int Width();
    static int Height();
    static double AspectRatio();

    static void DoEvents();
    static void DoWindowDrawing();
    static void DoResize();

    static void ToggleFullscreen();

    static void Unfocus();
    static void Focus();
};