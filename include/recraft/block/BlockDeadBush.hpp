#pragma once

#include "Block.hpp"

// TODO: Needs X pattern rendering for proper appearance
class BlockDeadBush : public Block {
public:
    BlockDeadBush() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    TextureSet getTextures(Metadata meta) const override { return TextureSet("deadbush"); }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
