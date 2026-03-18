#pragma once

#include "Block.hpp"

class BlockBookshelf : public Block {
public:
    BlockBookshelf() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        ts.faces[Direction::Top]    = "planks_oak";
        ts.faces[Direction::Bottom] = "planks_oak";
        ts.faces[Direction::North]  = "bookshelf";
        ts.faces[Direction::South]  = "bookshelf";
        ts.faces[Direction::West]   = "bookshelf";
        ts.faces[Direction::East]   = "bookshelf";
        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Should drop 3 books, requires item system
        return BlockID::Bookshelf;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};