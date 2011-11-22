#include "stdafx.h"

#include "graphics/VisibleChunk.h"

VisibleChunk::VisibleChunk() :
    model(),
    chunk(),
    visibleBlocks() {
}

VisibleChunk::~VisibleChunk() {
}

void VisibleChunk::Shutdown() {
    if (model) {
        model->Shutdown();
        delete model;
        model = nullptr;
    }
}