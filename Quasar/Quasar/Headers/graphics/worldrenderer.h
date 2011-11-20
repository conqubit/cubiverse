#pragma once

#include "containers/arraylist.h"
#include "graphics/visiblechunk.h"
#include "graphics/irender.h"
#include "level/world.h"
#include "level/chunk.h"
#include "level/block.h"

class WorldRenderer : public IRender {
public:
    WorldRenderer();
    ~WorldRenderer();
    void Init(World* w);
    void Shutdown();
    bool ConstructVisibleChunks();
    void Render();

private:
    ArrayList<VisibleChunk*> visibleChunks;
    World* world;
};