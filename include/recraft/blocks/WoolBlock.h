#pragma once

#include "Block.hpp"

class WoolBlock : public Block {
public:
    WoolBlock(BlockID id, const char* name) : Block(id, name) {
        setHasMetadata(true);
    }

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        const uint32_t dyes[] = {
            0xFFFFFF, 0xD87F33, 0xB24CD8, 0x6699D8,
            0xE5E533, 0x7FCC19, 0xF27FA5, 0x4C4C4C,
            0x999999, 0x4C7F99, 0x7F3FB2, 0x334CB2,
            0x664C33, 0x667F33, 0x993333, 0x191919
        };

        if (metadata < 16) {
            uint32_t color = dyes[metadata];
            out_rgb[0] = (color >> 16) & 0xFF;
            out_rgb[1] = (color >> 8) & 0xFF;
            out_rgb[2] = color & 0xFF;
        } else {
            Block::getColor(metadata, direction, out_rgb);
        }
    }
};
