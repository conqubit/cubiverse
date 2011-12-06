#pragma once

#include "math/Math.h"
#include "graphics/Model.h"
#include "graphics/IRender.h"

class Player : public IRender {
private:
    Vector3D vel;
public:
    Vector3D pos, dir, kvec, cameraUp;
    Vector3I playerUp;

    glm::dmat4 orientation;

    bool noclip;

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
    noclip(),
    yaw(), pitch(),
    bb(),

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

    Vector3D ToWorld(Vector3D v) {
        glm::dvec4 r = orientation * glm::dvec4(v.x, v.y, v.z, 1.0);
        return Vector3D(r.x, r.y, r.z);
    }

    Vector3D FromWorld(Vector3D v) {
        glm::dvec4 r = glm::core::function::matrix::inverse(orientation) * glm::dvec4(v.x, v.y, v.z, 1.0);
        return Vector3D(r.x, r.y, r.z);
    }

    void Render();
    void PickBlock();

    void DoInput();
    void UpdateVelocity();
    void DoJump();
    void DoCollision();
    void DoBlockPicking();
    void DoOrient();

    void UpdatePosition() {
        pos += vel;
    }

    /*Vector3D Vel() {
        return ToWorld(vel);
    }*/

    glm::mat4 View() {
        return glm::lookAt(Eye().ToGlmVec3(), (Eye() + ToWorld(dir)).ToGlmVec3(), ToWorld(cameraUp).ToGlmVec3());
    }

    BoundingBox BBox() {
        return BoundingBox(ToWorld(bb.Min()), ToWorld(bb.Max())).Offset(pos);
    }

    Vector3D Eye() {
        return pos + ToWorld(Vector3D(0, 0, eyeHeight));
    }
};