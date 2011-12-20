#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

WorldRenderer::WorldRenderer() :
world(nullptr),
vPos(0), vCol(1), vTex(2) {
}

int numBlocks = 4;


bool WorldRenderer::Init(World* w) {
    world = w;

    mf.shader = Res::GetShader("block");
    mf.texture = Res::GetTexture("blocks");
    mf.topology = GL_TRIANGLES;

    vPos = mf.AddAttribute<float>("position", 3);
    vCol = mf.AddAttribute<byte>("color", 4, true);
    vTex = mf.AddAttribute<float>("texcoord", 3);

    br = BlockRenderer(*this);

    return true;
}


void WorldRenderer::Shutdown() {
    ShutdownGraphics();
    world = nullptr;
    delete this;
}


void WorldRenderer::Render() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (!visibleChunks[i]) continue;
        visibleChunks[i]->model->Render();
    }
}


void WorldRenderer::InitGraphics() {
    for (int i = 0; i < world->numChunks; i++) {
        VisibleChunk* vs = ConstructNewVisibleChunk(world->chunks[i]);
        if (vs) {
            visibleChunks.push_back(vs);
        }
    }
}


void WorldRenderer::ShutdownGraphics() {
    for (int i = 0; i < visibleChunks.size(); i++) {
        if (visibleChunks[i]) {
            visibleChunks[i]->Shutdown();
        }
    }
    visibleChunks.clear();
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


void WorldRenderer::ConstructChunkModelData(Chunk* c,  VisibleChunk* vs) {
    mf.Clear();
    VEC3_RANGE_OFFSET(c->pos, Chunk::DIM_VEC) {
        int stiz = mf.VertexDataSize();
        br.ConstructBlock(p);
        stiz = mf.VertexDataSize() - stiz;
        if (stiz > 0) {
            VisibleChunk::VisibleBlock& vb = vs->visibleBlocks[Chunk::GetIndex(p)];
            vb.location = mf.VertexDataSize() - stiz;
            vb.size = stiz;
        }
    }
}


void WorldRenderer::UpdateMesh(const Vector3I& p) {
    Chunk* c = System::world->GetChunk(p);
    if (!c) return;
    int i;
    for (i = 0; i < visibleChunks.size(); i++) {
        VisibleChunk* vc = visibleChunks[i];

        if (!vc || vc->chunk != c) continue;

        mf.Clear();
        br.ConstructBlock(p);
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