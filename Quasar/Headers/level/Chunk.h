#pragma once

#include "math/Vector.h"
#include "level/Block.h"

class Chunk {
public:
    static const int DIM = 32;
    static const int SIZE = DIM * DIM * DIM;
    static const int MASK = DIM - 1;
    static const Vector3I DIM_VEC;

    Vector3I pos;
    byte data[SIZE];

    Chunk();
    ~Chunk();

    void Init(int t, int cx, int cy, int cz);
    void Fill(int t);

    int GetBlock(int x, int y, int z)const {
        return data[GetIndex(x, y, z)];
    }

    int SetBlock(int x, int y, int z, int t) {
        return data[GetIndex(x, y, z)] = t;
    }

    static int GetIndex(int x, int y, int z) {
        return (x & MASK) + (y & MASK) * DIM + (z & MASK) * DIM * DIM;
    }
};