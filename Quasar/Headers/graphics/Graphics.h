#pragma once

#include "graphics/IRender.h"

class Graphics {
public:
    static std::vector<IRender*> things;
    static glm::mat4 proj;
    static glm::mat4 ortho;

    static void Init();
    static void Shutdown();
    static void Render();
    static void SetProjection();

    static void ClearAll();
    static void ClearColor();
    static void ClearDepth();
};