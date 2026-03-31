#pragma once

#include "Block.hpp"

// TODO: Should not be obtainable in survival, placed programmatically by BlockPistonBase
// TODO: Should be removed when piston retracts
class BlockPistonExtension : public Block {
public:
    enum class Facing : uint8_t {
        Down = 0,
        Up = 1,
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        uint8_t facing = meta & 0x7;
        if (facing > 5)
            facing = 1;
        return static_cast<Facing>(facing);
    }

    static bool isSticky(Metadata meta) { return (meta >> 3) & 0x1; }

    BlockPistonExtension() : Block() { setOpaque(false); }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        Facing facing = facingFromMeta(meta);

        ts.faces[Direction::Top] = "piston_side";
        ts.faces[Direction::Bottom] = "piston_side";
        ts.faces[Direction::North] = "piston_side";
        ts.faces[Direction::South] = "piston_side";
        ts.faces[Direction::West] = "piston_side";
        ts.faces[Direction::East] = "piston_side";

        const char* front = isSticky(meta) ? "piston_top_sticky" : "piston_top_normal";

        switch (facing) {
        case Facing::Down:
            ts.faces[Direction::Bottom] = front;
            break;
        case Facing::Up:
            ts.faces[Direction::Top] = front;
            break;
        case Facing::North:
            ts.faces[Direction::North] = front;
            break;
        case Facing::South:
            ts.faces[Direction::South] = front;
            break;
        case Facing::West:
            ts.faces[Direction::West] = front;
            break;
        case Facing::East:
            ts.faces[Direction::East] = front;
            break;
        }

        return ts;
    }

    BlockID getDropID(Metadata meta) const override { return BlockID::Air; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
