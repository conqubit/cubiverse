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

void ConstructFace(ModelFactory& mf, int x, int y, int z, int xi, int yi, int zi, XMFLOAT4 color) {
    int count = mf.vertices.Count();
    mf.AddTriangle(count, count + 1, count + 2);
    mf.AddTriangle(count, count + 2, count + 3);
    Vector3F v = Vector3F(x, y, z);
    mf.vertices.Add(Vertex(v, color));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[yi], color));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi], color));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[zi], color));
}

bool WorldRenderer::ConstructVisibleChunks() {
    Shader* s = new Shader();
    if (!s->Init(L"shaders.hlsl", "_vshader", "_pshader")) {
        return false;
    }
    for (int i = 0; i < world->numChunks; i++) {
        Chunk* c = world->chunks[i];
        ModelFactory mf = ModelFactory();
        int numVisibleBlocks = 0;
        for (int x = c->x; x < c->x + Chunk::DIM; x++) {
        for (int y = c->y; y < c->y + Chunk::DIM; y++) {
        for (int z = c->z; z < c->z + Chunk::DIM; z++) {
            if (world->GetBlock(x, y, z) == Block::Air) {
                continue;
            }
            bool visible = false;
            if (world->GetBlock(x + 1, y, z) == Block::Air) {
                ConstructFace(mf, x + 1, y, z, 0, 1, 2, XMFLOAT4(1, 1, 1, 1));
                visible = true;
            }
            if (world->GetBlock(x - 1, y, z) == Block::Air) {
                ConstructFace(mf, x, y, z, 0, 2, 1, XMFLOAT4(.5, .5, .5, 1));
                visible = true;
            }
            if (world->GetBlock(x, y + 1, z) == Block::Air) {
                ConstructFace(mf, x, y + 1, z, 1, 2, 0, XMFLOAT4(.9, .9, .9, 1));
                visible = true;
            }
            if (world->GetBlock(x, y - 1, z) == Block::Air) {
                ConstructFace(mf, x, y, z, 1, 0, 2, XMFLOAT4(.8, .8, .8, 1));
                visible = true;
            }
            if (world->GetBlock(x, y, z + 1) == Block::Air) {
                ConstructFace(mf, x, y, z + 1, 2, 0, 1, XMFLOAT4(.7, .7, .7, 1));
                visible = true;
            }
            if (world->GetBlock(x, y, z - 1) == Block::Air) {
                ConstructFace(mf, x, y, z, 2, 1, 0, XMFLOAT4(.6, .6, .6, 1));
                visible = true;
            }
            if (visible) {
                //vc->visibleBlocks.Insert(Chunk::GetIndex(x, y, z));
                numVisibleBlocks++;
            }
        }}}
        if (numVisibleBlocks > 0) {
            mf.shader = s;
            VisibleChunk* vc = new VisibleChunk();
            vc->model = mf.Create();
            if (!vc->model) {
                return false;
            }
            visibleChunks.Add(vc);
        }
    }
    return true;
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i]->model->Render();
    }
}