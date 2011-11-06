#include "stdafx.h"

#include "graphics/worldrendering.h"

WorldRendering::WorldRendering() : world() {
}

WorldRendering::~WorldRendering() {
    world = nullptr;
}

void WorldRendering::Init(World* w) {
    world = w;
}

void WorldRendering::Shutdown() {
    // TODO: shutdown things
    world = nullptr;
}

void WorldRendering::ConstructVisibleChunks() {
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