#include "stdafx.h"
#include "system.h"

HINSTANCE System::hInstance = nullptr;
HWND System::hWindow = nullptr;
bool System::running = true;
Graphics* System::graphics = nullptr;
Input* System::input = nullptr;

bool System::Init(HINSTANCE hInst, HWND hWnd) {
    srand(time(nullptr));
    bool r;

    hInstance = hInst;
    hWindow = hWnd;
    input = new Input();
    r = input->init();
    if (!r) {
        MessageBoxA(hWindow, "Could not initialize input", "Error", MB_OK);
        return false;
    }

    graphics = new Graphics();
    r = graphics->init();
    if (!r) {
        return false;
    }
    ShowCursor(true);

    BringWindowToTop(GetNextWindow(hWindow, GW_HWNDNEXT));

    return true;
}

void System::Run() {
    MSG msg;
    while(running) {
        input->readInput();
        if (input->keyPressed(DIK_ESCAPE)) {
            //running = false;
            //break;
        }
        graphics->render();
        Sleep(5);

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
}

void System::Shutdown() {
    if (graphics) {
        graphics->shutdown();
        graphics = nullptr;
    }

    if (input) {
        input->shutdown();
        input = nullptr;
    }
}

LRESULT System::HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}