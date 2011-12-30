#pragma once

#include "math/Vector.h"
#include "world/Block.h"

class Chunk {
public:
	static const int Dim = 32; // Must be a power of 2.
	static const int Size = Dim * Dim * Dim;

	static const int Shift = 5;
	static const int ShiftTwice = Shift * 2;
	static const int Mask = Dim - 1;

	Vector3I pos;
	byte data[Size];

	Chunk();
	~Chunk();

	Vector3I Pos()const {
		return pos.ChunkToBlockCoords();
	}
	
	void Init(int t, const Vector3I& p);
	void Shutdown();

	void Fill(int t);

	Vector3I GetWorldPositionFromIndex(int i)const {
		return Pos() + Vector3I(i & Mask, (i >> Shift) & Mask, i >> ShiftTwice);
	}

	int GetBlock(const Vector3I& p)const {
		return data[GetIndex(p)];
	}

	int SetBlock(const Vector3I& p, int t) {
		return data[GetIndex(p)] = t;
	}

	static int GetIndex(const Vector3I& p) {
		return (p.x & Mask) | ((p.y & Mask) | (p.z & Mask) << Shift) << Shift;
	}
};