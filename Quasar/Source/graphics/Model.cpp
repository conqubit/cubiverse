#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/Shader.h"

#define BUFFER_OFFSET(i) ((void*)(i))

Model::Model() :
temp(),
texture(),
shader(),
vertexCount(),
indexCount(),
vertexBuffer(),
indexBuffer(),
vertexArrayObject(),
topology(GL_TRIANGLES) {
}

Model::~Model() {
}

bool Model::Init(const ModelFactory& mf, int buffExtra = 0) {
    shader = mf.shader;
    texture = mf.texture;
    vertexCount = mf.VertexCount();
    indexCount = mf.IndexCount();
    vertexBufferByteSize = mf.VertexDataByteSize() + buffExtra;

    glGenVertexArrays(1, &vertexArrayObject);

    Bind();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferByteSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataByteSize(), mf.VertexData());

    for (int i = 0; i < mf.AttributeCount(); i++) {
        GLuint L = glGetAttribLocation(shader->program, mf.GetAttribute(i).name.c_str());
        glEnableVertexAttribArray(L);
        glVertexAttribPointer(L, mf.GetAttribute(i).size, GL_FLOAT, GL_FALSE, mf.VertexByteStride(), BUFFER_OFFSET(mf.GetAttribute(i).byteOffset));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (texture) {
        glUniform1i(glGetUniformLocation(shader->program, "textureSampler"), 0);
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
        glBindSampler(0, texture->sampler);
    }

    //glGenBuffers(1, &indexBuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexBuffer, mf.IndexData(), GL_STATIC_DRAW);

    Unbind();

    return vertexBuffer /*indexBuffer*/ && shader && vertexArrayObject;
}

bool Model::Update(const ModelFactory& mf) {
    if (mf.VertexDataByteSize() <= vertexBufferByteSize) {
        shader = mf.shader;
        texture = mf.texture;
        vertexCount = mf.VertexCount();
        indexCount = mf.IndexCount();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataByteSize(), mf.VertexData());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return true;
    }
    return false;
}

void Model::Bind() {
    glBindVertexArray(vertexArrayObject);
}

void Model::Unbind() {
    glBindVertexArray(0);
}

void Model::Render() {
    shader->Bind();
    Bind();

    glm::mat4 m;

    if (!temp) {
        m = Graphics::proj * System::player->View() * world;
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE,
                           glm::value_ptr(m));
    } else {
        m = Graphics::ortho;
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE,
                           glm::value_ptr(m));
    }

    //if (topology != GL_QUADS && topology != GL_TRIANGLES) {
    //    glDrawElements(topology, indexCount, GL_INT, BUFFER_OFFSET(0));
    //} else {
    glDrawArrays(topology, 0, vertexCount);
    //}

    Unbind();
    shader->Unbind();
}

void Model::Shutdown() {
    //glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayObject);
}