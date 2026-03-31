#pragma once

#include "Block.hpp"

// TODO: Saplings should be rendered as an X pattern (two crossed quads) but
//  PolyGen does not support this yet. Currently rendered as a full block instead.
//  Also, After some duration, Saplings get replaced by grass blocks ingame...
class BlockSapling : public Block {
public:
    enum class Variant : uint8_t {
        Oak = 0,
        Spruce = 1,
        Birch = 2,
        Jungle = 3,
        Acacia = 4,
        DarkOak = 5,
    };

    // growth stage is stored in bits 3-4, variant in bits 0-2
    static Variant variantFromMeta(Metadata meta) {
        uint8_t variant = meta & 0x7;
        if (variant > 5)
            variant = 0;
        return static_cast<Variant>(variant);
    }

    static uint8_t growthFromMeta(Metadata meta) { return (meta >> 3) & 0x1; }

    BlockSapling() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        static const char* names[] = {
            "sapling_oak", "sapling_spruce", "sapling_birch", "sapling_jungle", "sapling_acacia", "sapling_roofed_oak",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Sapling; }

    Metadata getDropMeta(Metadata meta) const override {
        // strip growth stage, only keep variant
        return static_cast<Metadata>(variantFromMeta(meta));
    }
};
