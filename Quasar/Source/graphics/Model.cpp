#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/Shader.h"

#define BUFFER_OFFSET(i) ((void*)(i))

Model::Model() :
orthographic(),
texture(),
shader(),
vertexCount(),
indexCount(),
vertexBuffer(),
indexBuffer(),
vertexArrayObject(),
vertexBufferSize(),
topology(GL_TRIANGLES) {
}

Model::~Model() {
}

bool Model::Init(const ModelFactory& mf, int buffExtra) {
    shader = mf.shader;
    texture = mf.texture;
    vertexCount = mf.VertexCount();
    indexCount = mf.IndexCount();
    vertexBufferSize = mf.VertexDataSize() + buffExtra;

    glGenVertexArrays(1, &vertexArrayObject);

    Bind();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataSize(), mf.VertexData());

    for (int i = 0; i < mf.AttributeCount(); i++) {
        const ModelFactory::Attribute& attr = mf.GetAttribute(i);
        if (attr.hidden) continue;
        GLuint L = glGetAttribLocation(shader->program, attr.name.c_str());
        glEnableVertexAttribArray(L);
        glVertexAttribPointer(L, attr.count, attr.glType, attr.normalized, mf.VertexStride(), BUFFER_OFFSET(attr.offset));
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

void Model::EnableOrtho(bool ortho) {
    orthographic = ortho;
}

bool Model::Update(const ModelFactory& mf) {
    if (mf.VertexDataSize() <= vertexBufferSize) {
        shader = mf.shader;
        texture = mf.texture;
        vertexCount = mf.VertexCount();
        indexCount = mf.IndexCount();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataSize(), mf.VertexData());
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

byte* Model::Map(GLenum access) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    return (byte*)glMapBuffer(GL_ARRAY_BUFFER, access);
}

void Model::Unmap() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Render() {
    shader->Bind();
    Bind();

    glm::mat4 m;
    if (orthographic) {
        m = Graphics::GetOrtho() * world;
    } else {
        m = Graphics::GetViewProj() * world;
    }

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE, glm::value_ptr(m));

    glDrawArrays(topology, 0, vertexCount);

    Unbind();
    shader->Unbind();
}

void Model::Shutdown() {
    //glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayObject);
}