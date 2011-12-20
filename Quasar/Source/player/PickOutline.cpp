#include "stdafx.h"

#include "player/PickOutline.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"

void PickOutline::Render() {
    glLineWidth(3);
    glm::mat4 m = glm::translate(glm::mat4(), pos.ToGlmVec3());
    if (side.x == -1) {
        x0->world = m;
        x0->Render();
    }
    else if (side.x == 1) {
        x1->world = m;
        x1->Render();
    }
    else if (side.y == -1) {
        y0->world = m;
        y0->Render();
    }
    else if (side.y == 1) {
        y1->world = m;
        y1->Render();
    }
    else if (side.z == -1) {
        z0->world = m;
        z0->Render();
    }
    else if (side.z == 1) {
        z1->world = m;
        z1->Render();
    }
}

void PickOutline::InitGraphics() {
    ModelFactory mf;
    mf.AddAttribute<float>("position", 3);
    mf.AddAttribute<float>("color", 4);

    mf.shader = Res::GetShader("color");
    mf.topology = GL_LINE_LOOP;
    ColorF c(0, 0, 0, 0.5);

    double d = 0.0025;

    mf.Next().Set("position", 0 - d, 1 - d, 0 + d).Set("color", c);
    mf.Next().Set("position", 0 - d, 1 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 - d, 0 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 - d, 0 + d, 0 + d).Set("color", c);
    x0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 + d, 0 + d, 0 + d).Set("color", c);
    mf.Next().Set("position", 1 + d, 0 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 + d, 1 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 + d, 1 - d, 0 + d).Set("color", c);
    x1 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 0 + d, 0 - d, 0 + d).Set("color", c);
    mf.Next().Set("position", 0 + d, 0 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 - d, 1 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 - d, 0 + d).Set("color", c);
    y0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 - d, 1 + d, 0 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 + d, 1 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 + d, 0 + d).Set("color", c);
    y1 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 1 - d, 0 + d, 0 - d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 - d, 0 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 - d, 0 - d).Set("color", c);
    mf.Next().Set("position", 0 + d, 0 + d, 0 - d).Set("color", c);
    z0 = mf.Create();

    mf.Clear();
    mf.Next().Set("position", 0 + d, 0 + d, 1 + d).Set("color", c);
    mf.Next().Set("position", 0 + d, 1 - d, 1 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 1 - d, 1 + d).Set("color", c);
    mf.Next().Set("position", 1 - d, 0 + d, 1 + d).Set("color", c);
    z1 = mf.Create();
}

void PickOutline::ShutdownGraphics() {
    x0->Shutdown();
    x1->Shutdown();
    y0->Shutdown();
    y1->Shutdown();
    z0->Shutdown();
    z1->Shutdown();
    x0 = x1 = y0 = y1 = z0 = z1 = nullptr;
}