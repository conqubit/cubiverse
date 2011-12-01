#pragma once

#include "graphics/IRender.h"

class Graphics : public IRender {
public:
    Graphics();
    ~Graphics();
    bool Init();
    void Shutdown();
    void Render();
    void SetProjection();
    std::vector<IRender*> things;

    glm::mat4 proj;
};