#pragma once

#include "graphics/Graphics.h"
#include "graphics/Texture.h"
#include "graphics/ModelFactory.h"

#include "math/Math.h"
#include "graphics/IRender.h"
#include "graphics/Shader.h"

class ModelFactory;

class Model : public IRender {
public:
	struct VertexData {
		std::vector<ModelFactory::Attribute> attributes;
		byte* vertexData;
		int vertexStride;
	};

	Model();

	bool Init(const ModelFactory& mf, int buffExtra = 0);
	void Shutdown();

	virtual void Render();

	bool Update(const ModelFactory& mf);

	void EnableOrtho(bool ortho = true);

	void Bind(bool manualOverride = false);
	void Unbind();

	byte* Map(GLenum access);
	void Unmap();

	VertexData vertexDataState;

	bool orthographic;

	glm::mat4 world;

	Shader* shader;
	Texture* texture;

	int vertexCount;
	int indexCount;

	int vertexBufferSize;

	GLenum topology;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
};