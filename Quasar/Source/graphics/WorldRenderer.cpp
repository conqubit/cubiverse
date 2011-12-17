#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

WorldRenderer::WorldRenderer() :
world(), shader(), texture() {
}

WorldRenderer::~WorldRenderer() {
}

int numBlocks = 4;

ModelFactory mf;
int vPos;
int vCol;
int vTex;

bool WorldRenderer::Init(World* w) {
    world = w;
    shader = new Shader();
    if (!shader->Init("res/block.c.v.glsl", "res/block.c.f.glsl")) {
        return false;
    }

    sf::Image image;

    if (!image.LoadFromFile("res/blocks.png")) {
        return false;
    }

    int dim = image.GetWidth();

    texture = Texture::Create3DTexture(dim, dim, image.GetHeight() / dim, (byte*)image.GetPixelsPtr());

    mf = ModelFactory();

    mf.shader = shader;
    mf.texture = texture;
    mf.topology = GL_TRIANGLES;
    vPos = mf.AddAttribute<float>("position", 3);
    vCol = mf.AddAttribute<byte>("color", 4, true);
    vTex = mf.AddAttribute<float>("texcoord", 3);

    return true;
}

void WorldRenderer::Shutdown() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (visibleChunks[i]) {
            visibleChunks[i]->Shutdown();
            delete visibleChunks[i];
        }
    }
    visibleChunks.clear();

    texture->Shutdown();
    shader->Shutdown();

    delete texture;
    delete shader;

    world = nullptr;
}

void WorldRenderer::ConstructVisibleChunks() {
    for (int i = 0; i < world->numChunks; i++) {
        VisibleChunk* vs = ConstructNewVisibleChunk(world->chunks[i]);
        if (vs) {
            visibleChunks.push_back(vs);
        }
    }
}

VisibleChunk* WorldRenderer::ConstructNewVisibleChunk(Chunk* c) {
    if (!c) return nullptr;
    VisibleChunk* vs = new VisibleChunk();
    ConstructChunkModelData(c, vs);
    if (mf.VertexCount() > 0) {
        vs->chunk = c;
        vs->UpdateModel(mf);
        return vs;
    }
    delete vs;
    return nullptr;
}

void WorldRenderer::ConstructBlock(const Vector3I& p) {
    int b = world->GetBlock(p);
    if (b == Block::Air) {
        return;
    }

    if (world->GetBlock(p.x + 1, p.y, p.z) == Block::Air) {
        ConstructFace(b, Vector3I::AXIS_X, p, p.x + 1, p.y, p.z, 1, 2, 0, 0.85);
    }
    if (world->GetBlock(p.x - 1, p.y, p.z) == Block::Air) {
        ConstructFace(b, -Vector3I::AXIS_X, p, p.x, p.y, p.z, 2, 1, 0, 0.7);
    }
    if (world->GetBlock(p.x, p.y + 1, p.z) == Block::Air) {
        ConstructFace(b, Vector3I::AXIS_Y, p, p.x, p.y + 1, p.z, 2, 0, 1, 0.8);
    }
    if (world->GetBlock(p.x, p.y - 1, p.z) == Block::Air) {
        ConstructFace(b, -Vector3I::AXIS_Y, p, p.x, p.y, p.z, 0, 2, 1, 0.75);
    }
    if (world->GetBlock(p.x, p.y, p.z + 1) == Block::Air) {
        ConstructFace(b, Vector3I::AXIS_Z, p, p.x, p.y, p.z + 1, 0, 1, 2, 1.0);
    }
    if (world->GetBlock(p.x, p.y, p.z - 1) == Block::Air) {
        ConstructFace(b, -Vector3I::AXIS_Z, p, p.x, p.y, p.z, 1, 0, 2, 0.5);
    }
}

void WorldRenderer::ConstructChunkModelData(Chunk* c,  VisibleChunk* vs) {
    mf.Clear();
    VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
        int stiz = mf.VertexDataSize();
        ConstructBlock(p);
        stiz = mf.VertexDataSize() - stiz;
        if (stiz > 0) {
            VisibleChunk::VisibleBlock& vb = vs->visibleBlocks[Chunk::GetIndex(p)];
            vb.location = mf.VertexDataSize() - stiz;
            vb.size = stiz;
        }
    }
}

int GetTextureIndex(int block, const Vector3I& side, const Vector3I& up) {
    switch (block) {
    case Block::Stone:
        return 0;
    case Block::Dirt:
        return 1;
    case Block::Grass:
        if (side == up) {
            return 2;
        }
        return 1;
    case Block::Test:
        return 3;
    }
    return 0;
}

void WorldRenderer::ConstructFace(int block, const Vector3I& side, const Vector3I& p, int x, int y, int z, int xi, int yi, int zi, double b) {
    Vector3F v = Vector3F(x, y, z);
    Vector3I up = System::world->GetUp(p.ToDouble().Offset(0.5) + side.ToDouble() / 2.0);
    ColorB c = ColorD(b, b, b, 1.0).ToByte();

    float tz = (float)GetTextureIndex(block, side, up) / (float)numBlocks + 1.0f / ((float)numBlocks * 2.0f);

    mf.Next().Set(vPos, v).Set(vCol, c).Set(vTex, 0, 0, tz);
    mf.Next().Set(vPos, v + Vector3F::AXIS[yi]).Set(vCol, c).Set(vTex, 0, 1, tz);
    mf.Next().Set(vPos, v + Vector3F::AXIS[yi] + Vector3F::AXIS[xi]).Set(vCol, c).Set(vTex, 1, 1, tz);

    mf.Next().Set(vPos, v + Vector3F::AXIS[yi] + Vector3F::AXIS[xi]).Set(vCol, c).Set(vTex, 1, 1, tz);
    mf.Next().Set(vPos, v + Vector3F::AXIS[xi]).Set(vCol, c).Set(vTex, 1, 0, tz);
    mf.Next().Set(vPos, v).Set(vCol, c).Set(vTex, 0, 0, tz);
}

void WorldRenderer::UpdateMesh(const Vector3I& p) {
    Chunk* c = System::world->GetChunk(p);
    if (!c) return;
    int i;
    for (i = 0; i < visibleChunks.size(); i++) {
        VisibleChunk* vc = visibleChunks[i];

        if (!vc || vc->chunk != c) continue;

        mf.Clear();
        ConstructBlock(p);
        vc->UpdateBlock(Chunk::GetIndex(p), mf);

        return;
    }
    if (i == visibleChunks.size()) {
        VisibleChunk* vs = ConstructNewVisibleChunk(c);
        if (vs) {
            visibleChunks.push_back(vs);
        }
    }
}

void WorldRenderer::UpdateBlock(const Vector3I& p) {
    UpdateMesh(p);
    SIDES(
        if (Block::Visible(System::world->GetBlock(p + s))) {
            UpdateMesh(p + s);
        }
    );
}

void WorldRenderer::UpdateChunk(Chunk* c, const Vector3I& p) {
    if (c == nullptr) return;
    int i;
    for (i = 0; i < visibleChunks.size(); i++) {
        VisibleChunk* vc = visibleChunks[i];

        if (!vc || vc->chunk != c) continue;

        ConstructChunkModelData(c, vc);
        vc->UpdateModel(mf);
        return;
    }
    if (i == visibleChunks.size()) {
        VisibleChunk* vs = ConstructNewVisibleChunk(c);
        if (vs) {
            visibleChunks.push_back(vs);
        }
    }
}

void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (!visibleChunks[i]) continue;
        visibleChunks[i]->model->Render();
    }
}