#include "stdafx.h"

#include "graphics/Texture.h"

Texture::Texture() :
texture() {
}

Texture::~Texture() {
}

bool Texture::Init(string file) {
    sf::Image image;

    if (!image.LoadFromFile(file)) {
        return false;
    }
    
    glGenTextures(1, &texture);

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.GetWidth(), image.GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (byte*)image.GetPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    Unbind();

    return true;
}

void Texture::Shutdown() {
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, texture);
}