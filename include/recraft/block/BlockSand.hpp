#pragma once

#include "Block.hpp"

// TODO: Sand and Red Sand should be affected by gravity (falling block physics)
// This requires a physics/falling block system to be implemented first.
class BlockSand : public Block {
public:
    enum class Variant : uint8_t {
        Sand = 0,
        RedSand = 1,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 1)
            meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockSand() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        static const char* names[] = {
            "sand",
            "red_sand",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Sand; }

    Metadata getDropMeta(Metadata meta) const override { return static_cast<Metadata>(variantFromMeta(meta)); }
};
