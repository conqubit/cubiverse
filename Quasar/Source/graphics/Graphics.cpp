#include "stdafx.h"

#include "graphics/Graphics.h"
#include "System.h"

Graphics::Graphics() :
things(),
proj() {
}

Graphics::~Graphics() {
}

bool Graphics::Init() {
    glewInit(); 
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

void Graphics::SetProjection() {
    proj = glm::perspective(80.0f, (float)System::window.GetWidth() / System::window.GetHeight(), 0.01f, 250.0f);
}

void Graphics::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < things.size(); i++) {
        things[i]->Render();
    }
}