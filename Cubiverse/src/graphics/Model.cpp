#include "stdafx.h"

#include "System.h"

#include "graphics/Graphics.h"
#include "graphics/Model.h"
#include "graphics/Shader.h"

#define BUFFER_OFFSET(i) ((void*)(i))

Model::Model() :
orthographic(),
texture(),
shader(),
vertexCount(),
indexCount(),
vertexBuffer(),
indexBuffer(),
vertexArrayObject(),
vertexBufferSize(),
topology(GL_TRIANGLES) {
}

static inline bool CanUseVAO() {
	return GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object;
}

bool Model::Init(const ModelFactory& mf, int buffExtra) {
	if (!mf.shader) {
		return false;
	}
	shader = mf.shader;
	texture = mf.texture;
	topology = mf.topology;
	vertexCount = mf.VertexCount();
	indexCount = mf.IndexCount();
	vertexBufferSize = mf.VertexDataSize() + buffExtra;

	vertexDataState.attributes = mf.attributes;
	vertexDataState.vertexStride = mf.VertexStride();
	vertexDataState.vertexData = nullptr;

	glGenBuffers(1, &vertexBuffer);

	if (!vertexBuffer) {
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, nullptr, mf.usage);

	glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataSize(), mf.VertexData());

	if (CanUseVAO()) {
		glGenVertexArrays(1, &vertexArrayObject);

		if (!vertexArrayObject) {
			return false;
		}

		glBindVertexArray(vertexArrayObject);
		ManualBind();
		glBindVertexArray(0);
	}

	if (texture) {
		glUniform1i(glGetUniformLocation(shader->program, "textureSampler"), 0);
		texture->Bind();
		glBindSampler(0, texture->sampler);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glGenBuffers(1, &indexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexBuffer, mf.IndexData(), GL_STATIC_DRAW);

	return true;
}

void Model::Shutdown() {
	//glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayObject);
	delete this;
}

void Model::EnableOrtho(bool ortho) {
	orthographic = ortho;
}

bool Model::Update(const ModelFactory& mf) {
	if (mf.VertexDataSize() <= vertexBufferSize) {
		shader = mf.shader;
		texture = mf.texture;
		vertexCount = mf.VertexCount();
		indexCount = mf.IndexCount();
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mf.VertexDataSize(), mf.VertexData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}
	return false;
}

void Model::Bind() {
	if (CanUseVAO()) {
		glBindVertexArray(vertexArrayObject);
	} else {
		ManualBind();
	}
}

void Model::Unbind() {
	if (CanUseVAO()) {
		glBindVertexArray(0);
	} else {
		ManualUnbind();
	}
}

void Model::ManualBind() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	for (int i = 0; i < vertexDataState.attributes.size(); i++) {
		const ModelFactory::Attribute& attr = vertexDataState.attributes[i];
		if (attr.hidden) continue;
		glEnableVertexAttribArray(attr.shaderLoc);
		glVertexAttribPointer(attr.shaderLoc, attr.count, attr.glType, attr.normalized, vertexDataState.vertexStride, BUFFER_OFFSET(attr.offset));
	}
}

void Model::ManualUnbind() {
	for (int i = 0; i < vertexDataState.attributes.size(); i++) {
		const ModelFactory::Attribute& attr = vertexDataState.attributes[i];
		if (attr.hidden) continue;
		glDisableVertexAttribArray(attr.shaderLoc);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

byte* Model::Map(GLenum access) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	byte* ret = (byte*)glMapBuffer(GL_ARRAY_BUFFER, access);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return ret;
}

void Model::Unmap() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::mat4 m;

void Model::Render() {
	shader->Bind();
	Bind();

	if (orthographic) {
		m = Graphics::GetOrtho() * world;
	} else {
		m = Graphics::GetViewProj() * world;
	}

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "worldViewProjectionMat"), 1, GL_FALSE, glm::value_ptr(m));

	glDrawArrays(topology, 0, vertexCount);

	Unbind();
	shader->Unbind();
}