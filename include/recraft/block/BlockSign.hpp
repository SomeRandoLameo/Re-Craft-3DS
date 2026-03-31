#pragma once

#include "Block.hpp"

// TODO: Requires tile entity system for sign text
// TODO: Should open sign editing GUI when placed
class BlockSign : public Block {
public:
    BlockSign() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Standing_Sign; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
