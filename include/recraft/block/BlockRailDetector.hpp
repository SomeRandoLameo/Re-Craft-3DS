#pragma once

#include "BlockRailBase.hpp"

class BlockRailDetector : public BlockRailBase {
public:
    static bool isPowered(Metadata meta) {
        return (meta >> 3) & 0x1;
    }

    BlockRailDetector() : BlockRailBase() {}

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet(isPowered(meta) ? "rail_detector_powered" : "rail_detector");
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Detector_Rail;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};