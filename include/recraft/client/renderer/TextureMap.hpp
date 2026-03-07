#pragma once
extern "C" {
#include <c3d/texture.h>
}
#include <cstdint>
#include <string>
#include <vector>

class TextureMap {
public:
    TextureMap() = default;
    ~TextureMap() {
        m_icons.clear();
        C3D_TexDelete(&m_texture);
    };

    struct Icon {
        u32 Hash = 0x00000000;
        s16 u = 0, v = 0;
    };

    constexpr static int UvPrecision = 32768;

    void Init(const std::string& path);
    const Icon& Get(const char* filename);
    const Icon& Get(uint32_t id);
    C3D_Tex* GetTexture() { return &m_texture; }

    const int GetMapSize() const { return m_mapSize; }
    const int GetTileSize() const { return m_tileSize; }
    const int GetMapTiles() const { return m_mapSize / m_tileSize; }

private:
    C3D_Tex m_texture;
    std::vector<Icon> m_icons;
    int m_mapSize = 512;
    int m_tileSize = 16;
};
