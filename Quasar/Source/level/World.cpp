#include "stdafx.h"

#include "level/World.h"

World::World() :
    chunkWidth(),
    width(),
    numChunks() {
}

World::~World() {
}

void World::Init(int chunksX, int chunksY, int chunksZ) {
    chunkWidth = Vector3I(chunksX, chunksY, chunksZ);
    width = chunkWidth * Chunk::DIM;

    numChunks = chunksX * chunksY * chunksZ;
}

void World::Shutdown() {
    level.Shutdown();
}

void World::Fill(int t) {
    VEC3_RANGE(chunkWidth) {
        Chunk* c = new Chunk();
        c->Init(t, p);
        InsertChunk(c);
    }
}

void World::Fill(int t, Vector3I a, Vector3I b) {
    VEC3_RANGE_AB(a, b) {
        SetBlock(p, t);
    }
}

void World::Generate() {
    Vector3I min(0, 0, 0);
    Vector3I max = width.Offset(-1);
    Fill(Block::Grass, min.Offset(32), max.Offset(-32));
    Fill(Block::Dirt, min.Offset(33), max.Offset(-33));
    Fill(Block::Stone, min.Offset(37), max.Offset(-37));
}

bool World::Intersects(const BoundingBox& bb)const {
    VEC3_RANGE_AB(bb.Min().Floor(), bb.Max().Floor()) {
        if (Block::Solid(GetBlock(p))) {
            return true;
        }
    }
    return false;
}

Vector3I World::GetUp(const Vector3D& pos)const {
    Vector3D p = pos - (width.ToDouble() / 2.0);
    Vector3D a = p.Abs();
    if (a.x > a.y && a.x > a.z) {
        return Vector3I::AXIS_X * SIGN(p.x);
    } else if (a.y > a.z) {
        return Vector3I::AXIS_Y * SIGN(p.y);
    } else {
        return Vector3I::AXIS_Z * SIGN(p.z);
    }
}

Vector3D World::GetUpSmooth(const Vector3D& pos)const {
    double radius = 8.0;
    Vector3D p = pos - (width.ToDouble() / 2.0);
    Vector3D a = p.Abs();
    int x = 0, y = 1, z = 2;
    if (a.x > a.y && a.x > a.z) {
        z = 0; x = 2;
    } else if (a.y > a.z) {
        z = 1; y = 2;
    }
    a[z] -= radius;
    if (a[x] > a[z]) {
        a[x] = a[z];
    }
    if (a[y] > a[z]) {
        a[y] = a[z];
    }
    a[x] *= SIGN(p[x]);
    a[y] *= SIGN(p[y]);
    a[z] *= SIGN(p[z]);
    return (p - a).Normalize();
}