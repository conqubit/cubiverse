#include "stdafx.h"

#include "System.h"
#include "graphics/WorldRenderer.h"

HINSTANCE System::hInstance;
HWND      System::hWindow;
bool      System::running;

Graphics*      System::graphics;
Input*         System::input;
Player*        System::player;
World*         System::world;
WorldRenderer* System::worldRenderer;

extern RECT r;
bool System::Init(HINSTANCE hInst, HWND hWnd) {
    srand((unsigned int)time(nullptr));

    hInstance = hInst;
    hWindow = hWnd;

    input = new Input();
    if (!input->Init()) {
        MessageBoxA(hWindow, "Could not initialize input.", "Error", MB_OK);
        return false;
    }

    graphics = new Graphics();
    if (!graphics->Init()) {
        MessageBoxA(hWindow, "Could not initialize graphics.", "Error", MB_OK);
        return false;
    }

    ShowCursor(false);

    //BringWindowToTop(GetNextWindow(hWindow, GW_HWNDNEXT));

    world = new World();
    world->Init(2, 2, 4);
    world->Fill(Block::Air);
    world->Generate();

    worldRenderer = new WorldRenderer();
    worldRenderer->Init(world);

    worldRenderer->ConstructVisibleChunks();

    player = new Player();
    player->Init();

    graphics->things.Add(worldRenderer);
    graphics->things.Add(player);
    return true;
}

void System::Run() {
    if (running) return;
    running = true;
    while(running) {
        input->ReadInput();
        if (input->KeyPressed(DIK_ESCAPE)) {
            running = false;
            break;
        }

        Tick();
        graphics->Render();
        SetCursorPos(800 - r.left, 450 - r.top);
        Sleep(5);

        MSG msg;
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT) {
                running = false;
                break;
            }
        }
    }
}

void System::Tick() {
    player->Tick();
}

void System::Shutdown() {
    if (graphics) {
        graphics->Shutdown();
        graphics = nullptr;
    }

    if (input) {
        input->Shutdown();
        input = nullptr;
    }
}

LRESULT System::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case 0:
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}