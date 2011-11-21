#pragma once
#include "math/Math.h"

class FreeCamera {
public:
    FreeCamera();
    ~FreeCamera();
    void DoInput();
    XMMATRIX GetMat();

    Vector3D pos, dir;
    double yaw, pitch;
};