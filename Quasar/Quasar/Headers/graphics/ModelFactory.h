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

    ModelFactory() :
        vertices(),
        indices() {
    }

    ~ModelFactory() {
    }

    void AddTriangle(int a, int b, int c) {
        indices.Add(a);
        indices.Add(b);
        indices.Add(c);
    }

    Model* Create() {
        Model* m = new Model();
        if (!m->Init(*this)) {
            delete m;
            m = nullptr;
        }
        return m;
    }
};