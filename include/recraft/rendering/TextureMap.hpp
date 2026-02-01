#pragma once
extern "C" {
#include <c3d/texture.h>
}
#include <array>
#include <stdint.h>

void Texture_Load(C3D_Tex* result, const char* filename);

enum Texture {
    MapSize = 128,
    TileSize = 16,
    MapTiles = MapSize / TileSize
};

struct Texture_MapIcon {
	u32 textureHash;
	s16 u, v;
} ;

struct Texture_Map {
	C3D_Tex texture;
	std::array<Texture_MapIcon, Texture::MapTiles * Texture::MapTiles> icons;
};

void Texture_MapInit(Texture_Map* map, const char** files, int num_files);
Texture_MapIcon Texture_MapGetIcon(Texture_Map* map, const char* filename);

void Texture_TileImage8(u8* src, u8* dst, int size);
