#pragma once
extern "C" {
#include <c3d/texture.h>
}
#include <stdint.h>
#include <vector>


void Texture_Load(C3D_Tex* result, const char* filename);

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

    constexpr static int MapSize = 128;
    constexpr static int TileSize = 16;
    constexpr static int MapTiles = MapSize / TileSize;

    void Init(const char** files, int num_files);
    const Icon& Get(const char* filename);
    C3D_Tex* GetTexture() { return &m_texture; }

private:
    C3D_Tex m_texture;
    std::vector<Icon> m_icons;
};

void Texture_TileImage8(u8* src, u8* dst, int size);
