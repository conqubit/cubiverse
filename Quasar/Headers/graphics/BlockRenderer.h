#pragma once

class WorldRenderer;
class World;
class ModelFactory;

class BlockRenderer {
public:
	WorldRenderer* wr;
	World* world;
	ModelFactory* mf;

	int vPos, vCol, vTex;

	// State.
	int block;
	Vector3I pos, side, up;
	int numVertices;

	BlockRenderer();
	BlockRenderer(WorldRenderer& wr);

	ModelFactory& NextVertex();

	int ConstructBlock(const Vector3I& p);
	void ConstructFace(const Vector3I& v, int xi, int yi, int zi, float b);

	float GetTextureFloatIndex();
	int GetTextureIndex();
};