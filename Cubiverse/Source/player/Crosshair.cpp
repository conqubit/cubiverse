#include "stdafx.h"

#include "player/Crosshair.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"

void Crosshair::Render() {
	glLineWidth(2);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	model->Render();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Crosshair::InitGraphics() {
	ModelFactory mf;
	mf.AddAttribute<float>("position", 3);
	mf.AddAttribute<float>("color", 4);

	mf.shader = Res::GetShader("color");
	mf.topology = GL_LINES;

	mf.Next().Set("position", 0, 0, 0).Set("color", 1, 1, 1, 1);
	mf.Next().Set("position", -0.02, -0.02, 0).Set("color", 1, 1, 1, 1);
	mf.Next().Set("position", 0, 0, 0).Set("color", 1, 1, 1, 1);
	mf.Next().Set("position", 0.02, -0.02, 0).Set("color", 1, 1, 1, 1);
	
	model = mf.Create();
	model->EnableOrtho();
}

void Crosshair::ShutdownGraphics() {
	if (model) {
		model->Shutdown();
		model = nullptr;
	}
}