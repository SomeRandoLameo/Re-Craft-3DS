#include "blocks/CT_Block.h"

#include "rendering/TextureMap.h"
#include "rendering/VertexFmt.h"

static Texture_Map textureMap;

// PATH PREFIX
#define PPRX "romfs:/assets/textures/blocks/"

const char* blockTextures[] = {
        PPRX "stone.png",
        PPRX "dirt.png",
        PPRX "cobblestone.png",
        PPRX "grass_side.png",
        PPRX "grass_top.png",
        PPRX "stonebrick.png",
        PPRX "sand.png",
        PPRX "log_oak.png",
        PPRX "log_oak_top.png",
        PPRX "leaves_oak.png",
        PPRX "glass.png",
        PPRX "brick.png",
        PPRX "planks_oak.png",
        PPRX "wool.png",
        PPRX "bedrock.png",
        PPRX "gravel.png",
        PPRX "coarse_dirt.png",
        PPRX "door_top.png",
        PPRX "door_bottom.png",
        PPRX "snow_grass_side.png",
        PPRX "snow.png",
        PPRX "obsidian.png",
        PPRX "sandstone_side.png",
        PPRX "sandstone_top.png",
        PPRX "sandstone_bottom.png",
        PPRX "netherrack.png",
        PPRX "smooth_stone.png",
        PPRX "lava.png",
        PPRX "water.png",
        PPRX "grass_path_side.png",
        PPRX "grass_path_top.png",
        PPRX "crafting_table_side.png",
        PPRX "crafting_table_top.png",
        PPRX "iron_ore.png",
        PPRX "iron_block.png",
        PPRX "diamond_ore.png",
        PPRX "diamond_block.png",
        PPRX "gold_ore.png",
        PPRX "gold_block.png",
        PPRX "coal_ore.png",
        PPRX "coal_block.png",
        PPRX "emerald_ore.png",
        PPRX "emerald_block.png",
        PPRX "furnace_side.png",
        PPRX "furnace_front.png",
        PPRX "furnace_top.png"
};

static struct {
	Texture_MapIcon stone;
	Texture_MapIcon dirt;
	Texture_MapIcon cobblestone;
	Texture_MapIcon grass_side;
	Texture_MapIcon grass_top;
	Texture_MapIcon stonebrick;
	Texture_MapIcon sand;
	Texture_MapIcon oaklog_side;
	Texture_MapIcon oaklog_top;
	Texture_MapIcon leaves_oak;
	Texture_MapIcon glass;
	Texture_MapIcon brick;
	Texture_MapIcon oakplanks;
	Texture_MapIcon wool;
	Texture_MapIcon bedrock;
	Texture_MapIcon gravel;
	Texture_MapIcon coarse;
	Texture_MapIcon door_top;
	Texture_MapIcon door_bottom;
	Texture_MapIcon snow_grass_side;
	Texture_MapIcon snow;
	Texture_MapIcon obsidian;
	Texture_MapIcon netherrack;
	Texture_MapIcon sandstone_side;
	Texture_MapIcon sandstone_top;
	Texture_MapIcon sandstone_bottom;
	Texture_MapIcon smooth_stone;
	Texture_MapIcon grass_path_side;
	Texture_MapIcon grass_path_top;
	Texture_MapIcon crafting_table_side;
	Texture_MapIcon crafting_table_top;
	Texture_MapIcon lava;
	Texture_MapIcon water;
	Texture_MapIcon iron_ore;
	Texture_MapIcon iron_block;
	Texture_MapIcon gold_block;
	Texture_MapIcon gold_ore;
	Texture_MapIcon diamond_ore;
	Texture_MapIcon diamond_block;
	Texture_MapIcon emerald_block;
	Texture_MapIcon emerald_ore;
	Texture_MapIcon coal_block;
	Texture_MapIcon coal_ore;
	Texture_MapIcon furnace_front;
	Texture_MapIcon furnace_side;
	Texture_MapIcon furnace_top;
} icon;

