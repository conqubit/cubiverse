#pragma once

#include "math/Math.h"
#include "graphics/Model.h"
#include "graphics/IRender.h"

class Player : public IRenderObject {
private:
    Vector3D vel;
public:
    Vector3D pos, dir, kvec, cameraUp;
    Vector3I playerUp;
    Vector3D smoothUp;

    glm::dmat4 orientation;
    glm::dmat4 smoothOrientation;

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

    Player() :
    noclip(),
    yaw(), pitch(),
    bb(),

    selectedBlock(),
    picked(),

    counter(),
    inAir(), mouseStateLastTick() {
    }

    void Init();
    void Shutdown();

    virtual void InitGraphics();
    virtual void ShutdownGraphics();

    void Tick();

    Vector3D ToWorld(Vector3D v) {
        glm::dvec4 r = orientation * glm::dvec4(v.ToGlmVec4());
        return Vector3D(r.x, r.y, r.z);
    }

    Vector3D ToWorldSmooth(Vector3D v) {
        glm::dvec4 r = smoothOrientation * glm::dvec4(v.ToGlmVec4());
        return Vector3D(r.x, r.y, r.z);
    }

    Vector3D FromWorld(Vector3D v) {
        glm::dvec4 r = glm::core::function::matrix::inverse(orientation) * glm::dvec4(v.x, v.y, v.z, 1.0);
        return Vector3D(r.x, r.y, r.z);
    }

    Vector3D FromWorldSmooth(Vector3D v) {
        glm::dvec4 r = glm::core::function::matrix::inverse(smoothOrientation) * glm::dvec4(v.x, v.y, v.z, 1.0);
        return Vector3D(r.x, r.y, r.z);
    }

    virtual void Render();
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
        return glm::lookAt(Eye().ToGlmVec3(), //
                           (Eye() + ToWorldSmooth(dir)).ToGlmVec3(), //
                           ToWorldSmooth(cameraUp).ToGlmVec3());
    }

    BoundingBox BBox() {
        return BoundingBox(ToWorld(bb.Min()), ToWorld(bb.Max())).Offset(pos);
    }

    Vector3D Eye();
};