#include "stdafx.h"

#include "graphics/WorldRender.h"

WorldRender::WorldRender() :
    world() {
}

WorldRender::~WorldRender() {
    world = nullptr;
}

void WorldRender::Init(World* w) {
    world = w;
}

void WorldRender::Shutdown() {
    // TODO: shutdown things
    world = nullptr;
}

void WorldRender::ConstructVisibleChunks() {
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

void WorldRender::Render() {
    for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i].model.Render();
    }
}