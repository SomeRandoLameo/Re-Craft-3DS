#pragma once

#include "Block.hpp"

// TODO: Requires block entity/animation system for proper piston movement rendering
// TODO: Should be placed/removed programmatically by BlockPistonBase only
class BlockPistonMoving : public Block {
public:
    BlockPistonMoving() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        // No texture, rendered via block entity animation
        return TextureSet("piston_side");
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
