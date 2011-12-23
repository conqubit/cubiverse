#pragma once

#include "math/BoundingBox.h"
#include "level/Chunk.h"

#include "level/Level.h"

class World {
public:
    Vector3I chunkWidth;
    Vector3I width;

    int numChunks;

    Vector3I cmin, cmax;

    Level level;

    World();
    ~World();

    void Init(int chunksX, int chunksY, int chunksZ);
    void Shutdown();

    void Fill(int t);
    void Fill(int t, Vector3I a, Vector3I b);
    void Generate();

    Vector3I GetUp(const Vector3D& pos)const;

    Vector3D GetUpSmooth(const Vector3D& pos)const;

    bool Intersects(const BoundingBox& bb)const;

    int GetBlock(const Vector3I& p)const {
        Chunk* c = GetChunk(p);
        return c ? GetChunk(p)->GetBlock(p) : Block::Undefined;
    }

    int GetBlock(int x, int y, int z)const {
        return GetBlock(Vector3I(x, y, z));
    }

    void SetBlock(const Vector3I& p, int t) {
        Chunk* c = GetChunk(p);
        if (c) {
            c->SetBlock(p, t);
        }
    }

    Chunk* GetChunk(const Vector3I& p)const {
        if (!InBlockBounds(p)) return nullptr;
        return level.GetChunk(p);
    }

    void InsertChunk(Chunk* c) {
        level.InsertChunk(c);

        if (c->pos.x < cmin.x) cmin.x = c->pos.x;
        else if (c->pos.x > cmax.x) cmax.x = c->pos.x;

        if (c->pos.y < cmin.y) cmin.y = c->pos.y;
        else if (c->pos.y > cmax.y) cmax.y = c->pos.y;

        if (c->pos.z < cmin.z) cmin.z = c->pos.z;
        else if (c->pos.z > cmax.z) cmax.z = c->pos.z;
    }

    bool InBlockBounds(const Vector3I& p)const {
        return p.x >= 0 && p.x < width.x &&
               p.y >= 0 && p.y < width.y &&
               p.z >= 0 && p.z < width.z;
    }
};