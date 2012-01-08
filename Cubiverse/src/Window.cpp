#include "stdafx.h"

#include "Input.h"
#include "Game.h"
#include "graphics/Graphics.h"
#include "Window.h"
#include "System.h"
#include "Config.h"

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

GLFWvidmode desktopMode;
GLFWvidmode fullscreenMode;
string title;
int style;

bool fs = false;

struct WindowState {
	bool maximized;
	int posX;
	int posY;
};

WindowState stateBeforeFullscreen;



void GLFWCALL Window::KeyCallback(int key, int action) {
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_ESC:
			glfwEnable(GLFW_MOUSE_CURSOR);
			Input::Lock();
			break;
		}
		break;
	case GLFW_RELEASE:
		break;
	}
}


int GLFWCALL Window::CloseCallback() {
	System::Stop();
	return GL_TRUE;
}


void GLFWCALL Window::ResizeCallback(int width, int height) {
	glViewport(0, 0, width, height);
	Graphics::SetProjection();
	System::Update();
}



bool Window::Init() {
	glfwGetDesktopMode(&desktopMode);
	glfwGetVideoModes(&fullscreenMode, 1);

	desktopMode.Width;
	desktopMode.Height;

	title = "Cubiverse " + Config::Version;
	//style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;
	anti = Config::Graphics::MultiSampling;
	vsync = Config::Graphics::VSync;
	limitFrameRate = Config::Graphics::FrameRateLimit;
	//contextSettings = sf::ContextSettings(depth, stencil, anti, major, minor);

	glfwOpenWindow(desktopMode.Width / PHI, desktopMode.Height / PHI, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);
	glfwSetWindowPos(desktopMode.Width * (1 - 1 / PHI) / 2, desktopMode.Height * (1 - 1 / PHI) / 2);
	glfwSetWindowTitle(("Cubiverse " + Config::Version).c_str());

	//sfWindow.SetFramerateLimit(limitFrameRate);
	//sfWindow.EnableVerticalSync(vsync);

	//SetIcon("res/icon.png");


	return true;
}

void Window::SetCallbacks() {
	glfwSetWindowSizeCallback(ResizeCallback);
	glfwSetKeyCallback(KeyCallback);
	glfwSetWindowCloseCallback(CloseCallback);
}

void Window::ClearCallbacks() {
	glfwSetWindowSizeCallback(nullptr);
	glfwSetKeyCallback(nullptr);
	glfwSetWindowCloseCallback(nullptr);
}

void Window::SetIcon(string file) {
	return; //
	//sf::Image image;
	//image.LoadFromFile(file);
	//sfWindow.SetIcon(image.GetWidth(), image.GetHeight(), image.GetPixelsPtr());
}

bool Window::HasFocus() {
	return glfwGetWindowParam(GLFW_ACTIVE);
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
	return glfwGetWindowParam(GLFW_ICONIFIED);
}

bool Window::IsFullScreen() {
	return fs;
}

HWND Window::SystemHandle() {
	return GetActiveWindow();
	//return sfWindow.GetSystemHandle();
}

int Window::Width() {
	int width, height;
	glfwGetWindowSize(&width, &height);
	return width;
}

int Window::Height() {
	int width, height;
	glfwGetWindowSize(&width, &height);
	return height;
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

void Window::DoResize() {
	if (resize) {
		// Update OpenGL viewport.
		glViewport(0, 0, Width(), Height());
		// Adjust projection matrix, for potential changes in aspect ratio.
		Graphics::SetProjection();
		// For SFML draw calls. Otherwise SFML drawing will be to a stretched surface.
		//sfWindow.SetView((sf::View(sf::FloatRect(0, 0, Width(), Height()))));
		resize = false;
	}
}

void Window::ToggleFullscreen() {
	/*fs = !fs;

	Graphics::ShutdownGraphics();

	Input::Shutdown();
	Res::Shutdown();

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

	SetIcon("res/icon.png");

	sfWindow.ShowMouseCursor(Input::locked);

	Graphics::Init();
	Res::Init();
	Input::Init();

	Graphics::InitGraphics();*/
}

void Window::DoEvents() {

	glfwPollEvents();

	if (!Input::locked) {
		Input::SetMousePosition(Width() / 2, Height() / 2);
	}

	Input::ReadKeyboard();
	Input::ReadMouse();

	/*while (sfWindow.PollEvent(e)) {
		switch (e.Type) {
		case EventType::Resized:
			resize = true;
			if (!IsMaximized() && !IsFullScreen()) {
				desktopMode.Width = Width();
				desktopMode.Height = Height();
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
				Game::player->noclip = !Game::player->noclip;
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
	Window::DoResize();*/
}

void Window::Display() {
	glfwSwapBuffers();
}