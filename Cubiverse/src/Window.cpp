#include "stdafx.h"

#include "Input.h"
#include "Game.h"
#include "graphics/Graphics.h"
#include "Window.h"
#include "System.h"
#include "Config.h"

bool Window::focusBeforeEvents = true;

int anti = 0;
int depth = 24;
int stencil = 8;

int major = 2;
int minor = 1;

int limitFrameRate = 240;

bool vsync = false;

GLFWvidmode desktopMode;
GLFWvidmode fullscreenMode2[100];
GLFWvidmode fullscreenMode;
string title;

bool fs = false;

struct WindowState {
	bool maximized;
	int width, height;
	int x, y;
};

WindowState desktopModeState;

void GLFWCALL Window::KeyCallback(int key, int action) {
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_ESC:
			glfwEnable(GLFW_MOUSE_CURSOR);
			Input::Lock();
			break;
		case GLFW_KEY_F11:
			ToggleFullscreen();
			break;
		}
		break;
	case GLFW_RELEASE:
		break;
	}
}

void GLFWCALL Window::CharCallback(int character, int action) {
	switch (action) {
	case GLFW_PRESS:
		switch (character) {
		case 'z':
			Game::player->noclip = !Game::player->noclip;
			break;
		}
		break;
	case GLFW_RELEASE:
		break;
	}
}

void GLFWCALL Window::MouseButtonCallback(int button, int action) {
	switch (action) {
	case GLFW_PRESS:
		Input::Unlock();
		//switch (button) {
		//}
		break;
	case GLFW_RELEASE:
		//switch (button) {
		//}
		break;
	}
}

void GLFWCALL Window::MousePositionCallback(int x, int y) {
	//print(str(x) + ' ' + str(y));
}


int GLFWCALL Window::CloseCallback() {
	ClearCallbacks();
	System::Stop();
	return GL_FALSE;
}


void GLFWCALL Window::ResizeCallback(int width, int height) {
	if (!IsMaximized() && !fs) {
		desktopModeState.width = width;
		desktopModeState.height = height;
	}
	glViewport(0, 0, width, height);
	Graphics::SetProjection();
	System::Update();
}


bool Window::Init() {
	glfwGetDesktopMode(&desktopMode);

	desktopModeState.width = desktopMode.Width / PHI;
	desktopModeState.height = desktopMode.Height / PHI;
	desktopModeState.x = (desktopMode.Width - desktopModeState.width) / 2;
	desktopModeState.y = (desktopMode.Height - desktopModeState.height) / 2;

	title = "Cubiverse " + Config::Version;
	//style = sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize;
	anti = Config::Graphics::MultiSampling;
	vsync = Config::Graphics::VSync;
	limitFrameRate = Config::Graphics::FrameRateLimit;
	//contextSettings = sf::ContextSettings(depth, stencil, anti, major, minor);

	glfwOpenWindow(desktopModeState.width, desktopModeState.height, 8, 8, 8, 8, depth, stencil, GLFW_WINDOW);
	glfwSetWindowPos(desktopModeState.x, desktopModeState.y);
	glfwSetWindowTitle(("Cubiverse " + Config::Version).c_str());

	glfwSwapInterval(0);
	glfwDisable(GLFW_AUTO_POLL_EVENTS);

	//sfWindow.SetFramerateLimit(limitFrameRate);
	//sfWindow.EnableVerticalSync(vsync);

	//SetIcon("res/icon.png");


	return true;
}

void Window::SetCallbacks() {
	glfwSetWindowSizeCallback(ResizeCallback);
	glfwSetKeyCallback(KeyCallback);
	glfwSetCharCallback(CharCallback);
	glfwSetWindowCloseCallback(CloseCallback);
	glfwSetMouseButtonCallback(MouseButtonCallback);
	glfwSetMousePosCallback(MousePositionCallback);
}

void Window::ClearCallbacks() {
	glfwSetWindowSizeCallback(nullptr);
	glfwSetKeyCallback(nullptr);
	glfwSetCharCallback(nullptr);
	glfwSetWindowCloseCallback(nullptr);
	glfwSetMouseButtonCallback(nullptr);
	glfwSetMousePosCallback(nullptr);
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

void Window::Maximize() {
	ShowWindow(SystemHandle(), SW_MAXIMIZE);
}

bool Window::IsMaximized() {
	return IsZoomed(SystemHandle());
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
	int width;
	glfwGetWindowSize(&width, nullptr);
	return width;
}

int Window::Height() {
	int height;
	glfwGetWindowSize(nullptr, &height);
	return height;
}

int Window::X() {
	RECT r;
	GetWindowRect(SystemHandle(), &r);
	return r.left;
}

int Window::Y() {
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

void Window::ToggleFullscreen() {
	fs = !fs;

	ClearCallbacks();

	Graphics::ShutdownGraphics();

	Input::Shutdown();
	Res::Shutdown();

	if (fs) {
		desktopModeState.maximized = IsMaximized();
		desktopModeState.x = X();
		desktopModeState.y = Y();
		glfwCloseWindow();
		glfwOpenWindow(desktopMode.Width, desktopMode.Height, 8, 8, 8, 8, depth, stencil, GLFW_FULLSCREEN);
	} else {
		glfwCloseWindow();
		glfwOpenWindow(desktopModeState.width, desktopModeState.height, 8, 8, 8, 8, depth, stencil, GLFW_WINDOW);
		glfwSetWindowTitle(("Cubiverse " + Config::Version).c_str());
		if (desktopModeState.maximized) {
			Maximize();
		} else {
			glfwSetWindowPos(desktopModeState.x, desktopModeState.y);
		}
	}

	Display();

	Graphics::Init();
	Res::Init();
	Input::Init();

	Graphics::InitGraphics();

	SetCallbacks();
}

void Window::DoEvents() {

	bool oldHasFocus = HasFocus();

	glfwPollEvents();

	if (HasFocus() && !oldHasFocus) {
		Input::Lock();
	}

	Input::ReadKeyboard();
	Input::ReadMouse();
}

void Window::Display() {
	glfwSwapBuffers();
}