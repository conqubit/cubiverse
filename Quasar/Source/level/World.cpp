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

void World::Fill(int t, Vector3I a, Vector3I b) {
    VEC3_RANGE_AB(a, b) {
        SetBlock(p, t);
    }
}

void World::Generate() {
    Vector3I min(0, 0, 0);
    Vector3I max = width.Offset(-1);
    Fill(Block::Grass, min.Offset(32), max.Offset(-32));
    Fill(Block::Dirt, min.Offset(33), max.Offset(-33));
    Fill(Block::Stone, min.Offset(37), max.Offset(-37));
}

bool World::Intersects(const BoundingBox& bb) {
    VEC3_RANGE_AB(bb.Min().Floor(), bb.Max().Floor()) {
        if (GetBlock(p) != Block::Air) {
            return true;
        }
    }
    return false;
}