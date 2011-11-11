#pragma once
#include "math/math.h"

class FreeCamera {
public:
    FreeCamera();
    ~FreeCamera();
    void DoInput();
    XMMATRIX GetMat();

    Vector3D pos, dir;
    double yaw, pitch;
};