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
    bool Init(World* w);
    void Shutdown();
    void ConstructVisibleChunks();
    VisibleChunk* ConstructVisibleChunk(Chunk* c);
    void UpdateBlock(Vector3I p);
    void UpdateChunk(Chunk* c);
    void Render();

    ArrayList<VisibleChunk*> visibleChunks;
    World* world;
    Shader* shader;
};