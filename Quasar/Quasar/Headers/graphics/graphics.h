#pragma once

#include "direct3d.h"
#include "camera.h"

class Graphics {
public:
	Graphics();
	~Graphics();
	bool init();
	void render();
	void shutdown();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	FreeCamera* cam;

	Matrix viewMat;
	Matrix projMat;
	Direct3D* direct3D;
};