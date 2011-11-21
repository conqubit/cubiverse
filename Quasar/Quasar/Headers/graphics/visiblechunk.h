#pragma once

#include "level/World.h"
#include "level/Block.h"
#include "graphics/Model.h"
#include "containers/SkipList.h"

class VisibleChunk {
public:
    VisibleChunk();
    ~VisibleChunk();

    void Shutdown();

    Model* model;
    SkipList<int> visibleBlocks;
};