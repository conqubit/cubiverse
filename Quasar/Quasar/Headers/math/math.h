#pragma once

#include <math.h>
#include <xnamath.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "math/vector3.h"

#define square(x) (x)*(x)

#define PI     3.14159265358979323846
#define PI_2   1.57079632679489661923
#define SQRT_2 1.41421356237309504880

struct Vertex {
    Vector3F position;
    XMFLOAT4 color;

    Vertex() {
    }

    Vertex(Vector3F pos, XMFLOAT4 color) :
        position(pos), color(color) {
    }
};

class Matrix {
public:
    Matrix() {
        set(XMMatrixIdentity());
    }

    XMFLOAT4X4 data;

    XMMATRIX get() {
        return XMLoadFloat4x4(&data);
    }
    void set(const XMMATRIX& mat) {
        XMStoreFloat4x4(&data, mat);
    }
};