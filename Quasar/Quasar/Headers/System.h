#pragma once

#include "stdafx.h"
#include "graphics/Graphics.h"
#include "graphics/Direct3D.h"
#include "graphics/WorldRenderer.h"
#include "level/World.h"
#include "Input.h"
#include "Player.h"

class System {
public:
    static bool Init(HINSTANCE hInst, HWND hWnd);
    static void Run();
    static void Tick();
    static void Shutdown();
    static LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static HINSTANCE hInstance;
    static HWND      hWindow;
    static bool      running;

    static Graphics*      graphics;
    static Input*         input;
    static Player*        player;
    static World*         world;
    static WorldRenderer* worldRenderer;
};