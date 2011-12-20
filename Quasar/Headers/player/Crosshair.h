#pragma once

#include "graphics/IRender.h"

class Model;

class Crosshair : IRenderObject {
public:
    Model* model;

    Crosshair() :
    model(nullptr) {
    }

    virtual void Render();
    virtual void InitGraphics();
    virtual void ShutdownGraphics();
};