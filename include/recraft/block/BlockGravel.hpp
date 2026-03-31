#pragma once

#include "Block.hpp"

// TODO: Gravel should be affected by gravity (falling block physics)
// This requires a physics/falling block system to be implemented first.
class BlockGravel : public Block {
public:
    BlockGravel() : Block() {}

    TextureSet getTextures(Metadata meta) const override { return TextureSet("gravel"); }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Should drop flint with a 10% chance instead of gravel
        // when broken without silk touch. Requires tool/enchantment system.
        return BlockID::Gravel;
    }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
