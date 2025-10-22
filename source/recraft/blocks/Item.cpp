#include "Item.h"

#include "../rendering/TextureMap.h"
#include "../rendering/VertexFmt.h"

static Texture_Map textureMap;

// PATH PREFIX
#define PPRX "romfs:/assets/textures/blocks/"

#define ITEM_TEXTURE_FILES                                                                                                               \
	A(totem, "dirt.png")

#define A(i, n) PPRX n
const char* item_texture_files[] = {ITEM_TEXTURE_FILES};
#undef A

static struct {
	Texture_MapIcon totem;
} icon;

void Item_Init() {
	Texture_MapInit(&textureMap, item_texture_files, sizeof(item_texture_files) / sizeof(item_texture_files[0]));
#define A(i, n) icon.i = Texture_MapGetIcon(&textureMap, PPRX n)
	ITEM_TEXTURE_FILES;
#undef A
}
void Item_Deinit() { C3D_TexDelete(&textureMap.texture); }

void* Item_GetTextureMap() { return &textureMap.texture; }

void Item_GetTexture(Item item, Direction direction, uint8_t metadata, int16_t* out_uv) {
	Texture_MapIcon i = {0, 0, 0};
	switch (item) {
		case Item_Totem:
			i = icon.totem;
			break;
		default: break;
	}
	out_uv[0] = i.u;
	out_uv[1] = i.v;
}

const char* ItemNames[Items_Count] = {
    "Totem"
};
