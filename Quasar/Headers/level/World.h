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
    void Fill(int t, Vector3I a, Vector3I b);
    void Generate();

    Vector3I GetUp(Vector3D p) {
        p -= (width.ToDouble() / 2.0);
        Vector3D a = p.Abs();
        if (a.x > a.y && a.x > a.z) {
            return Vector3I::AXIS_X * SIGN(p.x);
        } else if (a.y > a.z) {
            return Vector3I::AXIS_Y * SIGN(p.y);
        } else {
            return Vector3I::AXIS_Z * SIGN(p.z);
        }
    }

    Vector3D GetUpSmooth(Vector3D p) {
        double radius = 8.0;
        p -= (width.ToDouble() / 2.0);
        Vector3D a = p.Abs();
        int x = 0, y = 1, z = 2;
        if (a.x > a.y && a.x > a.z) {
            z = 0; x = 2;
        } else if (a.y > a.z) {
            z = 1; y = 2;
        }
        a[z] -= radius;
        if (a[x] > a[z]) {
            a[x] = a[z];
        }
        if (a[y] > a[z]) {
            a[y] = a[z];
        }
        a[x] *= SIGN(p[x]);
        a[y] *= SIGN(p[y]);
        a[z] *= SIGN(p[z]);
        return (p - a).Normalize();
    }

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