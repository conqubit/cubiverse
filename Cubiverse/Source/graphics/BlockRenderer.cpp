#include "stdafx.h"

#include "graphics/BlockRenderer.h"
#include "world/World.h"
#include "graphics/ModelFactory.h"
#include "graphics/WorldRenderer.h"

extern int numBlocks;


BlockRenderer::BlockRenderer() :
wr(nullptr), world(nullptr), mf(nullptr),
vPos(0), vCol(1), vTex(2),
numVertices(0) {
}


BlockRenderer::BlockRenderer(WorldRenderer& wr) :
wr(&wr), world(wr.world), mf(&wr.mf),
vPos(wr.vPos), vCol(wr.vCol), vTex(wr.vTex),
numVertices(0) {
}


int BlockRenderer::ConstructBlock(const Vector3I& p) {
	pos = p;
	block = world->GetBlock(p);

	if (block == Block::Air) return 0;

	numVertices = 0;

	if (!Block::Opaque(world->GetBlock(p.x + 1, p.y, p.z))) {
		side = Vector3I(1, 0, 0);
		ConstructFace(p + side, 1, 2, 0, 0.85f);
	}
	if (!Block::Opaque(world->GetBlock(p.x - 1, p.y, p.z))) {
		side = Vector3I(-1, 0, 0);
		ConstructFace(p, 2, 1, 0, 0.7f);
	}
	if (!Block::Opaque(world->GetBlock(p.x, p.y + 1, p.z))) {
		side = Vector3I(0, 1, 0);
		ConstructFace(p + side, 2, 0, 1, 0.8f);
	}
	if (!Block::Opaque(world->GetBlock(p.x, p.y - 1, p.z))) {
		side = Vector3I(0, -1, 0);
		ConstructFace(p, 0, 2, 1, 0.75f);
	}
	if (!Block::Opaque(world->GetBlock(p.x, p.y, p.z + 1))) {
		side = Vector3I(0, 0, 1);
		ConstructFace(p + side, 0, 1, 2, 1.0f);
	}
	if (!Block::Opaque(world->GetBlock(p.x, p.y, p.z - 1))) {
		side = Vector3I(0, 0, -1);
		ConstructFace(p, 1, 0, 2, 0.5f);
	}

	return numVertices * mf->VertexStride();
}


inline ModelFactory& BlockRenderer::NextVertex() {
	numVertices++;
	return mf->Next();
}


void BlockRenderer::ConstructFace(const Vector3I& v, int xi, int yi, int zi, float b) {
	up = world->GetUp(pos.ToDouble().Offset(0.5) + side.ToDouble() / 2.0);
	ColorB c = ColorF(b, b, b).ToByte();
	float tz = GetTextureFloatIndex();

	NextVertex().Set(vPos, v).Set(vCol, c).Set(vTex, 0, 0, tz);
	NextVertex().Set(vPos, v + Vector3I::Axis[yi]).Set(vCol, c).Set(vTex, 0, 1, tz);
	NextVertex().Set(vPos, v + Vector3I::Axis[yi] + Vector3I::Axis[xi]).Set(vCol, c).Set(vTex, 1, 1, tz);

	NextVertex().Set(vPos, v + Vector3I::Axis[yi] + Vector3I::Axis[xi]).Set(vCol, c).Set(vTex, 1, 1, tz);
	NextVertex().Set(vPos, v + Vector3I::Axis[xi]).Set(vCol, c).Set(vTex, 1, 0, tz);
	NextVertex().Set(vPos, v).Set(vCol, c).Set(vTex, 0, 0, tz);
}


inline float BlockRenderer::GetTextureFloatIndex() {
	return (float) GetTextureIndex() / numBlocks + 1.0f / (numBlocks * 2);
}


int BlockRenderer::GetTextureIndex() {
	switch (block) {
	case Block::Stone:
		return 0;
	case Block::Dirt:
		return 1;
	case Block::Grass:
		if (side == up) return 2;
		return 1;
	case Block::Test:
		return 3;
	}
	return 0;
}