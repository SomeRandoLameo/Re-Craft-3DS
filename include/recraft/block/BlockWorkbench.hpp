#pragma once

#include "Block.hpp"

// TODO: Should open crafting GUI when right clicked, requires UI/container system
class BlockWorkbench : public Block {
public:
    BlockWorkbench() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        ts.faces[Direction::Top] = "crafting_table_top";
        ts.faces[Direction::Bottom] = "planks_oak";
        ts.faces[Direction::North] = "crafting_table_front";
        ts.faces[Direction::South] = "crafting_table_front";
        ts.faces[Direction::West] = "crafting_table_side";
        ts.faces[Direction::East] = "crafting_table_side";
        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Crafting_Table; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
