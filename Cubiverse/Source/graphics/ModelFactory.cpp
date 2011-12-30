#include "stdafx.h"

#include "graphics/ModelFactory.h"

#include "graphics/Model.h"

Model* ModelFactory::Create(int buffExtra) {

	for (int i = 0; i < AttributeCount(); i++) {
		if (attributes[i].hidden) continue;
		attributes[i].shaderLoc = glGetAttribLocation(shader->program, attributes[i].name.c_str());
	}

	Model* m = new Model();
	m->topology = topology;
	if (!m->Init(*this, buffExtra)) {
		delete m;
		return nullptr;
	}
	return m;
}