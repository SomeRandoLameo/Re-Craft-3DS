#pragma once

#include "Block.hpp"

// TODO: Needs torch pattern rendering for proper torch appearance
// TODO: Should drop when attached block is removed
// TODO: Particle effects (flame and smoke)
class BlockTorch : public Block {
public:
    enum class Facing : uint8_t {
        East = 1,
        West = 2,
        South = 3,
        North = 4,
        Up = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        switch (meta) {
        case 1:
            return Facing::East;
        case 2:
            return Facing::West;
        case 3:
            return Facing::South;
        case 4:
            return Facing::North;
        case 5:
        default:
            return Facing::Up;
        }
    }

    BlockTorch() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    virtual const char* getTextureName() const { return "torch_on"; }

    TextureSet getTextures(Metadata meta) const override { return TextureSet(getTextureName()); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Torch; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
