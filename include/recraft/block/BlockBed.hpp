#pragma once

#include "Block.hpp"

// TODO: Bed should have a custom AABB (height = 0.5625)
// TODO: Bed should set spawn point and allow sleeping to skip night
// TODO: Bed should explode in Nether/End dimensions
class BlockBed : public Block {
public:
    enum class Part : uint8_t {
        Foot = 0,
        Head = 1,
    };

    enum class Facing : uint8_t {
        South = 0,
        West = 1,
        North = 2,
        East = 3,
    };

    static Facing facingFromMeta(Metadata meta) { return static_cast<Facing>(meta & 0x3); }

    static Part partFromMeta(Metadata meta) { return (meta & 0x8) ? Part::Head : Part::Foot; }

    static bool isOccupied(Metadata meta) { return (meta & 0x4) && partFromMeta(meta) == Part::Head; }

    BlockBed() : Block() { setOpaque(false); }

    TextureSet getTextures(Metadata meta) const override {
        TextureSet ts;
        bool isHead = partFromMeta(meta) == Part::Head;

        ts.faces[Direction::Top] = isHead ? "bed_head_top" : "bed_feet_top";
        ts.faces[Direction::Bottom] = "bed_feet_end";
        ts.faces[Direction::North] = isHead ? "bed_head_side" : "bed_feet_side";
        ts.faces[Direction::South] = isHead ? "bed_head_side" : "bed_feet_side";
        ts.faces[Direction::West] = isHead ? "bed_head_end" : "bed_feet_end";
        ts.faces[Direction::East] = isHead ? "bed_head_end" : "bed_feet_end";
        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        // Only drop from head piece to avoid duplicate drops
        return partFromMeta(meta) == Part::Head ? BlockID::Bed : BlockID::Air;
    }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
