#include "blocks/Block.hpp"
#include "blocks/GrassBlock.h"
#include "blocks/LeavesBlock.h"
#include "rendering/VertexFmt.hpp"

const char* blockTextures[] = {"stone.png",
                               "dirt.png",
                               "cobblestone.png",
                               "grass_side.png",
                               "grass_top.png",
                               "stonebrick.png",
                               "sand.png",
                               "log_oak.png",
                               "log_oak_top.png",
                               "leaves_oak.png",
                               "glass.png",
                               "brick.png",
                               "redstone_torch_off.png",
                               "redstone_torch_on.png",
                               "planks_oak.png",
                               "sapling_oak.png",

                               "wool_colored_black.png",
                               "wool_colored_blue.png",
                               "wool_colored_brown.png",
                               "wool_colored_cyan.png",
                               "wool_colored_gray.png",
                               "wool_colored_green.png",
                               "wool_colored_light_blue.png",
                               "wool_colored_lime.png",
                               "wool_colored_magenta.png",
                               "wool_colored_orange.png",
                               "wool_colored_pink.png",
                               "wool_colored_purple.png",
                               "wool_colored_red.png",
                               "wool_colored_silver.png",
                               "wool_colored_white.png",
                               "wool_colored_yellow.png",

                               "bedrock.png",
                               "web.png",
                               "mob_spawner.png",
                               "torch_on.png",
                               "cobblestone_mossy.png",
                               "bookshelf.png",
                               "mushroom_brown.png",
                               "red.png",
                               "ladder.png",
                               "flower_rose.png",
                               "flower_dandelion.png",
                               "deadbush.png",
                               "tallgrass.png",
                               "rail_golden.png",
                               "rail_detector.png",
                               "rail_normal.png",

                               "piston_bottom.png",
                               "piston_side.png",
                               "piston_top_normal.png",
                               "piston_top_sticky.png",

                               "tnt_bottom.png",
                               "tnt_side.png",
                               "tnt_top.png",

                               "gravel.png",
                               "coarse_dirt.png",
                               "door_top.png",
                               "door_bottom.png",
                               "grass_side_snowed.png",
                               "snow.png",
                               "obsidian.png",
                               "sandstone_normal.png",
                               "sandstone_top.png",
                               "sandstone_bottom.png",
                               "sponge.png",
                               "netherrack.png",
                               "noteblock.png",
                               "smooth_stone.png",
                               "lava.png",
                               "water.png",
                               "grass_path_side.png",
                               "grass_path_top.png",
                               "crafting_table_side.png",
                               "crafting_table_top.png",
                               "iron_ore.png",
                               "redstone_ore.png",
                               "redstone_ore_turned.png",
                               "iron_block.png",
                               "diamond_ore.png",
                               "diamond_block.png",
                               "gold_ore.png",
                               "gold_block.png",
                               "lapis_ore.png",
                               "lapis_block.png",
                               "coal_ore.png",
                               "coal_block.png",
                               "emerald_ore.png",
                               "emerald_block.png",
                               "furnace_side.png",
                               "furnace_front_off.png",
                               "furnace_front_on.png",
                               "dispenser_front_horizontal.png",
                               "furnace_top.png"};

TextureMap BlockRegistry::m_textureMap;

