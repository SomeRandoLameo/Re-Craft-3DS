#pragma once

#include "Block.hpp"

// TODO: Requires tile entity system for inventory storage
// TODO: Double chest when two are placed side by side
// TODO: Trapped chest should emit redstone signal when opened
// TODO: Should not open when block is on top or ocelot is sitting on it
class BlockChest : public Block {
public:
    enum class Type {
        Basic,
        Trapped,
    };

    enum class Facing : uint8_t {
        North = 2,
        South = 3,
        West  = 4,
        East  = 5,
    };

    static Facing facingFromMeta(Metadata meta) {
        switch (meta) {
        case 3:  return Facing::South;
        case 4:  return Facing::West;
        case 5:  return Facing::East;
        case 2:
        default: return Facing::North;
        }
    }

    explicit BlockChest(Type type) : Block(), m_type(type) {
        setOpaque(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        ts.faces[Direction::Top]    = "chest_top";
        ts.faces[Direction::Bottom] = "chest_top";
        ts.faces[Direction::North]  = "chest_front";
        ts.faces[Direction::South]  = "chest_back";
        ts.faces[Direction::West]   = "chest_side";
        ts.faces[Direction::East]   = "chest_side";
        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        return m_type == Type::Trapped ? BlockID::Trapped_Chest : BlockID::Chest;
    }

    Metadata getDropMeta(Metadata meta) const override {
        return 0;
    }

private:
    Type m_type;
};