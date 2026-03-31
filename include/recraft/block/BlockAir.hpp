#pragma once

#include "Block.hpp"

class BlockAir : public Block {
public:
    BlockAir() : Block() {
        setOpaque(false);
        setSolid(false);
    }
};
