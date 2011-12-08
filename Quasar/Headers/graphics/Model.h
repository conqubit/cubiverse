#pragma once

#include "graphics/Graphics.h"
#include "graphics/Texture.h"

#include "math/Math.h"
#include "graphics/IRender.h"
#include "graphics/Shader.h"

class ModelFactory;

class Model : public IRender {
public:
    Model();
    ~Model();
    bool Init(const ModelFactory& mf, int buffExtra);
    void Render();

    bool Update(const ModelFactory& mf);

    void Shutdown();
    void Bind();
    void Unbind();

    bool temp;

    glm::mat4 world;

    Shader* shader;
    Texture* texture;

    int vertexCount;
    int indexCount;

    int vertexBufferByteSize;

    GLenum topology;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint vertexArrayObject;
};