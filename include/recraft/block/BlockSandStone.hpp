#pragma once

#include "Block.hpp"

class BlockSandStone : public Block {
public:
    enum class Variant : uint8_t {
        Normal = 0,
        Chiseled = 1,
        Smooth = 2,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 2)
            meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockSandStone() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        switch (variantFromMeta(meta)) {
        case Variant::Chiseled:
            ts.faces[Direction::Top] = "sandstone_top";
            ts.faces[Direction::Bottom] = "sandstone_bottom";
            ts.faces[Direction::North] = "sandstone_carved";
            ts.faces[Direction::South] = "sandstone_carved";
            ts.faces[Direction::West] = "sandstone_carved";
            ts.faces[Direction::East] = "sandstone_carved";
            break;
        case Variant::Smooth:
            ts.faces[Direction::Top] = "sandstone_top";
            ts.faces[Direction::Bottom] = "sandstone_bottom";
            ts.faces[Direction::North] = "sandstone_smooth";
            ts.faces[Direction::South] = "sandstone_smooth";
            ts.faces[Direction::West] = "sandstone_smooth";
            ts.faces[Direction::East] = "sandstone_smooth";
            break;
        case Variant::Normal:
        default:
            ts.faces[Direction::Top] = "sandstone_top";
            ts.faces[Direction::Bottom] = "sandstone_bottom";
            ts.faces[Direction::North] = "sandstone_normal";
            ts.faces[Direction::South] = "sandstone_normal";
            ts.faces[Direction::West] = "sandstone_normal";
            ts.faces[Direction::East] = "sandstone_normal";
            break;
        }
        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Sandstone; }

    Metadata getDropMeta(Metadata meta) const override { return static_cast<Metadata>(variantFromMeta(meta)); }
};
