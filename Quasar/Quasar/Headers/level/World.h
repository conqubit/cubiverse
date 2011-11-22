#pragma once

#include "math/BoundingBox.h"
#include "level/Chunk.h"

class World {
public:
    Vector3I chunkWidth;
    Vector3I width;
    int numChunks;
    Chunk** chunks;

    World();
    ~World();

    void Init(int chunksX, int chunksY, int chunksZ);
    void Shutdown();

    void Fill(int t);
    void Generate();

    bool Intersects(const BoundingBox& bb);

    int GetBlock(int x, int y, int z) {
        if (!InBlockBounds(x, y, z)) return Block::Air;
        return GetChunk(x, y, z)->GetBlock(x, y, z);
    }

    int GetBlock(Vector3I p) {
        return GetBlock(p.x, p.y, p.z);
    }

    bool SetBlock(int x, int y, int z, int t) {
        if (!InBlockBounds(x, y, z)) return false;
        GetChunk(x, y, z)->SetBlock(x, y, z, t);
        return true;
    }

    int SetBlock(Vector3I p, int t) {
        return SetBlock(p.x, p.y, p.z, t);
    }

    int GetChunkIndex(int cx, int cy, int cz) {
        return cx + cy * chunkWidth.x + cz * chunkWidth.x * chunkWidth.y;
    }

    int GetChunkIndex(Vector3I p) {
        return GetChunkIndex(p.x, p.y, p.z);
    }

    Chunk* GetChunk(int x, int y, int z) {
        if (!InBlockBounds(x, y, z)) return nullptr;
        return chunks[GetChunkIndex(x / Chunk::DIM, y / Chunk::DIM, z / Chunk::DIM)];
    }

    Chunk* GetChunk(Vector3I p) {
        return GetChunk(p.x, p.y, p.z);
    }

    bool InBlockBounds(int x, int y, int z) {
        return x >= 0 && x < width.x &&
               y >= 0 && y < width.y &&
               z >= 0 && z < width.z;
    }
};