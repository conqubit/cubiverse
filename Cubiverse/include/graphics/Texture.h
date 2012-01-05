#pragma once

class Texture {
public:
	int width;
	int height;
	int depth;

	GLenum type;
	GLuint texture;

	Texture();
	~Texture();

	void Shutdown();

	void Bind();
	void Unbind();

	static Texture* Create2DTexture(const string& file);
	static Texture* Create3DTexture(int width, int height, int depth, byte* data);
};