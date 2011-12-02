#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

WorldRenderer::WorldRenderer() :
world(), visibleChunks(), shader(), texture() {
}

WorldRenderer::~WorldRenderer() {
}

bool WorldRenderer::Init(World* w) {
    world = w;
    shader = new Shader();
    if (!shader->Init("res/block.v.glsl", "res/block.f.glsl")) {
        return false;
    }

    texture = new Texture();
    if (!texture->Init("res/grass.png")) {
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
    mf.AddAttribute("position", 3);
    mf.AddAttribute("color", 4);
    mf.AddAttribute("texcoord", 2);

    int numVisibleBlocks = 0;
    inc = 0;

    VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
        if (world->GetBlock(p) == Block::Air) {
            continue;
        }
        if (world->GetBlock(p.x + 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, p.x + 1, p.y, p.z, 0, 1, 2, 0.85);
        }
        if (world->GetBlock(p.x - 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 0, 2, 1, 0.7);
        }
        if (world->GetBlock(p.x, p.y + 1, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y + 1, p.z, 1, 2, 0, 0.8);
        }
        if (world->GetBlock(p.x, p.y - 1, p.z) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 1, 0, 2, 0.75);
        }
        if (world->GetBlock(p.x, p.y, p.z + 1) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z + 1, 2, 0, 1, 1.0);
        }
        if (world->GetBlock(p.x, p.y, p.z - 1) == Block::Air) {
            ConstructFace(mf, p.x, p.y, p.z, 2, 1, 0, 0.5);
        }
        numVisibleBlocks += inc;
    }
    if (numVisibleBlocks > 0) {
        mf.shader = shader;
        mf.texture = texture;
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
    //mf.AddTriangle(count, count + 1, count + 2);
    //mf.AddTriangle(count, count + 2, count + 3);
    Vector3F v = Vector3F(x, y, z);
    ColorF c(b, b, b);
    mf.Next().Set("position", v)
             .Set("color", c)
             .Set("texcoord", 0, 0);
    mf.Next().Set("position", v + Vector3F::AXIS[zi])
             .Set("color", c)
             .Set("texcoord", 1, 0);
    mf.Next().Set("position", v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi])
             .Set("color", c)
             .Set("texcoord", 1, 1);
    mf.Next().Set("position", v + Vector3F::AXIS[yi])
             .Set("color", c)
             .Set("texcoord", 0, 1);
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
    int i;
    for (i = 0; i < visibleChunks.size(); i++) {
        VisibleChunk* vc = visibleChunks[i];
        if (!vc) continue;

        if (vc->chunk != c) continue;
        vc->Shutdown();
        delete vc;

        visibleChunks[i] = ConstructVisibleChunk(c);
        return;
    }
    if (i == visibleChunks.size()) {
        visibleChunks.push_back(ConstructVisibleChunk(c));
    }
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (!visibleChunks[i]) continue;
        visibleChunks[i]->model->Render();
    }
}