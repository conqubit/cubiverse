#include "stdafx.h"

#include "graphics/VisibleChunk.h"

VisibleChunk::VisibleChunk() :
model(nullptr) {
}

inline void VisibleChunk::Render() {
	model->Render();
}

void VisibleChunk::Shutdown() {
	ShutdownGraphics();
	delete this;
}

void VisibleChunk::InitGraphics() {
}

void VisibleChunk::ShutdownGraphics() {
	if (model) {
		model->Shutdown();
		model = nullptr;
	}
}

void VisibleChunk::UpdateBlock(ushort index, ModelFactory& mf) {
	if (visibleBlocks.count(index) == 0 && mf.VertexCount() > 0) {
		AppendBlock(index, mf);
		return;
	}

	const VisibleBlock& b = visibleBlocks[index];

	if (mf.VertexDataSize() == b.size) {
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, b.location, mf.VertexDataSize(), mf.VertexData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}

	/*byte* bufferLocation = model->Map(GL_READ_WRITE) + b.location;
	memmove(bufferLocation, bufferLocation + b.size, model->vertexCount * mf.VertexStride() - (b.location + b.size));
	model->Unmap();
	for (auto i = visibleBlocks.begin(); i != visibleBlocks.end(); ++i) {
		if (i->second.location > b.location) {
			i->second.location -= b.size;
		}
	}

	model->vertexCount -= b.size / mf.VertexStride();*/

	byte* zeros = (byte*)malloc(b.size);
	ZeroMemory(zeros, b.size);

	glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, b.location, b.size, zeros);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(zeros);

	if (mf.VertexCount() > 0) {
		AppendBlock(index, mf);
	} else {
		visibleBlocks.erase(index);
	}
}

void VisibleChunk::AppendBlock(ushort index, ModelFactory& mf) {
	int vertexDataSize = model->vertexCount * mf.VertexStride();
	VisibleBlock& b = visibleBlocks[index];
	b.location = vertexDataSize;
	b.size = mf.VertexDataSize();
	model->vertexCount += b.size / mf.VertexStride();
	if (b.location + mf.VertexDataSize() <= model->vertexBufferSize) {
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, b.location, b.size, mf.VertexData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		byte* buffer = (byte*)malloc(vertexDataSize + mf.VertexDataSize());
		memcpy(buffer, model->Map(GL_READ_ONLY), vertexDataSize);
		model->Unmap();
		memcpy(buffer + vertexDataSize, mf.VertexData(), mf.VertexDataSize());
		mf.dataOverride = buffer;
		mf.sizeOverride = vertexDataSize + mf.VertexDataSize();
		UpdateModel(mf);
		free(buffer);
	}
}

void VisibleChunk::UpdateModel(ModelFactory& mf) {
	int buffExtra = mf.VertexStride() * 6   // vertices per face
									  * 64; // num faces to buffer

	if (!model) {
		model = mf.Create(buffExtra);
		return;
	}

	if (!model->Update(mf)) {
		if (model) {
			model->Shutdown();
		}
		model = mf.Create(buffExtra);
	}
}