#pragma once

#include "math/Math.h"
#include "Camera.h"

class Player {
public:
    Player();
    ~Player();

    void Init();
    void Shutdown();

    Vector3D pos;
    Vector3D vel;

    FreeCamera camera;
};