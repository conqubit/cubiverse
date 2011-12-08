#pragma once

#include "level/World.h"
#include "level/Block.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"

class VisibleChunk {
public:
    VisibleChunk();
    ~VisibleChunk();

    void UpdateModel(const ModelFactory& mf);

    void Shutdown();

    Model* model;
    Chunk* chunk;
};