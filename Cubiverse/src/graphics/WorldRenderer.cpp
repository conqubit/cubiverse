#include "stdafx.h"

#include "System.h"
#include "Game.h"
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

	mf.usage = GL_DYNAMIC_DRAW;
	mf.topology = GL_TRIANGLES;

	vPos = mf.AddAttribute<float>("position", 3);
	vCol = mf.AddAttribute<byte>("color", 4, true);
	vTex = mf.AddAttribute<float>("texcoord", 3);

	br = BlockRenderer(*this);

	return true;
}

void WorldRenderer::Shutdown() {
	ShutdownGraphics();
	for (auto i = visibleChunks.begin(); i != visibleChunks.end(); ++i) {
		i->second->Shutdown();
	}
	delete this;
}

void WorldRenderer::Render() {
	for (auto i = visibleChunks.begin(); i != visibleChunks.end(); ++i) {
		i->second->Render();
	}
}

void WorldRenderer::InitGraphics() {
	mf.shader = Res::GetShader("block");
	mf.texture = Res::GetTexture("blocks");

	for (auto i = visibleChunks.begin(); i != visibleChunks.end(); ++i) {
		ReconstructVisibleChunk(i->first);
	}

	VEC3_RANGE_AB(world->cmin, world->cmax) {
		Chunk* c = world->GetChunk(p);
		if (visibleChunks.count(c) == 0) {
			ConstructNewVisibleChunk(c);
		}
	}
}

void WorldRenderer::ShutdownGraphics() {
	for (auto i = visibleChunks.begin(); i != visibleChunks.end(); ++i) {
		i->second->ShutdownGraphics();
	}
}

void WorldRenderer::ConstructNewVisibleChunk(Chunk* c) {
	if (!c) return;

	VisibleChunk* vc = new VisibleChunk();
	visibleChunks[c] = vc;

	mf.Clear();

	VEC3_RANGE_OFFSET(c->Pos(), Vector3I(Chunk::Dim)) {
		int size = br.ConstructBlock(p);
		if (size != 0) {
			VisibleChunk::VisibleBlock& vb = vc->visibleBlocks[Chunk::GetIndex(p)];
			vb.location = mf.VertexDataSize() - size;
			vb.size = size;
		}
	}

	if (mf.VertexCount() > 0) {
		vc->UpdateModel(mf);
	} else {
		visibleChunks.erase(c);
		delete vc;
	}
}

void WorldRenderer::ReconstructVisibleChunk(Chunk* c) {
	if (visibleChunks.count(c) == 0) return;

	VisibleChunk* vc = visibleChunks[c];

	mf.Clear();

	for (auto i = vc->visibleBlocks.begin(); i != vc->visibleBlocks.end(); ++i) {
		int size = br.ConstructBlock(c->GetWorldPositionFromIndex(i->first));

		VisibleChunk::VisibleBlock& vb = vc->visibleBlocks[i->first];
		vb.location = mf.VertexDataSize() - size;
		vb.size = size;
	}

	if (mf.VertexCount() > 0) {
		vc->UpdateModel(mf);
	} else {
		visibleChunks.erase(c);
		delete vc;
	}
}

void WorldRenderer::UpdateBlock(const Vector3I& p) {
	UpdateMesh(p);
	SIDES(
		if (Block::Visible(world->GetBlock(p + s))) {
			UpdateMesh(p + s);
		}
	);
}

void WorldRenderer::UpdateMesh(const Vector3I& p) {
	Chunk* c = Game::world->GetChunkFromBlock(p);
	if (!c) return;
	if (visibleChunks.count(c) == 1) {
		mf.Clear();
		br.ConstructBlock(p);
		VisibleChunk* vc = visibleChunks[c];
		vc->UpdateBlock(Chunk::GetIndex(p), mf);
		if (vc->visibleBlocks.size() == 0) {
			visibleChunks.erase(c);
			delete vc;
		}
	} else {
		ConstructNewVisibleChunk(c);
	}
}