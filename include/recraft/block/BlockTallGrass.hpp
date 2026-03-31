#pragma once

#include "Block.hpp"

// TODO: Needs X pattern rendering for proper appearance
class BlockTallGrass : public Block {
public:
    enum class Variant : uint8_t {
        DeadShrub = 0,
        TallGrass = 1,
        Fern = 2,
    };

    static Variant variantFromMeta(Metadata meta) {
        if (meta > 2)
            meta = 1;
        return static_cast<Variant>(meta);
    }

    BlockTallGrass() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        switch (variantFromMeta(metadata)) {
        case Variant::TallGrass:
        case Variant::Fern:
            out_rgb[0] = 140;
            out_rgb[1] = 214;
            out_rgb[2] = 123;
            break;
        default:
            Block::getColor(metadata, direction, out_rgb);
            break;
        }
    }

    TextureSet getTextures(Metadata meta) const override {
        static const char* names[] = {
            "deadbush",
            "tallgrass",
            "fern",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        return TextureSet(names[idx]);
    }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Should drop seeds occasionally, requires item system
        return BlockID::Air;
    }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
