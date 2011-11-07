#include "stdafx.h"
#include "graphics/graphics.h"
#include "system.h"

Graphics::Graphics() :
    direct3D(),
    camera(),
    viewMat(),
    projMat() {
    projMat.set(XMMatrixPerspectiveFovLH(60.0f/180.0f*XM_PI, 1.7777f, 0.01f, 1000.0f));
}

Graphics::~Graphics() {
}

bool Graphics::Init() {
    direct3D = new Direct3D();

    if (!direct3D->Init()) {
        MessageBoxA(System::hWindow, "Could not initialize Direct3D", "Error", MB_OK);
        return false;
    }

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

    camera.DoInput();
    viewMat.set(camera.GetMat());

    direct3D->EndScene();
}