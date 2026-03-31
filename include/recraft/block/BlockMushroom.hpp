#pragma once

#include "BlockBush.hpp"

// TODO: Mushrooms should spread to nearby blocks in low light conditions
// TODO: Bonemeal should grow a giant mushroom
class BlockMushroom : public BlockBush {
public:
    BlockMushroom() : BlockBush() {}

    // TODO: How the hell does Minecraft determine these textures??
    TextureSet getTextures(Metadata meta) const override {
        switch (m_id) {
        case BlockID::Red_Mushroom:
            return TextureSet("mushroom_red");
        case BlockID::Brown_Mushroom:
        default:
            return TextureSet("mushroom_brown");
        }
    }

    BlockID getDropID(Metadata meta) const override { return m_id; }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
