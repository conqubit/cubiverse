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

    sf::Image image;

    if (!image.LoadFromFile("res/blocks2.png")) {
        return false;
    }

    texture = new Texture();
    texture->Init2DArray(4, image.GetWidth(), image.GetWidth(), (byte*)image.GetPixelsPtr());

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

enum Side {
    X0 = 0,
    X1 = 1,
    Y0 = 2,
    Y1 = 3,
    Z0 = 4,
    Z1 = 5
};


int inc = 0;

VisibleChunk* WorldRenderer::ConstructVisibleChunk(Chunk* c) {
    if (!c) return nullptr;

    ModelFactory mf = ModelFactory();
    mf.shader = shader;
    mf.texture = texture;
    mf.topology = GL_TRIANGLE_STRIP;
    mf.AddAttribute("position", 3);
    mf.AddAttribute("color", 4);
    mf.AddAttribute("texcoord", 3);

    int numVisibleBlocks = 0;
    inc = 0;

    VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
        int b = world->GetBlock(p);
        if (b == Block::Air) {
            continue;
        }
        if (world->GetBlock(p.x + 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, b, Side::X1, p.x + 1, p.y, p.z, 0, 1, 2, 0.85);
        }
        if (world->GetBlock(p.x - 1, p.y, p.z) == Block::Air) {
            ConstructFace(mf, b, Side::X0, p.x, p.y, p.z, 0, 2, 1, 0.7);
        }
        if (world->GetBlock(p.x, p.y + 1, p.z) == Block::Air) {
            ConstructFace(mf, b, Side::Y1, p.x, p.y + 1, p.z, 1, 2, 0, 0.8);
        }
        if (world->GetBlock(p.x, p.y - 1, p.z) == Block::Air) {
            ConstructFace(mf, b, Side::Y0, p.x, p.y, p.z, 1, 0, 2, 0.75);
        }
        if (world->GetBlock(p.x, p.y, p.z + 1) == Block::Air) {
            ConstructFace(mf, b, Side::Z1, p.x, p.y, p.z + 1, 2, 0, 1, 1.0);
        }
        if (world->GetBlock(p.x, p.y, p.z - 1) == Block::Air) {
            ConstructFace(mf, b, Side::Z0, p.x, p.y, p.z, 2, 1, 0, 0.5);
        }
        numVisibleBlocks += inc;
    }
    if (numVisibleBlocks > 0) {
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

int GetSliceIndex(int block, int side) {
    switch (block) {
    case Block::Stone:
        return 0;
    case Block::Dirt:
        return 1;
    case Block::Grass:
        if (side == Side::Z1) {
            return 2;
        }
        return 1;
    case Block::Test:
        return 3;
    }
}

void WorldRenderer::ConstructFace(ModelFactory& mf, int block, int side, int x, int y, int z, int xi, int yi, int zi, double b) {
    Vector3F v = Vector3F(x, y, z);
    ColorF c(b, b, b);

    int tz = GetSliceIndex(block, side);

    // Degenerate.
    mf.Next().Set("position", v).Set("texcoord", 0, 0, tz);

    mf.Next().Set("position", v)
             .Set("color", c).Set("texcoord", 0, 0, tz);
    mf.Next().Set("position", v + Vector3F::AXIS[yi])
             .Set("color", c).Set("texcoord", 0, 1, tz);
    mf.Next().Set("position", v + Vector3F::AXIS[zi])
             .Set("color", c).Set("texcoord", 1, 0, tz);
    mf.Next().Set("position", v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi])
             .Set("color", c).Set("texcoord", 1, 1, tz);

    // Degenerate.
    mf.Next().Set("position", v + Vector3F::AXIS[yi] + Vector3F::AXIS[zi]).Set("texcoord", 0, 0, tz);

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