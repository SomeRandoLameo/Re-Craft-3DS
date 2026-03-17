#pragma once

#include "Block.hpp"

class BlockRailBase : public Block {
public:
    enum class Shape : uint8_t {
        FlatNorthSouth = 0,
        FlatEastWest   = 1,
        AscendingEast  = 2,
        AscendingWest  = 3,
        AscendingNorth = 4,
        AscendingSouth = 5,
    };

    // Normal rail also has curved variants
    enum class ShapeCurved : uint8_t {
        CurvedSouthEast = 6,
        CurvedSouthWest = 7,
        CurvedNorthWest = 8,
        CurvedNorthEast = 9,
    };

    static Shape shapeFromMeta(Metadata meta) {
        uint8_t shape = meta & 0x7;
        if (shape > 5) shape = 0;
        return static_cast<Shape>(shape);
    }

    BlockRailBase() : Block() {
        setOpaque(false);
        setSolid(false);
    }

    // TODO: Rails should not be solid and should have a custom AABB (flat on ground)
    // TODO: Minecart system needed for rail functionality
};