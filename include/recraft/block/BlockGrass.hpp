#pragma once

#include "Block.hpp"

class BlockGrass : public Block {
public:
    enum class Variant : uint8_t {
        Normal = 0,
        Snowy = 1,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 1)
            meta = 0;
        return static_cast<Variant>(meta);
    }

    BlockGrass() : Block() {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        if (direction == Direction::Top) {
            if (variantFromMeta(metadata) == Variant::Snowy) {
                out_rgb[0] = 255;
                out_rgb[1] = 255;
                out_rgb[2] = 255;
            } else {
                out_rgb[0] = 140;
                out_rgb[1] = 214;
                out_rgb[2] = 123;
            }
        } else {
            Block::getColor(metadata, direction, out_rgb);
        }
    }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        switch (variantFromMeta(meta)) {
        case Variant::Snowy:
            ts.faces[Direction::Top] = "snow";
            ts.faces[Direction::Bottom] = "dirt";
            ts.faces[Direction::North] = "grass_side_snowed";
            ts.faces[Direction::South] = "grass_side_snowed";
            ts.faces[Direction::West] = "grass_side_snowed";
            ts.faces[Direction::East] = "grass_side_snowed";
            break;
        case Variant::Normal:
        default:
            ts.faces[Direction::Top] = "grass_top";
            ts.faces[Direction::Bottom] = "dirt";
            ts.faces[Direction::North] = "grass_side";
            ts.faces[Direction::South] = "grass_side";
            ts.faces[Direction::West] = "grass_side";
            ts.faces[Direction::East] = "grass_side";
            break;
        }
        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Dirt; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
