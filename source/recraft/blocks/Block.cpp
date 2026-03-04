#include "blocks/Block.hpp"
#include "blocks/GrassBlock.h"
#include "blocks/LeavesBlock.h"

TextureMap BlockRegistry::m_textureMap;

BlockRegistry::BlockRegistry() {
    m_textureMap.Init("romfs:/assets/minecraft/textures/blocks");

#define GETICON(x) m_textureMap.Get(x ".png")

    RegisterBlock(BlockID::Air, "air",
                  (new Block())
                      ->setAllSidesTexture(0, 0)
                      ->setOpaque(false)
                      ->setSolid(false)
                      ->setHardness(0.0f)
                      ->setUnlocalizedName("air"));

    RegisterBlock(BlockID::Stone, "stone",
                  (new Block())
                      ->setAllSidesTexture(GETICON("stone"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(1.5F)
                      ->setResistance(10.0F)
                      ->setSoundType(SoundType::STONE)
                      ->setUnlocalizedName("stone"));

    RegisterBlock(BlockID::Grass, "grass",
                  (new GrassBlock())
                      ->setTopBottomSidesTexture(GETICON("grass_top"), // top (grass_top)
                                                 GETICON("dirt"), // bottom (dirt)
                                                 GETICON("grass_side") // sides (grass_side)
                                                 )
                      //->setSoundType(SoundType::GRASS)
                      ->setHardness(0.6f));

    RegisterBlock(BlockID::Dirt, "dirt",
                  (new Block())
                      ->setAllSidesTexture(GETICON("dirt"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(BlockID::Cobblestone, "cobblestone",
                  (new Block())
                      ->setAllSidesTexture(GETICON("cobblestone"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(BlockID::Planks, "planks",
                  (new Block())
                      ->setAllSidesTexture(GETICON("planks_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Sapling, "sapling",
                  (new Block())
                      ->setAllSidesTexture(GETICON("sapling_oak"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(2.0f));
    // TODO BEGIN
    RegisterBlock(BlockID::Flowing_Water, "flowing_water", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Water, "water", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Flowing_Lava, "flowing_lava", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Lava, "lava", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO END
    RegisterBlock(BlockID::Sand, "sand",
                  (new Block())
                      ->setAllSidesTexture(GETICON("sand"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(BlockID::Gravel, "gravel",
                  (new Block())
                      ->setAllSidesTexture(GETICON("gravel"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(
        BlockID::Gold_Ore, "gold_ore",
        (new Block())->setAllSidesTexture(GETICON("gold_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(
        BlockID::Iron_Ore, "iron_ore",
        (new Block())->setAllSidesTexture(GETICON("iron_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(
        BlockID::Coal_Ore, "coal_ore",
        (new Block())->setAllSidesTexture(GETICON("coal_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Log, "log",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("log_oak_top"), // top (oaklog_top)
                                                 GETICON("log_oak_top"), // bottom (oaklog_top)
                                                 GETICON("log_oak") // sides (oaklog_side)
                                                 )
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Leaves, "leaves",
                  (new LeavesBlock())
                      ->setAllSidesTexture(GETICON("leaves_oak"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setHardness(0.2f));

    RegisterBlock(BlockID::Sponge, "sponge",
                  (new Block())
                      ->setAllSidesTexture(GETICON("sponge"))
                      //->setSoundType(SoundType::GRASS)
                      ->setOpaque(false)
                      ->setHardness(0.2f));

    RegisterBlock(BlockID::Glass, "glass",
                  (new Block())
                      ->setAllSidesTexture(GETICON("glass"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setHardness(0.3f));

    RegisterBlock(BlockID::Lapis_Ore, "lapis_ore",
                  (new Block())
                      ->setAllSidesTexture(GETICON("lapis_ore"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setHardness(0.3f));

    RegisterBlock(BlockID::Lapis_Block, "lapis_block",
                  (new Block())
                      ->setAllSidesTexture(GETICON("lapis_block"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setHardness(0.3f));

    RegisterBlock(BlockID::Dispenser, "dispenser",
                  (new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("dispenser_front_horizontal"), GETICON("furnace_side"))
                      //->setSoundType(SoundType::GLASS)
                      ->setOpaque(false)
                      ->setHardness(0.3f));

    RegisterBlock(BlockID::Sandstone, "sandstone",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("sandstone_top"), // top
                                                 GETICON("sandstone_bottom"), // bottom
                                                 GETICON("sandstone_normal") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Noteblock, "noteblock",
                  (new Block())
                      ->setAllSidesTexture(GETICON("noteblock"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));
    // TODO:
    RegisterBlock(BlockID::Bed, "bed", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(
        BlockID::Golden_Rail, "rail_golden",
        (new Block())->setAllSidesTexture(GETICON("rail_golden"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(
        BlockID::Detector_Rail, "rail_detector",
        (new Block())->setAllSidesTexture(GETICON("rail_detector"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(BlockID::Sticky_Piston, "sticky_piston",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("piston_top_sticky"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Web, "web",
                  (new Block())->setAllSidesTexture(GETICON("web"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(
        BlockID::Tallgrass, "tallgrass",
        (new Block())->setAllSidesTexture(GETICON("tallgrass"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(
        BlockID::Deadbush, "deadbush",
        (new Block())->setAllSidesTexture(GETICON("deadbush"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(BlockID::Piston, "piston",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("piston_top_normal"), // top
                                                 GETICON("piston_bottom"), // bottom
                                                 GETICON("piston_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Piston_Head, "piston_head",
                  (new Block())
                      ->setAllSidesTexture(GETICON("piston_top_normal"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Wool, "wool",
                  (new Block())
                      ->setAllSidesTexture(GETICON("wool_colored_white"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(0.8f));
    // TODO: What even is this?
    RegisterBlock(BlockID::Piston_Extension, "piston_extension", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Yellow_Flower, "yellow_flower",
                  (new Block())
                      ->setAllSidesTexture(GETICON("flower_dandelion"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Red_Flower, "red_flower",
                  (new Block())
                      ->setAllSidesTexture(GETICON("flower_rose"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Brown_Mushroom, "brown_mushroom",
                  (new Block())
                      ->setAllSidesTexture(GETICON("mushroom_brown"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Red_Mushroom, "red_mushroom",
                  (new Block())
                      ->setAllSidesTexture(GETICON("mushroom_red"))
                      //->setSoundType(SoundType::WOOD)
                      ->setHardness(0.8f));

    RegisterBlock(
        BlockID::Gold_Block, "gold_block",
        (new Block())->setAllSidesTexture(GETICON("gold_block"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(
        BlockID::Iron_Block, "iron_block",
        (new Block())->setAllSidesTexture(GETICON("iron_block"))->setSoundType(SoundType::STONE)->setHardness(0.8f));
    // TODO:
    RegisterBlock(BlockID::Double_Stone_Slab, "double_stone_slab", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO:
    RegisterBlock(BlockID::Stone_Slab, "stone_slab", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(
        BlockID::Brick_Block, "brick_block",
        (new Block())->setAllSidesTexture(GETICON("brick"))->setSoundType(SoundType::STONE)->setHardness(2.0f));

    RegisterBlock(BlockID::Tnt, "tnt",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("tnt_top"), // top
                                                 GETICON("tnt_bottom"), // bottom
                                                 GETICON("tnt_side") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Bookshelf, "bookshelf",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("planks_oak"), // top
                                                 GETICON("planks_oak"), // bottom
                                                 GETICON("bookshelf") // sides
                                                 )
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(2.0f));

    RegisterBlock(BlockID::Mossy_Cobblestone, "mossy_cobblestone",
                  (new Block())
                      ->setAllSidesTexture(GETICON("cobblestone_mossy"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Obsidian, "obsidian",
                  (new Block())
                      ->setAllSidesTexture(GETICON("obsidian"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(BlockID::Torch, "torch",
                  (new Block())
                      ->setAllSidesTexture(GETICON("torch_on"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));
    // TODO:
    RegisterBlock(BlockID::Fire, "fire", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Mob_Spawner, "mob_spawner",
                  (new Block())
                      ->setAllSidesTexture(GETICON("mob_spawner"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));
    // TODO:
    RegisterBlock(BlockID::Oak_Stairs, "oak_stairs", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );
    // TODO: Chest has an animated model, meaning it is hardcoded, not in a json
    RegisterBlock(BlockID::Chest, "chest", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Redstone_Wire, "redstone_wire", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(
        BlockID::Diamond_Ore, "diamond_ore",
        (new Block())->setAllSidesTexture(GETICON("diamond_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(
        BlockID::Diamond_Block, "diamond_block",
        (new Block())->setAllSidesTexture(GETICON("diamond_block"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Crafting_Table, "crafting_table",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("crafting_table_top"), GETICON("planks_oak"),
                                                 GETICON("crafting_table_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));
    // TODO: wheat has stages of growth
    RegisterBlock(BlockID::Wheat, "wheat", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    // TODO: Farmland has a dynamic texture, which is determined by its moisture
    // level.
    RegisterBlock(BlockID::Farmland, "farmland", (new Block())
                  //->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  //                           GETICON("planks_oak"),
                  //                           GETICON("crafting_table_side"))
                  //->setSoundType(SoundType::STONE)
                  //->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Furnace, "furnace",
                  (new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_off"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Lit_Furnace, "lit_furnace",
                  (new Block())
                      ->setTopBottomSouthSidesTexture(GETICON("furnace_top"), GETICON("furnace_top"),
                                                      GETICON("furnace_front_on"), GETICON("furnace_side"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));
    // TODO: Yea...
    RegisterBlock(BlockID::Standing_Sign, "standing_sign", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );
    // TODO:
    RegisterBlock(BlockID::Wooden_Door, "wooden_door", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Ladder, "ladder",
                  (new Block())
                      ->setAllSidesTexture(GETICON("ladder"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(1.5f));

    RegisterBlock(BlockID::Rail, "rail",
                  (new Block())
                      ->setAllSidesTexture(GETICON("rail_normal"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(1.5f));
    // TODO:
    RegisterBlock(BlockID::Stone_Stairs, "stone_stairs", (new Block())
                  //->setAllSidesTexture(GETICON("sapling_oak"))
                  //->setSoundType(SoundType::WOOD)
                  // ->setDestroyTime(2.0f)
    );

    RegisterBlock(BlockID::Wall_Sign, "wall_sign", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Lever, "lever", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Stone_Pressure_Plate, "stone_pressure_plate", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Iron_Door, "iron_door", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(BlockID::Wooden_Pressure_Plate, "wooden_pressure_plate", (new Block())
                  // ->setTopBottomSidesTexture(GETICON("crafting_table_top"),
                  // GETICON("planks_oak"),
                  //                             GETICON("crafting_table_side"))
                  //  ->setSoundType(SoundType::STONE)
                  // ->setDestroyTime(0.8f)
    );

    RegisterBlock(
        BlockID::Redstone_Ore, "redstone_ore",
        (new Block())->setAllSidesTexture(GETICON("redstone_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Lit_Redstone_Ore, "lit_redstone_ore",
                  (new Block())
                      ->setAllSidesTexture(GETICON("redstone_ore_turned"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Unlit_Redstone_Torch, "unlit_redstone_torch",
                  (new Block())
                      ->setAllSidesTexture(GETICON("redstone_torch_off"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Redstone_Torch, "redstone_torch",
                  (new Block())
                      ->setAllSidesTexture(GETICON("redstone_torch_on"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(0.8f));

    RegisterBlock(BlockID::Bedrock, "bedrock",
                  (new Block())
                      ->setAllSidesTexture(GETICON("bedrock"))
                      ->setSoundType(SoundType::STONE)
                      ->setHardness(-1.0f) // Unbreakable
    );

    /*RegisterBlock((new Block(BlockID::Smooth_Stone, "smooth_stone"))
                      ->setAllSidesTexture(GETICON("smooth_stone"))
                      ->setSoundType(SoundType::STONE)
                      ->setDestroyTime(0.8f));
                      Doesnt exist in 1.12 for some reason.
  */

    RegisterBlock(
        BlockID::Coal_Block, "coal_block",
        (new Block())->setAllSidesTexture(GETICON("coal_block"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Stone_Button, "stone_button", (new Block()));
    RegisterBlock(BlockID::Snow_Layer, "snow_layer", (new Block()));
    RegisterBlock(BlockID::Ice, "ice", (new Block()));

    RegisterBlock(BlockID::Snow, "snow",
                  (new Block())
                      ->setAllSidesTexture(GETICON("snow"))
                      //->setSoundType(SoundType::STONE)
                      ->setHardness(0.5f));

    RegisterBlock(BlockID::Cactus, "cactus", (new Block()));
    RegisterBlock(BlockID::Clay, "clay", (new Block()));
    RegisterBlock(BlockID::Reeds, "reeds", (new Block()));
    RegisterBlock(BlockID::Jukebox, "jukebox", (new Block()));
    RegisterBlock(BlockID::Fence, "fence", (new Block()));
    RegisterBlock(BlockID::Pumpkin, "pumpkin", (new Block()));

    RegisterBlock(
        BlockID::Netherrack, "netherrack",
        (new Block())->setAllSidesTexture(GETICON("netherrack"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Soul_Sand, "soul_sand", (new Block()));
    RegisterBlock(BlockID::Glowstone, "glowstone", (new Block()));
    RegisterBlock(BlockID::Portal, "portal", (new Block()));
    RegisterBlock(BlockID::Lit_Pumpkin, "lit_pumpkin", (new Block()));
    RegisterBlock(BlockID::Cake, "cake", (new Block()));
    RegisterBlock(BlockID::Unpowered_Repeater, "unpowered_repeater", (new Block()));
    RegisterBlock(BlockID::Powered_Repeater, "powered_repeater", (new Block()));
    RegisterBlock(BlockID::Stained_Glass, "stained_glass", (new Block()));
    RegisterBlock(BlockID::Trapdoor, "trapdoor", (new Block()));
    RegisterBlock(BlockID::Monster_Egg, "monster_egg", (new Block()));

    RegisterBlock(BlockID::Stonebrick, "stonebrick", (new Block()));

    RegisterBlock(BlockID::Brown_Mushroom_Block, "brown_mushroom_block", (new Block()));
    RegisterBlock(BlockID::Red_Mushroom_Block, "red_mushroom_block", (new Block()));
    RegisterBlock(BlockID::Iron_Bars, "iron_bars", (new Block()));
    RegisterBlock(BlockID::Glass_Pane, "glass_pane", (new Block()));
    RegisterBlock(BlockID::Melon_Block, "melon_block", (new Block()));
    RegisterBlock(BlockID::Pumpkin_Stem, "pumpkin_stem", (new Block()));
    RegisterBlock(BlockID::Melon_Stem, "melon_stem", (new Block()));
    RegisterBlock(BlockID::Vine, "vine", (new Block()));
    RegisterBlock(BlockID::Fence_Gate, "fence_gate", (new Block()));
    RegisterBlock(BlockID::Brick_Stairs, "brick_stairs", (new Block()));
    RegisterBlock(BlockID::Stone_Brick_Stairs, "stone_brick_stairs", (new Block()));
    RegisterBlock(BlockID::Mycelium, "mycelium", (new Block()));
    RegisterBlock(BlockID::Waterlily, "waterlily", (new Block()));
    RegisterBlock(BlockID::Nether_Brick, "nether_brick", (new Block()));
    RegisterBlock(BlockID::Nether_Brick_Fence, "nether_brick_fence", (new Block()));
    RegisterBlock(BlockID::Nether_Brick_Stairs, "nether_brick_stairs", (new Block()));
    RegisterBlock(BlockID::Nether_Wart, "nether_wart", (new Block()));
    RegisterBlock(BlockID::Enchanting_Table, "enchanting_table", (new Block()));
    RegisterBlock(BlockID::Brewing_Stand, "brewing_stand", (new Block()));
    RegisterBlock(BlockID::Cauldron, "cauldron", (new Block()));
    RegisterBlock(BlockID::End_Portal, "end_portal", (new Block()));
    RegisterBlock(BlockID::End_Portal_Frame, "end_portal_frame", (new Block()));
    RegisterBlock(BlockID::End_Stone, "end_stone", (new Block()));
    RegisterBlock(BlockID::Dragon_Egg, "dragon_egg", (new Block()));
    RegisterBlock(BlockID::Redstone_Lamp, "redstone_lamp", (new Block()));
    RegisterBlock(BlockID::Lit_Redstone_Lamp, "lit_redstone_lamp", (new Block()));
    RegisterBlock(BlockID::Double_Wooden_Slab, "double_wooden_slab", (new Block()));
    RegisterBlock(BlockID::Wooden_Slab, "wooden_slab", (new Block()));
    RegisterBlock(BlockID::Cocoa, "cocoa", (new Block()));
    RegisterBlock(BlockID::Sandstone_Stairs, "sandstone_stairs", (new Block()));

    RegisterBlock(
        BlockID::Emerald_Ore, "emerald_ore",
        (new Block())->setAllSidesTexture(GETICON("emerald_ore"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Ender_Chest, "ender_chest", (new Block()));
    RegisterBlock(BlockID::Tripwire_Hook, "tripwire_hook", (new Block()));
    RegisterBlock(BlockID::Tripwire, "tripwire", (new Block()));

    RegisterBlock(
        BlockID::Emerald_Block, "emerald_block",
        (new Block())->setAllSidesTexture(GETICON("emerald_block"))->setSoundType(SoundType::STONE)->setHardness(0.8f));

    RegisterBlock(BlockID::Spruce_Stairs, "spruce_stairs", (new Block()));
    RegisterBlock(BlockID::Birch_Stairs, "birch_stairs", (new Block()));
    RegisterBlock(BlockID::Jungle_Stairs, "jungle_stairs", (new Block()));
    RegisterBlock(BlockID::Command_Block, "command_block", (new Block()));
    RegisterBlock(BlockID::Beacon, "beacon", (new Block()));
    RegisterBlock(BlockID::Cobblestone_Wall, "cobblestone_wall", (new Block()));
    RegisterBlock(BlockID::Flower_Pot, "flower_pot", (new Block()));
    RegisterBlock(BlockID::Carrots, "carrots", (new Block()));
    RegisterBlock(BlockID::Potatoes, "potatoes", (new Block()));
    RegisterBlock(BlockID::Wooden_Button, "wooden_button", (new Block()));
    RegisterBlock(BlockID::Skull, "skull", (new Block()));
    RegisterBlock(BlockID::Anvil, "anvil", (new Block()));
    RegisterBlock(BlockID::Trapped_Chest, "trapped_chest", (new Block()));
    RegisterBlock(BlockID::Light_Weighted_Pressure_Plate, "light_weighted_pressure_plate", (new Block()));
    RegisterBlock(BlockID::Heavy_Weighted_Pressure_Plate, "heavy_weighted_pressure_plate", (new Block()));
    RegisterBlock(BlockID::Unpowered_Comparator, "unpowered_comparator", (new Block()));
    RegisterBlock(BlockID::Powered_Comparator, "powered_comparator", (new Block()));
    RegisterBlock(BlockID::Daylight_Detector, "daylight_detector", (new Block()));
    RegisterBlock(BlockID::Redstone_Block, "redstone_block", (new Block()));
    RegisterBlock(BlockID::Quartz_Ore, "quartz_ore", (new Block()));
    RegisterBlock(BlockID::Hopper, "hopper", (new Block()));
    RegisterBlock(BlockID::Quartz_Block, "quartz_block", (new Block()));
    RegisterBlock(BlockID::Quartz_Stairs, "quartz_stairs", (new Block()));
    RegisterBlock(BlockID::Activator_Rail, "activator_rail", (new Block()));
    RegisterBlock(BlockID::Dropper, "dropper", (new Block()));
    RegisterBlock(BlockID::Stained_Hardened_Clay, "stained_hardened_clay", (new Block()));
    RegisterBlock(BlockID::Stained_Glass_Pane, "stained_glass_pane", (new Block()));
    RegisterBlock(BlockID::Leaves2, "leaves2", (new Block()));
    RegisterBlock(BlockID::Log2, "log2", (new Block()));
    RegisterBlock(BlockID::Acacia_Stairs, "acacia_stairs", (new Block()));
    RegisterBlock(BlockID::Dark_Oak_Stairs, "dark_oak_stairs", (new Block()));
    RegisterBlock(BlockID::Slime, "slime", (new Block()));
    RegisterBlock(BlockID::Barrier, "barrier", (new Block()));
    RegisterBlock(BlockID::Iron_Trapdoor, "iron_trapdoor", (new Block()));
    RegisterBlock(BlockID::Prismarine, "prismarine", (new Block()));
    RegisterBlock(BlockID::Sea_Lantern, "sea_lantern", (new Block()));
    RegisterBlock(BlockID::Hay_Block, "hay_block", (new Block()));
    RegisterBlock(BlockID::Carpet, "carpet", (new Block()));
    RegisterBlock(BlockID::Hardened_Clay, "hardened_clay", (new Block()));
    RegisterBlock(BlockID::Packed_Ice, "packed_ice", (new Block()));
    RegisterBlock(BlockID::Double_Plant, "double_plant", (new Block()));
    RegisterBlock(BlockID::Standing_Banner, "standing_banner", (new Block()));
    RegisterBlock(BlockID::Wall_Banner, "wall_banner", (new Block()));
    RegisterBlock(BlockID::Daylight_Detector_Inverted, "daylight_detector_inverted", (new Block()));
    RegisterBlock(BlockID::Red_Sandstone, "red_sandstone", (new Block()));
    RegisterBlock(BlockID::Red_Sandstone_Stairs, "red_sandstone_stairs", (new Block()));
    RegisterBlock(BlockID::Double_Stone_Slab2, "double_stone_slab2", (new Block()));
    RegisterBlock(BlockID::Stone_Slab2, "stone_slab2", (new Block()));
    RegisterBlock(BlockID::Spruce_Fence_Gate, "spruce_fence_gate", (new Block()));
    RegisterBlock(BlockID::Birch_Fence_Gate, "birch_fence_gate", (new Block()));
    RegisterBlock(BlockID::Jungle_Fence_Gate, "jungle_fence_gate", (new Block()));
    RegisterBlock(BlockID::Dark_Oak_Fence_Gate, "dark_oak_fence_gate", (new Block()));
    RegisterBlock(BlockID::Acacia_Fence_Gate, "acacia_fence_gate", (new Block()));
    RegisterBlock(BlockID::Spruce_Fence, "spruce_fence", (new Block()));
    RegisterBlock(BlockID::Birch_Fence, "birch_fence", (new Block()));
    RegisterBlock(BlockID::Jungle_Fence, "jungle_fence", (new Block()));
    RegisterBlock(BlockID::Dark_Oak_Fence, "dark_oak_fence", (new Block()));
    RegisterBlock(BlockID::Acacia_Fence, "acacia_fence", (new Block()));
    RegisterBlock(BlockID::Spruce_Door, "spruce_door", (new Block()));
    RegisterBlock(BlockID::Birch_Door, "birch_door", (new Block()));
    RegisterBlock(BlockID::Jungle_Door, "jungle_door", (new Block()));
    RegisterBlock(BlockID::Acacia_Door, "acacia_door", (new Block()));
    RegisterBlock(BlockID::Dark_Oak_Door, "dark_oak_door", (new Block()));
    RegisterBlock(BlockID::End_Rod, "end_rod", (new Block()));
    RegisterBlock(BlockID::Chorus_Plant, "chorus_plant", (new Block()));
    RegisterBlock(BlockID::Chorus_Flower, "chorus_flower", (new Block()));
    RegisterBlock(BlockID::Purpur_Block, "purpur_block", (new Block()));
    RegisterBlock(BlockID::Purpur_Pillar, "purpur_pillar", (new Block()));
    RegisterBlock(BlockID::Purpur_Stairs, "purpur_stairs", (new Block()));
    RegisterBlock(BlockID::Purpur_Double_Slab, "purpur_double_slab", (new Block()));
    RegisterBlock(BlockID::Purpur_Slab, "purpur_slab", (new Block()));
    RegisterBlock(BlockID::End_Bricks, "end_bricks", (new Block()));
    RegisterBlock(BlockID::Beetroots, "beetroots", (new Block()));

    RegisterBlock(BlockID::Grass_Path, "grass_path",
                  (new Block())
                      ->setTopBottomSidesTexture(GETICON("grass_path_top"), GETICON("dirt"), GETICON("grass_path_side"))
                      //->setSoundType(SoundType::GRASS)
                      ->setHardness(0.6f));

    RegisterBlock(BlockID::End_Gateway, "end_gateway", (new Block()));
    RegisterBlock(BlockID::Repeating_Command_Block, "repeating_command_block", (new Block()));
    RegisterBlock(BlockID::Chain_Command_Block, "chain_command_block", (new Block()));
    RegisterBlock(BlockID::Frosted_Ice, "frosted_ice", (new Block()));
    RegisterBlock(BlockID::Magma, "magma", (new Block()));
    RegisterBlock(BlockID::Nether_Wart_Block, "nether_wart_block", (new Block()));
    RegisterBlock(BlockID::Red_Nether_Brick, "red_nether_brick", (new Block()));
    RegisterBlock(BlockID::Bone_Block, "bone_block", (new Block()));
    RegisterBlock(BlockID::Structure_Void, "structure_void", (new Block()));
    RegisterBlock(BlockID::Observer, "observer", (new Block()));
    RegisterBlock(BlockID::White_Shulker_Box, "white_shulker_box", (new Block()));
    RegisterBlock(BlockID::Orange_Shulker_Box, "orange_shulker_box", (new Block()));
    RegisterBlock(BlockID::Magenta_Shulker_Box, "magenta_shulker_box", (new Block()));
    RegisterBlock(BlockID::Light_Blue_Shulker_Box, "light_blue_shulker_box", (new Block()));
    RegisterBlock(BlockID::Yellow_Shulker_Box, "yellow_shulker_box", (new Block()));
    RegisterBlock(BlockID::Lime_Shulker_Box, "lime_shulker_box", (new Block()));
    RegisterBlock(BlockID::Pink_Shulker_Box, "pink_shulker_box", (new Block()));
    RegisterBlock(BlockID::Gray_Shulker_Box, "gray_shulker_box", (new Block()));
    RegisterBlock(BlockID::Silver_Shulker_Box, "silver_shulker_box", (new Block()));
    RegisterBlock(BlockID::Cyan_Shulker_Box, "cyan_shulker_box", (new Block()));
    RegisterBlock(BlockID::Purple_Shulker_Box, "purple_shulker_box", (new Block()));
    RegisterBlock(BlockID::Blue_Shulker_Box, "blue_shulker_box", (new Block()));
    RegisterBlock(BlockID::Brown_Shulker_Box, "brown_shulker_box", (new Block()));
    RegisterBlock(BlockID::Green_Shulker_Box, "green_shulker_box", (new Block()));
    RegisterBlock(BlockID::Red_Shulker_Box, "red_shulker_box", (new Block()));
    RegisterBlock(BlockID::Black_Shulker_Box, "black_shulker_box", (new Block()));
    RegisterBlock(BlockID::White_Glazed_Terracotta, "white_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Orange_Glazed_Terracotta, "orange_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Magenta_Glazed_Terracotta, "magenta_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Light_Blue_Glazed_Terracotta, "light_blue_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Yellow_Glazed_Terracotta, "yellow_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Lime_Glazed_Terracotta, "lime_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Pink_Glazed_Terracotta, "pink_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Gray_Glazed_Terracotta, "gray_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Silver_Glazed_Terracotta, "silver_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Cyan_Glazed_Terracotta, "cyan_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Purple_Glazed_Terracotta, "purple_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Blue_Glazed_Terracotta, "blue_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Brown_Glazed_Terracotta, "brown_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Green_Glazed_Terracotta, "green_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Red_Glazed_Terracotta, "red_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Black_Glazed_Terracotta, "black_glazed_terracotta", (new Block()));
    RegisterBlock(BlockID::Concrete, "concrete", (new Block()));
    RegisterBlock(BlockID::Concrete_Powder, "concrete_powder", (new Block()));
    RegisterBlock(
        BlockID::Structure_Block, "structure_block",
        (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("structureBlock"));
}

BlockRegistry::~BlockRegistry() {}
