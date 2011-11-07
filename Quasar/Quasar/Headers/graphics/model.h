#pragma once

#include <d3d11.h>
#include <d3dx10.h>
#include "math.h"
#include "graphics/shader.h"

#define MF_CLOCKWISE true
#define MF_COUNTER_CLOCKWISE false

struct ModelDesc {
    int vertexCount;
    int indexCount;
    Vertex* vertices;
    int* indices;

    Shader shader;

    ModelDesc() :
        vertexCount(),
        indexCount(),
        vertices(),
        indices(),
        shader() {
    }

    ~ModelDesc() {
    }

    void DeleteBuffers() {
        delete[] indices;
        delete[] vertices;
        indices = nullptr;
        vertices = nullptr;
    }
};

class Model {
public:
    Model();
    ~Model();
    bool Init(ModelDesc* modelDesc);
    void Render();
    void Shutdown();

    Matrix mat;
    Shader shader;
    int vertexCount;
    int indexCount;
private:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
};

class ModelFactory {
public:
    static ModelDesc CreateGlobe(double radius, int width, int height, bool clockwise);
    static Model* CreateBox(Shader* shader);
};