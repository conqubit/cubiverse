#include "stdafx.h"

#include "System.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"
#include "graphics/Shader.h"

#define BUFFER_OFFSET(i) ((void*)(i))

Model::Model() :
temp(),
world(),
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
    vertexCount = mf.VertexCount();
    indexCount = mf.IndexCount();

    glGenVertexArrays(1, &vertexArrayObject);
    
    Bind();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, mf.VertexData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(Vector3F)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexBuffer, mf.IndexData(), GL_STATIC_DRAW);*/

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

    if (!temp) {
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "proj"), 1, GL_FALSE,
                           glm::value_ptr(System::graphics->proj));
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "view"), 1, GL_FALSE,
                           glm::value_ptr(System::player->View()));
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "world"), 1, GL_FALSE,
                           glm::value_ptr(world));
    } else {
        glm::mat4 m;
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "proj"), 1, GL_FALSE,
                           glm::value_ptr(m));
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "view"), 1, GL_FALSE,
                           glm::value_ptr(m));
        glUniformMatrix4fv(glGetUniformLocationARB(shader->program, "world"), 1, GL_FALSE,
                           glm::value_ptr(m));
    }

    /*if (topology != GL_QUADS && topology != GL_TRIANGLES) {
        glDrawElements(topology, indexCount, GL_INT, BUFFER_OFFSET(0));
    } else {*/
    glDrawArrays(topology, 0, vertexCount);
    //}

    Unbind();
    shader->Unbind();
}

void Model::Shutdown() {
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &vertexBuffer);
}