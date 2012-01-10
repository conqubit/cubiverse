#include "stdafx.h"

#include "Input.h"
#include "Game.h"
#include "graphics/Graphics.h"
#include "Window.h"
#include "System.h"
#include "Config.h"

int depth = 24;
int stencil = 0;

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

bool Window::Init() {
	glfwGetDesktopMode(&desktopMode);

	desktopModeState.width = desktopMode.Width / PHI;
	desktopModeState.height = desktopMode.Height / PHI;
	desktopModeState.x = (desktopMode.Width - desktopModeState.width) / 2;
	desktopModeState.y = (desktopMode.Height - desktopModeState.height) / 2;

	title = "Cubiverse " + Config::Version;

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, Config::Graphics::MultiSampling);

	bool opened = glfwOpenWindow(desktopModeState.width, desktopModeState.height, 8, 8, 8, 0, depth, stencil, GLFW_WINDOW);
	if (!opened) {
		return false;
	}

	glfwSetWindowPos(desktopModeState.x, desktopModeState.y);
	glfwSetWindowTitle(title.c_str());

	glfwSwapInterval(ShouldVSync());
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
}

void Window::ClearCallbacks() {
	glfwSetWindowSizeCallback(nullptr);
	glfwSetKeyCallback(nullptr);
	glfwSetCharCallback(nullptr);
	glfwSetWindowCloseCallback(nullptr);
	glfwSetMouseButtonCallback(nullptr);
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

bool Window::IsFullscreen() {
	return fs;
}

bool Window::ShouldVSync() {
	return Config::Graphics::VSync && IsFullscreen();
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
	return (double)Width() / (Height() > 1 ? Height() : 1);
}

void Window::ToggleFullscreen() {
	fs = !fs;

	ClearCallbacks();

	Graphics::ShutdownGraphics();

	Input::Shutdown();
	Res::Shutdown();

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, Config::Graphics::MultiSampling);

	if (fs) {
		desktopModeState.maximized = IsMaximized();
		desktopModeState.x = X();
		desktopModeState.y = Y();
		glfwCloseWindow();
		glfwOpenWindow(desktopMode.Width, desktopMode.Height, 8, 8, 8, 8, depth, stencil, GLFW_FULLSCREEN);
	} else {
		glfwCloseWindow();
		glfwOpenWindow(desktopModeState.width, desktopModeState.height, 8, 8, 8, 8, depth, stencil, GLFW_WINDOW);
		glfwSetWindowTitle(title.c_str());
		if (desktopModeState.maximized) {
			Maximize();
		} else {
			glfwSetWindowPos(desktopModeState.x, desktopModeState.y);
		}
	}

	glfwSwapInterval(ShouldVSync());
	glfwDisable(GLFW_AUTO_POLL_EVENTS);

	if (Input::locked) {
		Input::Lock();
	} else {
		Input::Unlock();
	}

	Graphics::Init();
	Res::Init();
	Input::Init();

	Graphics::InitGraphics();

	SetCallbacks();
}

void Window::Display() {
	static double oldTime = glfwGetTime();

	if (Config::Graphics::FrameRateLimit > 0 && !ShouldVSync()) {
		double newTime = glfwGetTime();
		double delta = newTime - oldTime;
		double frameDelta = 1.0 / Config::Graphics::FrameRateLimit;
		if (delta < frameDelta) {
			double remaining = frameDelta - delta;
			System::SleepSeconds(frameDelta - delta);
		}
		oldTime = newTime;
	}

	glfwSwapBuffers();
}


/******** EVENTS ********/

void Window::DoEvents() {
	bool oldFocus = HasFocus();

	glfwPollEvents();

	bool newFocus = HasFocus();
	if (newFocus != oldFocus) {
		FocusChangedCallback(newFocus);
	}

	Input::ReadMouse();
}


/******** CALLBACKS ********/

void GLFWCALL Window::KeyCallback(int key, int action) {
	Game::KeyCallback(key, action);

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
	Game::KeyCallback(toupper(character), action);

	switch (action) {
	case GLFW_PRESS:
		break;
	case GLFW_RELEASE:
		break;
	}
}

void GLFWCALL Window::MouseButtonCallback(int button, int action) {
	switch (action) {
	case GLFW_PRESS:
		Input::Unlock();
		break;
	case GLFW_RELEASE:
		break;
	}
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
	glViewport(0, 0, width, height > 1 ? height : 1);
	Graphics::SetProjection();
	System::Update();
}

void Window::FocusChangedCallback(bool focused) {
	static bool lockedState = false;

	if (!focused) {
		lockedState = Input::locked;
		Input::Lock();
	} else {
		if (lockedState) {
			Input::Lock();
		} else {
			Input::Unlock();
		}
	}
}