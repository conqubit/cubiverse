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
    void ConstructChunkModelData(Chunk* c, VisibleChunk* vs);

    VisibleChunk* ConstructNewVisibleChunk(Chunk* c);

    static void ConstructFace(int block, const Vector3I& side, const Vector3I& p, int x, int y, int z, int xi, int yi, int zi, double b);
    void UpdateBlock(const Vector3I& p);
    void UpdateChunk(Chunk* c, const Vector3I& p);
    void Render();
    void ConstructBlock(const Vector3I& p);
    void UpdateMesh(const Vector3I& p);
    std::vector<VisibleChunk*> visibleChunks;
    World* world;
};