#pragma once

#include "math/Math.h"

template <typename T>
struct BoundingBox;

typedef BoundingBox<int>	BoundingBoxI;
typedef BoundingBox<float>	BoundingBoxF;
typedef BoundingBox<double>	BoundingBoxD;

template <typename T>
struct BoundingBox {
	static BoundingBox<T> Block(Vector3I p) {
		return BoundingBox<T>(p, p.Offset(1));
	}

	Vector3<T> min, max;

	BoundingBox() : min(), max() {
	}

	template <typename U>
	BoundingBox(const Vector3<U>& a, const Vector3<U>& b) :
	min(MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z)),
	max(MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z)) {
	}

	BoundingBox(T x1, T y1, T z1, T x2, T y2, T z2) :
	min(MIN(x1, x2), MIN(y1, y2), MIN(z1, z2)),
	max(MAX(x1, x2), MAX(y1, y2), MAX(z1, z2)) {
	}

	~BoundingBox() {
	}

	template <typename U>
	bool Intersects(const BoundingBox<U>& o)const {
		return	min.x < o.max.x && max.x > o.min.x &&
				min.y < o.max.y && max.y > o.min.y &&
				min.z < o.max.z && max.z > o.min.z;
	}

	BoundingBox<T> Offset(const Vector3D& offset)const {
		return BoundingBox<T>(min + offset, max + offset);
	}
};