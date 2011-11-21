#include "stdafx.h"
#include "graphics/Graphics.h"
#include "System.h"

Graphics::Graphics() :
    direct3D(),
    camera(),
    viewMat(),
    projMat() {
}

Graphics::~Graphics() {
}

bool Graphics::Init() {
    direct3D = new Direct3D();

    if (!direct3D->Init()) {
        MessageBoxA(System::hWindow, "Could not initialize Direct3D", "Error", MB_OK);
        return false;
    }

    projMat.set(XMMatrixPerspectiveFovLH(60.0f / 180.0f * PI, 1.7777f, 0.01f, 1000.0f));

    return true;
}

void Graphics::Shutdown() {
    if (direct3D) {
        direct3D->Shutdown();
        direct3D = nullptr;
    }
}

void Graphics::Render() {
    direct3D->BeginScene();

    System::player->camera.DoInput();
    viewMat.set(System::player->camera.GetMat());

    for (int i = 0; i < things.Count(); i++) {
        things[i]->Render();
    }

    direct3D->EndScene();
}