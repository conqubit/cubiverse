#pragma once

#include <xnamath.h>
#include <math.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "math/vector3.h"

#define square(x) (x)*(x)

struct Vertex {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    D3DXCOLOR color;
    XMFLOAT2 texcoord;

    Vertex() {
    }

    Vertex(XMFLOAT3 pos, XMFLOAT3 norm, D3DXCOLOR col) : position(pos), normal(norm), color(col) {
    }
};

class Vec3 {
public:
    Vec3() :
        data() {
    }

    XMFLOAT3 data;

    XMVECTOR get() {
        return XMLoadFloat3(&data);
    }
    void set(const XMVECTOR& vec) {
        XMStoreFloat3(&data, vec);
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