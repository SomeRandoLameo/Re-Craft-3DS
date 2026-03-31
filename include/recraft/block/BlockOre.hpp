#pragma once

#include "Block.hpp"

#pragma once

#include "Block.hpp"

class BlockOre : public Block {
public:
    BlockOre() : Block() {}

    // TODO: How the hell deos minecraft determine these??? this is not the right design...
    TextureSet getTextures(Metadata meta) const override {
        static const struct {
            const char* unlocalizedName;
            const char* textureName;
        } remap[] = {
            {"oreGold", "gold_ore"},       {"oreIron", "iron_ore"},        {"oreCoal", "coal_ore"},
            {"oreLapis", "lapis_ore"},     {"oreDiamond", "diamond_ore"},  {"oreRedstone", "redstone_ore"},
            {"oreEmerald", "emerald_ore"}, {"netherquartz", "quartz_ore"},
        };

        for (auto& entry : remap) {
            if (m_unlocalizedName == entry.unlocalizedName) {
                return TextureSet(entry.textureName);
            }
        }

        // fallback
        return TextureSet(m_unlocalizedName.c_str());
    }

    BlockID getDropID(Metadata meta) const override {
        // TODO: Requires item system
        switch (m_id) {
        case BlockID::Lit_Redstone_Ore:
            return BlockID::Redstone_Ore;
        default:
            return m_id;
        }
    }

    Metadata getDropMeta(Metadata meta) const override { return 0; }
};
