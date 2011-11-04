#include "stdafx.h"
#include "graphics/graphics.h"
#include "system.h"

Graphics::Graphics() {
    projMat.set(XMMatrixPerspectiveFovLH(60.0f/180.0f*XM_PI, 1.7777f, 0.01f, 1000.0f));
    direct3D = nullptr;
}

Graphics::~Graphics() {
}

bool Graphics::init() {
    cam = new FreeCamera();
    bool r;
    direct3D = new Direct3D();

    r = direct3D->init();
    if (!r) {
        MessageBoxA(System::hWindow, "Could not initialize Direct3D", "Error", MB_OK);
        return false;
    }

    if (!r) {
        return false;
    }

    return true;
}

void Graphics::shutdown() {
    if (direct3D) {
        direct3D->shutdown();
        direct3D = nullptr;
    }
}

ID3D11Device* Graphics::getDevice() {
    return direct3D->device;
}
ID3D11DeviceContext* Graphics::getDeviceContext() {
    return direct3D->deviceContext;
}

void Graphics::render() {
    direct3D->beginScene();

    cam->doInput();
    viewMat.set(cam->getMat());

    direct3D->endScene();
}