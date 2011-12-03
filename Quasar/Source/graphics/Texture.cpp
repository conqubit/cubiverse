#include "stdafx.h"

#include "graphics/Texture.h"

Texture::Texture() :
texture() {
}

Texture::~Texture() {
}

bool Texture::Init(const string& file) {
    type = GL_TEXTURE_2D;
    sf::Image image;

    if (!image.LoadFromFile(file)) {
        return false;
    }

    width = image.GetWidth();
    height = image.GetHeight();
    
    glGenTextures(1, &texture);

    Bind();

    glTexImage2D(type, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr());

    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Unbind();

    return true;
}

bool Texture::Init2DArray(int count, int width, int height, byte* ptr) {
    this->width = width;
    this->height = height;
    type = GL_TEXTURE_2D_ARRAY;
    glGenTextures(1, &texture);
    Bind();
    glTexImage3D(type, 0, GL_RGBA8, width, height, count, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

    glGenSamplers(1, &sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Unbind();
    return true;
}

void Texture::Shutdown() {
}

void Texture::Bind() {
    glBindTexture(type, texture);
}

void Texture::Unbind() {
    glBindTexture(type, texture);
}