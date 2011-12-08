#pragma once

#include "graphics/Model.h"
#include "graphics/Shader.h"

class ModelFactory {
public:
    struct Attribute {
        string name;
        int size;
        int offset;
        int byteOffset;

        Attribute(const string& name, int size, int offset) :
        name(name), size(size), offset(offset), byteOffset(offset * sizeof(float)) {
        }
    };

private:
    std::vector<float> vertexData;
    std::vector<int> indices;

    int stride;
    int pos;

    std::vector<Attribute> attributes;

public:
    Shader* shader;
    Texture* texture;

    GLenum topology;

    ModelFactory() :
    stride(),
    pos(),

    shader(),
    texture(),
    topology(GL_TRIANGLES) {
    }

    ~ModelFactory() {
    }

    void AddAttribute(const string& name, int size) {
        attributes.push_back(Attribute(name, size, stride));
        stride += size;
        pos = -stride;
    }

    const Attribute& GetAttribute(const string& name)const {
        for (int i = 0; i < attributes.size(); i++) {
            if (name == attributes[i].name) {
                return attributes[i];
            }
        }
    }

    const Attribute& GetAttribute(int index)const {
        return attributes[index];
    }

    int AttributeCount()const {
        return (int)attributes.size();
    }

    ModelFactory& Next() {
        pos += stride;
        vertexData.resize(pos + stride);
        return *this;
    }

    ModelFactory& Set(const string& name, float x) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = x;
        return *this;
    }

    ModelFactory& Set(const string& name, float x, float y) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = x;
        vertexData[pos + attr.offset + 1] = y;
        return *this;
    }

    ModelFactory& Set(const string& name, float x, float y, float z) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = x;
        vertexData[pos + attr.offset + 1] = y;
        vertexData[pos + attr.offset + 2] = z;
        return *this;
    }

    ModelFactory& Set(const string& name, float x, float y, float z, float w) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = x;
        vertexData[pos + attr.offset + 1] = y;
        vertexData[pos + attr.offset + 2] = z;
        vertexData[pos + attr.offset + 3] = w;
        return *this;
    }

    template <typename U>
    ModelFactory& Set(const string& name, const Vector3<U>& v) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = (float)v.x;
        vertexData[pos + attr.offset + 1] = (float)v.y;
        vertexData[pos + attr.offset + 2] = (float)v.z;
        return *this;
    }

    ModelFactory& Set(const string& name, const ColorF& c) {
        const Attribute& attr = GetAttribute(name);
        vertexData[pos + attr.offset + 0] = c.r;
        vertexData[pos + attr.offset + 1] = c.g;
        vertexData[pos + attr.offset + 2] = c.b;
        vertexData[pos + attr.offset + 3] = c.a;
        return *this;
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
        return (int)vertexData.size() / stride;
    }

    int VertexByteStride()const {
        return stride * sizeof(float);
    }

    int VertexDataByteSize()const {
        return vertexData.size() * sizeof(float);
    }

    int VertexDataFloatSize()const {
        return vertexData.size();
    }

    int IndexCount()const {
        return (int)indices.size();
    }

    float* VertexData()const {
        return (float*)vertexData.data();
    }

    int* IndexData()const {
        return (int*)indices.data();
    }

    Model* Create(int buffExtra = 0)const {
        Model* m = new Model();
        m->topology = topology;
        if (!m->Init(*this, buffExtra)) {
            delete m;
            return nullptr;
        }
        return m;
    }

    void Clear() {
        vertexData.clear();
        indices.clear();
        pos = -stride;
    }

    static Model* CreateWireframeDebugBox(const BoundingBox& b, const ColorF& color, Shader* shader) {
        ModelFactory mf;
        mf.shader = shader;
        mf.topology = GL_LINES;
        mf.AddAttribute("position", 3);
        mf.AddAttribute("color", 4);

        mf.Next().Set("position", b.Min()).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Min().y, b.Min().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Min().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Max().y, b.Min().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Max().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Min().x, b.Max().y, b.Min().z).Set("color", color);

        mf.Next().Set("position", b.Min().x, b.Max().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Min()).Set("color", color);


        mf.Next().Set("position", b.Min().x, b.Min().y, b.Max().z).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Min().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Min().y, b.Max().z).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Max().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Max().y, b.Max().z).Set("color", color);
        mf.Next().Set("position", b.Min().x, b.Max().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Min().x, b.Max().y, b.Max().z).Set("color", color);
        mf.Next().Set("position", b.Min().x, b.Min().y, b.Max().z).Set("color", color);


        mf.Next().Set("position", b.Min()).Set("color", color);
        mf.Next().Set("position", b.Min().x, b.Min().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Min().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Min().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Max().x, b.Max().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Max().x, b.Max().y, b.Max().z).Set("color", color);

        mf.Next().Set("position", b.Min().x, b.Max().y, b.Min().z).Set("color", color);
        mf.Next().Set("position", b.Min().x, b.Max().y, b.Max().z).Set("color", color);
        
        return mf.Create();
    }
};