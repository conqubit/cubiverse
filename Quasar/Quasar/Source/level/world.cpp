#include "stdafx.h"

#include "level/Block.h"
#include "level/World.h"

World::World() :
    chunkWidth(),
    width(),
    numChunks(),
    chunks() {
}

World::~World() {
}

void World::Init(int chunksX, int chunksY, int chunksZ) {
    chunkWidth = Vector3I(chunksX, chunksY, chunksZ);
    width = chunkWidth * Chunk::DIM;

    numChunks = chunksX * chunksY * chunksZ;
    chunks = new Chunk*[numChunks]();
}

void World::Shutdown() {
    for (int i = 0; i < numChunks; i++) {
        delete chunks[i];
    }
    delete[] chunks;
    chunks = nullptr;
}

void World::Fill(int t) {
    VEC3_RANGE(chunkWidth) {
        int i = GetChunkIndex(p);
        chunks[i] = new Chunk();
        chunks[i]->Init(t, p.x, p.y, p.z);
    }
}

void World::GenerateSphere(int t) {
    VEC3_RANGE(width) {
        if ((p - (width / 2)).Length() < min(min(width.x, width.y), width.z) / 2) {
            SetBlock(p, t);
        }
    }
}