void Block_Init() {
	Texture_MapInit(&textureMap, blockTextures, sizeof(blockTextures) / sizeof(blockTextures[0]));

    icon.stone               = Texture_MapGetIcon(&textureMap, blockTextures[0]);
    icon.dirt                = Texture_MapGetIcon(&textureMap, blockTextures[1]);
    icon.cobblestone         = Texture_MapGetIcon(&textureMap, blockTextures[2]);
    icon.grass_side          = Texture_MapGetIcon(&textureMap, blockTextures[3]);
    icon.grass_top           = Texture_MapGetIcon(&textureMap, blockTextures[4]);
    icon.stonebrick          = Texture_MapGetIcon(&textureMap, blockTextures[5]);
    icon.sand                = Texture_MapGetIcon(&textureMap, blockTextures[6]);
    icon.oaklog_side         = Texture_MapGetIcon(&textureMap, blockTextures[7]);
    icon.oaklog_top          = Texture_MapGetIcon(&textureMap, blockTextures[8]);
    icon.leaves_oak          = Texture_MapGetIcon(&textureMap, blockTextures[9]);
    icon.glass               = Texture_MapGetIcon(&textureMap, blockTextures[10]);
    icon.brick               = Texture_MapGetIcon(&textureMap, blockTextures[11]);
    icon.oakplanks           = Texture_MapGetIcon(&textureMap, blockTextures[12]);
    icon.wool                = Texture_MapGetIcon(&textureMap, blockTextures[13]);
    icon.bedrock             = Texture_MapGetIcon(&textureMap, blockTextures[14]);
    icon.gravel              = Texture_MapGetIcon(&textureMap, blockTextures[15]);
    icon.coarse              = Texture_MapGetIcon(&textureMap, blockTextures[16]);
    icon.door_top            = Texture_MapGetIcon(&textureMap, blockTextures[17]);
    icon.door_bottom         = Texture_MapGetIcon(&textureMap, blockTextures[18]);
    icon.snow_grass_side     = Texture_MapGetIcon(&textureMap, blockTextures[19]);
    icon.snow                = Texture_MapGetIcon(&textureMap, blockTextures[20]);
    icon.obsidian            = Texture_MapGetIcon(&textureMap, blockTextures[21]);
    icon.sandstone_side      = Texture_MapGetIcon(&textureMap, blockTextures[22]);
    icon.sandstone_top       = Texture_MapGetIcon(&textureMap, blockTextures[23]);
    icon.sandstone_bottom    = Texture_MapGetIcon(&textureMap, blockTextures[24]);
    icon.netherrack          = Texture_MapGetIcon(&textureMap, blockTextures[25]);
    icon.smooth_stone        = Texture_MapGetIcon(&textureMap, blockTextures[26]);
    icon.lava                = Texture_MapGetIcon(&textureMap, blockTextures[27]);
    icon.water               = Texture_MapGetIcon(&textureMap, blockTextures[28]);
    icon.grass_path_side     = Texture_MapGetIcon(&textureMap, blockTextures[29]);
    icon.grass_path_top      = Texture_MapGetIcon(&textureMap, blockTextures[30]);
    icon.crafting_table_side = Texture_MapGetIcon(&textureMap, blockTextures[31]);
    icon.crafting_table_top  = Texture_MapGetIcon(&textureMap, blockTextures[32]);
    icon.iron_ore            = Texture_MapGetIcon(&textureMap, blockTextures[33]);
    icon.iron_block          = Texture_MapGetIcon(&textureMap, blockTextures[34]);
    icon.diamond_ore         = Texture_MapGetIcon(&textureMap, blockTextures[35]);
    icon.diamond_block       = Texture_MapGetIcon(&textureMap, blockTextures[36]);
    icon.gold_ore            = Texture_MapGetIcon(&textureMap, blockTextures[37]);
    icon.gold_block          = Texture_MapGetIcon(&textureMap, blockTextures[38]);
    icon.coal_ore            = Texture_MapGetIcon(&textureMap, blockTextures[39]);
    icon.coal_block          = Texture_MapGetIcon(&textureMap, blockTextures[40]);
    icon.emerald_ore         = Texture_MapGetIcon(&textureMap, blockTextures[41]);
    icon.emerald_block       = Texture_MapGetIcon(&textureMap, blockTextures[42]);
    icon.furnace_side        = Texture_MapGetIcon(&textureMap, blockTextures[43]);
    icon.furnace_front       = Texture_MapGetIcon(&textureMap, blockTextures[44]);
    icon.furnace_top         = Texture_MapGetIcon(&textureMap, blockTextures[45]);

}
void Block_Deinit() { C3D_TexDelete(&textureMap.texture); }

void* Block_GetTextureMap() { return &textureMap.texture; }

