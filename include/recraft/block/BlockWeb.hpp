#pragma once

#include "Block.hpp"

// TODO: Web should slow down entities moving through it
class BlockWeb : public Block {
public:
    BlockWeb() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet("web");
    }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Should drop string, requires item system
        return BlockID::Air;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};