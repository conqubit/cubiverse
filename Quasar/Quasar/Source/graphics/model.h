#pragma once

#include <d3d11.h>
#include <d3dx10.h>
#include "math.h"

#define MF_CLOCKWISE true
#define MF_COUNTER_CLOCKWISE false

class Shader;

struct ModelDesc {
	Vertex* vertices;
	int vertexCount;

	int* indices;
	int indexCount;

	Shader* shader;

	ModelDesc() {
		vertices = nullptr;
		indices = nullptr;
		shader = nullptr;
		vertexCount = 0;
		indexCount = 0;
	}

	void deleteBuffers() {
		delete[] indices;
		delete[] vertices;
		indices = nullptr;
		vertices = nullptr;
	}
};

class Model {
public:
	Model();
	~Model();
	bool init(ModelDesc* modelDesc);
	void render();
	void shutdown();

	Matrix mat;
	Shader* shader;
	int vertexCount;
	int indexCount;
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

class ModelFactory {
public:
	static ModelDesc CreateGlobe(double radius, int width, int height, bool clockwise);
	static Model* CreateBox(Shader* shader);
};