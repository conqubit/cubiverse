#include "stdafx.h"
#include "resource.h"

#include "System.h"

#define MAX_LOADSTRING 100

HINSTANCE hInstance;
HWND hWindow;

ATOM				RegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);

int width = 1600;
int height = 900;
RECT r = {0, 0, width, height};

int main() {
    return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hInstance = hInst;

    RegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return 0;
    }

    if (!System::Init(hInstance, hWindow)) {
        return 0;
    }
    System::Run();
    System::Shutdown();

    return 0;
}

ATOM RegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wc;
    ZeroStruct(wc);
    wc.cbSize = sizeof(WNDCLASSEX);

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= WindowProc;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    wc.hCursor			= LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName	= L"TestWindowClass";
    wc.hIconSm			= LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wc);
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow) {
   AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);

   hWindow = CreateWindowEx(0, L"TestWindowClass", L"Slacker", WS_OVERLAPPEDWINDOW,
      0, 0, r.right-r.left, r.bottom-r.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWindow) {
      return FALSE;
   }

   ShowWindow(hWindow, nCmdShow);
   UpdateWindow(hWindow);

   return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return System::HandleMsg(hWnd, message, wParam, lParam);
    }
    return 0;
}