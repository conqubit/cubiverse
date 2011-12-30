#pragma once

#include "graphics/IRender.h"

class Graphics {
public:
	static std::vector<IRenderObject*> things;
	static glm::mat4 proj;
	static glm::mat4 ortho;

	static bool Init();
	static void Shutdown();
	static void Render();
	static void SetProjection();

	static void InitGraphics();
	static void ShutdownGraphics();

	static const glm::mat4&  GetOrtho();
	static glm::mat4 GetViewProj();

	static void ClearAll();
	static void ClearColor();
	static void ClearDepth();
};