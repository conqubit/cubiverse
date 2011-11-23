#pragma once

#include "math/Math.h"
#include "graphics/Model.h"
#include "graphics/Shader.h"
#include "containers/ArrayList.h"

class ModelFactory {
public:
    ArrayList<Vertex> vertices;
    ArrayList<unsigned int> indices;
    Shader* shader;
    D3D_PRIMITIVE_TOPOLOGY topology;

    ModelFactory() :
    vertices(),
    indices(),
    topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {
    }

    ~ModelFactory() {
    }

    void AddTriangle(int a, int b, int c) {
        indices.Add(a);
        indices.Add(b);
        indices.Add(c);
    }

    void AddSegment(int a, int b) {
        indices.Add(a);
        indices.Add(b);
    }

    Model* Create() {
        Model* m = new Model();
        m->topology = topology;
        if (!m->Init(*this)) {
            delete m;
            m = nullptr;
        }
        return m;
    }
};