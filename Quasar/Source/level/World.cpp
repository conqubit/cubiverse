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
    Fill(Block::Stone, Vector3I(16, 16, 16), Vector3I(width.x - 17, width.y - 17, width.z - 17));
    //Fill(Block::Dirt, Vector3I(16, 16, 81), Vector3I(width.x - 17, width.y - 17, 85));
    Fill(Block::Grass, Vector3I(16, 16, width.z - 17),Vector3I(width.x - 17, width.y - 17, width.z - 17));
}

bool World::Intersects(const BoundingBox& bb) {
    VEC3_RANGE_AB(bb.Min().Floor(), bb.Max().Floor()) {
        if (GetBlock(p) != Block::Air) {
            return true;
        }
    }
    return false;
}