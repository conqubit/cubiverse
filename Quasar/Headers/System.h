#pragma once

#include "stdafx.h"

#include <windows.h>

#include "graphics/Graphics.h"
#include "graphics/WorldRenderer.h"
#include "level/World.h"
#include "Input.h"
#include "Player.h"

class System {
public:
    static bool             running;
    static sf::RenderWindow window;
    static Graphics*        graphics;
    static Player*          player;
    static World*           world;
    static WorldRenderer*   worldRenderer;
    static bool             focus;
    static bool             temp;

    static bool Init();

    static void Start();
    static void Stop();

    static void Tick();

    static void Shutdown();
    static void DoEvents();

    static HWND WindowHandle();
    static void renderthread();
};