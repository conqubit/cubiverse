#pragma once

#include "math/Math.h"

class BoundingBox {
private:
    Vector3D min, max;

public:
    static BoundingBox Block(int x, int y, int z) {
        return BoundingBox(x, y, z, x + 1, y + 1, z + 1);
    }

    static BoundingBox Block(Vector3I p) {
        return BoundingBox(p.x, p.y, p.z, p.x + 1, p.y + 1, p.z + 1);
    }


    BoundingBox() : min(), max() {
    }

    BoundingBox(Vector3D a, Vector3D b) :
    min(MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z)),
    max(MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z)) {
    }

    BoundingBox(double x1, double y1, double z1, double x2, double y2, double z2) :
    min(MIN(x1, x2), MIN(y1, y2), MIN(z1, z2)),
    max(MAX(x1, x2), MAX(y1, y2), MAX(z1, z2)) {
    }

    ~BoundingBox() {
    }

    const Vector3D& Min()const {
        return min;
    }

    const Vector3D& Max()const {
        return max;
    }

    bool Intersects(const BoundingBox& o)const {
        return min.x < o.max.x && max.x > o.min.x &&
               min.y < o.max.y && max.y > o.min.y &&
               min.z < o.max.z && max.z > o.min.z;
    }

    BoundingBox Offset(const Vector3D& offset)const {
        return BoundingBox(min + offset, max + offset);
    }
};