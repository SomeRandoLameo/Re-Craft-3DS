#pragma once

#include "Block.hpp"

class BlockDispenser : public Block {
public:
    enum class Facing : uint8_t {
        Down  = 0,
        Up    = 1,
        North = 2,
        South = 3,
        West  = 4,
        East  = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        uint8_t facing = meta & 0x7;
        if (facing > 5) facing = 2;
        return static_cast<Facing>(facing);
    }

    static bool isTriggered(Metadata meta) {
        return (meta >> 3) & 0x1;
    }

    BlockDispenser() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        Facing facing = facingFromMeta(meta);

        ts.faces[Direction::Top]    = "furnace_top";
        ts.faces[Direction::Bottom] = "furnace_top";
        ts.faces[Direction::North]  = "furnace_side";
        ts.faces[Direction::South]  = "furnace_side";
        ts.faces[Direction::West]   = "furnace_side";
        ts.faces[Direction::East]   = "furnace_side";

        switch (facing) {
        case Facing::Down:  ts.faces[Direction::Bottom] = "dispenser_front_vertical"; break;
        case Facing::Up:    ts.faces[Direction::Top]    = "dispenser_front_vertical"; break;
        case Facing::North: ts.faces[Direction::North]  = "dispenser_front"; break;
        case Facing::South: ts.faces[Direction::South]  = "dispenser_front"; break;
        case Facing::West:  ts.faces[Direction::West]   = "dispenser_front"; break;
        case Facing::East:  ts.faces[Direction::East]   = "dispenser_front"; break;
        }

        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Dispenser;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }
};