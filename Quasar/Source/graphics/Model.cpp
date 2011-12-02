#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/Shader.h"

#define BUFFER_OFFSET(i) ((void*)(i))

Model::Model() :
temp(),
world(),
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

bool Model::Init(const ModelFactory& mf) {
    shader = mf.shader;
    texture = mf.texture;
    vertexCount = mf.VertexCount();
    indexCount = mf.IndexCount();

    glGenVertexArrays(1, &vertexArrayObject);

    Bind();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mf.VertexDataByteSize(), mf.VertexData(), GL_STATIC_DRAW);

    for (int i = 0; i < mf.AttributeCount(); i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, mf.GetAttribute(i).size, GL_FLOAT, GL_FALSE, mf.VertexByteStride(), BUFFER_OFFSET(mf.GetAttribute(i).byteOffset));
    }

    if(texture) {
        glUniform1i(glGetUniformLocation(shader->program, "textureSampler"), 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glGenBuffers(1, &indexBuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexBuffer, mf.IndexData(), GL_STATIC_DRAW);

    Unbind();

    return vertexBuffer /*indexBuffer*/ && shader && vertexArrayObject;
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
        m = System::graphics->proj * System::player->View() * world;
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE,
                           glm::value_ptr(m));
    } else {
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE,
                           glm::value_ptr(m));
    }

    if (texture) {
        glUniform1i(glGetUniformLocation(shader->program, "textureSampler"), 0);
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
    }

    //if (topology != GL_QUADS && topology != GL_TRIANGLES) {
    //    glDrawElements(topology, indexCount, GL_INT, BUFFER_OFFSET(0));
    //} else {
    glDrawArrays(topology, 0, vertexCount);
    //}

    if (texture) texture->Unbind();
    Unbind();
    shader->Unbind();
}

void Model::Shutdown() {
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &vertexBuffer);
}