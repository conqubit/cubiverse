#include "stdafx.h"

#include "level/chunk.h"

const Vector3I Chunk::DIM_VEC = Vector3I(Chunk::DIM, Chunk::DIM, Chunk::DIM);

Chunk::Chunk() : pos() {
}

Chunk::~Chunk() {
}

void Chunk::Init(int t, int cx, int cy, int cz) {
    pos = Vector3I(cx, cy, cz) * Chunk::DIM;
    Fill(t);
}

void Chunk::Fill(int t) {
    memset(data, t, Chunk::SIZE);
}