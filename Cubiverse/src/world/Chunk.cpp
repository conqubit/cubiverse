#include "stdafx.h"

#include "world/Chunk.h"

void Chunk::Init(int t, const Vector3I& p) {
	cpos = p;
	Fill(t);
}

void Chunk::Shutdown() {
	delete this;
}

void Chunk::Fill(int t) {
	memset(data, t, Chunk::Size);
}