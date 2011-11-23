#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/Shader.h"

Model::Model() : 
shader(),
vertexCount(),
indexCount(),
vertexBuffer(),
indexBuffer(),
topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
}

Model::~Model() {
}

bool Model::Init(const ModelFactory& mf) {
    mat.set(XMMatrixIdentity());
    shader = mf.shader;
    vertexCount = mf.vertices.Count();
    indexCount = mf.indices.Count();

    D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;

    HRESULT r;
    
    if (vertexCount == 0 || indexCount == 0) {
        return false;
    }

    ZeroStruct(vertexBufferDesc);
    vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
    vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;

    ZeroStruct(vertexData);
    vertexData.pSysMem = mf.vertices.DataPtr();

    r = System::graphics->Device()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(r)) return false;

    ZeroStruct(indexBufferDesc);
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;

    ZeroStruct(indexData);
    indexData.pSysMem = mf.indices.DataPtr();

    r = System::graphics->Device()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    if (FAILED(r)) return false;

    return true;
}

void Model::Render() {
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    System::graphics->Device()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    System::graphics->Device()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    System::graphics->Device()->IASetPrimitiveTopology(topology);

    shader->Render(this);
}

void Model::Shutdown() {
    if (indexBuffer) {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }
    if (vertexBuffer) {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
}