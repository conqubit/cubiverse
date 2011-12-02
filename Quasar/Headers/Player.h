#pragma once

#include "math/Math.h"
#include "graphics/Model.h"
#include "graphics/IRender.h"

class Player : public IRender {
public:
    Vector3D pos, vel, dir, up, kvec;

    double height;
    double eyeHeight;

    double yaw, pitch;
    BoundingBox bb;

    Vector3I pickedBlock, side;
    Model* selectedBlock;
    bool picked;

    int counter;
    bool inAir, mouseStateLastTick;

    Model* cursor;

    Player() :
    pos(), vel(), dir(), up(), kvec(),
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

    void Init(Vector3D p);
    void Shutdown();
    void Tick();

    void Render();
    void PickBlock();

    void DoInput();
    void UpdateVelocity();
    void DoJump();
    void DoCollision();
    void DoBlockPicking();

    void UpdatePosition() {
        pos += vel;
    }

    glm::mat4 View() {
        return glm::lookAt(Eye().ToGlmVec(), (Eye() + dir).ToGlmVec(), up.ToGlmVec());
    }

    BoundingBox BBox() {
        return bb.Offset(pos);
    }

    Vector3D Eye() {
        return Vector3D(pos.x, pos.y, pos.z + eyeHeight);
    }
};