#pragma once

#include "math/Vector.h"
#include "level/Block.h"

class Chunk {
public:
    static const int DIM = 32; // Must be a power of 2.
    static const int SIZE = DIM * DIM * DIM;

    static const int SHIFT = 5;
    static const int SHIFT_TWICE = SHIFT * 2;
    static const int MASK = DIM - 1;

    static const Vector3I DIM_VEC;

    Vector3I ToChunkCoord(const Vector3I& p) {
        return p >> SHIFT;
    }

    Vector3I pos;
    byte data[SIZE];

    Chunk();
    ~Chunk();

    Vector3I Pos()const {
        return pos * DIM;
    }

    void Init(int t, const Vector3I& p);
    void Shutdown();

    void Fill(int t);

    Vector3I GetWorldPositionFromIndex(int i)const {
        return Pos() + Vector3I(i & MASK, (i >> SHIFT) & MASK, i >> SHIFT_TWICE);
    }

    int GetBlock(const Vector3I& p)const {
        return data[GetIndex(p)];
    }

    int SetBlock(const Vector3I& p, int t) {
        return data[GetIndex(p)] = t;
    }

    static int GetIndex(const Vector3I& p) {
        return (p.x & MASK) + ((p.y & MASK) << SHIFT) + ((p.z & MASK) << SHIFT_TWICE);
    }
};