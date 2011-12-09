#include "stdafx.h"

#include "System.h"
#include "Window.h"
#include "graphics/WorldRenderer.h"
#include "Player.h"

bool             System::running;
Player*          System::player;
World*           System::world;
WorldRenderer*   System::worldRenderer;

sf::Clock timer;

std::ofstream Error;

#include "btBulletDynamicsCommon.h"

bool System::Init() {
    btBoxShape b = btBoxShape(btVector3(1, 1, 1));

    Error.open("error.txt", std::ios::app);
    std::cerr.rdbuf(Error.rdbuf());
    sf::Err().rdbuf(std::cerr.rdbuf());

    srand((unsigned int)time(nullptr));

    Window::Init();
    Window::Maximize();
    Window::sfWindow.Display();

    print("OpenGL version: " + str(glGetString(GL_VERSION)));

    uint majorv = Window::sfWindow.GetSettings().MajorVersion;
    uint minorv = Window::sfWindow.GetSettings().MinorVersion;

    if (majorv < 2 || majorv == 2 && minorv < 1) {
        printerr("OpenGL 2.1+ is required. Version found on this computer: " + str(glGetString(GL_VERSION)));
        return false;
    }

    Graphics::Init();

    Window::sfWindow.ShowMouseCursor(false);

    world = new World();
    world->Init(4, 4, 4);
    world->Fill(Block::Air);
    world->Generate();
    
    worldRenderer = new WorldRenderer();

    if (!worldRenderer->Init(world)) {
        printerr("Failed to initialize the world renderer.");
        return false;
    }

    worldRenderer->ConstructVisibleChunks();

    player = new Player();
    player->pos = world->width.ToDouble() / 2.0;
    player->pos.z = world->width.z - 8;
    player->playerUp = Vector3I(0, 0, 1);
    player->smoothUp = Vector3D(0, 0, 1);

    player->Init();

    Graphics::things.push_back(worldRenderer);
    Graphics::things.push_back(player);

    return true;
}

int oldTime;

int tick;
int accum;
int delta;

int64 fpsTicks;
int fpsCount;
int currentFPS;

int64 oldtime;
int64 newtime;
int64 freq;

void System::Start() {
    if (running) return;
    running = true;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&oldtime);

    tick = freq / 1000 * 5;
    accum = tick;

    while(running) {
        QueryPerformanceCounter((LARGE_INTEGER*)&newtime);

        delta = (newtime - oldtime);
        fpsTicks += delta;
        fpsCount++;

        DoTicks();
        Graphics::Render();
        Window::DoWindowDrawing();
        Window::sfWindow.Display();

        oldtime = newtime;
    }
}

void System::DoTicks() {
    accum += delta;
    while (accum >= tick) {
        Tick();
        accum -= tick;
    }
}

void System::Tick() {
    Window::DoEvents();
    Window::DoResize();
    player->Tick();
}

void System::Stop() {
    running = false;
}

void System::Shutdown() {
    Graphics::Shutdown();
}