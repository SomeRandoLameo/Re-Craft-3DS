#pragma once

#include "BlockRailBase.hpp"

class BlockRailPowered : public BlockRailBase {
public:
    static bool isPowered(Metadata meta) {
        return (meta >> 3) & 0x1;
    }

    BlockRailPowered() : BlockRailBase() {}

    TextureSet getTextures(Metadata meta) const override {
        // TODO: X pattern rendering needed for proper rail appearance
        return TextureSet(isPowered(meta) ? "rail_golden_powered" : "rail_golden");
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Golden_Rail;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};