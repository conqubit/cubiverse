#include "stdafx.h"

#include "level/block.h"
#include "level/world.h"

World::World() :
    chunksX(), chunksY(), chunksZ(),
    widthX(), widthY(), widthZ(),
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

void World::Fill(int t) {
    for (int cx = 0; cx < chunksX; cx++) {
    for (int cy = 0; cy < chunksY; cy++) {
    for (int cz = 0; cz < chunksZ; cz++) {
        int i = GetChunkIndex(cx, cy, cz);
        chunks[i] = new Chunk();
        chunks[i]->Init(t, cx, cy, cz);
    }}}
}

void World::GenerateSphere(int t) {
    for (int x = 0; x < widthX; x++) {
    for (int y = 0; y < widthY; y++) {
    for (int z = 0; z < widthZ; z++) {
        if (Vector3F(x - widthX/2, y - widthY/2, z - widthZ/2).Length() < min(min(widthX, widthY), widthZ) / 2) {
            SetBlock(x, y, z, t);
        }
    }}}
}