#include "stdafx.h"

#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

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
    for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i]->Shutdown();
        delete visibleChunks[i];
    }
    world = nullptr;
}

void ConstructFace(ModelFactory& mf, int x, int y, int z, int xi, int yi, int zi, float b) {
    int count = mf.vertices.Count();
    mf.AddTriangle(count, count + 1, count + 2);
    mf.AddTriangle(count, count + 2, count + 3);
    Vector3F v = Vector3F(x, y, z);
    mf.vertices.Add(Vertex(v, XMFLOAT4(b, b, b, 1.0f)));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[yi], XMFLOAT4(b * 0.9f, b * 0.9f, b * 0.9f, 1.0f)));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi], XMFLOAT4(b * 0.7f, b * 0.7f, b * 0.7f, 1.0f)));
    mf.vertices.Add(Vertex(v + Vector3F::AXIS[zi], XMFLOAT4(b * 0.8f, b * 0.8f, b * 0.8f, 1.0f)));
}

bool WorldRenderer::ConstructVisibleChunks() {
    Shader* s = new Shader();
    if (!s->Init(L"shaders.hlsl", "_vshader", "_pshader")) {
        return false;
    }
    int total = 0;
    for (int i = 0; i < world->numChunks; i++) {
        Chunk* c = world->chunks[i];
        ModelFactory mf = ModelFactory();
        int numVisibleBlocks = 0;
        VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
            if (world->GetBlock(p) == Block::Air) {
                continue;
            }
            bool visible = false;
            if (world->GetBlock(p.x + 1, p.y, p.z) == Block::Air) { // X +
                ConstructFace(mf, p.x + 1, p.y, p.z, 0, 1, 2, 0.9f);
                visible = true;
            }
            if (world->GetBlock(p.x - 1, p.y, p.z) == Block::Air) { // X -
                ConstructFace(mf, p.x, p.y, p.z, 0, 2, 1, 0.6f);
                visible = true;
            }
            if (world->GetBlock(p.x, p.y + 1, p.z) == Block::Air) { // Y +
                ConstructFace(mf, p.x, p.y + 1, p.z, 1, 2, 0, 0.8f);
                visible = true;
            }
            if (world->GetBlock(p.x, p.y - 1, p.z) == Block::Air) { // Y -
                ConstructFace(mf, p.x, p.y, p.z, 1, 0, 2, 0.7f);
                visible = true;
            }
            if (world->GetBlock(p.x, p.y, p.z + 1) == Block::Air) { // Z +
                ConstructFace(mf, p.x, p.y, p.z + 1, 2, 0, 1, 1.0f);
                visible = true;
            }
            if (world->GetBlock(p.x, p.y, p.z - 1) == Block::Air) { // Z -
                ConstructFace(mf, p.x, p.y, p.z, 2, 1, 0, 0.5f);
                visible = true;
            }
            if (visible) {
                //vc->visibleBlocks.Insert(Chunk::GetIndex(x, y, z));
                numVisibleBlocks++;
            }
        }
        if (numVisibleBlocks > 0) {
            total += numVisibleBlocks;
            mf.shader = s;
            Model* m = mf.Create();
            if (!m) continue;
            VisibleChunk* vc = new VisibleChunk();
            vc->model = m;
            visibleChunks.Add(vc);
        }
    }
    print(total);
    return true;
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.Count(); i++) {
        visibleChunks[i]->model->Render();
    }
}