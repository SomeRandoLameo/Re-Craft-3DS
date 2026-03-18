#pragma once

#include "Block.hpp"

// TODO: TNT should explode when ignited by fire, redstone, or flint and steel
// TODO: TNT should have a primed entity that flies through the air before exploding
class BlockTNT : public Block {
public:
    BlockTNT() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        ts.faces[Direction::Top]    = "tnt_top";
        ts.faces[Direction::Bottom] = "tnt_bottom";
        ts.faces[Direction::North]  = "tnt_side";
        ts.faces[Direction::South]  = "tnt_side";
        ts.faces[Direction::West]   = "tnt_side";
        ts.faces[Direction::East]   = "tnt_side";
        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Tnt;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};