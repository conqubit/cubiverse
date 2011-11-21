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
    indexBuffer() {
}

Model::~Model() {
}

bool Model::Init(const ModelFactory& mf) {
    shader = mf.shader;
    vertexCount = mf.vertices.Count();
    indexCount = mf.indices.Count();

    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;

    HRESULT r;
    
    if (vertexCount == 0 || indexCount == 0) {
        return false;
    }

    ZeroStruct(vertexBufferDesc);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    ZeroStruct(vertexData);
    vertexData.pSysMem = mf.vertices.DataPtr();

    r = System::graphics->Device()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(r)) return false;

    ZeroStruct(indexBufferDesc);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    ZeroStruct(indexData);
    indexData.pSysMem = mf.indices.DataPtr();

    r = System::graphics->Device()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    if (FAILED(r)) return false;

    return true;
}

void Model::Render() {
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    System::graphics->DeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    System::graphics->DeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    System::graphics->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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