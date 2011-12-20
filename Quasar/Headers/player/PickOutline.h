#pragma once

#include "graphics/IRender.h"

class Model;

class PickOutline : IRenderObject {
public:
    Model* x0;
    Model* x1;
    Model* y0;
    Model* y1;
    Model* z0;
    Model* z1;

    Vector3I pos, side;

    PickOutline() {
        x0 = x1 = y0 = y1 = z0 = z1 = nullptr;
    }

    virtual void Render();
    virtual void InitGraphics();
    virtual void ShutdownGraphics();
};