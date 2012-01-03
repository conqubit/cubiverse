#pragma once

#include "math/BoundingBox.h"

#include "world/Chunk.h"
#include "world/Level.h"

class World {
public:
	Vector3I chunkWidth;
	Vector3I width;

	int numChunks;

	Vector3I cmin, cmax;

	Level level;

	World();
	~World();

	void Init(int chunksX, int chunksY, int chunksZ);
	void Shutdown();

	void Fill(int t);
	void Fill(int t, Vector3I a, Vector3I b);
	void Generate();

	Vector3I GetUp(const Vector3D& pos)const;

	Vector3D GetUpSmooth(const Vector3D& pos)const;

	bool Intersects(const BoundingBoxD& bb)const;

	int GetBlock(const Vector3I& p)const {
		Chunk* c = GetChunkFromBlock(p);
		return c ? c->GetBlock(p) : Block::Undefined;
	}

	int GetBlock(int x, int y, int z)const {
		return GetBlock(Vector3I(x, y, z));
	}

	void SetBlock(const Vector3I& p, int t) {
		Chunk* c = GetChunkFromBlock(p);
		if (!c) {
			c = new Chunk();
			c->cpos = p.ToChunkCoords();
			c->Fill(Block::Air);
			InsertChunk(c);
		}
		c->SetBlock(p, t);
	}

	Chunk* GetChunk(const Vector3I& cp)const {
		return level.GetChunk(cp);
	}

	Chunk* GetChunkFromBlock(const Vector3I& p)const {
		return level.GetChunk(p.ToChunkCoords());
	}

	void InsertChunk(Chunk* c) {
		level.InsertChunk(c);

		if (c->cpos.x < cmin.x) cmin.x = c->cpos.x;
		else if (c->cpos.x > cmax.x) cmax.x = c->cpos.x;

		if (c->cpos.y < cmin.y) cmin.y = c->cpos.y;
		else if (c->cpos.y > cmax.y) cmax.y = c->cpos.y;

		if (c->cpos.z < cmin.z) cmin.z = c->cpos.z;
		else if (c->cpos.z > cmax.z) cmax.z = c->cpos.z;
	}

	bool InBlockBounds(const Vector3I& p)const {
		return p.x >= 0 && p.x < width.x &&
			   p.y >= 0 && p.y < width.y &&
			   p.z >= 0 && p.z < width.z;
	}
};