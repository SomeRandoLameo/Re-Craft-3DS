#pragma once

#include "Block.hpp"

class BlockObsidian : public Block {
public:
    BlockObsidian() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet("obsidian");
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Obsidian;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};