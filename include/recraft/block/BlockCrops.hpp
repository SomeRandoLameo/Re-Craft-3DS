#pragma once

#include "BlockBush.hpp"

// TODO: Should only grow on farmland
// TODO: Growth should be affected by light level and nearby farmland moisture
// TODO: Should drop seeds when not fully grown, crop item when fully grown
// TODO: Bonemeal should advance growth stage
class BlockCrops : public BlockBush {
public:
    static constexpr uint8_t MaxAge = 7;

    static uint8_t ageFromMeta(Metadata meta) {
        return meta & 0x7;
    }

    static bool isMaxAge(Metadata meta) {
        return ageFromMeta(meta) >= MaxAge;
    }

    BlockCrops() : BlockBush() {
        setOpaque(false);
        setSolid(false);
    }

    virtual const char* getTexturePrefix() const {
        return "wheat_stage_";
    }

    TextureSet getTextures(Metadata meta) const override {
        // TODO: Needs X pattern rendering for proper crop appearance
        static char buffer[32];
        snprintf(buffer, sizeof(buffer), "%s%d", getTexturePrefix(), ageFromMeta(meta));
        return TextureSet(buffer);
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Wheat;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};