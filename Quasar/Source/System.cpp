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


int anti = 4;

bool System::Init() {
    srand((unsigned int)time(nullptr));

    window.Create(sf::VideoMode(300, 300, 32), "Quasar", sf::Style::Default, sf::ContextSettings(24, 8, anti, 3, 3));
    SendMessage(System::WindowHandle(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);

    //window.SetFramerateLimit(200);

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
    player->Init();

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

void System::Start() {
    if (running) return;
    running = true;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&oldtime);

    tick = freq / 1000 * 5;
    accum = tick;

    while(running) {
        QueryPerformanceCounter((LARGE_INTEGER*)&newtime);
        int delta = (newtime - oldtime);

        accum += delta;
        while (accum >= tick) {
            Tick();
            accum -= tick;
        }
        oldtime = newtime;

        graphics->Render();

        glDisable(GL_CULL_FACE);
        sf::Text text("Position: " + player->pos.ToString());
        text.SetCharacterSize(24);
        window.Draw(text);
        glEnable(GL_CULL_FACE);

        window.Display();
        
    }
}

void System::Tick() {
    DoEvents();
    player->Tick();
}

bool fs = false;

sf::VideoMode prev;

void System::DoEvents() {
    while (window.PollEvent(e)) {
        switch (e.Type) {
        case EventType::Resized:
            glViewport(0, 0, e.Size.Width, e.Size.Height);
            graphics->SetProjection();
            window.SetView((sf::View(sf::FloatRect(0, 0, e.Size.Width, e.Size.Height))));
            //window.SetSize(e.Size.Width, e.Size.Height);
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
                    window.Create(sf::VideoMode::GetFullscreenModes()[0], "Quasar", sf::Style::Fullscreen, sf::ContextSettings(24, 8, anti, 2, 0));
                    //window.SetFramerateLimit(120);
                } else {
                    window.Create(sf::VideoMode(1366, 768, 32), "Quasar", sf::Style::Default, sf::ContextSettings(24, 8, anti, 2, 0));
                }

                graphics->Init();
                Input::Init();

                window.ShowMouseCursor(false);

                worldRenderer->Init(world);
                worldRenderer->ConstructVisibleChunks();
                player->Init();

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