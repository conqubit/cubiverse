#include "stdafx.h"

#include "graphics/Graphics.h"
#include "Window.h"
#include "System.h"

sf::RenderWindow Window::sfWindow;
bool Window::temp = false;

bool Window::focus = true;

int anti = 0;
int depth = 24;
int stencil = 8;

int major = 2;
int minor = 1;

int limitFrameRate = 240;

bool vsync = false;

bool resize = false;

sf::VideoMode videoMode;
string title;
int style;
sf::ContextSettings contextSettings;

bool fs = false;

sf::Font f;

struct WindowState {
    bool maximized;
    int posX;
    int posY;
};

WindowState stateBeforeFullscreen;

bool Window::Init() {

    sf::VideoMode d = sf::VideoMode::GetDesktopMode();

    videoMode = sf::VideoMode((double)d.Width / PHI, (double)d.Height / PHI, 24);

    title = "Quasar";

    style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;

    contextSettings = sf::ContextSettings(depth, stencil, anti, major, minor);

    sfWindow.Create(videoMode, title, style, contextSettings);
    sfWindow.SetFramerateLimit(limitFrameRate);
    sfWindow.EnableVerticalSync(vsync);

    f.LoadFromFile("res/consola.ttf");

    return true;
}

void Window::Focus() {
    focus = true;
}

void Window::Unfocus() {
    focus = false;
}

void Window::Maximize() {
    ShowWindow(SystemHandle(), SW_MAXIMIZE);
}

bool Window::IsMaximized() {
    return IsZoomed(SystemHandle()) != 0;
}

bool Window::IsMinimized() {
    return IsIconic(SystemHandle()) != 0;
}

bool Window::IsFullScreen() {
    return fs;
}

sf::WindowHandle Window::SystemHandle() {
    return sfWindow.GetSystemHandle();
}

int Window::Width() {
    return sfWindow.GetWidth();
}

int Window::Height() {
    return sfWindow.GetHeight();
}

int Window::PosX() {
    RECT r;
    GetWindowRect(SystemHandle(), &r);
    return r.left;
}

int Window::PosY() {
    RECT r;
    GetWindowRect(SystemHandle(), &r);
    return r.top;
}

double Window::AspectRatio() {
    return (double)Width() / Height();
}

extern int64 fpsTicks;
extern int fpsCount;
extern int currentFPS;

extern int64 freq;

void Window::DoWindowDrawing() {
    sfWindow.SaveGLStates();

    double factor = (double)Height() / 800.0;

    Vector3D p = System::player->pos - System::world->width.ToDouble() / 2.0;
    sf::Text text("Position: " + p.ToString(2), f, 20 * factor);
    text.SetPosition(5 * factor, 0);
    sfWindow.Draw(text);

    if (fpsTicks >= freq / 2) {
        currentFPS = fpsCount;
        fpsTicks = 0;
        fpsCount = 0;
    }

    text.SetString("FPS: " + str(currentFPS * 2));
    text.SetPosition(5 * factor, 30 * factor);
    sfWindow.Draw(text);

    text.SetString("Up: " + System::player->playerUp.ToString());
    text.SetPosition(5 * factor, 60 * factor);
    sfWindow.Draw(text);

    sfWindow.RestoreGLStates();
}

void Window::DoResize() {
    if (resize) {
        // Update OpenGL size.
        glViewport(0, 0, Width(), Height());
        // Adjust projection matrix, for potential changes in aspect ratio.
        Graphics::SetProjection();
        // For SFML draw calls. Otherwise SFML drawing will be to a stretched surface.
        sfWindow.SetView((sf::View(sf::FloatRect(0, 0, Width(), Height()))));
        resize = false;
    }
}

void Window::ToggleFullscreen() {
    fs = !fs;

    System::player->Shutdown();
    System::worldRenderer->Shutdown();
    Graphics::Shutdown();
    Input::Shutdown();
    resize = true;

    if (fs) {
        stateBeforeFullscreen.maximized = IsMaximized();
        stateBeforeFullscreen.posX = PosX();
        stateBeforeFullscreen.posY = PosY();
        sfWindow.Create(sf::VideoMode::GetFullscreenModes()[0], title, sf::Style::Fullscreen, contextSettings);
    } else {
        sfWindow.Create(videoMode, title, style, contextSettings);
        if (stateBeforeFullscreen.maximized) {
            Maximize();
        } else {
            sfWindow.SetPosition(stateBeforeFullscreen.posX, stateBeforeFullscreen.posY);
        }
    }

    sfWindow.Display();

    sfWindow.SetFramerateLimit(limitFrameRate);
    sfWindow.EnableVerticalSync(vsync);

    sfWindow.ShowMouseCursor(Input::locked);

    Graphics::Init();

    System::worldRenderer->Init(System::world);
    System::worldRenderer->ConstructVisibleChunks();

    System::player->InitGraphics();

    Graphics::things.push_back(System::worldRenderer);
    Graphics::things.push_back(System::player);
}

sf::Event e;

void Window::DoEvents() {
    while (sfWindow.PollEvent(e)) {
        switch (e.Type) {
        case EventType::Resized:
            resize = true;
            if (!IsMaximized() && !IsFullScreen()) {
                videoMode.Width = Width();
                videoMode.Height = Height();
            }
            break;
        case EventType::Closed:
            System::Stop();
            return;
        case EventType::KeyPressed:
            switch (e.Key.Code) {
            case Key::Escape:
                sfWindow.ShowMouseCursor(true);
                Input::Lock();
                break;
            case Key::F11:
                ToggleFullscreen();
                break;
            case Key::Z:
                System::player->noclip = !System::player->noclip;
                break;
            }
            break;
        case EventType::MouseButtonPressed:
            if (!focus) {
                temp = true;
            } else {
                temp = false;
            }
            sfWindow.ShowMouseCursor(false);
            Input::Unlock();
            break;
        case EventType::LostFocus:
            Unfocus();
            Input::Lock();
            sfWindow.ShowMouseCursor(true);
            break;
        case EventType::GainedFocus:
            Focus();
            break;
        case EventType::MouseLeft:
            break;
        case EventType::MouseMoved:
            Input::mx = e.MouseMove.X;
            Input::my = e.MouseMove.Y;
            break;
        }
    }
    if (!Input::locked) {
        Input::SetMousePosition(Width() / 2, Height() / 2);
    }
    Input::ReadKeyboard();
    Input::ReadMouse();
}