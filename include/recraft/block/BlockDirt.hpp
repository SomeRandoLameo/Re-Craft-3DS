#pragma once

#include "Block.hpp"

class BlockDirt : public Block {
public:
    enum class Variant : uint8_t {
        Dirt = 0,
        CoarseDirt = 1,
        Podzol = 2,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 2)
            meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockDirt() : Block() {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        Block::getColor(metadata, direction, out_rgb);
    }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        switch (variantFromMeta(meta)) {
        case Variant::CoarseDirt:
            ts = TextureSet("coarse_dirt");
            break;
        case Variant::Podzol:
            ts.faces[Direction::Top] = "dirt_podzol_top";
            ts.faces[Direction::Bottom] = "dirt";
            ts.faces[Direction::North] = "dirt_podzol_side";
            ts.faces[Direction::South] = "dirt_podzol_side";
            ts.faces[Direction::West] = "dirt_podzol_side";
            ts.faces[Direction::East] = "dirt_podzol_side";
            break;
        case Variant::Dirt:
        default:
            ts = TextureSet("dirt");
            break;
        }
        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Dirt; }

    Metadata getDropMeta(Metadata meta) const override { return static_cast<Metadata>(variantFromMeta(meta)); }
};
