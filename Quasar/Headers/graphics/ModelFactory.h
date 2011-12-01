#pragma once

#include "graphics/Model.h"
#include "graphics/Shader.h"

class ModelFactory {
public:
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    Shader* shader;
    GLenum topology;

    ColorF colorState;

    ModelFactory() :
    vertices(),
    indices(),
    shader(),
    topology(GL_TRIANGLES),
    colorState(0, 0, 0, 1) {
    }

    ~ModelFactory() {
    }

    void AddVertex(const Vertex& v) {
        vertices.push_back(v);
    }

    void AddVertex(const Vector3F& v, const ColorF& c) {
        AddVertex(Vertex(v, c));
    }

    void AddVertex(double x, double y, double z, double r, double g, double b) {
        AddVertex(Vertex(Vector3F(x, y, z), ColorF(r, g, b)));
    }

    void AddVertex(double x, double y, double z) {
        AddVertex(Vertex(Vector3F(x, y, z), colorState));
    }

    void AddTriangle(int a, int b, int c) {
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }

    void AddSegment(int a, int b) {
        indices.push_back(a);
        indices.push_back(b);
    }

    int VertexCount()const {
        return (int)vertices.size();
    }

    int IndexCount()const {
        return (int)indices.size();
    }

    Vertex* VertexData()const {
        return (Vertex*)vertices.data();
    }

    int* IndexData()const {
        return (int*)indices.data();
    }

    Model* Create()const {
        Model* m = new Model();
        m->topology = topology;
        if (!m->Init(*this)) {
            delete m;
            return nullptr;
        }
        return m;
    }

    void Clear() {
        vertices.clear();
        indices.clear();
    }
};