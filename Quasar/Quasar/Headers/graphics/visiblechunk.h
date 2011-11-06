#pragma once

#include "level/world.h"
#include "level/block.h"
#include "graphics/model.h"
#include "containers/skiplist.h"

class VisibleChunk {
public:
    VisibleChunk();
    ~VisibleChunk();

    Model model;
    SkipList<int> visibleBlocks;
};