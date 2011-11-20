#include "stdafx.h"

#include "level/chunk.h"

Chunk::Chunk() :
    x(), y(), z() {
}

Chunk::~Chunk() {
}

void Chunk::Init(int t, int cx, int cy, int cz) {
    x = cx * DIM;
    y = cy * DIM;
    z = cz * DIM;
    memset(data, t, Chunk::SIZE);
}

void Chunk::Fill(int t) {
    memset(data, t, Chunk::SIZE);
}