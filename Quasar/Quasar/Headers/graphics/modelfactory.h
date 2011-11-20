#pragma once

#include "math/math.h"
#include "graphics/model.h"
#include "graphics/shader.h"
#include "containers/arraylist.h"

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
            m = nullptr;
        }
        return m;
    }
};