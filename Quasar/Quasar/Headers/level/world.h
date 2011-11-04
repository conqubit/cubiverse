#pragma once

#include "level/chunk.h"

class World {
public:
    World();
    ~World();
    void Init(int chunksX, int chunksY, int chunksZ);
    void Shutdown();

    Block GetBlock(int x, int y, int z);
    Chunk* GetChunk(int cx, int cy, int cz);

    bool InBlockBounds(int x, int y, int z) {
        return x >= 0 && x < widthX &&
               y >= 0 && y < widthY &&
               z >= 0 && z < widthZ;
    }

    void Generate();
    void ConstructChunkModel(Chunk* c);

    int chunksX, chunksY, chunksZ;
    int widthX, widthY, widthZ;
    int numChunks;

    Chunk* chunks;
};