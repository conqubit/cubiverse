#include "stdafx.h"

#include "level/block.h"
#include "level/world.h"

World::World() :
    chunksX(),
    chunksY(),
    chunksZ(),
    numChunks(),
    chunks() {
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
    chunks = new Chunk*[numChunks]();
}

void World::Shutdown() {
    for (int i = 0; i < numChunks; i++) {
        delete chunks[i];
    }
    delete[] chunks;
    chunks = nullptr;
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