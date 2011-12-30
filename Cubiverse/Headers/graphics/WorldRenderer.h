#pragma once

#include "graphics/BlockRenderer.h"
#include "graphics/VisibleChunk.h"
#include "graphics/IRender.h"

#include "world/World.h"
#include "world/Chunk.h"
#include "world/Block.h"

class WorldRenderer : public IRenderObject {
public:
	std::map<Chunk*, VisibleChunk*> visibleChunks;
	World* world;
	ModelFactory mf;

	int vPos, vCol, vTex;

	BlockRenderer br;

	WorldRenderer();

	bool Init(World* w);
	void Shutdown();

	virtual void InitGraphics();
	virtual void ShutdownGraphics();

	void ConstructChunkModelData(Chunk* c);
	void ReconstructChunkModelData(Chunk* c);

	void ConstructNewVisibleChunk(Chunk* c);
	void ReconstructVisibleChunk(Chunk* c);

	static void ConstructFace(int block, const Vector3I& side, const Vector3I& p, int x, int y, int z, int xi, int yi, int zi, double b);
	void ConstructBlock(const Vector3I& p);
	void UpdateBlock(const Vector3I& p);
	void UpdateChunk(Chunk* c, const Vector3I& p);
	void Render();
	void UpdateMesh(const Vector3I& p);
};