#pragma once

#include "containers/ArrayList.h"
#include "graphics/IRender.h"
#include "graphics/Direct3D.h"
#include "Camera.h"

class Graphics : public IRender {
public:
    Graphics();
    ~Graphics();
    bool Init();
    void Render();
    void Shutdown();

    ID3D10Device* Device() {
        return direct3D->device;
    }

    ArrayList<IRender*> things;

    Direct3D* direct3D;

    FreeCamera camera;
    Matrix viewMat;
    Matrix projMat;
};