#include "stdafx.h"

#include "System.h"
#include "graphics/WorldRenderer.h"

bool             System::running;
sf::RenderWindow System::window;
Graphics*        System::graphics;
Player*          System::player;
World*           System::world;
WorldRenderer*   System::worldRenderer;
bool             System::focus = true;

bool             System::temp = false;

sf::Clock timer;


int anti = 0;
int depth = 24;
int stencil = 0;

int major = 3;
int minor = 3;

int limitFrameRate = 240;

bool System::Init() {
    srand((unsigned int)time(nullptr));

    window.Create(sf::VideoMode(1366, 768, 32), "Quasar", sf::Style::Default, sf::ContextSettings(depth, stencil, anti, major, minor));
    window.SetFramerateLimit(limitFrameRate);

    SendMessage(System::WindowHandle(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    print("OpenGL version: " + str(glGetString(GL_VERSION)));

    graphics = new Graphics();
    if (!graphics->Init()) {
        return false;
    }

    window.ShowMouseCursor(false);
    //window.EnableVerticalSync(true);

    while (!Input::Init()) {
        Input::Init();
    }

    world = new World();
    world->Init(4, 4, 4);
    world->Fill(Block::Air);
    world->Generate();
    
    worldRenderer = new WorldRenderer();
    worldRenderer->Init(world);

    worldRenderer->ConstructVisibleChunks();

    player = new Player();
    Vector3D p = world->width.ToDouble() / 2.0;
    p.z = world->width.z - 8;
    player->Init(p);

    graphics->things.push_back(worldRenderer);
    graphics->things.push_back(player);

    return true;
}

HWND System::WindowHandle() {
    return (HWND)window.GetSystemHandle();
}

int oldTime;

int tick;
int accum;

int64 oldtime;
int64 newtime;
int64 freq;

sf::Event e;

sf::Font f;

void System::Start() {
    if (running) return;
    running = true;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&oldtime);

    f.LoadFromFile("res/consola.ttf");

    tick = freq / 1000 * 5;
    accum = tick;

    while(running) {
        DoTicks();

        graphics->Render();
        DoWindowDrawing();
        window.Display();
    }
}

void System::DoTicks() {
    QueryPerformanceCounter((LARGE_INTEGER*)&newtime);
    int delta = (newtime - oldtime);

    accum += delta;
    while (accum >= tick) {
        Tick();
        accum -= tick;
    }
    oldtime = newtime;
}

void System::Tick() {
    DoEvents();
    player->Tick();
}

void System::DoWindowDrawing() {
    window.SaveGLStates();
    sf::Text text("Position: " + player->pos.ToString(), f, 24);
    text.SetPosition(5, 0);
    window.Draw(text);
    window.RestoreGLStates();
}

bool fs = false;

sf::VideoMode prev;

void System::DoEvents() {
    while (window.PollEvent(e)) {
        switch (e.Type) {
        case EventType::Resized:
            // Update OpenGL size.
            glViewport(0, 0, e.Size.Width, e.Size.Height);
            // Adjust projection matrix, for potential changes in aspect ratio.
            graphics->SetProjection();
            // For SFML draw calls. Otherwise SFML drawing will be to a stretched surface.
            window.SetView((sf::View(sf::FloatRect(0, 0, e.Size.Width, e.Size.Height))));
            break;
        case EventType::Closed:
            Stop();
            return;
        case EventType::KeyPressed:
            switch (e.Key.Code) {
            case Key::Escape:
                if (!focus) {
                    Stop();
                    return;
                }
                window.ShowMouseCursor(true);
                focus = false;
                break;
            case Key::F11:
                fs = !fs;

                player->Shutdown();
                worldRenderer->Shutdown();
                graphics->Shutdown();

                if (fs) {
                    window.Create(sf::VideoMode::GetFullscreenModes()[0], "Quasar", sf::Style::Fullscreen, sf::ContextSettings(depth, stencil, anti, major, minor));
                    window.SetFramerateLimit(limitFrameRate);
                } else {
                    window.Create(sf::VideoMode(1366, 768, 32), "Quasar", sf::Style::Default, sf::ContextSettings(depth, stencil, anti, major, minor));
                    window.SetFramerateLimit(limitFrameRate);
                }

                graphics->Init();
                Input::Init();

                window.ShowMouseCursor(false);

                worldRenderer->Init(world);
                worldRenderer->ConstructVisibleChunks();
                player->Init(player->pos);

                graphics->things.push_back(worldRenderer);
                graphics->things.push_back(player);

                break;
            }
            break;
        case EventType::MouseButtonPressed:
            if (!focus) {
                temp = true;
            } else {
                temp = false;
            }
            window.ShowMouseCursor(false);
            focus = true;
            break;
        case EventType::LostFocus:
            focus = false;
            break;
        case EventType::GainedFocus:
            focus = true;
            break;
        case EventType::MouseLeft:
            break;
        case EventType::MouseMoved:
            Input::mx = e.MouseMove.X;
            Input::my = e.MouseMove.Y;
            break;
        }
    }
    if (focus) {
        Input::SetMousePosition(window.GetWidth() / 2, window.GetHeight() / 2);
    }
    Input::ReadMouse();
}

void System::Stop() {
    running = false;
}

void System::Shutdown() {
    if (graphics) {
        graphics->Shutdown();
        graphics = nullptr;
    }
}