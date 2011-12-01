#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

WorldRenderer::WorldRenderer() : world(), visibleChunks() {
}

WorldRenderer::~WorldRenderer() {
}

bool WorldRenderer::Init(World* w) {
    world = w;
    shader = new Shader();
    if (!shader->Init("test.v.glsl", "test.f.glsl")) {
        return false;
    }
    return true;
}

void WorldRenderer::Shutdown() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (!visibleChunks[i]) continue;
        visibleChunks[i]->Shutdown();
        delete visibleChunks[i];
    }
    visibleChunks.clear();
    world = nullptr;
}

void WorldRenderer::ConstructVisibleChunks() {
    for (int i = 0; i < world->numChunks; i++) {
        visibleChunks.push_back(ConstructVisibleChunk(world->chunks[i]));
    }
}


int inc = 0;

VisibleChunk* WorldRenderer::ConstructVisibleChunk(Chunk* c) {
    if (!c) return nullptr;
    ModelFactory mf = ModelFactory();
    int numVisibleBlocks = 0;
    inc = 0;
    VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
        if (world->GetBlock(p) == Block::Air) {
            continue;
        }
        if (world->GetBlock(p.x + 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, p.x + 1, p.y, p.z, 0, 1, 2, 0.9f);
        }
        if (world->GetBlock(p.x - 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 0, 2, 1, 0.6f);
        }
        if (world->GetBlock(p.x, p.y + 1, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y + 1, p.z, 1, 2, 0, 0.8f);
        }
        if (world->GetBlock(p.x, p.y - 1, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 1, 0, 2, 0.7f);
        }
        if (world->GetBlock(p.x, p.y, p.z + 1) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z + 1, 2, 0, 1, 1.0f);
        }
        if (world->GetBlock(p.x, p.y, p.z - 1) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 2, 1, 0, 0.5f);
        }
        numVisibleBlocks += inc;
    }
    if (numVisibleBlocks > 0) {
        mf.shader = shader;
        mf.topology = GL_QUADS;
        Model* m = mf.Create();
        if (!m) {
            return nullptr;
        }
        VisibleChunk* vc = new VisibleChunk();
        vc->chunk = c;
        vc->model = m;
        return vc;
    }
    return nullptr;
}

void WorldRenderer::ConstructFace(ModelFactory& mf, int x, int y, int z, int xi, int yi, int zi, double b) {
    int count = mf.VertexCount();
    mf.AddTriangle(count, count + 1, count + 2);
    mf.AddTriangle(count, count + 2, count + 3);
    Vector3F v = Vector3F(x, y, z);
    mf.AddVertex(v, ColorF(b, b, b));
    mf.AddVertex(v + Vector3F::AXIS[yi], ColorF(b * 0.9, b * 0.9, b * 0.9));
    mf.AddVertex(v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi], ColorF(b * 0.7, b * 0.7, b * 0.7));
    mf.AddVertex(v + Vector3F::AXIS[zi], ColorF(b * 0.8, b * 0.8, b * 0.8));
    inc = 1;
}

void WorldRenderer::UpdateBlock(Vector3I p) {
    Chunk* c = System::world->GetChunk(p);
    if (c == nullptr) return;
    UpdateChunk(c);
    Chunk* c2;
    SIDES(
        if ((c2 = System::world->GetChunk(p + s)) != c) {
            UpdateChunk(c2);
        }
    );
}

void WorldRenderer::UpdateChunk(Chunk* c) {
    if (c == nullptr) return;
    for (int i = 0; i < visibleChunks.size(); i++) {
        VisibleChunk* vc = visibleChunks[i];
        if (vc) {
            if (vc->chunk != c) continue;
            vc->Shutdown();
            delete vc;
        }
        visibleChunks[i] = ConstructVisibleChunk(c);
        return;
    }
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (!visibleChunks[i]) continue;
        visibleChunks[i]->model->Render();
    }
}