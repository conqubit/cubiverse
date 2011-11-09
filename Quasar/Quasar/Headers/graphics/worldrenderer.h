#pragma once

#include "containers/arraylist.h"
#include "graphics/visiblechunk.h"
#include "level/world.h"
#include "level/chunk.h"
#include "level/block.h"

class WorldRenderer {
public:
    WorldRenderer();
    ~WorldRenderer();
    void Init(World* w);
    void Shutdown();
    void ConstructVisibleChunks();
    void Render();

private:
    ArrayList<VisibleChunk> visibleChunks;
    World* world;
};