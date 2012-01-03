#include "stdafx.h"

#include "graphics/ModelFactory.h"
#include "graphics/Model.h"
#include "graphics/Shader.h"

Model* ModelFactory::Create(int buffExtra) {

	for (int i = 0; i < AttributeCount(); i++) {
		if (attributes[i].hidden) continue;
		attributes[i].shaderLoc = glGetAttribLocation(shader->program, attributes[i].name.c_str());
	}

	Model* m = new Model();
	if (!m->Init(*this, buffExtra)) {
		delete m;
		m = nullptr;
	}
	return m;
}