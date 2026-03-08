#pragma once

#include "Block.hpp"
//TODO: Works, but texture mapping errors on variants
class BlockStone : public Block {
public:
    enum class Variant : uint8_t {
        Stone         = 0,
        Granite       = 1,
        GraniteSmooth = 2,
        Diorite       = 3,
        DioriteSmooth = 4,
        Andesite      = 5,
        AndesiteSmooth= 6,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 6) meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockID getDropID(Metadata meta) const override {
        return variantFromMeta(meta) == Variant::Stone
            ? BlockID::Cobblestone
            : BlockID::Stone;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return variantFromMeta(meta) == Variant::Stone ? 0 : meta;
    }
};