#pragma once

#include "Block.hpp"

// TODO: Note block sounds require a tile entity system and sound engine integration
class BlockNote : public Block {
public:
    BlockNote() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet("noteblock");
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Noteblock;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};