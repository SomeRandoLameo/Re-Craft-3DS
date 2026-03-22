//
// Created by elias on 22.03.26.
//
#include "item/Item.hpp"
#include "block/Block.hpp"

void ItemRegistry::RegisterItemBlock(BlockID block, Item* item) {
    int id = static_cast<int>(static_cast<u8>(block));

    m_registry[id] = item;
    m_occupied[id] = true;
    m_blockToItem[static_cast<u8>(block)] = item;
}

void ItemRegistry::Init() {

    RegisterItemBlock(BlockID::Air);
    RegisterItemBlock(BlockID::Stone);
    RegisterItemBlock(BlockID::Grass);
    RegisterItemBlock(BlockID::Dirt);
    RegisterItemBlock(BlockID::Cobblestone);
    RegisterItemBlock(BlockID::Planks);
    RegisterItemBlock(BlockID::Sapling);
    RegisterItemBlock(BlockID::Bedrock);
    RegisterItemBlock(BlockID::Sand);
    RegisterItemBlock(BlockID::Gravel);
    RegisterItemBlock(BlockID::Gold_Ore);
    RegisterItemBlock(BlockID::Iron_Ore);
    RegisterItemBlock(BlockID::Coal_Ore);
    RegisterItemBlock(BlockID::Log);
    RegisterItemBlock(BlockID::Leaves);
    RegisterItemBlock(BlockID::Sponge);
    RegisterItemBlock(BlockID::Glass);
    RegisterItemBlock(BlockID::Lapis_Ore);
    RegisterItemBlock(BlockID::Lapis_Block);
    RegisterItemBlock(BlockID::Dispenser);
    RegisterItemBlock(BlockID::Sandstone);
    RegisterItemBlock(BlockID::Noteblock);
    RegisterItemBlock(BlockID::Golden_Rail);
    RegisterItemBlock(BlockID::Detector_Rail);
    RegisterItemBlock(BlockID::Sticky_Piston);
    RegisterItemBlock(BlockID::Web);
    RegisterItemBlock(BlockID::Tallgrass);
    RegisterItemBlock(BlockID::Deadbush);
    RegisterItemBlock(BlockID::Piston);
    RegisterItemBlock(BlockID::Wool);
    RegisterItemBlock(BlockID::Yellow_Flower);
    RegisterItemBlock(BlockID::Red_Flower);
    RegisterItemBlock(BlockID::Brown_Mushroom);
    RegisterItemBlock(BlockID::Red_Mushroom);
    RegisterItemBlock(BlockID::Gold_Block);
    RegisterItemBlock(BlockID::Iron_Block);

    // Stone slab — item always places as half slab
    RegisterItemBlock(BlockID::Stone_Slab);
    RegisterItemBlock(BlockID::Double_Stone_Slab);

    RegisterItemBlock(BlockID::Brick_Block);
    RegisterItemBlock(BlockID::Tnt);
    RegisterItemBlock(BlockID::Bookshelf);
    RegisterItemBlock(BlockID::Mossy_Cobblestone);
    RegisterItemBlock(BlockID::Obsidian);
    RegisterItemBlock(BlockID::Torch);
    RegisterItemBlock(BlockID::Mob_Spawner);
    RegisterItemBlock(BlockID::Oak_Stairs);
    RegisterItemBlock(BlockID::Chest);
    RegisterItemBlock(BlockID::Diamond_Ore);
    RegisterItemBlock(BlockID::Diamond_Block);
    RegisterItemBlock(BlockID::Crafting_Table);
    RegisterItemBlock(BlockID::Farmland);
    RegisterItemBlock(BlockID::Furnace);
    RegisterItemBlock(BlockID::Standing_Sign);
    RegisterItemBlock(BlockID::Ladder);
    RegisterItemBlock(BlockID::Rail);
    RegisterItemBlock(BlockID::Stone_Stairs);
    RegisterItemBlock(BlockID::Lever);
    RegisterItemBlock(BlockID::Stone_Pressure_Plate);
    RegisterItemBlock(BlockID::Wooden_Pressure_Plate);
    RegisterItemBlock(BlockID::Redstone_Ore);
    RegisterItemBlock(BlockID::Redstone_Torch);
    RegisterItemBlock(BlockID::Stone_Button);
    RegisterItemBlock(BlockID::Snow_Layer);
    RegisterItemBlock(BlockID::Ice);
    RegisterItemBlock(BlockID::Snow);
    RegisterItemBlock(BlockID::Cactus);
    RegisterItemBlock(BlockID::Clay);
    RegisterItemBlock(BlockID::Jukebox);
    RegisterItemBlock(BlockID::Fence);
    RegisterItemBlock(BlockID::Spruce_Fence);
    RegisterItemBlock(BlockID::Birch_Fence);
    RegisterItemBlock(BlockID::Jungle_Fence);
    RegisterItemBlock(BlockID::Dark_Oak_Fence);
    RegisterItemBlock(BlockID::Acacia_Fence);
    RegisterItemBlock(BlockID::Pumpkin);
    RegisterItemBlock(BlockID::Netherrack);
    RegisterItemBlock(BlockID::Soul_Sand);
    RegisterItemBlock(BlockID::Glowstone);
    RegisterItemBlock(BlockID::Lit_Pumpkin);
    RegisterItemBlock(BlockID::Trapdoor);
    RegisterItemBlock(BlockID::Monster_Egg);
    RegisterItemBlock(BlockID::Stonebrick);
    RegisterItemBlock(BlockID::Brown_Mushroom_Block);
    RegisterItemBlock(BlockID::Red_Mushroom_Block);
    RegisterItemBlock(BlockID::Iron_Bars);
    RegisterItemBlock(BlockID::Glass_Pane);
    RegisterItemBlock(BlockID::Melon_Block);
    RegisterItemBlock(BlockID::Fence_Gate);
    RegisterItemBlock(BlockID::Spruce_Fence_Gate);
    RegisterItemBlock(BlockID::Birch_Fence_Gate);
    RegisterItemBlock(BlockID::Jungle_Fence_Gate);
    RegisterItemBlock(BlockID::Dark_Oak_Fence_Gate);
    RegisterItemBlock(BlockID::Acacia_Fence_Gate);
    RegisterItemBlock(BlockID::Brick_Stairs);
    RegisterItemBlock(BlockID::Stone_Brick_Stairs);
    RegisterItemBlock(BlockID::Mycelium);
    RegisterItemBlock(BlockID::Waterlily);
    RegisterItemBlock(BlockID::Nether_Brick);
    RegisterItemBlock(BlockID::Nether_Brick_Fence);
    RegisterItemBlock(BlockID::Nether_Brick_Stairs);
    RegisterItemBlock(BlockID::Enchanting_Table);
    RegisterItemBlock(BlockID::End_Portal_Frame);
    RegisterItemBlock(BlockID::End_Stone);
    RegisterItemBlock(BlockID::End_Bricks);
    RegisterItemBlock(BlockID::Dragon_Egg);
    RegisterItemBlock(BlockID::Redstone_Lamp);

    // Wooden slab
    RegisterItemBlock(BlockID::Wooden_Slab);
    RegisterItemBlock(BlockID::Double_Wooden_Slab);

    RegisterItemBlock(BlockID::Sandstone_Stairs);
    RegisterItemBlock(BlockID::Emerald_Ore);
    RegisterItemBlock(BlockID::Ender_Chest);
    RegisterItemBlock(BlockID::Tripwire_Hook);
    RegisterItemBlock(BlockID::Emerald_Block);
    RegisterItemBlock(BlockID::Spruce_Stairs);
    RegisterItemBlock(BlockID::Birch_Stairs);
    RegisterItemBlock(BlockID::Jungle_Stairs);
    RegisterItemBlock(BlockID::Command_Block);
    RegisterItemBlock(BlockID::Beacon);
    RegisterItemBlock(BlockID::Cobblestone_Wall);
    RegisterItemBlock(BlockID::Wooden_Button);
    RegisterItemBlock(BlockID::Anvil);
    RegisterItemBlock(BlockID::Trapped_Chest);
    RegisterItemBlock(BlockID::Light_Weighted_Pressure_Plate);
    RegisterItemBlock(BlockID::Heavy_Weighted_Pressure_Plate);
    RegisterItemBlock(BlockID::Daylight_Detector);
    RegisterItemBlock(BlockID::Redstone_Block);
    RegisterItemBlock(BlockID::Quartz_Ore);
    RegisterItemBlock(BlockID::Hopper);
    RegisterItemBlock(BlockID::Quartz_Block);
    RegisterItemBlock(BlockID::Quartz_Stairs);
    RegisterItemBlock(BlockID::Activator_Rail);
    RegisterItemBlock(BlockID::Dropper);
    RegisterItemBlock(BlockID::Stained_Hardened_Clay);
    RegisterItemBlock(BlockID::Barrier);
    RegisterItemBlock(BlockID::Iron_Trapdoor);
    RegisterItemBlock(BlockID::Hay_Block);
    RegisterItemBlock(BlockID::Carpet);
    RegisterItemBlock(BlockID::Hardened_Clay);
    RegisterItemBlock(BlockID::Coal_Block);
    RegisterItemBlock(BlockID::Packed_Ice);
    RegisterItemBlock(BlockID::Acacia_Stairs);
    RegisterItemBlock(BlockID::Dark_Oak_Stairs);
    RegisterItemBlock(BlockID::Slime);
    RegisterItemBlock(BlockID::Grass_Path);
    RegisterItemBlock(BlockID::Stained_Glass);
    RegisterItemBlock(BlockID::Stained_Glass_Pane);
    RegisterItemBlock(BlockID::Prismarine);
    RegisterItemBlock(BlockID::Sea_Lantern);
    RegisterItemBlock(BlockID::Red_Sandstone);
    RegisterItemBlock(BlockID::Red_Sandstone_Stairs);
    RegisterItemBlock(BlockID::Stone_Slab2);
    RegisterItemBlock(BlockID::Double_Stone_Slab2);
    RegisterItemBlock(BlockID::Repeating_Command_Block);
    RegisterItemBlock(BlockID::Chain_Command_Block);
    RegisterItemBlock(BlockID::Magma);
    RegisterItemBlock(BlockID::Nether_Wart_Block);
    RegisterItemBlock(BlockID::Red_Nether_Brick);
    RegisterItemBlock(BlockID::Bone_Block);
    RegisterItemBlock(BlockID::Structure_Void);
    RegisterItemBlock(BlockID::Observer);
    RegisterItemBlock(BlockID::White_Shulker_Box);
    RegisterItemBlock(BlockID::Orange_Shulker_Box);
    RegisterItemBlock(BlockID::Magenta_Shulker_Box);
    RegisterItemBlock(BlockID::Light_Blue_Shulker_Box);
    RegisterItemBlock(BlockID::Yellow_Shulker_Box);
    RegisterItemBlock(BlockID::Lime_Shulker_Box);
    RegisterItemBlock(BlockID::Pink_Shulker_Box);
    RegisterItemBlock(BlockID::Gray_Shulker_Box);
    RegisterItemBlock(BlockID::Silver_Shulker_Box);
    RegisterItemBlock(BlockID::Cyan_Shulker_Box);
    RegisterItemBlock(BlockID::Purple_Shulker_Box);
    RegisterItemBlock(BlockID::Blue_Shulker_Box);
    RegisterItemBlock(BlockID::Brown_Shulker_Box);
    RegisterItemBlock(BlockID::Green_Shulker_Box);
    RegisterItemBlock(BlockID::Red_Shulker_Box);
    RegisterItemBlock(BlockID::Black_Shulker_Box);
    RegisterItemBlock(BlockID::White_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Orange_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Magenta_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Light_Blue_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Yellow_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Lime_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Pink_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Gray_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Silver_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Cyan_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Purple_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Blue_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Brown_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Green_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Red_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Black_Glazed_Terracotta);
    RegisterItemBlock(BlockID::Concrete);
    RegisterItemBlock(BlockID::Concrete_Powder);
    RegisterItemBlock(BlockID::Structure_Block);

    // TODO: Non-block items (tools, food, weapons etc.) need their own
    // Item subclasses and IDs separate from block IDs
}