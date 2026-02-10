#include "rendering/TextureMap.hpp"

#include <stb_image.h>

#include <3ds.h>
#include <citro3d.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void tileImage32(u32* src, u8* dst, int sizex, int sizez);

void Texture_Load(C3D_Tex* result, const char* filename) {
    int width = 0, height = 0, c = 0;
    unsigned char* image = stbi_load(filename, &width, &height, &c, 4);
    if (image != NULL) {
        uint32_t* imgInLinRam = (uint32_t*)linearAlloc(width * height * sizeof(uint32_t));

        if (width < 64 || height < 64) {
            /* For small images you handled pixels differently: operate on 32-bit pixels after casting. */
            uint32_t* src32 = (uint32_t*)image;
            for (int j = 0; j < (int)height; j++)
                for (int i = 0; i < (int)width; i++) {
                    src32[i + j * width] = __builtin_bswap32(src32[i + j * width]);
                }
            /* When width/height < 64 you later call tileImage32(image, ...),
               but tileImage32 expects u32* src; we can call with (u32*)src32 where appropriate. */
        } else {
            uint32_t* src32 = (uint32_t*)image;
            for (int i = 0; i < (int)(width * height); i++) {
                imgInLinRam[i] = __builtin_bswap32(src32[i]);
            }
        }

        C3D_TexInitVRAM(result, width, height, GPU_RGBA8);

        if (width < 64 || height < 64) {
            /* For small images we must tile using the byte buffer interpreted as u32*
               Create a temporary linear buffer for the tiled image. */
            uint32_t* src32 = (uint32_t*)image;
            tileImage32(src32, (u8*)imgInLinRam, width, height);
        }

        GSPGPU_FlushDataCache(imgInLinRam, width * height * sizeof(uint32_t));
        free(image);

        if (width < 64 || height < 64) {
            C3D_SyncTextureCopy(imgInLinRam, 0, (u32*)result->data, 0, width * height * sizeof(uint32_t), 8);
        } else {
            C3D_SyncDisplayTransfer(
                (uint32_t*)imgInLinRam, GX_BUFFER_DIM(width, height), (u32*)result->data, GX_BUFFER_DIM(width, height),
                (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |
                 GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |
                 GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO)));
        }

        linearFree(imgInLinRam);
    } else {
        Crash("Failed to load texture %s\n", filename);
    }
}

// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
static inline u32 morton_interleave(u32 x, u32 y) {
    u32 i = (x & 7) | ((y & 7) << 8); // ---- -210
    i = (i ^ (i << 2)) & 0x1313; // ---2 --10
    i = (i ^ (i << 1)) & 0x1515; // ---2 -1-0
    i = (i | (i >> 7)) & 0x3F;
    return i;
}
// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
static inline u32 get_morton_offset(u32 x, u32 y, u32 bytes_per_pixel) {
    u32 i = morton_interleave(x, y);
    unsigned int offset = (x & ~7) * 8;
    return (i + offset) * bytes_per_pixel;
}
// from sf2d
// https://github.com/xerpi/sf2dlib/blob/effe77ea81d21c26bad457d4f5ed8bb16ce7b753/libsf2d/source/sf2d_texture.c
void tileImage32(u32* src, u8* dst, int sizex, int sizey) {
    for (int j = 0; j < sizey; j++) {
        for (int i = 0; i < sizex; i++) {
            u32 coarse_y = j & ~7;
            u32 dst_offset = get_morton_offset(i, j, 4) + coarse_y * sizex * 4;

            u32 v = src[i + (sizey - 1 - j) * sizex];
            *(u32*)(dst + dst_offset) = v;
        }
    }
}
void Texture_TileImage8(uint8_t* src, uint8_t* dst, int size) {
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            u32 coarse_y = j & ~7;
            u32 dst_offset = get_morton_offset(i, j, 1) + coarse_y * size;

            u8 v = src[i + (size - 1 - j) * size];
            *(dst + dst_offset) = v;
        }
    }
}
void downscaleImage(u8* data, int size) {
    int i, j;
    for (j = 0; j < size; j++) {
        for (i = 0; i < size; i++) {
            const u32 offset = (i + j * size) * 4;
            const u32 offset2 = (i * 2 + j * 2 * size * 2) * 4;
            data[offset + 0] = (data[offset2 + 0 + 0] + data[offset2 + 4 + 0] + data[offset2 + size * 4 * 2 + 0] +
                                data[offset2 + (size * 2 + 1) * 4 + 0]) /
                4;
            data[offset + 1] = (data[offset2 + 0 + 1] + data[offset2 + 4 + 1] + data[offset2 + size * 4 * 2 + 1] +
                                data[offset2 + (size * 2 + 1) * 4 + 1]) /
                4;
            data[offset + 2] = (data[offset2 + 0 + 2] + data[offset2 + 4 + 2] + data[offset2 + size * 4 * 2 + 2] +
                                data[offset2 + (size * 2 + 1) * 4 + 2]) /
                4;
            data[offset + 3] = (data[offset2 + 0 + 3] + data[offset2 + 4 + 3] + data[offset2 + size * 4 * 2 + 3] +
                                data[offset2 + (size * 2 + 1) * 4 + 3]) /
                4;
        }
    }
}

