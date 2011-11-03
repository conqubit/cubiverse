#include "stdafx.h"
#include "level/chunk.h"

Chunk::Chunk() {
	cx = 0;
	cy = 0;
}

Chunk::~Chunk() {
}

void Chunk::Init() {
	ZeroMemory(data, CHUNK_SIZE);
}

void Chunk::Fill(unsigned char type) {
	memset(data, type, CHUNK_SIZE);
}