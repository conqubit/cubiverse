#pragma once

#include "graphics/direct3d.h"
#include "camera.h"

class Graphics {
public:
    Graphics();
    ~Graphics();
    bool Init();
    void Render();
    void Shutdown();

    ID3D11Device* Device() {
        return direct3D->device;
    }

    ID3D11DeviceContext* DeviceContext() {
        return direct3D->deviceContext;
    }

    Direct3D* direct3D;

    FreeCamera camera;
    Matrix viewMat;
    Matrix projMat;
};