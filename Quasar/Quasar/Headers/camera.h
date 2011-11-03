#pragma once
#include "math2.h"

class FreeCamera {
public:
	FreeCamera();
	void doInput();
	XMMATRIX getMat();
	Vector3 pos;
	Vector3 dir;
	float yaw;
	float pitch;
protected:
	~FreeCamera();
};