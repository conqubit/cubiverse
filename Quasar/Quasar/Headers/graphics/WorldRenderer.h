#pragma once

#include "containers/ArrayList.h"
#include "graphics/VisibleChunk.h"
#include "graphics/IRender.h"
#include "level/World.h"
#include "level/Chunk.h"
#include "level/Block.h"

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