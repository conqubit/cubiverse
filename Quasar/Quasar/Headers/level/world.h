#pragma once

#include "level/chunk.h"

class World {
public:
    World();
    ~World();
    void Init(int chunksX, int chunksY, int chunksZ);
    void Shutdown();

    Block GetBlock(int x, int y, int z) {
        if (!InBlockBounds(x, y, z)) return Block::Undefined;
        GetChunk(x / Chunk::DIM, y / Chunk::DIM, z / Chunk::DIM)->GetBlock(x, y, z);
    }

    Chunk* GetChunk(int cx, int cy, int cz) {
        return chunks[(cx + cy * chunksX + cz * chunksX * chunksY)];
    }

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

    Chunk** chunks;
};