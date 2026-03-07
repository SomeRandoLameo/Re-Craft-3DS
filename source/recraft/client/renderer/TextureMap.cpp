#include <3ds.h>
#include <citro3d.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stb/stb_image.h"

#include "ReCraftCore.hpp"
#include "amethyst/include/amethyst.hpp"

#include "client/renderer/TextureMap.hpp"
#include "misc/Crash.hpp"

uint32_t hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

unsigned int D7_TileIndex(const int& x, const int& y, const int& w) {
    return (((y >> 3) * (w >> 3) + (x >> 3)) << 6) +
        ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3));
}

void TextureMap::Init(const std::string& path) {
    int locX = 0;
    int locY = 0;

    m_mapSize = 1024;
    m_tileSize = 16;
    { // determinate tilesize
        int w = 0;
        int h = 0;
        int c = 0;
        auto buf = stbi_load(std::string(path + "/dirt.png").c_str(), &w, &h, &c, 4);
        if (buf && w == h) {
            m_tileSize = w;
        }
        if (buf) {
            stbi_image_free(buf);
        }
    }
    const int mipmapLevels = 2;
    const int maxSize = 4 * m_mapSize * m_mapSize;

    C3D_TexInit(&m_texture, m_mapSize, m_mapSize, GPU_RGBA8);
    /*C3D_TexInitParams param;
    param.width = m_mapSize;
    param.height = m_mapSize;
    param.maxLevel = 2;
    param.format = GPU_RGBA8;
    param.type = GPU_TEX_2D;
    param.onVram = true;
     if (!C3D_TexInitWithParams(&m_texture, nullptr, param))
         printf("Couldn't alloc texture memory\n");*/
    C3D_TexSetFilter(&m_texture, GPU_NEAREST, GPU_NEAREST);
    // Error Tex
    for (int x = 0; x < m_tileSize; x++) {
        for (int y = 0; y < m_tileSize; y++) {
            int idx = D7_TileIndex(x, (m_mapSize - m_tileSize + y), m_mapSize) * 4;
            bool oc = ((x / (m_tileSize / 2) + y / (m_tileSize / 2)) % 2) == 0;
            reinterpret_cast<u8*>(m_texture.data)[idx + 0] = 255;
            reinterpret_cast<u8*>(m_texture.data)[idx + 1] = oc ? 255 : 0;
            reinterpret_cast<u8*>(m_texture.data)[idx + 2] = 0;
            reinterpret_cast<u8*>(m_texture.data)[idx + 3] = oc ? 255 : 0;
        }
    }
    locX += m_tileSize;

    int c = 0;
    for (const auto& it : std::filesystem::directory_iterator(path)) {
        int w = 0, h = 0, c1 = 0;
        std::string file_ = it.path().string();
        LOG("Adding Texture: {}", file_);
        unsigned char* image = stbi_load(file_.c_str(), &w, &h, &c1, 4);

        if (image != nullptr && c1 == 3) {
            LOG("Reloading Image: {} ({}, {}, {}) in mode 3", file_, w, h, c);
            stbi_image_free(image);
            image = stbi_load(file_.c_str(), &w, &h, &c1, 3);
        }

        if (image != nullptr && w == m_tileSize && h == m_tileSize) {
            for (int x = 0; x < m_tileSize; x++) {
                for (int y = 0; y < m_tileSize; y++) {
                    int src = (((m_tileSize - 1) - y) * m_tileSize + x) * c1;
                    int dst = D7_TileIndex(locX + x, (m_mapSize - 1) - (locY + y), m_mapSize) * 4;
                    if (c1 == 3) {
                        reinterpret_cast<u8*>(m_texture.data)[dst] = 255;
                    } else {
                        reinterpret_cast<u8*>(m_texture.data)[dst] = image[src + 3];
                    }
                    reinterpret_cast<u8*>(m_texture.data)[dst + 1] = image[src + 2];
                    reinterpret_cast<u8*>(m_texture.data)[dst + 2] = image[src + 1];
                    reinterpret_cast<u8*>(m_texture.data)[dst + 3] = image[src];
                }
            }

            Icon icon;
            icon.Hash = hash((char*)it.path().filename().string().c_str());
            int m = UvPrecision / m_mapSize;
            icon.u = locX * m;
            icon.v = locY * m;
            m_icons.push_back(icon);

            locX += m_tileSize;
            if (locX == m_mapSize) {
                locY += m_tileSize;
                locX = 0;
            }
        } else {
            LOG("Image Size({}, {}) doesn't match or ptr is null(internal error) or "
                "c is neither 3 or 4 for {}",
                w, h, it.path().filename().string());
        }
        if (image)
            stbi_image_free(image);
        c++;
    }

    m_texture.border = 0x00000000;
    C3D_TexFlush(&m_texture);
    C3D_TexSetWrap(&m_texture, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
}

const TextureMap::Icon& TextureMap::Get(const char* filename) {
    uint32_t h = hash(filename);
    for (size_t i = 0; i < GetMapTiles() * GetMapTiles(); i++) {
        if (h == m_icons[i].Hash) {
            return m_icons[i];
        }
    }
    static Icon nullicn2;
    return nullicn2;
}
