#pragma once

#include "level/World.h"
#include "level/Block.h"
#include "graphics/Model.h"

class VisibleChunk {
public:
    VisibleChunk();
    ~VisibleChunk();

    void Shutdown();

    Model* model;
    Chunk* chunk;
};