//TODO: Blocks are all blocks, not inventory blocks (slots), but for now this works...
void Block_GetTexture(mc::inventory::Slot block, Direction direction, int16_t* out_uv) {
	Texture_MapIcon i = {0, 0, 0};
	switch (block.GetItemId()) {
		case Block_Air:
			return;
		case Block_Dirt:
			i = icon.dirt;
			break;
		case Block_Stone:
			i = icon.stone;
			break;
		case Block_Grass:
			switch (direction) {
				case Direction_Top:
					i = icon.grass_top;
					break;
				case Direction_Bottom:
					i = icon.dirt;
					break;
				default:
					i = icon.grass_side;
					break;
			}
			break;
		case Block_Cobblestone:
			i = icon.cobblestone;
			break;
		case Block_Log:
			switch (direction) {
				case Direction_Bottom:
				case Direction_Top:
					i = icon.oaklog_top;
					break;
				default:
					i = icon.oaklog_side;
					break;
			}
			break;
		case Block_Gravel:
			i = icon.gravel;
			break;
		case Block_Sand:
			i = icon.sand;
			break;
		case Block_Leaves:
			i = icon.leaves_oak;
			break;
		case Block_Glass:
			i = icon.glass;
			break;
		case Block_Stonebrick:
			i = icon.stonebrick;
			break;
		case Block_Brick:
			i = icon.brick;
			break;
		case Block_Planks:
			i = icon.oakplanks;
			break;
		case Block_Wool:
			i = icon.wool;
			break;
		case Block_Bedrock:
			i = icon.bedrock;
			break;
		case Block_Coarse:
			i = icon.coarse;
			break;
		case Block_Door_Top:
			i = icon.door_top;
			break;
		case Block_Door_Bottom:
			i = icon.door_bottom;
			break;
		case Block_Snow_Grass:
			switch (direction) {
				case Direction_Top:
					i = icon.snow;
					break;
				case Direction_Bottom:
					i = icon.dirt;
					break;
				default:
					i = icon.snow_grass_side;
					break;
			}
			break;
		case Block_Snow:
			i = icon.snow;
			break;
		case Block_Obsidian:
			i = icon.obsidian;
			break;
		case Block_Netherrack:
			i = icon.netherrack;
			break;
		case Block_Sandstone:
			switch (direction) {
				case Direction_Bottom:
					i = icon.sandstone_bottom;
					break;
				case Direction_Top:
					i = icon.sandstone_top;
					break;
				default:
					i = icon.sandstone_side;
					break;
			}
			break;
		case Block_Smooth_Stone:
			i = icon.smooth_stone;
			break;
		case Block_Crafting_Table:
			switch (direction) {
				case Direction_Bottom:
					i = icon.oakplanks;
					break;
				case Direction_Top:
					i = icon.crafting_table_top;
					break;
				default:
					i = icon.crafting_table_side;
					break;
			}
			break;
		case Block_Lava:
			i=icon.lava;
			break;
		case Block_Water:
			i=icon.water;
			break;
		case Block_Grass_Path:
			switch (direction) {
				case Direction_Bottom:
					i = icon.dirt;
					break;
				case Direction_Top:
					i = icon.grass_path_top;
					break;
				default:
					i = icon.grass_path_side;
					break;
			}
			break;
		case Block_Gold_Block:
			i=icon.gold_block;
			break;
		case Block_Gold_Ore:
			i=icon.gold_ore;
			break;
		case Block_Coal_Block:
			i=icon.coal_block;
			break;
		case Block_Coal_Ore:
			i=icon.coal_ore;
			break;
		case Block_Iron_Block:
			i=icon.iron_block;
			break;
		case Block_Iron_Ore:
			i=icon.iron_ore;
			break;
		case Block_Diamond_Block:
			i=icon.diamond_block;
			break;
		case Block_Diamond_Ore:
			i=icon.diamond_ore;
			break;
		case Block_Emerald_Block:
			i=icon.emerald_block;
			break;
		case Block_Emerald_Ore:
			i=icon.emerald_ore;
			break;
		case Block_Furnace:
			switch (direction) {
				case Direction_South:
					i = icon.furnace_front;
					break;
				case Direction_Top:
					i=icon.furnace_top;
					break;
				default:
					i = icon.furnace_side;
					break;
			}
			break;
		default: break;
	}
	out_uv[0] = i.u;
	out_uv[1] = i.v;
}

#define extractR(c) ((c >> 16) & 0xff)
#define extractG(c) (((c) >> 8) & 0xff)
#define extractB(c) ((c)&0xff)

void Block_GetColor(Block block, uint8_t metadata, Direction direction, uint8_t out_rgb[]) {
	if ((block == Block_Grass && direction == Direction_Top) || block == Block_Leaves) {
		out_rgb[0] = 140;
		out_rgb[1] = 214;
		out_rgb[2] = 123;
		return;
	}
	// white, orange, magenta, light blue, yellow, lime, pink, gray, silver, cyan, purple, blue, green, red, black
	const uint32_t dies[] = {(16777215), (14188339), (11685080), (6724056), (15066419), (8375321), (15892389), (5000268),
				 (10066329), (5013401),  (8339378),  (3361970), (6704179),  (6717235), (10040115), (1644825)};
	if (block == Block_Wool) {
		out_rgb[0] = extractR(dies[metadata]);
		out_rgb[1] = extractG(dies[metadata]);
		out_rgb[2] = extractB(dies[metadata]);
	} else {
		out_rgb[0] = 255;
		out_rgb[1] = 255;
		out_rgb[2] = 255;
	}
}

bool Block_Opaque(Block block, uint8_t metadata) { return block != Block_Air && block != Block_Glass && block != Block_Door_Top && block != Block_Door_Bottom; }
