#pragma once

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
    void ConstructChunkModelData(Chunk* c);

    VisibleChunk* ConstructNewVisibleChunk(Chunk* c);

    static void ConstructFace(ModelFactory& mf, int block, const Vector3I& side, const Vector3I& p, int x, int y, int z, int xi, int yi, int zi, double b);
    void UpdateBlock(Vector3I p);
    void UpdateChunk(Chunk* c);
    void Render();

    std::vector<VisibleChunk*> visibleChunks;
    World* world;
    Shader* shader;
    Texture* texture;
};