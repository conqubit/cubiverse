#pragma once

class Texture {
public:
    int width;
    int height;
    int depth;

    GLenum type;
    GLuint texture;
    GLuint sampler;

    Texture();
    ~Texture();

    void Shutdown();

    void Bind();
    void Unbind();

    static Texture* Create2DTexture(const string& file);
    static Texture* Create3DTexture(int count, int width, int height, byte* data);
};