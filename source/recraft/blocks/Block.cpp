#include "blocks/Block.hpp"
#include "blocks/GrassBlock.h"
#include "blocks/LeavesBlock.h"
#include "blocks/WoolBlock.h"
#include "rendering/VertexFmt.hpp"

// PATH PREFIX
#define PPRX "romfs:/assets/textures/blocks/"

const char* blockTextures[] = {"romfs:/assets/notexture.png",
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
                               PPRX "furnace_top.png"};

TextureMap BlockRegistry::m_textureMap;


BlockRegistry::BlockRegistry() {
    m_textureMap.Init(blockTextures, sizeof(blockTextures) / sizeof(blockTextures[0]));
#define GETICON(x) m_textureMap.Get(PPRX x ".png")

    RegisterBlock((new Block(BlockID::Air, "air"))
                      ->setAllSidesTexture(0, 0)
                      ->setOpaque(false)
                      ->setSolid(false)
                      ->setDestroyTime(0.0f));

    RegisterBlock((new Block(BlockID::Stone, "stone"))
                      ->setAllSidesTexture(GETICON("stone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));

    RegisterBlock((new Block(BlockID::Dirt, "dirt"))
                      ->setAllSidesTexture(GETICON("dirt"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Cobblestone, "cobblestone"))
                      ->setAllSidesTexture(GETICON("cobblestone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new GrassBlock(BlockID::Grass, "grass"))
                      ->setTopBottomSidesTexture(GETICON("grass_top"), // top (grass_top)
                                                 GETICON("dirt"), // bottom (dirt)
                                                 GETICON("grass_side") // sides (grass_side)
                                                 )
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock((new Block(BlockID::Stonebrick, "stonebrick"))
                      ->setAllSidesTexture(GETICON("stonebrick"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Sand, "sand"))
                      ->setAllSidesTexture(GETICON("sand"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Log, "log"))
                      ->setTopBottomSidesTexture(GETICON("log_oak_top"), // top (oaklog_top)
                                                 GETICON("log_oak_top"), // bottom (oaklog_top)
                                                 GETICON("log_oak") // sides (oaklog_side)
                                                 )
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new LeavesBlock(BlockID::Leaves, "leaves"))
                      ->setAllSidesTexture(GETICON("leaves_oak"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.2f));

    RegisterBlock((new Block(BlockID::Glass, "glass"))
                      ->setAllSidesTexture(GETICON("glass"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock((new Block(BlockID::Brick, "brick"))
                      ->setAllSidesTexture(GETICON("brick"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Planks, "planks"))
                      ->setAllSidesTexture(GETICON("planks_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new WoolBlock(BlockID::Wool, "wool"))
                      ->setAllSidesTexture(GETICON("wool"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Bedrock, "bedrock"))
                      ->setAllSidesTexture(GETICON("bedrock"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(-1.0f) // Unbreakable
    );

    RegisterBlock((new Block(BlockID::Gravel, "gravel"))
                      ->setAllSidesTexture(GETICON("gravel"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Coarse, "coarse_dirt"))
                      ->setAllSidesTexture(GETICON("coarse_dirt"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    // TODO: Doors or multiblock structures
    // icon.door_top            = Texture_MapGetIcon(&textureMap, blockTextures[17]);
    // icon.door_bottom         = Texture_MapGetIcon(&textureMap, blockTextures[18]);


    RegisterBlock((new Block(BlockID::Snow_Grass, "snow_grass"))
                      ->setTopBottomSidesTexture(GETICON("snow"), // top
                                                 GETICON("dirt"), // bottom
                                                 GETICON("snow_grass_side") // sides
                                                 )
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock((new Block(BlockID::Snow, "snow"))
                      ->setAllSidesTexture(GETICON("snow"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Obsidian, "obsidian"))
                      ->setAllSidesTexture(GETICON("obsidian"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Sandstone, "sandstone"))
                      ->setTopBottomSidesTexture(GETICON("sandstone_top"), // top
                                                 GETICON("sandstone_bottom"), // bottom
                                                 GETICON("sandstone_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Netherrack, "netherrack"))
                      ->setAllSidesTexture(GETICON("netherrack"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Smooth_Stone, "smooth_stone"))
                      ->setAllSidesTexture(GETICON("smooth_stone"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Lava, "lava"))
                      ->setAllSidesTexture(GETICON("lava"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Water, "water"))
                      ->setAllSidesTexture(GETICON("water"))
                      ->setSoundType(SoundType::STONE)
                      ->setOpaque(false)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Grass_Path, "grass_path"))
                      ->setTopBottomSidesTexture(GETICON("grass_path_top"), GETICON("dirt"), GETICON("grass_path_side"))
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock((new Block(BlockID::Crafting_Table, "crafting_table"))
                      ->setTopBottomSidesTexture(GETICON("crafting_table_top"), GETICON("planks_oak"),
                                                 GETICON("crafting_table_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Iron_Ore, "iron_ore"))
                      ->setAllSidesTexture(GETICON("iron_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Iron_Block, "iron_block"))
                      ->setAllSidesTexture(GETICON("iron_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Diamond_Ore, "diamond_ore"))
                      ->setAllSidesTexture(GETICON("diamond_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Diamond_Block, "diamond_block"))
                      ->setAllSidesTexture(GETICON("diamond_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Gold_Ore, "gold_ore"))
                      ->setAllSidesTexture(GETICON("gold_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Gold_Block, "gold_block"))
                      ->setAllSidesTexture(GETICON("gold_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Coal_Ore, "coal_ore"))
                      ->setAllSidesTexture(GETICON("coal_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Coal_Block, "coal_block"))
                      ->setAllSidesTexture(GETICON("coal_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Emerald_Ore, "emerald_ore"))
                      ->setAllSidesTexture(GETICON("emerald_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Emerald_Block, "emerald_block"))
                      ->setAllSidesTexture(GETICON("emerald_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    // TODO: Propper furnace texture mapping
    RegisterBlock((new Block(BlockID::Furnace, "furnace"))
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));


    m_blocks.resize(static_cast<size_t>(BlockID::Count));
}

BlockRegistry::~BlockRegistry() {}
