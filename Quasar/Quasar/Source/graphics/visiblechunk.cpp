#include "stdafx.h"

#include "graphics/visiblechunk.h"

VisibleChunk::VisibleChunk() :
    model(), 
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