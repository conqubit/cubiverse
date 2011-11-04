#include "stdafx.h"

#include "level/block.h"
#include "level/world.h"

World::World() {
    chunksX = 0;
    chunksY = 0;
    chunksZ = 0;
    numChunks = 0;
    chunks = nullptr;
}

World::~World() {
}

void World::Init(int chunksX, int chunksY, int chunksZ) {
    this->chunksX = chunksX;
    this->chunksY = chunksY;
    this->chunksZ = chunksZ;
    widthX = chunksX * Chunk::DIM;
    widthY = chunksY * Chunk::DIM;
    widthZ = chunksZ * Chunk::DIM;

    numChunks = chunksX * chunksY * chunksZ;
    chunks = new Chunk[numChunks];
    ZeroMemory(chunks, numChunks);
}

void World::Shutdown() {
    delete[] chunks;
    chunks = nullptr;
}

Block World::GetBlock(int x, int y, int z) {
    if (!InBlockBounds(x, y, z)) return Block::Undefined;
    GetChunk(x / Chunk::DIM, y / Chunk::DIM, z / Chunk::DIM)->GetBlock(x, y, z);
}

Chunk* World::GetChunk(int cx, int cy, int cz) {
    return chunks + (cx + cy * chunksX + cz * chunksX * chunksY);
}

void World::Generate() {
    for (int cx = 0; cx < chunksX; cx++) {
        for (int cy = 0; cy < chunksY; cy++) {
            for (int cz = 0; cz < chunksZ; cz++) {
                Chunk* c = GetChunk(cx, cy, cz);
                c->Init(cx, cy, cz);
                c->Fill(Block::Stone);
            }
        }
    }
}

void World::ConstructChunkModel(Chunk* c) {
    for (int x = c->x; x < c->x + Chunk::DIM; x++) {
        for (int y = c->y; y < c->y + Chunk::DIM; y++) {
            for (int z = c->z; z < c->z + Chunk::DIM; z++) {
                
            }
        }
    }
}