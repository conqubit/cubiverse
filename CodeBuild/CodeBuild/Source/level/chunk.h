#pragma once

#define CHUNK_DIM 32
#define CHUNK_SIZE (32 * 32 * 32)

class Chunk {
public:
	Chunk();
	~Chunk();
	void Init();
	int cx, cy;
	unsigned char data[CHUNK_SIZE];
	void Fill(unsigned char type);
};