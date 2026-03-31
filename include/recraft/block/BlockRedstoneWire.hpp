#pragma once

#include "Block.hpp"

// TODO: Redstone propagation system needed for wire to actually carry power
// TODO: Wire color should change based on power level (dark red = 0, bright red = 15)
// TODO: Wire connections should be computed dynamically from neighboring blocks
// TODO: Should drop redstone dust item, requires item system
class BlockRedstoneWire : public Block {
public:
    static uint8_t powerFromMeta(Metadata meta) { return meta & 0xF; }

    // Color multiplier based on power level, matching vanilla
    static void getPowerColor(uint8_t power, uint8_t& r, uint8_t& g, uint8_t& b) {
        float f = (float)power / 15.0f;
        float f1 = f * 0.6f + (power == 0 ? 0.3f : 0.4f);
        float f2 = f * f * 0.7f - 0.5f;
        float f3 = f * f * 0.6f - 0.7f;
        if (f2 < 0.0f)
            f2 = 0.0f;
        if (f3 < 0.0f)
            f3 = 0.0f;
        r = (uint8_t)(f1 * 255.0f);
        g = (uint8_t)(f2 * 255.0f);
        b = (uint8_t)(f3 * 255.0f);
    }

    BlockRedstoneWire() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        uint8_t r, g, b;
        getPowerColor(powerFromMeta(metadata), r, g, b);
        out_rgb[0] = r;
        out_rgb[1] = g;
        out_rgb[2] = b;
    }

    TextureSet getTextures(Metadata meta) const override { return TextureSet("redstone_dust_cross"); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
