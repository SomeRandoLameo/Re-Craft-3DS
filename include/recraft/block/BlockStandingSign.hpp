#pragma once

#include "BlockSign.hpp"

class BlockStandingSign : public BlockSign {
public:
    static uint8_t rotationFromMeta(Metadata meta) {
        return meta & 0xF;
    }

    BlockStandingSign() : BlockSign() {}

    TextureSet getTextures(Metadata meta) const override {
        // TODO: Signs should render as a flat entity with text, not a full block
        return TextureSet("planks_oak");
    }
};