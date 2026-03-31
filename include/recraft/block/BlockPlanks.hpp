#pragma once

#include "Block.hpp"

class BlockPlanks : public Block {
public:
    enum class Variant : uint8_t {
        Oak = 0,
        Spruce = 1,
        Birch = 2,
        Jungle = 3,
        Acacia = 4,
        DarkOak = 5,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 5)
            meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockPlanks() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        static const char* names[] = {
            "planks_oak", "planks_spruce", "planks_birch", "planks_jungle", "planks_acacia", "planks_big_oak",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Planks; }

    Metadata getDropMeta(Metadata meta) const override { return static_cast<Metadata>(variantFromMeta(meta)); }
};
