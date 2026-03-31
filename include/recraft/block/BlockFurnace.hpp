#pragma once

#include "Block.hpp"

// TODO: Requires tile entity system for smelting logic
// TODO: Should open furnace GUI when right clicked
// TODO: Lit furnace should emit flame/smoke particles
// TODO: Should switch between BlockID::Furnace and BlockID::Lit_Furnace based on burn state
class BlockFurnace : public Block {
public:
    enum class Facing : uint8_t {
        North = 2,
        South = 3,
        West = 4,
        East = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        switch (meta) {
        case 3:
            return Facing::South;
        case 4:
            return Facing::West;
        case 5:
            return Facing::East;
        case 2:
        default:
            return Facing::North;
        }
    }

    explicit BlockFurnace(bool isBurning) : Block(), m_isBurning(isBurning) {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        Facing facing = facingFromMeta(meta);

        const char* front = m_isBurning ? "furnace_front_on" : "furnace_front_off";

        ts.faces[Direction::Top] = "furnace_top";
        ts.faces[Direction::Bottom] = "furnace_top";
        ts.faces[Direction::North] = "furnace_side";
        ts.faces[Direction::South] = "furnace_side";
        ts.faces[Direction::West] = "furnace_side";
        ts.faces[Direction::East] = "furnace_side";

        switch (facing) {
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

    BlockID getDropID(Metadata meta) const override { return BlockID::Furnace; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }

private:
    bool m_isBurning;
};
