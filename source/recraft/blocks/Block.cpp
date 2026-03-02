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

    RegisterBlock(BlockID::Air, "air", (new Block())
                      ->setAllSidesTexture(0, 0)
                      ->setOpaque(false)
                      ->setSolid(false)
                      ->setDestroyTime(0.0f));

    RegisterBlock(BlockID::Stone, "stone",(new Block())
                      ->setAllSidesTexture(GETICON("stone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));

    RegisterBlock(BlockID::Grass, "grass",(new GrassBlock())
                      ->setTopBottomSidesTexture(GETICON("grass_top"), // top (grass_top)
                                                 GETICON("dirt"), // bottom (dirt)
                                                 GETICON("grass_side") // sides (grass_side)
                                                 )
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock(BlockID::Dirt, "dirt",(new Block())
                      ->setAllSidesTexture(GETICON("dirt"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Cobblestone, "cobblestone",(new Block())
                      ->setAllSidesTexture(GETICON("cobblestone"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Planks, "planks",(new Block())
                      ->setAllSidesTexture(GETICON("planks_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Sapling, "sapling",(new Block())
                      ->setAllSidesTexture(GETICON("sapling_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));
    // TODO BEGIN
    RegisterBlock(BlockID::Flowing_Water, "flowing_water",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Water, "water",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Flowing_Lava, "flowing_lava",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Lava, "lava",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO END
    RegisterBlock(BlockID::Sand, "sand",(new Block())
                      ->setAllSidesTexture(GETICON("sand"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Gravel, "gravel",(new Block())
                      ->setAllSidesTexture(GETICON("gravel"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Gold_Ore, "gold_ore",(new Block())
                      ->setAllSidesTexture(GETICON("gold_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Iron_Ore, "iron_ore",(new Block())
                      ->setAllSidesTexture(GETICON("iron_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Coal_Ore, "coal_ore",(new Block())
                      ->setAllSidesTexture(GETICON("coal_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Log, "log",(new Block())
                      ->setTopBottomSidesTexture(GETICON("log_oak_top"), // top (oaklog_top)
                                                 GETICON("log_oak_top"), // bottom (oaklog_top)
                                                 GETICON("log_oak") // sides (oaklog_side)
                                                 )
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Leaves, "leaves",(new LeavesBlock())
                      ->setAllSidesTexture(GETICON("leaves_oak"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.2f));

    RegisterBlock(BlockID::Sponge, "sponge",(new Block())
                      ->setAllSidesTexture(GETICON("sponge"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.2f));

    RegisterBlock(BlockID::Glass, "glass",(new Block())
                      ->setAllSidesTexture(GETICON("glass"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock(BlockID::Lapis_Ore, "lapis_ore",(new Block())
                      ->setAllSidesTexture(GETICON("lapis_ore"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock(BlockID::Lapis_Block, "lapis_block",(new Block())
                      ->setAllSidesTexture(GETICON("lapis_block"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock(BlockID::Dispenser, "dispenser",(new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("dispenser_front_horizontal"), GETICON("furnace_side"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setDestroyTime(0.3f));

    RegisterBlock(BlockID::Sandstone, "sandstone",(new Block())
                      ->setTopBottomSidesTexture(GETICON("sandstone_top"), // top
                                                 GETICON("sandstone_bottom"), // bottom
                                                 GETICON("sandstone_normal") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Noteblock, "noteblock",(new Block())
                      ->setAllSidesTexture(GETICON("noteblock"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock(BlockID::Bed, "bed",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Golden_Rail, "rail_golden",(new Block())
                      ->setAllSidesTexture(GETICON("rail_golden"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Detector_Rail, "rail_detector",(new Block())
                      ->setAllSidesTexture(GETICON("rail_detector"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Sticky_Piston, "sticky_piston",(new Block())
                      ->setTopBottomSidesTexture(GETICON("piston_top_sticky"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Web, "web",(new Block())
                      ->setAllSidesTexture(GETICON("web"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Tallgrass, "tallgrass",(new Block())
                      ->setAllSidesTexture(GETICON("tallgrass"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Deadbush, "deadbush",(new Block())
                      ->setAllSidesTexture(GETICON("deadbush"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Piston, "piston",(new Block())
                      ->setTopBottomSidesTexture(GETICON("piston_top_normal"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Piston_Head, "piston_head",(new Block())
                      ->setAllSidesTexture(GETICON("piston_top_normal"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Wool, "wool",(new Block())
                      ->setAllSidesTexture(GETICON("wool_colored_white"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));
    // TODO: What even is this?
    RegisterBlock(BlockID::Piston_Extension, "piston_extension",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Yellow_Flower, "yellow_flower",(new Block())
                      ->setAllSidesTexture(GETICON("flower_dandelion"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Red_Flower, "red_flower",(new Block())
                      ->setAllSidesTexture(GETICON("flower_rose"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Brown_Mushroom, "brown_mushroom",(new Block())
                      ->setAllSidesTexture(GETICON("mushroom_brown"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Red_Mushroom, "red_mushroom",(new Block())
                      ->setAllSidesTexture(GETICON("mushroom_red"))
                      //->setSoundType(SoundType::WOOD)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Gold_Block, "gold_block",(new Block())
                      ->setAllSidesTexture(GETICON("gold_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Iron_Block, "iron_block",(new Block())
                      ->setAllSidesTexture(GETICON("iron_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO:
    RegisterBlock(BlockID::Double_Stone_Slab, "double_stone_slab",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO:
    RegisterBlock(BlockID::Stone_Slab, "stone_slab",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Brick_Block, "brick_block", (new Block())
                      ->setAllSidesTexture(GETICON("brick"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Tnt, "tnt",(new Block())
                      ->setTopBottomSidesTexture(GETICON("tnt_top"), // top
                                                 GETICON("tnt_bottom"), // bottom
                                                 GETICON("tnt_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Bookshelf, "bookshelf",(new Block())
                      ->setTopBottomSidesTexture(GETICON("planks_oak"), // top
                                                 GETICON("planks_oak"), // bottom
                                                 GETICON("bookshelf") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(2.0f));

    RegisterBlock(BlockID::Mossy_Cobblestone, "mossy_cobblestone",(new Block())
                      ->setAllSidesTexture(GETICON("cobblestone_mossy"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Obsidian, "obsidian",(new Block())
                      ->setAllSidesTexture(GETICON("obsidian"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Torch, "torch",(new Block())
                      ->setAllSidesTexture(GETICON("torch_on"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock(BlockID::Fire, "fire",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Mob_Spawner, "mob_spawner",(new Block())
                      ->setAllSidesTexture(GETICON("mob_spawner"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));
    // TODO:
    RegisterBlock(BlockID::Oak_Stairs, "oak_stairs",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO: Chest has an animated model, meaning it is hardcoded, not in a json
    RegisterBlock(BlockID::Chest, "chest",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Redstone_Wire, "redstone_wire",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Diamond_Ore, "diamond_ore",(new Block())
                      ->setAllSidesTexture(GETICON("diamond_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Diamond_Block, "diamond_block",(new Block())
                      ->setAllSidesTexture(GETICON("diamond_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Crafting_Table, "crafting_table",(new Block())
                      ->setTopBottomSidesTexture(GETICON("crafting_table_top"), GETICON("planks_oak"),
                                                 GETICON("crafting_table_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO: wheat has stages of growth
    RegisterBlock(BlockID::Wheat, "wheat",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    // TODO: Farmland has a dynamic texture, which is determined by its moisture
    // level.
    RegisterBlock(BlockID::Farmland, "farmland",(new Block())
                  //->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  //                           GETICON("planks_oak"),
                  //                           GETICON("crafting_table_side"))
                  //->setSoundType(SoundType::STONE)
                  //->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Furnace, "furnace",(new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_off"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Lit_Furnace, "lit_furnace",(new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_on"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
    // TODO: Yea...
    RegisterBlock(BlockID::Standing_Sign, "standing_sign",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );
    // TODO:
    RegisterBlock(BlockID::Wooden_Door, "wooden_door",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Ladder, "ladder",(new Block())
                      ->setAllSidesTexture(GETICON("ladder"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));

    RegisterBlock(BlockID::Rail, "rail",(new Block())
                      ->setAllSidesTexture(GETICON("rail_normal"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(1.5f));
    // TODO:
    RegisterBlock(BlockID::Stone_Stairs, "stone_stairs",(new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Wall_Sign, "wall_sign",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Lever, "lever",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Stone_Pressure_Plate, "stone_pressure_plate",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Iron_Door, "iron_door",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Wooden_Pressure_Plate, "wooden_pressure_plate",(new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Redstone_Ore, "redstone_ore",(new Block())
                      ->setAllSidesTexture(GETICON("redstone_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Lit_Redstone_Ore, "lit_redstone_ore",(new Block())
                      ->setAllSidesTexture(GETICON("redstone_ore_turned"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Unlit_Redstone_Torch, "unlit_redstone_torch",(new Block())
                      ->setAllSidesTexture(GETICON("redstone_torch_off"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Redstone_Torch, "redstone_torch",(new Block())
                      ->setAllSidesTexture(GETICON("redstone_torch_on"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Bedrock, "bedrock",(new Block())
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

    RegisterBlock(BlockID::Coal_Block, "coal_block",(new Block())
                      ->setAllSidesTexture(GETICON("coal_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Stone_Button, "",(new Block()));
    RegisterBlock(BlockID::Snow_Layer, "",(new Block()));
    RegisterBlock(BlockID::Ice, "",(new Block()));

    RegisterBlock(BlockID::Snow, "snow",(new Block())
                      ->setAllSidesTexture(GETICON("snow"))
                      //->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.5f));

    RegisterBlock(BlockID::Cactus, "",(new Block()));
    RegisterBlock(BlockID::Clay, "",(new Block()));
    RegisterBlock(BlockID::Reeds, "",(new Block()));
    RegisterBlock(BlockID::Jukebox, "",(new Block()));
    RegisterBlock(BlockID::Fence, "",(new Block()));
    RegisterBlock(BlockID::Pumpkin, "",(new Block()));

    RegisterBlock(BlockID::Netherrack, "netherrack",(new Block())
                      ->setAllSidesTexture(GETICON("netherrack"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Soul_Sand, "",(new Block()));
    RegisterBlock(BlockID::Glowstone, "",(new Block()));
    RegisterBlock(BlockID::Portal, "",(new Block()));
    RegisterBlock(BlockID::Lit_Pumpkin, "",(new Block()));
    RegisterBlock(BlockID::Cake, "",(new Block()));
    RegisterBlock(BlockID::Unpowered_Repeater, "",(new Block()));
    RegisterBlock(BlockID::Powered_Repeater, "",(new Block()));
    RegisterBlock(BlockID::Stained_Glass, "",(new Block()));
    RegisterBlock(BlockID::Trapdoor, "",(new Block()));
    RegisterBlock(BlockID::Monster_Egg, "",(new Block()));

    RegisterBlock(BlockID::Stonebrick, "",(new Block()));

    RegisterBlock(BlockID::Brown_Mushroom_Block, "",(new Block()));
    RegisterBlock(BlockID::Red_Mushroom_Block, "",(new Block()));
    RegisterBlock(BlockID::Iron_Bars, "",(new Block()));
    RegisterBlock(BlockID::Glass_Pane, "",(new Block()));
    RegisterBlock(BlockID::Melon_Block, "",(new Block()));
    RegisterBlock(BlockID::Pumpkin_Stem, "",(new Block()));
    RegisterBlock(BlockID::Melon_Stem, "",(new Block()));
    RegisterBlock(BlockID::Vine, "",(new Block()));
    RegisterBlock(BlockID::Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Brick_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Stone_Brick_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Mycelium, "",(new Block()));
    RegisterBlock(BlockID::Waterlily, "",(new Block()));
    RegisterBlock(BlockID::Nether_Brick, "",(new Block()));
    RegisterBlock(BlockID::Nether_Brick_Fence, "",(new Block()));
    RegisterBlock(BlockID::Nether_Brick_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Nether_Wart, "",(new Block()));
    RegisterBlock(BlockID::Enchanting_Table, "",(new Block()));
    RegisterBlock(BlockID::Brewing_Stand, "",(new Block()));
    RegisterBlock(BlockID::Cauldron, "",(new Block()));
    RegisterBlock(BlockID::End_Portal, "",(new Block()));
    RegisterBlock(BlockID::End_Portal_Frame, "",(new Block()));
    RegisterBlock(BlockID::End_Stone, "",(new Block()));
    RegisterBlock(BlockID::Dragon_Egg, "",(new Block()));
    RegisterBlock(BlockID::Redstone_Lamp, "",(new Block()));
    RegisterBlock(BlockID::Lit_Redstone_Lamp, "",(new Block()));
    RegisterBlock(BlockID::Double_Wooden_Slab, "",(new Block()));
    RegisterBlock(BlockID::Wooden_Slab, "",(new Block()));
    RegisterBlock(BlockID::Cocoa, "",(new Block()));
    RegisterBlock(BlockID::Sandstone_Stairs, "",(new Block()));

    RegisterBlock(BlockID::Emerald_Ore, "emerald_ore",(new Block())
                      ->setAllSidesTexture(GETICON("emerald_ore"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Ender_Chest, "",(new Block()));
    RegisterBlock(BlockID::Tripwire_Hook, "",(new Block()));
    RegisterBlock(BlockID::Tripwire, "",(new Block()));

    RegisterBlock(BlockID::Emerald_Block, "emerald_block",(new Block())
                      ->setAllSidesTexture(GETICON("emerald_block"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));

    RegisterBlock(BlockID::Spruce_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Birch_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Jungle_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Command_Block, "",(new Block()));
    RegisterBlock(BlockID::Beacon, "",(new Block()));
    RegisterBlock(BlockID::Cobblestone_Wall, "",(new Block()));
    RegisterBlock(BlockID::Flower_Pot, "",(new Block()));
    RegisterBlock(BlockID::Carrots, "",(new Block()));
    RegisterBlock(BlockID::Potatoes, "",(new Block()));
    RegisterBlock(BlockID::Wooden_Button, "",(new Block()));
    RegisterBlock(BlockID::Skull, "",(new Block()));
    RegisterBlock(BlockID::Anvil, "",(new Block()));
    RegisterBlock(BlockID::Trapped_Chest, "",(new Block()));
    RegisterBlock(BlockID::Light_Weighted_Pressure_Plate, "",(new Block()));
    RegisterBlock(BlockID::Heavy_Weighted_Pressure_Plate, "",(new Block()));
    RegisterBlock(BlockID::Unpowered_Comparator, "",(new Block()));
    RegisterBlock(BlockID::Powered_Comparator, "",(new Block()));
    RegisterBlock(BlockID::Daylight_Detector, "",(new Block()));
    RegisterBlock(BlockID::Redstone_Block, "",(new Block()));
    RegisterBlock(BlockID::Quartz_Ore, "",(new Block()));
    RegisterBlock(BlockID::Hopper, "",(new Block()));
    RegisterBlock(BlockID::Quartz_Block, "",(new Block()));
    RegisterBlock(BlockID::Quartz_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Activator_Rail, "",(new Block()));
    RegisterBlock(BlockID::Dropper, "",(new Block()));
    RegisterBlock(BlockID::Stained_Hardened_Clay, "",(new Block()));
    RegisterBlock(BlockID::Stained_Glass_Pane, "",(new Block()));
    RegisterBlock(BlockID::Leaves2, "",(new Block()));
    RegisterBlock(BlockID::Log2, "",(new Block()));
    RegisterBlock(BlockID::Acacia_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Dark_Oak_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Slime, "",(new Block()));
    RegisterBlock(BlockID::Barrier, "",(new Block()));
    RegisterBlock(BlockID::Iron_Trapdoor, "",(new Block()));
    RegisterBlock(BlockID::Prismarine, "",(new Block()));
    RegisterBlock(BlockID::Sea_Lantern, "",(new Block()));
    RegisterBlock(BlockID::Hay_Block, "",(new Block()));
    RegisterBlock(BlockID::Carpet, "",(new Block()));
    RegisterBlock(BlockID::Hardened_Clay, "",(new Block()));
    RegisterBlock(BlockID::Packed_Ice, "",(new Block()));
    RegisterBlock(BlockID::Double_Plant, "",(new Block()));
    RegisterBlock(BlockID::Standing_Banner, "",(new Block()));
    RegisterBlock(BlockID::Wall_Banner, "",(new Block()));
    RegisterBlock(BlockID::Daylight_Detector_Inverted, "",(new Block()));
    RegisterBlock(BlockID::Red_Sandstone, "",(new Block()));
    RegisterBlock(BlockID::Red_Sandstone_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Double_Stone_Slab2, "",(new Block()));
    RegisterBlock(BlockID::Stone_Slab2, "",(new Block()));
    RegisterBlock(BlockID::Spruce_Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Birch_Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Jungle_Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Dark_Oak_Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Acacia_Fence_Gate, "",(new Block()));
    RegisterBlock(BlockID::Spruce_Fence, "",(new Block()));
    RegisterBlock(BlockID::Birch_Fence, "",(new Block()));
    RegisterBlock(BlockID::Jungle_Fence, "",(new Block()));
    RegisterBlock(BlockID::Dark_Oak_Fence, "",(new Block()));
    RegisterBlock(BlockID::Acacia_Fence, "",(new Block()));
    RegisterBlock(BlockID::Spruce_Door, "",(new Block()));
    RegisterBlock(BlockID::Birch_Door, "",(new Block()));
    RegisterBlock(BlockID::Jungle_Door, "",(new Block()));
    RegisterBlock(BlockID::Acacia_Door, "",(new Block()));
    RegisterBlock(BlockID::Dark_Oak_Door, "",(new Block()));
    RegisterBlock(BlockID::End_Rod, "",(new Block()));
    RegisterBlock(BlockID::Chorus_Plant, "",(new Block()));
    RegisterBlock(BlockID::Chorus_Flower, "",(new Block()));
    RegisterBlock(BlockID::Purpur_Block, "",(new Block()));
    RegisterBlock(BlockID::Purpur_Pillar, "",(new Block()));
    RegisterBlock(BlockID::Purpur_Stairs, "",(new Block()));
    RegisterBlock(BlockID::Purpur_Double_Slab, "",(new Block()));
    RegisterBlock(BlockID::Purpur_Slab, "",(new Block()));
    RegisterBlock(BlockID::End_Bricks, "",(new Block()));
    RegisterBlock(BlockID::Beetroots, "",(new Block()));

    RegisterBlock(BlockID::Grass_Path,"",(new Block())
                      ->setTopBottomSidesTexture(GETICON("grass_path_top"), GETICON("dirt"), GETICON("grass_path_side"))
                      //->setSoundType(SoundType::GRASS)
                      ->setDestroyTime(0.6f));

    RegisterBlock(BlockID::End_Gateway, "",(new Block()));
    RegisterBlock(BlockID::Repeating_Command_Block, "",(new Block()));
    RegisterBlock(BlockID::Chain_Command_Block, "",(new Block()));
    RegisterBlock(BlockID::Frosted_Ice, "",(new Block()));
    RegisterBlock(BlockID::Magma, "",(new Block()));
    RegisterBlock(BlockID::Nether_Wart_Block, "",(new Block()));
    RegisterBlock(BlockID::Red_Nether_Brick, "",(new Block()));
    RegisterBlock(BlockID::Bone_Block, "",(new Block()));
    RegisterBlock(BlockID::Structure_Void, "",(new Block()));
    RegisterBlock(BlockID::Observer, "",(new Block()));
    RegisterBlock(BlockID::White_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Orange_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Magenta_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Light_Blue_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Yellow_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Lime_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Pink_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Gray_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Silver_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Cyan_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Purple_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Blue_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Brown_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Green_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Red_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::Black_Shulker_Box, "",(new Block()));
    RegisterBlock(BlockID::White_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Orange_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Magenta_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Light_Blue_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Yellow_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Lime_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Pink_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Gray_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Silver_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Cyan_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Purple_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Blue_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Brown_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Green_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Red_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Black_Glazed_Terracotta, "",(new Block()));
    RegisterBlock(BlockID::Concrete, "",(new Block()));
    RegisterBlock(BlockID::Concrete_Powder, "",(new Block()));
    RegisterBlock(BlockID::Structure_Block, "",(new Block()));

    m_blocks.resize(static_cast<size_t>(BlockID::Count));
}

BlockRegistry::~BlockRegistry() {}
