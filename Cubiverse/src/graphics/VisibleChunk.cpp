#include "stdafx.h"

#include "graphics/VisibleChunk.h"

VisibleChunk::VisibleChunk() : model(nullptr) {
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
	if (visibleBlocks.count(index) == 0 && mf.VertexDataSize() > 0) {
		AppendBlock(index, mf);
		return;
	}

	const VisibleBlock& b = visibleBlocks[index];

	if (mf.VertexDataSize() < b.size || mf.VertexDataSize() > b.size) {
		byte* zeros = (byte*)malloc(b.size);
		ZeroMemory(zeros, b.size);

		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, b.location, b.size, zeros);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		free(zeros);
	}

	if (mf.VertexDataSize() == 0) {
		visibleBlocks.erase(index);
	} else if (mf.VertexDataSize() <= b.size) {
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, b.location, mf.VertexDataSize(), mf.VertexData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		AppendBlock(index, mf);
	}
}

void VisibleChunk::AppendBlock(ushort index, ModelFactory& mf) {
	VisibleBlock b;
	b.size = mf.VertexDataSize();

	int oldSize = model->vertexCount * mf.VertexStride();

	if (oldSize + b.size <= model->vertexBufferSize) {
		b.location = oldSize;
		model->vertexCount += b.size / mf.VertexStride();
		glBindBuffer(GL_ARRAY_BUFFER, model->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, b.location, b.size, mf.VertexData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	} else {
		// big enough assuming no consolidation, may not actually write to the end
		byte* buffer = (byte*)malloc(oldSize + b.size);
		int buffOffset = 0;

		byte* vertexData = model->Map(GL_READ_ONLY);
		for (auto i = visibleBlocks.begin(); i != visibleBlocks.end(); ++i) {
			VisibleBlock& b = i->second;
			memcpy(buffer + buffOffset, vertexData + b.location, b.size);
			b.location = buffOffset;
			buffOffset += b.size;
		}
		model->Unmap();

		memcpy(buffer + buffOffset, mf.VertexData(), b.size);

		b.location = buffOffset;
		int newSize = buffOffset + b.size;

		mf.dataOverride = buffer;
		mf.sizeOverride = newSize;
		UpdateModel(mf);

		free(buffer);
	}
	visibleBlocks[index] = b;
}

void VisibleChunk::UpdateModel(ModelFactory& mf) {
	int buffExtra = mf.VertexStride() * 6   // vertices per face
									  * 64; // num faces to buffer
	if (!model) {
		model = mf.Create(buffExtra);
	}
	else if (!model->Update(mf)) {
		model->Shutdown();
		model = mf.Create(buffExtra);
	}
}