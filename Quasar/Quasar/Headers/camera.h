#pragma once
#include "math/math.h"

class FreeCamera {
public:
    FreeCamera();
    ~FreeCamera();
    void DoInput();
    XMMATRIX GetMat();

    Vec3 pos;
    Vec3 dir;
    float yaw;
    float pitch;
};