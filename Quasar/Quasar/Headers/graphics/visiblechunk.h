#pragma once

#include "stdafx.h"
#include "level/world.h"
#include "level/block.h"
#include "graphics/model.h"
#include "containers/skiplist.h"

class VisibleChunk {
public:
    static bool Create(World* w, Chunk* c);
    VisibleChunk();
    ~VisibleChunk();

    Model model;
    SkipList< vertices;
};