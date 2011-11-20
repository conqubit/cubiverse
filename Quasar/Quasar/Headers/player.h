#pragma once

#include "math/math.h"
#include "camera.h"

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