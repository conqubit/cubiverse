#include "stdafx.h"
#include "graphics/shader.h"
#include "system.h"
#include <stdio.h>

Shader::Shader() {
}
Shader::~Shader() {
}

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, LPCWSTR shaderFile) {
    fputs((char*)(errorMessage->GetBufferPointer()), fopen("shader-error.txt", "w"));
    errorMessage->Release();
    MessageBox(System::hWindow, L"Error compiling shader.", shaderFile, MB_OK);
}

bool Shader::init(LPCWSTR shaderFile, LPCSTR vertexShaderName, LPCSTR pixelShaderName) {
    HRESULT r;
    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vsBuffer = nullptr;
    ID3D10Blob* psBuffer = nullptr;

    r = D3DX11CompileFromFile(shaderFile, nullptr, nullptr, vertexShaderName, "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsBuffer, &errorMessage, nullptr);

    if (FAILED(r)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, shaderFile);
        }
        return false;
    }

    r = D3DX11CompileFromFile(shaderFile, nullptr, nullptr, pixelShaderName, "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &psBuffer, &errorMessage, nullptr);

    if (FAILED(r)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, shaderFile);
        }
        return false;
    }

    r = System::graphics->getDevice()->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &vertexShader);
    if (FAILED(r)) {
        return false;
    }

    r = System::graphics->getDevice()->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(r)) {
        return false;
    }

    const int numElements = 4;
    D3D11_INPUT_ELEMENT_DESC ld[numElements];
    ZeroMemory(ld, sizeof(ld));

    ld[0].SemanticName = "POSITION";
    ld[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    ld[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    ld[1].SemanticName = "NORMAL";
    ld[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    ld[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    ld[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    ld[2].SemanticName = "COLOR";
    ld[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    ld[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    ld[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    ld[3].SemanticName = "TEXCOORD";
    ld[3].Format = DXGI_FORMAT_R32G32_FLOAT;
    ld[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    ld[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    r = System::graphics->getDevice()->CreateInputLayout(ld, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);
    if (FAILED(r)) {
        return false;
    }

    vsBuffer->Release();
    psBuffer->Release();

    D3D11_BUFFER_DESC buffDesc;
    ZeroMemory(&buffDesc, sizeof(buffDesc));

    buffDesc.Usage = D3D11_USAGE_DYNAMIC;
    buffDesc.ByteWidth = sizeof(MatrixBuffer);
    buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    r = System::graphics->getDevice()->CreateBuffer(&buffDesc, nullptr, &matrixBuffer);
    if (FAILED(r)) {
        return false;
    }

    return true;
}
void Shader::shutdown() {
    matrixBuffer->Release();
    inputLayout->Release();
    pixelShader->Release();
    vertexShader->Release();
}
void Shader::updateConstants(XMMATRIX* mat) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    System::graphics->getDeviceContext()->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    MatrixBuffer* data = (MatrixBuffer*)mappedResource.pData;
    data->camPos = System::graphics->cam->pos.data;
    data->worldViewProj = XMMatrixTranspose(*mat);
    System::graphics->getDeviceContext()->Unmap(matrixBuffer, 0);

    System::graphics->getDeviceContext()->VSSetConstantBuffers(0, 1, &matrixBuffer);
    System::graphics->getDeviceContext()->PSSetConstantBuffers(0, 1, &matrixBuffer);
}
void Shader::render(Model* model) {
    XMMATRIX m = System::graphics->viewMat.get() * System::graphics->projMat.get();
    updateConstants(&m);

    System::graphics->getDeviceContext()->IASetInputLayout(inputLayout);
    System::graphics->getDeviceContext()->VSSetShader(vertexShader, nullptr, 0);
    System::graphics->getDeviceContext()->PSSetShader(pixelShader, nullptr, 0);

    System::graphics->getDeviceContext()->DrawIndexed(model->indexCount, 0, 0);
}