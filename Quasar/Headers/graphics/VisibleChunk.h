#pragma once

#include "level/World.h"
#include "level/Block.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"

class VisibleChunk {
public:
    struct VisibleBlock {
        int location;
        short size;

        VisibleBlock() :
        location(), size() {
        }
        VisibleBlock(int location, short size) :
        location(location), size(size) {
        }
    };

    VisibleChunk();

    void UpdateBlock(ushort index, ModelFactory& mf);

    void AppendBlock(ushort index, ModelFactory& mf);

    void UpdateModel(const ModelFactory& mf);

    void Shutdown();

    Model* model;
    Chunk* chunk;

    std::map<ushort, VisibleBlock> visibleBlocks;
};