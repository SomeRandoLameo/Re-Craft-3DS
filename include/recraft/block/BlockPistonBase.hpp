#pragma once

#include "Block.hpp"

// TODO: Piston pushing/pulling logic requires block movement system
class BlockPistonBase : public Block {
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

    static bool isExtended(Metadata meta) { return (meta >> 3) & 0x1; }

    bool m_isSticky;

    BlockPistonBase(bool isSticky) : Block(), m_isSticky(isSticky) {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        Facing facing = facingFromMeta(meta);
        bool extended = isExtended(meta);

        const char* front = extended ? "piston_top_sticky" // extended sticky shows different face
                                     : (m_isSticky ? "piston_top_sticky" : "piston_top_normal");

        ts.faces[Direction::Top] = "piston_inner";
        ts.faces[Direction::Bottom] = "piston_bottom";
        ts.faces[Direction::North] = "piston_side";
        ts.faces[Direction::South] = "piston_side";
        ts.faces[Direction::West] = "piston_side";
        ts.faces[Direction::East] = "piston_side";

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

    BlockID getDropID(Metadata meta) const override { return m_isSticky ? BlockID::Sticky_Piston : BlockID::Piston; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
