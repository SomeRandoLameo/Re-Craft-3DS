#pragma once

#include "Block.hpp"

class BlockOldLog : public Block {
public:
    enum class Variant : uint8_t {
        Oak    = 0,
        Spruce = 1,
        Birch  = 2,
        Jungle = 3,
    };

    enum class Axis : uint8_t {
        Y    = 0,
        X    = 1,
        Z    = 2,
        Bark = 3,
    };

    static Variant variantFromMeta(Metadata meta) {
        uint8_t variant = meta & 0x3;
        return static_cast<Variant>(variant);
    }

    static Axis axisFromMeta(Metadata meta) {
        uint8_t axis = (meta >> 2) & 0x3;
        return static_cast<Axis>(axis);
    }

    BlockOldLog() : Block() {}

    TextureSet getTextures(Metadata meta) const override {
        static const char* topNames[] = {
            "log_oak_top",
            "log_spruce_top",
            "log_birch_top",
            "log_jungle_top",
        };
        static const char* sideNames[] = {
            "log_oak",
            "log_spruce",
            "log_birch",
            "log_jungle",
        };

        uint8_t idx = static_cast<uint8_t>(variantFromMeta(meta));
        Axis axis = axisFromMeta(meta);

        TextureSet ts;
        switch (axis) {
        case Axis::X:
            ts.faces[Direction::Top]    = sideNames[idx];
            ts.faces[Direction::Bottom] = sideNames[idx];
            ts.faces[Direction::North]  = topNames[idx];
            ts.faces[Direction::South]  = topNames[idx];
            ts.faces[Direction::West]   = sideNames[idx];
            ts.faces[Direction::East]   = sideNames[idx];
            break;
        case Axis::Z:
            ts.faces[Direction::Top]    = sideNames[idx];
            ts.faces[Direction::Bottom] = sideNames[idx];
            ts.faces[Direction::North]  = sideNames[idx];
            ts.faces[Direction::South]  = sideNames[idx];
            ts.faces[Direction::West]   = topNames[idx];
            ts.faces[Direction::East]   = topNames[idx];
            break;
        case Axis::Bark:
            ts = TextureSet(sideNames[idx]);
            break;
        case Axis::Y:
        default:
            ts.faces[Direction::Top]    = topNames[idx];
            ts.faces[Direction::Bottom] = topNames[idx];
            ts.faces[Direction::North]  = sideNames[idx];
            ts.faces[Direction::South]  = sideNames[idx];
            ts.faces[Direction::West]   = sideNames[idx];
            ts.faces[Direction::East]   = sideNames[idx];
            break;
        }
        return ts;
    }

    BlockID getDropID(Metadata meta) const override {
        return BlockID::Log;
    }

    Metadata getDropMeta(Metadata meta) const override {
        // strip axis bits, only keep wood type
        return static_cast<Metadata>(variantFromMeta(meta));
    }
};