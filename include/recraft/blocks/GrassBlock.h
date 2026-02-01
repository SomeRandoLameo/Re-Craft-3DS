#pragma once

#include "Block.hpp"

class GrassBlock : public Block {
public:
    GrassBlock(BlockID id, const char* name) : Block(id, name) {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        if (direction == Direction::Top) {
            out_rgb[0] = 140;
            out_rgb[1] = 214;
            out_rgb[2] = 123;
        } else {
            Block::getColor(metadata, direction, out_rgb);
        }
    }
};
