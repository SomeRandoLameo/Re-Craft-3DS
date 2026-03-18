#pragma once

#include "BlockBush.hpp"

class BlockFlower : public BlockBush {
public:
    enum class FlowerColor {
        Yellow,
        Red,
    };

    enum class FlowerType {
        // Yellow
        Dandelion   = 0,
        // Red
        Poppy       = 0,
        BlueOrchid  = 1,
        Allium      = 2,
        Houstonia   = 3,
        RedTulip    = 4,
        OrangeTulip = 5,
        WhiteTulip  = 6,
        PinkTulip   = 7,
        OxeyeDaisy  = 8,
    };

    BlockFlower() : BlockBush() {}

    virtual FlowerColor getBlockType() const = 0;

    TextureSet getTextures(Metadata meta) const override {
        if (getBlockType() == FlowerColor::Yellow) {
            return TextureSet("flower_dandelion");
        }

        static const char* redNames[] = {
            "flower_rose",
            "flower_blue_orchid",
            "flower_allium",
            "flower_houstonia",
            "flower_tulip_red",
            "flower_tulip_orange",
            "flower_tulip_white",
            "flower_tulip_pink",
            "flower_oxeye_daisy",
        };

        if (meta > 8) meta = 0;
        return TextureSet(redNames[meta]);
    }
};