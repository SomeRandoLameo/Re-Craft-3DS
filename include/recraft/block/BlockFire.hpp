#pragma once

#include "Block.hpp"

// TODO: Fire should spread to flammable neighboring blocks
// TODO: Fire should extinguish in rain
// TODO: Fire on netherrack/magma should burn forever
// TODO: Particle effects (flame and smoke)
class BlockFire : public Block {
public:
    // Age 0-15, stored in metadata
    static uint8_t ageFromMeta(Metadata meta) { return meta & 0xF; }

    BlockFire() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    // TODO: This will be interesting...
    TextureSet getTextures(Metadata meta) const override { return TextureSet("fire_layer_0"); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
