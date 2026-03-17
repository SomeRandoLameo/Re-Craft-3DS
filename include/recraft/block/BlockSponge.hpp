#pragma once

#include "Block.hpp"

class BlockSponge : public Block {
public:
    enum class Variant : uint8_t {
        Dry = 0,
        Wet = 1,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 1) meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockSponge() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        switch (variantFromMeta(meta)) {
        case Variant::Wet:
            return TextureSet("sponge_wet");
        case Variant::Dry:
        default:
            return TextureSet("sponge");
        }
    }

    Metadata getDropMeta(Metadata meta) const override {
        return static_cast<Metadata>(variantFromMeta(meta));
    }
};