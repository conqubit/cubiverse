#include "stdafx.h"

#include "level/chunk.h"

Chunk::Chunk() {
}

Chunk::~Chunk() {
}

void Chunk::Init(int cx, int cy, int cz) {
    this->x = cx * DIM;
    this->y = cy * DIM;
    this->z = cz * DIM;
    ZeroMemory(data, Chunk::SIZE);
}

void Chunk::Fill(Block type) {
    memset(data, type, Chunk::SIZE);
}