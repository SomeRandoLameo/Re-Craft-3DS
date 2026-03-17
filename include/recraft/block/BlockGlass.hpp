#pragma once

#include "Block.hpp"

class BlockGlass : public Block {
public:
    BlockGlass() : Block() {
        setOpaque(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet("glass");
    }

    BlockID getDropID(Metadata meta) const override {
        // Glass drops nothing when broken without silk touch
        return BlockID::Air;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};