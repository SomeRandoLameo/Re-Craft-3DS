#pragma once

#include "BlockStoneSlab.hpp"

class BlockHalfStoneSlab : public BlockStoneSlab {
public:
    BlockHalfStoneSlab() : BlockStoneSlab() {}

    bool isDouble() const override { return false; }

    BlockID getDropID(Metadata meta) const override { return BlockID::Stone_Slab; }
};
