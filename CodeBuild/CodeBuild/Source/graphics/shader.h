#pragma once

#include "model.h"
#include "direct3d.h"

class Model;

struct MatrixBuffer {
	XMMATRIX worldViewProj;
	XMFLOAT3 camPos;
	float padding;
};

class Shader {
public:
	Shader();
	~Shader();
	bool init(LPCWSTR shaderFile, LPCSTR vertexShaderName, LPCSTR pixelShaderName);
	void shutdown();
	void updateConstants(XMMATRIX* mat);
	void render(Model* model);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* matrixBuffer;
};