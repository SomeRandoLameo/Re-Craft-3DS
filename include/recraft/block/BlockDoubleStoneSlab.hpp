#pragma once

#include "BlockStoneSlab.hpp"

class BlockDoubleStoneSlab : public BlockStoneSlab {
public:
    BlockDoubleStoneSlab() : BlockStoneSlab() {}

    bool isDouble() const override { return true; }

    TextureSet getTextures(Metadata meta) const override {
        // Seamless double slab uses top texture on all faces
        if (isSeamless(meta)) {
            TextureSet ts;
            const char* top = nullptr;
            switch (variantFromMeta(meta)) {
            case Variant::Sand:
                top = "sandstone_top";
                break;
            case Variant::Quartz:
                top = "quartz_block_top";
                break;
            case Variant::Stone:
            default:
                top = "stone_slab_top";
                break;
            }
            ts.faces[Direction::Top] = top;
            ts.faces[Direction::Bottom] = top;
            ts.faces[Direction::North] = top;
            ts.faces[Direction::South] = top;
            ts.faces[Direction::West] = top;
            ts.faces[Direction::East] = top;
            return ts;
        }

        return BlockStoneSlab::getTextures(meta);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Double_Stone_Slab; }
};
