#include "stdafx.h"

#include "system.h"
#include "graphics/Shader.h"
#include "graphics/Model.h"

Shader::Shader() :
    vertexShader(),
    pixelShader(),
    inputLayout(),
    matrixBuffer() {
}
Shader::~Shader() {
}

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, LPCWSTR shaderFile) {
    fputs((char*)(errorMessage->GetBufferPointer()), fopen("shader-error.txt", "w"));
    errorMessage->Release();
    MessageBox(System::hWindow, L"Error compiling shader.", shaderFile, MB_OK);
}

bool Shader::Init(LPCWSTR shaderFile, LPCSTR vertexShaderName, LPCSTR pixelShaderName) {
    HRESULT r;
    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vsBuffer = nullptr;
    ID3D10Blob* psBuffer = nullptr;

    r = D3DX10CompileFromFile(shaderFile, nullptr, nullptr, vertexShaderName, "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsBuffer, &errorMessage, nullptr);

    if (FAILED(r)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, shaderFile);
        }
        return false;
    }

    r = D3DX10CompileFromFile(shaderFile, nullptr, nullptr, pixelShaderName, "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &psBuffer, &errorMessage, nullptr);

    if (FAILED(r)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, shaderFile);
        }
        return false;
    }

    r = System::graphics->Device()->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &vertexShader);
    if (FAILED(r)) return false;

    r = System::graphics->Device()->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), &pixelShader);
    if (FAILED(r)) return false;

    const int numElements = 2;
    D3D10_INPUT_ELEMENT_DESC ld[numElements];
    ZeroStruct(ld);

    ld[0].SemanticName = "POSITION";
    ld[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    ld[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;

    /*ld[1].SemanticName = "NORMAL";
    ld[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    ld[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
    ld[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;*/

    ld[1].SemanticName = "COLOR";
    ld[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    ld[1].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
    ld[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;

    /*ld[3].SemanticName = "TEXCOORD";
    ld[3].Format = DXGI_FORMAT_R32G32_FLOAT;
    ld[3].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
    ld[3].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;*/

    r = System::graphics->Device()->CreateInputLayout(ld, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);
    if (FAILED(r)) return false;

    vsBuffer->Release();
    psBuffer->Release();

    D3D10_BUFFER_DESC buffDesc;
    ZeroStruct(buffDesc);

    buffDesc.Usage = D3D10_USAGE_DYNAMIC;
    buffDesc.ByteWidth = sizeof(MatrixBuffer);
    buffDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    buffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    r = System::graphics->Device()->CreateBuffer(&buffDesc, nullptr, &matrixBuffer);
    if (FAILED(r)) return false;

    return true;
}
void Shader::Shutdown() {
    matrixBuffer->Release();
    inputLayout->Release();
    pixelShader->Release();
    vertexShader->Release();
}
void Shader::UpdateConstants(XMMATRIX& mat) {
    MatrixBuffer* data;

    matrixBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&data);
    data->worldViewProj = XMMatrixTranspose(mat);
    matrixBuffer->Unmap();

    System::graphics->Device()->VSSetConstantBuffers(0, 1, &matrixBuffer);
    System::graphics->Device()->PSSetConstantBuffers(0, 1, &matrixBuffer);
}
void Shader::Render(Model* model) {
    XMMATRIX m = model->mat.get() * System::graphics->viewMat.get() * System::graphics->projMat.get();
    UpdateConstants(m);

    System::graphics->Device()->IASetInputLayout(inputLayout);
    System::graphics->Device()->VSSetShader(vertexShader);
    System::graphics->Device()->PSSetShader(pixelShader);

    System::graphics->Device()->DrawIndexed(model->indexCount, 0, 0);
}