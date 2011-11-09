#include "stdafx.h"
#include "system.h"

HINSTANCE System::hInstance = nullptr;
HWND System::hWindow = nullptr;
bool System::running = true;
Graphics* System::graphics = nullptr;
Input* System::input = nullptr;


#include "containers/arraylist.h"
#include "level/chunk.h"

bool System::Init(HINSTANCE hInst, HWND hWnd) {
    srand((unsigned int)time(nullptr));

    hInstance = hInst;
    hWindow = hWnd;
    input = new Input();
    if (!input->Init()) {
        MessageBoxA(hWindow, "Could not initialize input", "Error", MB_OK);
        return false;
    }

    graphics = new Graphics();
    if (!graphics->Init()) {
        return false;
    }

    ShowCursor(true);

    BringWindowToTop(GetNextWindow(hWindow, GW_HWNDNEXT));





    Vector3i v(Vector3d());




    return true;
}

void System::Run() {
    MSG msg;
    while(running) {
        input->ReadInput();
        if (input->KeyPressed(DIK_ESCAPE)) {
            //running = false;
            //break;
        }
        graphics->Render();
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