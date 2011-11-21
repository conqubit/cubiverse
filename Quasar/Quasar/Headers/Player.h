#pragma once

#include "math/Math.h"
#include "Camera.h"

class Player {
public:
    Player();
    ~Player();

    void Init();
    void Shutdown();
    void Tick();
    Vector3D MovementVector();
    XMMATRIX GetMat();

    Vector3D pos, vel, dir;
    double yaw, pitch;
    BoundingBox bb;
    bool inAir;
};