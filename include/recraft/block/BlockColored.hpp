#pragma once

#include "Block.hpp"

class BlockColored : public Block {
public:
    static const char* textureFromMeta(const std::string& prefix, Metadata meta) {
        static const char* suffixes[] = {
            "white",
            "orange",
            "magenta",
            "light_blue",
            "yellow",
            "lime",
            "pink",
            "gray",
            "silver",
            "cyan",
            "purple",
            "blue",
            "brown",
            "green",
            "red",
            "black",
        };

        if (meta > 15) meta = 0;
        static char buffer[64];
        snprintf(buffer, sizeof(buffer), "%s_%s", prefix.c_str(), suffixes[meta]);
        return buffer;
    }

    explicit BlockColored(const std::string& texturePrefix) : Block(), m_texturePrefix(texturePrefix) {}

    TextureSet getTextures(Metadata meta) const override {
        return TextureSet(textureFromMeta(m_texturePrefix, meta));
    }

    Metadata getDropMeta(Metadata meta) const override {
        return meta & 0xF;
    }

private:
    std::string m_texturePrefix;
};