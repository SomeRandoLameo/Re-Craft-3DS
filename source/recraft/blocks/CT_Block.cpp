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

// This is not really a block initializer, rather it initializes Block Textures
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
// This is not really a block deinitializer, rather it deinitializes Block Textures
void Block_Deinit() { C3D_TexDelete(&textureMap.texture); }

void* Block_GetTextureMap() { return &textureMap.texture; }

//TODO: find a smarter solution
void Block_GetTexture(mc::block::BlockPtr block, Direction direction, int16_t* out_uv) {
	Texture_MapIcon i = {0, 0, 0};
    auto name = block->GetName();

    if(name == "minecraft:air") return;
    if(name == "minecraft:dirt") i = icon.dirt;
    if(name == "minecraft:stone") i = icon.stone;
    if(name == "minecraft:grass_block") {
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
    }
    if(name == "minecraft:cobblestone") i = icon.cobblestone;
    if(name == "minecraft:oak_log") {
        switch (direction) {
        case Direction_Bottom:
        case Direction_Top:
            i = icon.oaklog_top;
            break;
        default:
            i = icon.oaklog_side;
            break;
        }
    }
    if(name == "minecraft:gravel") i = icon.gravel;
    if(name == "minecraft:sand") i = icon.sand;
    if(name == "minecraft:oak_leaves") i = icon.leaves_oak;
    if(name == "minecraft:glass") i = icon.glass;
    if(name == "minecraft:stone_bricks") i = icon.stonebrick;
    if(name == "minecraft:bricks") i = icon.brick;
    if(name == "minecraft:oak_planks") i = icon.oakplanks;

    // TODO: wool in craftus is metadata driven, so same texture for now
    if(name == "minecraft:white_wool") i = icon.wool;
    if(name == "minecraft:orange_wool") i = icon.wool;
    if(name == "minecraft:magenta_wool") i = icon.wool;
    if(name == "minecraft:light_blue_wool") i = icon.wool;
    if(name == "minecraft:yellow_wool") i = icon.wool;
    if(name == "minecraft:lime_wool") i = icon.wool;
    if(name == "minecraft:pink_wool") i = icon.wool;
    if(name == "minecraft:gray_wool") i = icon.wool;
    if(name == "minecraft:light_gray_wool") i = icon.wool;
    if(name == "minecraft:cyan_wool") i = icon.wool;
    if(name == "minecraft:purple_wool") i = icon.wool;
    if(name == "minecraft:blue_wool") i = icon.wool;
    if(name == "minecraft:brown_wool") i = icon.wool;
    if(name == "minecraft:green_wool") i = icon.wool;
    if(name == "minecraft:red_wool") i = icon.wool;
    if(name == "minecraft:black_wool") i = icon.wool;

    if(name == "minecraft:bedrock") i = icon.bedrock;
    if(name == "minecraft:coarse_dirt") i = icon.coarse;

    /* TODO: How the hell do i represend top & bottom doors, if only full doors exist...
    //	case Block_Door_Top:
    //		i = icon.door_top;
    //		break;
    //	case Block_Door_Bottom:
    //		i = icon.door_bottom;
    //		break;
    */

    /* TODO: Block_Snow_Grass is a metadata thingy of grass_block. i cannot represent it, thus it doesnt exist for now...
    //  case Block_Snow_Grass:
    //      switch (direction) {
    //      case Direction_Top:
    //          i = icon.snow;
    //          break;
    //      case Direction_Bottom:
    //          i = icon.dirt;
    //          break;
    //      default:
    //          i = icon.snow_grass_side;
    //          break;
    //      }
    //      break;
    */

    if(name == "minecraft:snow_block") i = icon.snow;
    if(name == "minecraft:obsidian") i = icon.obsidian;
    if(name == "minecraft:netherrack") i = icon.netherrack;
    if(name == "minecraft:sandstone") {
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
    }
    if(name == "minecraft:smooth_sandstone") i = icon.smooth_stone;
    if(name == "minecraft:crafting_table") {
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
    }
    if(name == "minecraft:lava") i=icon.lava;
    if(name == "minecraft:water") i=icon.water;
    if(name == "minecraft:grass_path") {
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
    }
    if(name == "minecraft:gold_block") i=icon.gold_block;
    if(name == "minecraft:gold_ore") i=icon.gold_ore;
    if(name == "minecraft:coal_block") i=icon.coal_block;
    if(name == "minecraft:coal_ore") i=icon.coal_ore;
    if(name == "minecraft:iron_block") i=icon.iron_block;
    if(name == "minecraft:iron_ore") i=icon.iron_ore;
    if(name == "minecraft:diamond_block") i=icon.diamond_block;
    if(name == "minecraft:diamond_ore") i=icon.diamond_ore;
    if(name == "minecraft:emerald_block") i=icon.emerald_block;
    if(name == "minecraft:emerald_ore") i=icon.emerald_ore;
    if(name == "minecraft:furnace") {
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
    };

    //TODO: Add missing texture for default

	out_uv[0] = i.u;
	out_uv[1] = i.v;
}

void Block_GetColor(mc::block::BlockPtr block, uint8_t metadata, Direction direction, uint8_t out_rgb[]) {
    /**
     * TODO: Rewrite to precompiled hash table (performenc)
     *
    if ((block == Block_Grass && direction == Direction_Top) || block == Block_Leaves) {
		out_rgb[0] = 140;
		out_rgb[1] = 214;
		out_rgb[2] = 123;
		return;
	}
	//  purple, blue, green, red, black
	const uint32_t dies[] = {, , , , , , , ,
				 , ,  ,  , ,  , , };

	    "minecraft:white_wool",         (16777215)
        "minecraft:orange_wool",        (14188339)
        "minecraft:magenta_wool",       (11685080)
        "minecraft:light_blue_wool",    (6724056)
        "minecraft:yellow_wool",        (15066419)
        "minecraft:lime_wool",          (8375321)
        "minecraft:pink_wool",          (15892389)
        "minecraft:gray_wool",          (5000268)
        "minecraft:light_gray_wool",    (10066329)
        "minecraft:cyan_wool",          (5013401)
        "minecraft:purple_wool",        (8339378)
        "minecraft:blue_wool",          (3361970)
        "minecraft:brown_wool",         (6704179)
        "minecraft:green_wool",         (6717235)
        "minecraft:red_wool",           (10040115)
        "minecraft:black_wool",         (1644825)

        Use the flattened blocks instead of:
    if (block->GetName() == Block_Wool) {
		out_rgb[0] = ((dies[metadata] >> 16) & 0xff);
		out_rgb[1] = (((dies[metadata]) >> 8) & 0xff);
		out_rgb[2] = ((dies[metadata]) & 0xff);
	} else {
		out_rgb[0] = 255;
		out_rgb[1] = 255;
		out_rgb[2] = 255;
	}*/
}

