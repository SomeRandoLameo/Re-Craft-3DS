#pragma once

#include "Block.hpp"

class LeavesBlock : public Block {
public:
    LeavesBlock() : Block() {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        out_rgb[0] = 140;
        out_rgb[1] = 214;
        out_rgb[2] = 123;
    }
};