#pragma once

#include "Block.hpp"

// TODO: Needs X pattern rendering for proper appearance
// TODO: Should only be placeable on grass/dirt
class BlockBush : public Block {
public:
    BlockBush() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
