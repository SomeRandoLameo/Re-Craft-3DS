#pragma once

#include "Block.hpp"

// TODO: Stair shape (straight/inner/outer) should be computed from neighboring stairs
// TODO: Custom AABB for stair collision
class BlockStairs : public Block {
public:
    enum class Half : uint8_t {
        Bottom = 0,
        Top    = 1,
    };

    enum class Facing : uint8_t {
        North = 0,
        South = 1,
        West  = 2,
        East  = 3,
    };

    static Facing facingFromMeta(Metadata meta) {
        switch (5 - (meta & 0x3)) {
        case 2: return Facing::North;
        case 3: return Facing::South;
        case 4: return Facing::West;
        case 5: return Facing::East;
        default: return Facing::North;
        }
    }

    static Half halfFromMeta(Metadata meta) {
        return (meta & 0x4) ? Half::Top : Half::Bottom;
    }

    explicit BlockStairs(const Block* modelBlock, Metadata modelMeta)
        : Block(), m_modelBlock(modelBlock), m_modelMeta(modelMeta) {
        setOpaque(false);
    }

    TextureSet getTextures(Metadata meta) const override {
        return m_modelBlock->getTextures(m_modelMeta);
    }

    BlockID getDropID(Metadata meta) const override {
        return m_modelBlock->getDropID(m_modelMeta);
    }

    Metadata getDropMeta(Metadata meta) const override {
        return m_modelMeta;
    }

private:
    const Block* m_modelBlock;
    Metadata     m_modelMeta;
};