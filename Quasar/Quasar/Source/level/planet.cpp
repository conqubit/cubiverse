#include "stdafx.h"
#include "planet.h"

Planet::Planet() {
	csx = 0;
	csy = 0;
	csz = 0;
	numChunks = 0;
	chunks = nullptr;
}

Planet::~Planet() {
}

void Planet::Init(int csx, int csy, int csz) {
	this->csx = csx;
	this->csy = csy;
	this->csz = csz;

	numChunks = csx * csy * csz;
	chunks = new Chunk[numChunks];
	ZeroMemory(chunks, numChunks);
}

void Planet::Shutdown() {
	delete[] chunks;
	chunks = nullptr;
}

void Planet::Generate() {
	for (int i = 0; i < numChunks; i++) {
		chunks[i].Fill(1);
	}
}

void Planet::EnumerateVisibleBlocks() {
	for (int i = 0; i < numChunks; i++) {

	}
}