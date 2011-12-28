#pragma once

#include "graphics/IRender.h"

class Scene : IRender {
public:
	Scene();

	virtual void Render();

};