void TextureMap::Init(const char** files, int num_files) {
    int locX = 0;
    int locY = 0;

    // printf("TextureMapInit %s\n", files);

    const int mipmapLevels = 2;
    const int maxSize = 4 * MapSize * MapSize;


    C3D_TexInit(&m_texture, MapSize, MapSize, GPU_RGBA8);
    /*C3D_TexInitParams param;
    param.width = MapSize;
    param.height = MapSize;
    param.maxLevel = 2;
    param.format = GPU_RGBA8;
    param.type = GPU_TEX_2D;
    param.onVram = true;
     if (!C3D_TexInitWithParams(&m_texture, nullptr, param))
         printf("Couldn't alloc texture memory\n");*/
    C3D_TexSetFilter(&m_texture, GPU_NEAREST, GPU_NEAREST);

    int filei = 0;
    const char* filename = files[filei];
    int c = 0;
    while (filename != NULL && c < (MapTiles * MapTiles) && filei < num_files) {
        int w = 0, h = 0, c1 = 0;
        unsigned char* image = stbi_load(filename, &w, &h, &c1, 4);

        if (image != nullptr && c1 == 3) {
            stbi_image_free(image);
            image = stbi_load(filename, &w, &h, &c1, 3);
        }

        if (image != nullptr && w == TileSize && h == TileSize) {

            for (int x = 0; x < TileSize; x++) {
                for (int y = 0; y < TileSize; y++) {
                    int src = (((TileSize - 1) - y) * TileSize + x) * c1;
                    int dst = D7_TileIndex(locX + x, (MapSize - 1) - (locY + y), MapSize) * 4;
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
            icon.Hash = hash((char*)filename);
            icon.u = 256 * locX;
            icon.v = 256 * locY;
            m_icons.push_back(icon);

            locX += TileSize;
            if (locX == MapSize) {
                locY += TileSize;
                locX = 0;
            }
        } else {
            printf("Image size(%u, %u) doesn't match or ptr null(internal error) or c not 4 for '%s'\n", w, h,
                   filename ? filename : "(null)");
        }
        if (image)
            stbi_image_free(image);
        filename = files[++filei];
        c++;
    }

    m_texture.border = 0x00000000;
    C3D_TexFlush(&m_texture);
    C3D_TexSetWrap(&m_texture, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
}

const TextureMap::Icon& TextureMap::Get(const char* filename) {
    uint32_t h = hash(filename);
    for (size_t i = 0; i < MapTiles * MapTiles; i++) {
        if (h == m_icons[i].Hash) {
            return m_icons[i];
        }
    }
    static Icon nullicn2;
    return nullicn2;
}