BlockRegistry::BlockRegistry() {
    m_textureMap.Init("romfs:/assets/minecraft/textures/blocks");

#define GETICON(x) m_textureMap.Get(x ".png")

    RegisterBlock((new Block(BlockID::Air, "air"))
                      ->setAllSidesTexture(0, 0)
                      ->setOpaque(false)
                      ->setSolid(false)
                      ->setDestroyTime(0.0f));

    RegisterBlock((new Block(BlockID::Stone, "stone"))
                      ->setAllSidesTexture(GETICON("stone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));

    RegisterBlock((new GrassBlock(BlockID::Grass, "grass"))
                      ->setTopBottomSidesTexture(GETICON("grass_top"), // top (grass_top)
                                                 GETICON("dirt"), // bottom (dirt)
                                                 GETICON("grass_side") // sides (grass_side)
                                                 )
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock((new Block(BlockID::Dirt, "dirt"))
                      ->setAllSidesTexture(GETICON("dirt"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Cobblestone, "cobblestone"))
                      ->setAllSidesTexture(GETICON("cobblestone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Planks, "planks"))
                      ->setAllSidesTexture(GETICON("planks_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Sapling, "sapling"))
                      ->setAllSidesTexture(GETICON("sapling_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));
    // TODO BEGIN
    RegisterBlock((new Block(BlockID::Flowing_Water, "flowing_water"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Water, "water"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Flowing_Lava, "flowing_lava"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Lava, "lava"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO END
    RegisterBlock((new Block(BlockID::Sand, "sand"))
                      ->setAllSidesTexture(GETICON("sand"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Gravel, "gravel"))
                      ->setAllSidesTexture(GETICON("gravel"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Gold_Ore, "gold_ore"))
                      ->setAllSidesTexture(GETICON("gold_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Iron_Ore, "iron_ore"))
                      ->setAllSidesTexture(GETICON("iron_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Coal_Ore, "coal_ore"))
                      ->setAllSidesTexture(GETICON("coal_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

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

    RegisterBlock((new LeavesBlock(BlockID::Sponge, "sponge"))
                      ->setAllSidesTexture(GETICON("sponge"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.2f));

    RegisterBlock((new Block(BlockID::Glass, "glass"))
                      ->setAllSidesTexture(GETICON("glass"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock((new Block(BlockID::Lapis_Ore, "lapis_ore"))
                      ->setAllSidesTexture(GETICON("lapis_ore"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock((new Block(BlockID::Lapis_Block, "lapis_block"))
                      ->setAllSidesTexture(GETICON("lapis_block"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock((new Block(BlockID::Dispenser, "dispenser"))
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("dispenser_front_horizontal"), GETICON("furnace_side"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock((new Block(BlockID::Sandstone, "sandstone"))
                      ->setTopBottomSidesTexture(GETICON("sandstone_top"), // top
                                                 GETICON("sandstone_bottom"), // bottom
                                                 GETICON("sandstone_normal") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Noteblock, "noteblock"))
                      ->setAllSidesTexture(GETICON("noteblock"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock((new Block(BlockID::Bed, "bed"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Golden_Rail, "rail_golden"))
                      ->setAllSidesTexture(GETICON("rail_golden"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Detector_Rail, "rail_detector"))
                      ->setAllSidesTexture(GETICON("rail_detector"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Sticky_Piston, "sticky_piston"))
                      ->setTopBottomSidesTexture(GETICON("piston_top_sticky"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Web, "web"))
                      ->setAllSidesTexture(GETICON("web"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Tallgrass, "tallgrass"))
                      ->setAllSidesTexture(GETICON("tallgrass"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Deadbush, "deadbush"))
                      ->setAllSidesTexture(GETICON("deadbush"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Piston, "piston"))
                      ->setTopBottomSidesTexture(GETICON("piston_top_normal"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Piston_Head, "piston_head"))
                      ->setAllSidesTexture(GETICON("piston_top_normal"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Wool, "wool"))
                      ->setAllSidesTexture(GETICON("wool_colored_white"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));
    // TODO: What even is this?
    RegisterBlock((new Block(BlockID::Piston_Extension, "piston_extension"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Yellow_Flower, "yellow_flower"))
                      ->setAllSidesTexture(GETICON("flower_dandelion"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Red_Flower, "red_flower"))
                      ->setAllSidesTexture(GETICON("flower_rose"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Brown_Mushroom, "brown_mushroom"))
                      ->setAllSidesTexture(GETICON("mushroom_brown"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Red_Mushroom, "red_mushroom"))
                      ->setAllSidesTexture(GETICON("mushroom_red"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Gold_Block, "gold_block"))
                      ->setAllSidesTexture(GETICON("gold_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Iron_Block, "iron_block"))
                      ->setAllSidesTexture(GETICON("iron_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO:
    RegisterBlock((new Block(BlockID::Double_Stone_Slab, "double_stone_slab"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO:
    RegisterBlock((new Block(BlockID::Stone_Slab, "stone_slab"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Brick_Block, "brick_block"))
                      ->setAllSidesTexture(GETICON("brick"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Tnt, "tnt"))
                      ->setTopBottomSidesTexture(GETICON("tnt_top"), // top
                                                 GETICON("tnt_bottom"), // bottom
                                                 GETICON("tnt_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Bookshelf, "bookshelf"))
                      ->setTopBottomSidesTexture(GETICON("planks_oak"), // top
                                                 GETICON("planks_oak"), // bottom
                                                 GETICON("bookshelf") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock((new Block(BlockID::Mossy_Cobblestone, "mossy_cobblestone"))
                      ->setAllSidesTexture(GETICON("cobblestone_mossy"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Obsidian, "obsidian"))
                      ->setAllSidesTexture(GETICON("obsidian"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Torch, "torch"))
                      ->setAllSidesTexture(GETICON("torch_on"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock((new Block(BlockID::Fire, "fire"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Mob_Spawner, "mob_spawner"))
                      ->setAllSidesTexture(GETICON("mob_spawner"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock((new Block(BlockID::Oak_Stairs, "oak_stairs"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO: Chest has an animated model, meaning it is hardcoded, not in a json
    RegisterBlock((new Block(BlockID::Chest, "chest"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Redstone_Wire, "redstone_wire"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Diamond_Ore, "diamond_ore"))
                      ->setAllSidesTexture(GETICON("diamond_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Diamond_Block, "diamond_block"))
                      ->setAllSidesTexture(GETICON("diamond_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Crafting_Table, "crafting_table"))
                      ->setTopBottomSidesTexture(GETICON("crafting_table_top"), GETICON("planks_oak"),
                                                 GETICON("crafting_table_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO: wheat has stages of growth
    RegisterBlock((new Block(BlockID::Wheat, "wheat"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    // TODO: Farmland has a dynamic texture, which is determined by its moisture
    // level.
    RegisterBlock((new Block(BlockID::Farmland, "farmland"))
                  //->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  //                           GETICON("planks_oak"),
                  //                           GETICON("crafting_table_side"))
                  //->setSoundType(SoundType::STONE)
                  //->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Furnace, "furnace"))
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_off"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Lit_Furnace, "lit_furnace"))
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_on"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO: Yea...
    RegisterBlock((new Block(BlockID::Standing_Sign, "standing_sign"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );
    // TODO:
    RegisterBlock((new Block(BlockID::Wooden_Door, "wooden_door"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Ladder, "ladder"))
                      ->setAllSidesTexture(GETICON("ladder"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));

    RegisterBlock((new Block(BlockID::Rail, "rail"))
                      ->setAllSidesTexture(GETICON("rail_normal"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));
    // TODO:
    RegisterBlock((new Block(BlockID::Stone_Stairs, "stone_stairs"))
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock((new Block(BlockID::Wall_Sign, "wall_sign"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Lever, "lever"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Stone_Pressure_Plate, "stone_pressure_plate"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Iron_Door, "iron_door"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Wooden_Pressure_Plate, "wooden_pressure_plate"))
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock((new Block(BlockID::Redstone_Ore, "redstone_ore"))
                      ->setAllSidesTexture(GETICON("redstone_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Lit_Redstone_Ore, "lit_redstone_ore"))
                      ->setAllSidesTexture(GETICON("redstone_ore_turned"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Unlit_Redstone_Torch, "unlit_redstone_torch"))
                      ->setAllSidesTexture(GETICON("redstone_torch_off"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Redstone_Torch, "redstone_torch"))
                      ->setAllSidesTexture(GETICON("redstone_torch_on"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Bedrock, "bedrock"))
                      ->setAllSidesTexture(GETICON("bedrock"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(-1.0f) // Unbreakable
    );

    /*RegisterBlock((new Block(BlockID::Smooth_Stone, "smooth_stone"))
                      ->setAllSidesTexture(GETICON("smooth_stone"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
                      Doesnt exist in 1.12 for some reason.
  */

    RegisterBlock((new Block(BlockID::Coal_Block, "coal_block"))
                      ->setAllSidesTexture(GETICON("coal_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Stone_Button, "")));
    RegisterBlock((new Block(BlockID::Snow_Layer, "")));
    RegisterBlock((new Block(BlockID::Ice, "")));

    RegisterBlock((new Block(BlockID::Snow, "snow"))
                      ->setAllSidesTexture(GETICON("snow"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock((new Block(BlockID::Cactus, "")));
    RegisterBlock((new Block(BlockID::Clay, "")));
    RegisterBlock((new Block(BlockID::Reeds, "")));
    RegisterBlock((new Block(BlockID::Jukebox, "")));
    RegisterBlock((new Block(BlockID::Fence, "")));
    RegisterBlock((new Block(BlockID::Pumpkin, "")));

    RegisterBlock((new Block(BlockID::Netherrack, "netherrack"))
                      ->setAllSidesTexture(GETICON("netherrack"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Soul_Sand, "")));
    RegisterBlock((new Block(BlockID::Glowstone, "")));
    RegisterBlock((new Block(BlockID::Portal, "")));
    RegisterBlock((new Block(BlockID::Lit_Pumpkin, "")));
    RegisterBlock((new Block(BlockID::Cake, "")));
    RegisterBlock((new Block(BlockID::Unpowered_Repeater, "")));
    RegisterBlock((new Block(BlockID::Powered_Repeater, "")));
    RegisterBlock((new Block(BlockID::Stained_Glass, "")));
    RegisterBlock((new Block(BlockID::Trapdoor, "")));
    RegisterBlock((new Block(BlockID::Monster_Egg, "")));

    RegisterBlock((new Block(BlockID::Stonebrick, "")));

    RegisterBlock((new Block(BlockID::Brown_Mushroom_Block, "")));
    RegisterBlock((new Block(BlockID::Red_Mushroom_Block, "")));
    RegisterBlock((new Block(BlockID::Iron_Bars, "")));
    RegisterBlock((new Block(BlockID::Glass_Pane, "")));
    RegisterBlock((new Block(BlockID::Melon_Block, "")));
    RegisterBlock((new Block(BlockID::Pumpkin_Stem, "")));
    RegisterBlock((new Block(BlockID::Melon_Stem, "")));
    RegisterBlock((new Block(BlockID::Vine, "")));
    RegisterBlock((new Block(BlockID::Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Brick_Stairs, "")));
    RegisterBlock((new Block(BlockID::Stone_Brick_Stairs, "")));
    RegisterBlock((new Block(BlockID::Mycelium, "")));
    RegisterBlock((new Block(BlockID::Waterlily, "")));
    RegisterBlock((new Block(BlockID::Nether_Brick, "")));
    RegisterBlock((new Block(BlockID::Nether_Brick_Fence, "")));
    RegisterBlock((new Block(BlockID::Nether_Brick_Stairs, "")));
    RegisterBlock((new Block(BlockID::Nether_Wart, "")));
    RegisterBlock((new Block(BlockID::Enchanting_Table, "")));
    RegisterBlock((new Block(BlockID::Brewing_Stand, "")));
    RegisterBlock((new Block(BlockID::Cauldron, "")));
    RegisterBlock((new Block(BlockID::End_Portal, "")));
    RegisterBlock((new Block(BlockID::End_Portal_Frame, "")));
    RegisterBlock((new Block(BlockID::End_Stone, "")));
    RegisterBlock((new Block(BlockID::Dragon_Egg, "")));
    RegisterBlock((new Block(BlockID::Redstone_Lamp, "")));
    RegisterBlock((new Block(BlockID::Lit_Redstone_Lamp, "")));
    RegisterBlock((new Block(BlockID::Double_Wooden_Slab, "")));
    RegisterBlock((new Block(BlockID::Wooden_Slab, "")));
    RegisterBlock((new Block(BlockID::Cocoa, "")));
    RegisterBlock((new Block(BlockID::Sandstone_Stairs, "")));

    RegisterBlock((new Block(BlockID::Emerald_Ore, "emerald_ore"))
                      ->setAllSidesTexture(GETICON("emerald_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Ender_Chest, "")));
    RegisterBlock((new Block(BlockID::Tripwire_Hook, "")));
    RegisterBlock((new Block(BlockID::Tripwire, "")));

    RegisterBlock((new Block(BlockID::Emerald_Block, "emerald_block"))
                      ->setAllSidesTexture(GETICON("emerald_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock((new Block(BlockID::Spruce_Stairs, "")));
    RegisterBlock((new Block(BlockID::Birch_Stairs, "")));
    RegisterBlock((new Block(BlockID::Jungle_Stairs, "")));
    RegisterBlock((new Block(BlockID::Command_Block, "")));
    RegisterBlock((new Block(BlockID::Beacon, "")));
    RegisterBlock((new Block(BlockID::Cobblestone_Wall, "")));
    RegisterBlock((new Block(BlockID::Flower_Pot, "")));
    RegisterBlock((new Block(BlockID::Carrots, "")));
    RegisterBlock((new Block(BlockID::Potatoes, "")));
    RegisterBlock((new Block(BlockID::Wooden_Button, "")));
    RegisterBlock((new Block(BlockID::Skull, "")));
    RegisterBlock((new Block(BlockID::Anvil, "")));
    RegisterBlock((new Block(BlockID::Trapped_Chest, "")));
    RegisterBlock((new Block(BlockID::Light_Weighted_Pressure_Plate, "")));
    RegisterBlock((new Block(BlockID::Heavy_Weighted_Pressure_Plate, "")));
    RegisterBlock((new Block(BlockID::Unpowered_Comparator, "")));
    RegisterBlock((new Block(BlockID::Powered_Comparator, "")));
    RegisterBlock((new Block(BlockID::Daylight_Detector, "")));
    RegisterBlock((new Block(BlockID::Redstone_Block, "")));
    RegisterBlock((new Block(BlockID::Quartz_Ore, "")));
    RegisterBlock((new Block(BlockID::Hopper, "")));
    RegisterBlock((new Block(BlockID::Quartz_Block, "")));
    RegisterBlock((new Block(BlockID::Quartz_Stairs, "")));
    RegisterBlock((new Block(BlockID::Activator_Rail, "")));
    RegisterBlock((new Block(BlockID::Dropper, "")));
    RegisterBlock((new Block(BlockID::Stained_Hardened_Clay, "")));
    RegisterBlock((new Block(BlockID::Stained_Glass_Pane, "")));
    RegisterBlock((new Block(BlockID::Leaves2, "")));
    RegisterBlock((new Block(BlockID::Log2, "")));
    RegisterBlock((new Block(BlockID::Acacia_Stairs, "")));
    RegisterBlock((new Block(BlockID::Dark_Oak_Stairs, "")));
    RegisterBlock((new Block(BlockID::Slime, "")));
    RegisterBlock((new Block(BlockID::Barrier, "")));
    RegisterBlock((new Block(BlockID::Iron_Trapdoor, "")));
    RegisterBlock((new Block(BlockID::Prismarine, "")));
    RegisterBlock((new Block(BlockID::Sea_Lantern, "")));
    RegisterBlock((new Block(BlockID::Hay_Block, "")));
    RegisterBlock((new Block(BlockID::Carpet, "")));
    RegisterBlock((new Block(BlockID::Hardened_Clay, "")));
    RegisterBlock((new Block(BlockID::Packed_Ice, "")));
    RegisterBlock((new Block(BlockID::Double_Plant, "")));
    RegisterBlock((new Block(BlockID::Standing_Banner, "")));
    RegisterBlock((new Block(BlockID::Wall_Banner, "")));
    RegisterBlock((new Block(BlockID::Daylight_Detector_Inverted, "")));
    RegisterBlock((new Block(BlockID::Red_Sandstone, "")));
    RegisterBlock((new Block(BlockID::Red_Sandstone_Stairs, "")));
    RegisterBlock((new Block(BlockID::Double_Stone_Slab2, "")));
    RegisterBlock((new Block(BlockID::Stone_Slab2, "")));
    RegisterBlock((new Block(BlockID::Spruce_Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Birch_Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Jungle_Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Dark_Oak_Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Acacia_Fence_Gate, "")));
    RegisterBlock((new Block(BlockID::Spruce_Fence, "")));
    RegisterBlock((new Block(BlockID::Birch_Fence, "")));
    RegisterBlock((new Block(BlockID::Jungle_Fence, "")));
    RegisterBlock((new Block(BlockID::Dark_Oak_Fence, "")));
    RegisterBlock((new Block(BlockID::Acacia_Fence, "")));
    RegisterBlock((new Block(BlockID::Spruce_Door, "")));
    RegisterBlock((new Block(BlockID::Birch_Door, "")));
    RegisterBlock((new Block(BlockID::Jungle_Door, "")));
    RegisterBlock((new Block(BlockID::Acacia_Door, "")));
    RegisterBlock((new Block(BlockID::Dark_Oak_Door, "")));
    RegisterBlock((new Block(BlockID::End_Rod, "")));
    RegisterBlock((new Block(BlockID::Chorus_Plant, "")));
    RegisterBlock((new Block(BlockID::Chorus_Flower, "")));
    RegisterBlock((new Block(BlockID::Purpur_Block, "")));
    RegisterBlock((new Block(BlockID::Purpur_Pillar, "")));
    RegisterBlock((new Block(BlockID::Purpur_Stairs, "")));
    RegisterBlock((new Block(BlockID::Purpur_Double_Slab, "")));
    RegisterBlock((new Block(BlockID::Purpur_Slab, "")));
    RegisterBlock((new Block(BlockID::End_Bricks, "")));
    RegisterBlock((new Block(BlockID::Beetroots, "")));

    RegisterBlock((new Block(BlockID::Grass_Path, "grass_path"))
                      ->setTopBottomSidesTexture(GETICON("grass_path_top"), GETICON("dirt"), GETICON("grass_path_side"))
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock((new Block(BlockID::End_Gateway, "")));
    RegisterBlock((new Block(BlockID::Repeating_Command_Block, "")));
    RegisterBlock((new Block(BlockID::Chain_Command_Block, "")));
    RegisterBlock((new Block(BlockID::Frosted_Ice, "")));
    RegisterBlock((new Block(BlockID::Magma, "")));
    RegisterBlock((new Block(BlockID::Nether_Wart_Block, "")));
    RegisterBlock((new Block(BlockID::Red_Nether_Brick, "")));
    RegisterBlock((new Block(BlockID::Bone_Block, "")));
    RegisterBlock((new Block(BlockID::Structure_Void, "")));
    RegisterBlock((new Block(BlockID::Observer, "")));
    RegisterBlock((new Block(BlockID::White_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Orange_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Magenta_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Light_Blue_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Yellow_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Lime_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Pink_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Gray_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Silver_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Cyan_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Purple_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Blue_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Brown_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Green_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Red_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::Black_Shulker_Box, "")));
    RegisterBlock((new Block(BlockID::White_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Orange_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Magenta_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Light_Blue_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Yellow_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Lime_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Pink_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Gray_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Silver_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Cyan_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Purple_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Blue_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Brown_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Green_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Red_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Black_Glazed_Terracotta, "")));
    RegisterBlock((new Block(BlockID::Concrete, "")));
    RegisterBlock((new Block(BlockID::Concrete_Powder, "")));
    RegisterBlock((new Block(BlockID::Structure_Block, "")));

    m_blocks.resize(static_cast<size_t>(BlockID::Count));
}

BlockRegistry::~BlockRegistry() {}
