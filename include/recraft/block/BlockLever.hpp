#pragma once

#include "Block.hpp"

// TODO: Should emit redstone signal when powered
// TODO: Should drop when attached block is removed
class BlockLever : public Block {
public:
    enum class Orientation : uint8_t {
        Down_X = 0,
        East = 1,
        West = 2,
        South = 3,
        North = 4,
        Up_Z = 5,
        Up_X = 6,
        Down_Z = 7,
    };

    static Orientation orientationFromMeta(Metadata meta) {
        uint8_t o = meta & 0x7;
        if (o > 7)
            o = 0;
        return static_cast<Orientation>(o);
    }

    static bool isPowered(Metadata meta) { return (meta >> 3) & 0x1; }

    BlockLever() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    TextureSet getTextures(Metadata meta) const override { return TextureSet("lever"); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Lever; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
