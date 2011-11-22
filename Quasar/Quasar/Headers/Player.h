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

    Vector3I pickedBlock;
    Vector3I side;
    int counter;
    bool picked, lastMouse;
    void PickBlock();
    Vector3D MovementVector();
    XMMATRIX GetMat();

    Vector3D Eye() {
        return Vector3D(pos.x, pos.y, pos.z + 5.0 / 3.0);
    }

    Vector3D pos, vel, dir;
    double yaw, pitch;
    BoundingBox bb;
    bool inAir;
};