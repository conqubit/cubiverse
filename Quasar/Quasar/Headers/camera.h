#pragma once
#include "math2.h"

class FreeCamera {
public:
    FreeCamera();
    ~FreeCamera();
    void DoInput();
    XMMATRIX GetMat();

    Vector3 pos;
    Vector3 dir;
    float yaw;
    float pitch;
};