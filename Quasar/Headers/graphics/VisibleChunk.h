#pragma once

#include "level/World.h"
#include "level/Block.h"
#include "graphics/Model.h"
#include "graphics/ModelFactory.h"

class VisibleChunk : IRenderObject {
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

    std::map<ushort, VisibleBlock> visibleBlocks;
    Model* model;

    VisibleChunk();

    void Shutdown();

    virtual void Render();
    virtual void InitGraphics();
    virtual void ShutdownGraphics();

    void UpdateBlock(ushort index, ModelFactory& mf);
    void AppendBlock(ushort index, ModelFactory& mf);
    void UpdateModel(const ModelFactory& mf);
};