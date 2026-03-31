#pragma once

#include "BlockSign.hpp"

class BlockWallSign : public BlockSign {
public:
    enum class Facing : uint8_t {
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        switch (meta) {
        case 3:
            return Facing::South;
        case 4:
            return Facing::West;
        case 5:
            return Facing::East;
        case 2:
        default:
            return Facing::North;
        }
    }

    BlockWallSign() : BlockSign() {}

    TextureSet getTextures(Metadata meta) const override {
        // TODO: Signs should render as a flat entity with text, not a full block
        return TextureSet("planks_oak");
    }
};
