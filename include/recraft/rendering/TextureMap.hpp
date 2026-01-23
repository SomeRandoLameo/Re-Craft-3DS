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

typedef struct {
	uint32_t textureHash;
	int16_t u, v;
} Texture_MapIcon;

typedef struct {
	C3D_Tex texture;
	std::array<Texture_MapIcon, Texture::MapTiles * Texture::MapTiles> icons;
} Texture_Map;

void Texture_MapInit(Texture_Map* map, const char** files, int num_files);
Texture_MapIcon Texture_MapGetIcon(Texture_Map* map, const char* filename);

void Texture_TileImage8(uint8_t* src, uint8_t* dst, int size);