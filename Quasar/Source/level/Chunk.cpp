#include "stdafx.h"

#include "level/Chunk.h"

const Vector3I Chunk::DIM_VEC = Vector3I(Chunk::DIM, Chunk::DIM, Chunk::DIM);

Chunk::Chunk() : pos() {
}

Chunk::~Chunk() {
}

void Chunk::Init(int t, const Vector3I& p) {
    pos = p;
    Fill(t);
}

void Chunk::Fill(int t) {
    memset(data, t, Chunk::SIZE);
}