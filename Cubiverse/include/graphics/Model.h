#pragma once

#include "graphics/Texture.h"
#include "graphics/ModelFactory.h"

#include "math/Math.h"
#include "graphics/IRender.h"

class Shader;
class ModelFactory;

class Model : public IRender {
public:
	Model();

	bool Init(const ModelFactory& mf, int buffExtra = 0);
	void Shutdown();

	virtual void Render();

	bool Update(const ModelFactory& mf);

	void EnableOrtho(bool ortho = true);

	void Bind();
	void Unbind();

	byte* Map(GLenum access);
	void Unmap();

	glm::mat4 world;

	int vertexCount;
	int indexCount;

	GLuint vertexBuffer;
	GLuint indexBuffer;

	int vertexBufferSize;

private:
	void ManualBind();
	void ManualUnbind();

	struct VertexData {
		std::vector<ModelFactory::Attribute> attributes;
		byte* vertexData;
		int vertexStride;
	};

	VertexData vertexDataState;

	bool orthographic;

	Shader* shader;
	Texture* texture;

	GLenum topology;
	GLuint vertexArrayObject;
};