#include "stdafx.h"

#include "graphics/Texture.h"

Texture::Texture() :
width(), height(), depth(),
type(), texture() {
}

Texture::~Texture() {
}

void Texture::Shutdown() {
	glDeleteTextures(1, &texture);
	delete this;
}

void Texture::Bind() {
	glBindTexture(type, texture);
}

void Texture::Unbind() {
	glBindTexture(type, texture);
}

Texture* Texture::Create2DTexture(const string& file) {
	/*Texture* tex = new Texture();
	tex->type = GL_TEXTURE_2D;

	sf::Image image;

	if (!image.LoadFromFile(file)) {
		return false;
	}

	tex->width = image.GetWidth();
	tex->height = image.GetHeight();
	
	glGenTextures(1, &tex->texture);

	tex->Bind();

	glTexImage2D(tex->type, 0, GL_RGBA8, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr());

	glTexParameteri(tex->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(tex->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(tex->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex->type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	tex->Unbind();

	return tex;*/
}

Texture* Texture::Create3DTexture(int width, int height, int depth, byte* data) {
	Texture* tex = new Texture();

	tex->width = width;
	tex->height = height;
	tex->depth = depth;

	tex->type = GL_TEXTURE_3D;

	glGenTextures(1, &tex->texture);

	tex->Bind();

	glTexImage3D(tex->type, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(tex->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(tex->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(tex->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex->type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	tex->Unbind();

	return tex;
}