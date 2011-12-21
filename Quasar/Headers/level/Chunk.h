#pragma once

#include "math/Vector.h"
#include "level/Block.h"

class Chunk {
public:
    static const int DIM = 32; // Must be a power of 2.
    static const int SQR_DIM = DIM * DIM;
    static const int SIZE = DIM * DIM * DIM;
    static const int MASK = DIM - 1;
    static const Vector3I DIM_VEC;

    Vector3I pos;
    byte data[SIZE];

    Chunk();
    ~Chunk();

    void Init(int t, int cx, int cy, int cz);
    void Fill(int t);

    Vector3I GetWorldPositionFromIndex(int i)const {
        return pos + Vector3I(i & MASK, (i / DIM) & MASK, i / SQR_DIM);
    }

    int GetBlock(int x, int y, int z)const {
        return data[GetIndex(x, y, z)];
    }

    int SetBlock(int x, int y, int z, int t) {
        return data[GetIndex(x, y, z)] = t;
    }

    static int GetIndex(int x, int y, int z) {
        return (x & MASK) + (y & MASK) * DIM + (z & MASK) * DIM * DIM;
    }

    static int GetIndex(const Vector3I& p) {
        return (p.x & MASK) + (p.y & MASK) * DIM + (p.z & MASK) * DIM * DIM;
    }
};