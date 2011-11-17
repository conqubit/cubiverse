#include "stdafx.h"

#include "graphics/model.h"
#include "graphics/modelfactory.h"
#include "graphics/worldrenderer.h"

WorldRenderer::WorldRenderer() :
    world(),
    visibleChunks() {
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

void ConstructFace(ModelFactory& mf, int x, int y, int z) {
    int count = mf.vertices.Count();
    mf.AddTriangle(count, count + 1, count + 2);
    mf.AddTriangle(count, count + 2, count + 3);
    mf.vertices.Add(Vertex(x, y, z));
    mf.vertices.Add(Vertex(x, y + 1, z));
    mf.vertices.Add(Vertex(x, y + 1, z + 1));
    mf.vertices.Add(Vertex(x, y, z + 1));
}

void WorldRenderer::ConstructVisibleChunks() {
    Shader s = Shader();
    s.Init(L"shaders.hlsl", "_vshader", "_pshader");
    for (int i = 0; i < world->numChunks; i++) {
        Chunk* c = world->chunks[i];
        VisibleChunk* vc = new VisibleChunk();
        ModelFactory mf = ModelFactory();
        mf.shader = s;
        for (int x = c->x; x < c->x + Chunk::DIM; x++) {
        for (int y = c->y; y < c->y + Chunk::DIM; y++) {
        for (int z = c->z; z < c->z + Chunk::DIM; z++) {
            if (world->GetBlock(x, y, z) == Block::Air) {
                continue;
            }
            bool visible = false;
            if (world->GetBlock(x + 1, y, z) == Block::Air) {
                ConstructFace(mf, x + 1, y, z);
                visible = true;
            }
            if (world->GetBlock(x - 1, y, z) == Block::Air) {
                ConstructFace(mf, x, z, y);
                visible = true;
            }
            if (world->GetBlock(x, y + 1, z) == Block::Air) {
                ConstructFace(mf, y + 1, z, x);
                visible = true;
            }
            if (world->GetBlock(x, y - 1, z) == Block::Air) {
                ConstructFace(mf, y, x, z);
                visible = true;
            }
            if (world->GetBlock(x, y, z + 1) == Block::Air) {
                ConstructFace(mf, z + 1, x, y);
                visible = true;
            }
            if (world->GetBlock(x, y, z - 1) == Block::Air) {
                ConstructFace(mf, z, y, x);
                visible = true;
            }
            if (visible) {
                vc->visibleBlocks.Insert(Chunk::GetIndex(x, y, z));
            }
        }}}
        vc->model = mf.Create();
        visibleChunks.Add(vc);
    }
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i]->model.Render();
    }
}