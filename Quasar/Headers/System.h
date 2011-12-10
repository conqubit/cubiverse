#pragma once

#include "stdafx.h"

#include <windows.h>

#include "graphics/Graphics.h"
#include "graphics/WorldRenderer.h"
#include "level/World.h"
#include "Input.h"
#include "Player.h"
#include "Logger.h"

class System {
public:
    static bool             running;
    static Player*          player;
    static World*           world;
    static WorldRenderer*   worldRenderer;
    static FileLogger       errorLog;

    static bool Init();
    static void Shutdown();

    static void Start();
    static void Stop();

    static void DoTicks();
    static void Tick();
};