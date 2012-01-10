#include "stdafx.h"

#include "System.h"
#include "Window.h"
#include "Game.h"
#include "graphics/Graphics.h"

std::vector<IRenderObject*> Graphics::things;
glm::mat4 Graphics::proj;
glm::mat4 Graphics::ortho;

bool Graphics::Init() {
	glewInit();

	if (!GLEW_VERSION_2_1) {
		printerr("At least OpenGL version 2.1+ is required. Version found on this computer: " + str(glGetString(GL_VERSION)));
		printerr("Try updating your video drivers.");
		return false;
	}

	glClearDepth(1.0);
	glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	SetProjection();

	return true;
}

void Graphics::Shutdown() {
	things.clear();
}

void Graphics::InitGraphics() {
	for (int i = 0; i < things.size(); i++) {
		things[i]->InitGraphics();
	}
}

void Graphics::ShutdownGraphics() {
	for (int i = 0; i < things.size(); i++) {
		things[i]->ShutdownGraphics();
	}
}

void Graphics::SetProjection() {
	double aspect = Window::AspectRatio();
	proj = glm::perspective((float)Config::Graphics::FOV, (float)aspect, 0.05f, 250.0f);
	ortho = glm::ortho(-aspect, aspect, -1.0, 1.0, 1.0, -1.0);
}

const glm::mat4& Graphics::GetOrtho() {
	return ortho;
}

glm::mat4 Graphics::GetViewProj() {
	return proj * Game::player->View();
}

void Graphics::Render() {
	for (int i = 0; i < things.size(); i++) {
		things[i]->Render();
	}
}

void Graphics::ClearAll() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::ClearColor() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}