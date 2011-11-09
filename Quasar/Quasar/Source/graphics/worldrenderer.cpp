#include "stdafx.h"

#include "graphics/worldrenderer.h"

WorldRenderer::WorldRenderer() :
    world() {
}

WorldRenderer::~WorldRenderer() {
}

void WorldRenderer::Init(World* w) {
    world = w;
}

void WorldRenderer::Shutdown() {
    // TODO: shutdown things
    world = nullptr;
}

void WorldRenderer::ConstructVisibleChunks() {
    /*for (int x = c->x; x < c->x + Chunk::DIM; x++) {
        for (int y = c->y; y < c->y + Chunk::DIM; y++) {
            for (int z = c->z; z < c->z + Chunk::DIM; z++) {
                if (w->GetBlock(x, y, z) != Block::Air) {
                    continue;
                }

            }
        }
    }*/
}

void WorldRenderer::Render() {
    /*for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i].model.Render();
    }*/
}