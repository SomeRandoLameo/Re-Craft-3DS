#pragma once

#include "Block.hpp"

// TODO: Requires tile entity system for mob spawning logic
// TODO: Should drop XP (15-45) when broken
// TODO: Should render spinning mob inside using tile entity renderer
class BlockMobSpawner : public Block {
public:
    BlockMobSpawner() : Block() { setOpaque(false); }

    TextureSet getTextures(Metadata meta) const override { return TextureSet("mob_spawner"); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
