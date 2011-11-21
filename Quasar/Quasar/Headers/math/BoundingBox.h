#pragma once

#include "math/Vector3.h"

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


    BoundingBox() {
    }

    BoundingBox(Vector3D a, Vector3D b) {
        this->min = Vector3D(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
        this->max = Vector3D(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
    }

    BoundingBox(double x1, double y1, double z1, double x2, double y2, double z2) {
        this->min = Vector3D(min(x1, x2), min(y1, y2), min(z1, z2));
        this->max = Vector3D(max(x1, x2), max(y1, y2), max(z1, z2));
    }

    ~BoundingBox() {
    }

    Vector3D Min()const {
        return min;
    }

    Vector3D Max()const {
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

    Vector3D operator[](int i)const {
        return (&min)[i];
    }
};