#include "stdafx.h"

#include "level/Chunk.h"

Chunk::Chunk() : pos() {
}

Chunk::~Chunk() {
}

void Chunk::Init(int t, const Vector3I& p) {
	pos = p;
	Fill(t);
}

void Chunk::Shutdown() {
	delete this;
}

void Chunk::Fill(int t) {
	memset(data, t, Chunk::Size);
}