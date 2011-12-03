#pragma once

class Texture {
public:
    int width;
    int height;

    GLenum type;
    GLuint texture;
    GLuint sampler;

    Texture();
    ~Texture();

    bool Init(const string& file);
    void Shutdown();

    void Bind();
    void Unbind();
    bool Init2DArray(int count, int width, int height, byte* ptr);
};