#pragma once

#include "level/chunk.h"

class World {
public:
    World();
    ~World();

    void Init(int chunksX, int chunksY, int chunksZ);
    void Shutdown();

    void Fill(int t);
    void GenerateSphere(int t);

    int GetBlock(int x, int y, int z) {
        if (!InBlockBounds(x, y, z)) return Block::Air;
        return GetChunk(x / Chunk::DIM, y / Chunk::DIM, z / Chunk::DIM)->GetBlock(x, y, z);
    }

    int GetBlock(Vector3I p) {
        return GetBlock(p.x, p.y, p.z);
    }

    bool SetBlock(int x, int y, int z, int t) {
        if (!InBlockBounds(x, y, z)) return false;
        GetChunk(x / Chunk::DIM, y / Chunk::DIM, z / Chunk::DIM)->SetBlock(x, y, z, t);
        return true;
    }

    int SetBlock(Vector3I p, int t) {
        return SetBlock(p.x, p.y, p.z, t);
    }

    int GetChunkIndex(int cx, int cy, int cz) {
        return cx + cy * chunkWidth.x + cz * chunkWidth.x * chunkWidth.z;
    }

    int GetChunkIndex(Vector3I p) {
        return GetChunkIndex(p.x, p.y, p.z);
    }

    Chunk* GetChunk(int cx, int cy, int cz) {
        return chunks[GetChunkIndex(cx, cy, cz)];
    }

    bool InBlockBounds(int x, int y, int z) {
        return x >= 0 && x < width.x &&
               y >= 0 && y < width.y &&
               z >= 0 && z < width.z;
    }

    Vector3I chunkWidth;
    Vector3I width;
    int numChunks;
    Chunk** chunks;
};