#pragma once

#include <d3d11.h>
#include <d3dx10.h>

#include "math/math.h"
#include "graphics/irender.h"
#include "graphics/shader.h"

class ModelFactory;

class Model : public IRender {
public:
    Model();
    ~Model();
    bool Init(const ModelFactory& mf);
    void Render();
    void Shutdown();

    Matrix mat;
    Shader* shader;
    int vertexCount;
    int indexCount;
private:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
};