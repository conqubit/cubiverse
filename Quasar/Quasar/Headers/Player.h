#pragma once

#include "math/Math.h"
#include "graphics/Model.h"
#include "graphics/IRender.h"

class Player : public IRender {
public:
    Vector3D pos, vel, dir, kvec;
    double yaw, pitch;
    BoundingBox bb;

    Vector3I pickedBlock, side;
    Model* selectedBlock;
    bool picked;

    int counter;
    bool inAir, mouseStateLastTick;

    Model* cursor;

    Player() :
    pos(), vel(), dir(), kvec(),
    yaw(), pitch(),
    bb(),

    pickedBlock(), side(),
    selectedBlock(),
    picked(),

    counter(),
    inAir(), mouseStateLastTick(),
    
    cursor() {
    }

    ~Player() {
    }

    void Init();
    void Shutdown();
    void Tick();

    void Render();
    void PickBlock();

    void DoMouseKeyboardInput();
    void UpdateVelocity();
    void DoJump();
    void DoCollision();
    void DoBlockPicking();

    void UpdatePosition() {
        pos += vel;
    }

    XMMATRIX GetViewMatrix() {
        return XMMatrixLookToLH((pos + Vector3D(0, 0, 5.0 / 3.0)).ToXMVector(), dir.ToXMVector(), Vector3F::AXIS_Z.ToXMVector());
    }

    Vector3D Eye() {
        return Vector3D(pos.x, pos.y, pos.z + 5.0 / 3.0);
    }
};