#pragma once

#include "Block.hpp"

class BlockOldLeaves : public Block {
public:
    enum class Variant : uint8_t {
        Oak = 0,
        Spruce = 1,
        Birch = 2,
        Jungle = 3,
    };

    static Variant variantFromMeta(Metadata meta) { return static_cast<Variant>(meta & 0x3); }

    static bool isPlayerPlaced(Metadata meta) { return (meta >> 2) & 0x1; }

    static bool shouldCheckDecay(Metadata meta) { return (meta >> 3) & 0x1; }

    BlockOldLeaves() : Block() { setOpaque(false); }

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        // TODO: Biome tinting, for now use a static foliage color
        out_rgb[0] = 140;
        out_rgb[1] = 214;
        out_rgb[2] = 123;
    }

    TextureSet getTextures(Metadata meta) const override {
        static const char* names[] = {
            "leaves_oak",
            "leaves_spruce",
            "leaves_birch",
            "leaves_jungle",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Leaves should drop saplings with a small chance
        // and oak leaves should also drop apples. Requires item system.
        return BlockID::Air;
    }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
