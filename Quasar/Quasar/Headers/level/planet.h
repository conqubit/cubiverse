#pragma once

#include "chunk.h"

class Planet {
public:
	Planet();
	~Planet();
	void Init(int csx, int csy, int csz);
	void Shutdown();
	void Generate();
	void EnumerateVisibleBlocks();

	// Number of chunks for each dimension; forms a rectangular prism.
	int csx, csy, csz;
	int numChunks;

	Chunk* chunks;
};