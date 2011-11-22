#pragma once

#include "math/Vector3.h"
#include "graphics/Direct3D.h"

class Model;

struct MatrixBuffer {
    XMMATRIX worldViewProj;
};

class Shader {
public:
    Shader();
    ~Shader();
    bool Init(LPCWSTR shaderFile, LPCSTR vertexShaderName, LPCSTR pixelShaderName);
    void Shutdown();
    void UpdateConstants(XMMATRIX& mat);
    void Render(Model* model);

    ID3D10VertexShader* vertexShader;
    ID3D10PixelShader* pixelShader;
    ID3D10InputLayout* inputLayout;
    ID3D10Buffer* matrixBuffer;
};