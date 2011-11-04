#include "graphics/visiblechunk.h"

VisibleChunk::VisibleChunk() {
}

VisibleChunk::~VisibleChunk() {
}

bool VisibleChunk::Create(World* w, Chunk* c) {
    for (int x = c->x; x < c->x + Chunk::DIM; x++) {
        for (int y = c->y; y < c->y + Chunk::DIM; y++) {
            for (int z = c->z; z < c->z + Chunk::DIM; z++) {
                if (w->GetBlock(x, y, z) != Block::Air) {
                    continue;
                }

            }
        }
    }
}