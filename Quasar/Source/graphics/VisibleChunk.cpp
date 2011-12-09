#include "stdafx.h"

#include "graphics/VisibleChunk.h"

VisibleChunk::VisibleChunk() :
model(),
chunk() {
}

VisibleChunk::~VisibleChunk() {
}

void VisibleChunk::UpdateModel(const ModelFactory& mf) {

    int buffExtra = mf.VertexByteStride() * 6 /*vertices per face*/
                                          * 64 /*num faces to buffer*/ ;

    if (!model) {
        model = mf.Create(buffExtra);
        return;
    }

    if (!model->Update(mf)) {
        if (model) model->Shutdown();
        model = mf.Create(buffExtra);
    }
}

void VisibleChunk::Shutdown() {
    if (model) {
        model->Shutdown();
        delete model;
        model = nullptr;
    }
}