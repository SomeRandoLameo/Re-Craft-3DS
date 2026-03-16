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

    TextureSet getTextures(Metadata meta) const override {
        // Texture filenames must match what's loaded by TextureMap::Init()
        // under romfs:/assets/minecraft/textures/blocks/
        static const char* names[] = {
            "stone.png",           // Stone
            "stone_granite.png",   // Granite
            "stone_granite_smooth.png",
            "stone_diorite.png",   // Diorite
            "stone_diorite_smooth.png",
            "stone_andesite.png",  // Andesite
            "stone_andesite_smooth.png",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }
};