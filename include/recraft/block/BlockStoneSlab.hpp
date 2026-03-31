#pragma once

#include "BlockSlab.hpp"

class BlockStoneSlab : public BlockSlab {
public:
    enum class Variant : uint8_t {
        Stone = 0,
        Sand = 1,
        Wood = 2,
        Cobblestone = 3,
        Brick = 4,
        SmoothBrick = 5,
        NetherBrick = 6,
        Quartz = 7,
    };

    static Variant variantFromMeta(Metadata meta) {
        uint8_t variant = meta & 0x7;
        if (variant > 7)
            variant = 0;
        return static_cast<Variant>(variant);
    }

    BlockStoneSlab() : BlockSlab() {}

    bool isDouble() const override { return false; }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        const char* side = nullptr;
        const char* top = nullptr;

        switch (variantFromMeta(meta)) {
        case Variant::Sand:
            side = "sandstone_normal";
            top = "sandstone_top";
            break;
        case Variant::Wood:
            side = "planks_oak";
            top = "planks_oak";
            break;
        case Variant::Cobblestone:
            side = "cobblestone";
            top = "cobblestone";
            break;
        case Variant::Brick:
            side = "brick";
            top = "brick";
            break;
        case Variant::SmoothBrick:
            side = "stonebrick";
            top = "stonebrick";
            break;
        case Variant::NetherBrick:
            side = "nether_brick";
            top = "nether_brick";
            break;
        case Variant::Quartz:
            side = "quartz_block_side";
            top = "quartz_block_top";
            break;
        case Variant::Stone:
        default:
            side = "stone_slab_side";
            top = "stone_slab_top";
            break;
        }

        ts.faces[Direction::Top] = top;
        ts.faces[Direction::Bottom] = top;
        ts.faces[Direction::North] = side;
        ts.faces[Direction::South] = side;
        ts.faces[Direction::West] = side;
        ts.faces[Direction::East] = side;
        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Stone_Slab; }

    Metadata getDropMeta(Metadata meta) const override { return static_cast<Metadata>(variantFromMeta(meta)); }
};
