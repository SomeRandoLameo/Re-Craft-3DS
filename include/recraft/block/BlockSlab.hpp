#pragma once

#include "Block.hpp"

// TODO: Slab should have a custom AABB (half height)
// TODO: Top/bottom placement based on where player clicks
class BlockSlab : public Block {
public:
    enum class Half : uint8_t {
        Bottom = 0,
        Top = 1,
    };

    static Half halfFromMeta(Metadata meta) { return (meta & 0x8) ? Half::Top : Half::Bottom; }

    static bool isSeamless(Metadata meta) { return (meta & 0x8) != 0; }

    BlockSlab() : Block() {}

    virtual bool isDouble() const = 0;

    bool isOpaque(Metadata metadata) const override { return isDouble(); }
};
