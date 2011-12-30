#include "stdafx.h"

#include "world/Block.h"

bool Block::solid[256];
bool Block::opaque[256];

// Because C++ doesn't have designated initializers :(
void Block::Init() {
	solid[Air]			= 0;
	solid[Stone]		= 1;
	solid[Dirt]			= 1;
	solid[Grass]		= 1;
	solid[Test] 		= 1;
	solid[Undefined]	= 0;

	opaque[Air]			= 0;
	opaque[Stone]		= 1;
	opaque[Dirt]		= 1;
	opaque[Grass]		= 1;
	opaque[Test]		= 1;
	opaque[Undefined]	= 0;
}