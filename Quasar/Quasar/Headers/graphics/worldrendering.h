#pragma once

#include "containers/arraylist.h"
#include "graphics/visiblechunk.h"
#include "level/world.h"
#include "level/chunk.h"
#include "level/block.h"

class WorldRendering {
public:
    WorldRendering();
    ~WorldRendering();
    void Init(World* w);
    void Shutdown();
    void ConstructVisibleChunks();

private:
    ArrayList<VisibleChunk> visibleChunks;
    World* world;
};