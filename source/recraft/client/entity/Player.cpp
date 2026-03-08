#include "client/entity/Player.hpp"

#include "client/entity/Damage.hpp"
#include "client/gui/DebugUI.hpp"
#include "input/InputManager.hpp"
#include "mcbridge/MCBridge.hpp"
#include "world/CT_World.hpp"

const float MaxWalkVelocity = 4.3f;
const float MaxFallVelocity = -50.f;
const float GravityPlusFriction = 10.f;

// TODO: [Server thread/WARN]: Nintendo3DS moved too quickly!
// This playercontroller needs to be rewritten to move like a proper Minecraft player.
// This implementation seems off and is just craftus at core.
// Since we are aiming for Minecraft compatibility, this needs to be fixed and adjusted to controller.

// Also, this player is a mess code-wise.
// This needs to be restructured into an entity base class and player derived class. (Entity because mobs, Particles,
// TileEntities etc. will share a lot of code too.) Inventory should also be separated.

Player::Player(World* world) : Entity(world), m_input(new PlayerInput()) {
    heightOffset = 1.62f;
    SetSize(0.6f, 1.8f);
    InitializeInventory();
}

// TODO: Separate from player
void Player::InitializeInventory() {
    int slotIndex = 0;
    // inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Air), 1, 0); // Testing
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Air), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Grass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dirt), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cobblestone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Planks), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sapling), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Bedrock), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Flowing_Water), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Water), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Flowing_Lava), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lava), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sand), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gravel), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gold_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Coal_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Log), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Leaves), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sponge), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Glass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lapis_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lapis_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dispenser), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sandstone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Noteblock), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Bed), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Golden_Rail), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Detector_Rail), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sticky_Piston), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Web), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Tallgrass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Deadbush), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Piston), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Piston_Head), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wool), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Piston_Extension), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Yellow_Flower), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Flower), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brown_Mushroom), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Mushroom), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gold_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Double_Stone_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brick_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Tnt), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Bookshelf), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Mossy_Cobblestone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Obsidian), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Torch), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Fire), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Mob_Spawner), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Oak_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Chest), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Redstone_Wire), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Diamond_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Diamond_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Crafting_Table), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wheat), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Farmland), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Furnace), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lit_Furnace), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Standing_Sign), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wooden_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Ladder), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Rail), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wall_Sign), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lever), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Pressure_Plate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wooden_Pressure_Plate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Redstone_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lit_Redstone_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Unlit_Redstone_Torch), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Redstone_Torch), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Button), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Snow_Layer), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Ice), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Snow), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cactus), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Clay), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Reeds), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Jukebox), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Pumpkin), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Netherrack), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Soul_Sand), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Glowstone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Portal), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lit_Pumpkin), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cake), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Unpowered_Repeater), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Powered_Repeater), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stained_Glass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Trapdoor), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Monster_Egg), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stonebrick), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brown_Mushroom_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Mushroom_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Bars), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Glass_Pane), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Melon_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Pumpkin_Stem), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Melon_Stem), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Vine), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brick_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Brick_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Mycelium), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Waterlily), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Nether_Brick), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Nether_Brick_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Nether_Brick_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Nether_Wart), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Enchanting_Table), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brewing_Stand), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cauldron), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Portal), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Portal_Frame), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Stone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dragon_Egg), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Redstone_Lamp), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lit_Redstone_Lamp), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Double_Wooden_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wooden_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cocoa), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sandstone_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Emerald_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Ender_Chest), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Tripwire_Hook), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Tripwire), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Emerald_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Spruce_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Birch_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Jungle_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Command_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Beacon), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cobblestone_Wall), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Flower_Pot), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Carrots), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Potatoes), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wooden_Button), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Skull), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Anvil), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Trapped_Chest), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Light_Weighted_Pressure_Plate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Heavy_Weighted_Pressure_Plate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Unpowered_Comparator), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Powered_Comparator), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Daylight_Detector), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Redstone_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Quartz_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Hopper), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Quartz_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Quartz_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Activator_Rail), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dropper), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stained_Hardened_Clay), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stained_Glass_Pane), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Leaves2), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Log2), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Acacia_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dark_Oak_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Slime), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Barrier), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Trapdoor), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Prismarine), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sea_Lantern), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Hay_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Carpet), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Hardened_Clay), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Coal_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Packed_Ice), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Double_Plant), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Standing_Banner), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wall_Banner), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Daylight_Detector_Inverted), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Sandstone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Sandstone_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Double_Stone_Slab2), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone_Slab2), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Spruce_Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Birch_Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Jungle_Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dark_Oak_Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Acacia_Fence_Gate), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Spruce_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Birch_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Jungle_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dark_Oak_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Acacia_Fence), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Spruce_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Birch_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Jungle_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Acacia_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dark_Oak_Door), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Rod), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Chorus_Plant), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Chorus_Flower), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purpur_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purpur_Pillar), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purpur_Stairs), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purpur_Double_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purpur_Slab), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Bricks), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Beetroots), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Grass_Path), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::End_Gateway), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Repeating_Command_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Chain_Command_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Frosted_Ice), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Magma), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Nether_Wart_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Nether_Brick), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Bone_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Structure_Void), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Observer), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::White_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Orange_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Magenta_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Light_Blue_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Yellow_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lime_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Pink_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gray_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Silver_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cyan_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purple_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Blue_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brown_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Green_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Black_Shulker_Box), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::White_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Orange_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Magenta_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Light_Blue_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Yellow_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lime_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Pink_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gray_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Silver_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cyan_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Purple_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Blue_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brown_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Green_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Red_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Black_Glazed_Terracotta), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Concrete), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Concrete_Powder), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Structure_Block), 1, 0);

    for (auto& i : quickSelectBar) {
        i = mc::inventory::Slot(static_cast<s32>(BlockID::Air), 0, 0);
    }
}

void Player::Update(Damage* dmg) {
    if (m_input->isLooking) {
        view.x = -sinf(yRot) * cosf(xRot);
        view.y = sinf(xRot);
        view.z = -cosf(yRot) * cosf(xRot);
    }

    blockInSight = Raycast_Cast(m_world, position + mc::Vector3f(0, EyeHeight, 0), view, &viewRayCast);
    blockInActionRange = blockInSight && viewRayCast.distSqr < 3.5f * 3.5f * 3.5f;

    if (gamemode != 1) {
        HandleFallDamage();
        HandleFireDamage();
        HandleHunger();
    }

    HandleRespawn(dmg);
}

void Player::UpdateMovement(DebugUI* dbg, float dt) {
    Damage dmg;

    m_input->update();

    // Look Direction
    float lookUp = getCtrlLook()->up;
    float lookDown = getCtrlLook()->down;
    float lookLeft = getCtrlLook()->left;
    float lookRight = getCtrlLook()->right;
    bool isLook = m_input->isLooking; // cpad not 0
    // Walk Movement
    float moveUp = getCtrlMove()->up / PAD_MAXF;
    float moveDown = getCtrlMove()->down / PAD_MAXF;
    float moveLeft = getCtrlMove()->left / PAD_MAXF;
    float moveRight = getCtrlMove()->right / PAD_MAXF;
    bool isMoveDiff = m_input->isMoveDiff; // stick changed
    bool isMove = m_input->isMoving; // stick not 0
    // Actions
    bool isJump = Input::isPressed(INP_JUMP);
    bool isSneak = Input::isPressed(INP_SNEAK);
    float jump = isJump ? 1.f : 0.f;
    float sneak = isSneak ? 1.f : 0.f;

    // C Stick / Direction
    if (isLook) { // stick used
        // yawPrev = yRot;
        yRot += DEG_TO_RAD(lookLeft + -lookRight) * dt;
        xRot += DEG_TO_RAD(-lookDown + lookUp) * dt;
        xRot = CLAMP(xRot, DEG_TO_RAD(-89.9f), DEG_TO_RAD(89.9f));
    }

    // C Pad / Walking
    if (isMoveDiff || isLook) { // pad changed or stick used
        // if(yRot != yawPrev) { // TODO
        forwardVec = mc::Vector3f(-sinf(yRot), 0.f, -cosf(yRot));
        rightVec = Vector3f_crs(forwardVec, vecY);
        //}

        movement = (forwardVec * (moveUp - moveDown)) + (rightVec * (moveRight - moveLeft));
    } else if (!isMove && isMoveDiff) { // no pad input but diff on last cycle
        movement = vecZero;
    }

    // Calculate Speed
    if (isMoveDiff || isJump || isSneak) {
        speed = MaxWalkVelocity * Vector3f_mag(mc::Vector3f(-moveLeft + moveRight, -sneak + jump, -moveUp + moveDown));
    }

    // Flying
    if (flying) {
        if (Input::isPressed(INP_FLYUP)) {
            movement += vecY;
        } else if (Input::isPressed(INP_FLYDOWN)) {
            movement -= vecY;
        } else if (movement.y != 0.f) {
            movement.y = 0;
        }
    }

    // Apply Speed
    if (Vector3f_MagSqr(movement) > 0.f) {
        bobbing += speed * 3.5f * dt;
        movement.Normalize();
        movement *= speed;
    }

    // TODO: Segfault sometimes when rejoining the world, this is the problem.
    mc::inventory::Slot curSlot = quickSelectBar[quickSelectBarSlot];
    auto curStack = MCBridge::MCLIBSlotToCTItemStack(curSlot);
    bool slotEmpty = (curStack.block == BlockID::Air);

    bool hitEntity = viewRayCast.entity;

    if (Input::isPressed(INP_ATTACK)) {
        if (hitEntity) {
            HurtEntity();
        } else {
            BreakBlock();
        }
    }

    if (Input::isPressed(INP_USE)) {
        if (slotEmpty) {
            Interact(dbg);
        } else {
            PlaceBlock();
        }
    }


    if (Input::isPressed(INP_JUMP)) {
        Jump(mc::Vector3f(movement.x, movement.y, movement.z));
    }


    if (m_flyTimer >= 0.f) {
        if (Input::isPressed(INP_JUMP)) {
            flying ^= true;
        }

        m_flyTimer += dt;
        if (m_flyTimer > 0.25f) {
            m_flyTimer = -1.f;
        }

    } else if (Input::isReleased(INP_JUMP)) {
        m_flyTimer = 0.f;
    }

    releasedCrouch = Input::isReleased(INP_SNEAK);
    crouching ^= !flying && releasedCrouch;

    bool switchBlockLeft = Input::isReleased(INP_HOTBAR_LEFT);
    bool switchBlockRight = Input::isReleased(INP_HOTBAR_RIGHT);

    if (switchBlockLeft && --quickSelectBarSlot == -1) {
        quickSelectBarSlot = 8;
    }

    if (switchBlockRight && ++quickSelectBarSlot == 9) {
        quickSelectBarSlot = 0;
    }
    /*
        if (m_openedCmd) {
            dt = 0.f;
            m_openedCmd = false;
        }

        bool cmdLine = Input::isReleased(INP_CHAT);
        if (cmdLine) {
            CommandLine_Activate(world, this, debugUi);
            m_openedCmd = true;
        }
    */
    Move(dt, mc::Vector3f(movement.x, movement.y, movement.z));
    Update(&dmg);
}

void Player::HandleFallDamage() {
    if (velocity.y <= -12) {
        rndy = round(velocity.y);
        if (m_world->GetBlockID(mc::Vector3i(position.x, position.y - 1, position.z)) != BlockID::Air) {
            hp = hp + rndy;
            rndy = 0;
        }
    }
}

void Player::HandleFireDamage() {
    if (m_world->GetBlockID(ToVector3i(ToVector3d(position))) == BlockID::Lava) {
        //  DebugUI_Log("ur burning lol");
        // OvertimeDamage("Fire", 10);
    }
}

void Player::HandleHunger() {
    svcSleepThread(10000000);
    hungertimer = hungertimer + 1;
    if (hungertimer == 400 && hunger != 0) {
        hunger = hunger - 1;
        hungertimer = 0;
    }
    if (hunger == 0) {
        svcSleepThread(10000000);
        if (hungertimer == 400) {
            hp = hp - 1;
            hungertimer = 0;
        }
    }
}

void Player::HandleRespawn(Damage* dmg) {
    if (hp <= 0) { // Whaaat iis gooing oon heear??!
        if (difficulty != 4) {
            if (!spawnset) {
                if (dmg->cause == NULL) {
                    //  DebugUI_Log("Player died");
                } else {
                    // DebugUI_Log("Died by %s", dmg->cause);
                }
                // DebugUI_Log("No spawn was set");
                position.x = 0.0;

                int spawnY = 1;
                while (m_world->GetBlockID(ToVector3i(spawnPos)) != BlockID::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGenType::SuperFlat;
                position.y = shouldOffset ? spawnY + 1 : spawnY;
                position.z = 0.0;
            }
            if (spawnset) {
                if (dmg->cause == NULL) {
                    //    DebugUI_Log("Player died");
                } else {
                    //     DebugUI_Log("Died by %s", dmg->cause);
                }
                position.x = spawnPos.x;

                int spawnY = 1;
                while (m_world->GetBlockID(ToVector3i(spawnPos)) != BlockID::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGenType::SuperFlat;
                position.y = shouldOffset ? spawnY + 1 : spawnY;
                position.z = spawnPos.z;
            }
            hp = 20;
            hunger = 20;
        } else {
            // DebugUI_Log("lol ur world is gone");
        }
    }
}

void Player::Jump(mc::Vector3f accl) {
    if (onGround && !flying) {
        velocity.x = accl.x * 1.1f;
        velocity.z = accl.z * 1.1f;
        velocity.y = 6.7f;
        jumped = true;
    }
}

void Player::Move(float dt, mc::Vector3f accl) {
    breakPlaceTimeout -= dt;
    simStepAccum += dt;
    const float SimStep = 1.f / 60.f;

    while (simStepAccum >= SimStep) {
        velocity.y -= GravityPlusFriction * SimStep * 2.f;
        if (velocity.y < MaxFallVelocity) {
            velocity.y = MaxFallVelocity;
        }

        if (flying) {
            velocity.y = 0.f;
        }


        float speedFactor = 1.f;
        if (!onGround && !flying) {
            speedFactor = jumped ? 0.2f : 0.6f;
        } else if (flying) {
            speedFactor = 2.f;
        } else if (crouching) {
            speedFactor = 0.5f;
        }

        mc::Vector3f newPos = position + ((velocity * SimStep) + (accl * (SimStep * speedFactor)));

        mc::Vector3f finalPos = position;

        bool wallCollision = false, wasGrounded = onGround;

        onGround = false;
        for (int j = 0; j < 3; j++) {
            int i = (int[]){0, 2, 1}[j];
            bool collision = false;
            mc::Vector3f axisStep = finalPos;
            axisStep.values[i] = newPos.values[i];

            // Player AABB for this axis step
            mc::AABB playerBox(
                mc::Vector3d(axisStep.x - CollisionBoxSize / 2.0, axisStep.y,                axisStep.z - CollisionBoxSize / 2.0),
                mc::Vector3d(axisStep.x + CollisionBoxSize / 2.0, axisStep.y + Height,       axisStep.z + CollisionBoxSize / 2.0)
            );

            for (int x = -1; x < 2; x++) {
                for (int y = 0; y < 3; y++) {
                    for (int z = -1; z < 2; z++) {
                        auto blockPos = mc::Vector3i(FastFloor(axisStep.x) + x, FastFloor(axisStep.y) + y,
                                                     FastFloor(axisStep.z) + z);

                        if (m_world->GetBlockID(blockPos) != BlockID::Air &&
                            m_world->GetBlockID(blockPos) != BlockID::Lava &&
                            m_world->GetBlockID(blockPos) != BlockID::Water) {
                            mc::AABB blockBox(
                                mc::Vector3d(blockPos.x,     blockPos.y,     blockPos.z),
                                mc::Vector3d(blockPos.x + 1, blockPos.y + 1, blockPos.z + 1)
                            );

                            collision |= playerBox.Intersects(blockBox);
                        }
                    }
                }
            }

            if (!collision) {
                finalPos.values[i] = newPos.values[i];
            } else if (i == 1) {
                if (velocity.y < 0.f || accl.y < 0.f) {
                    onGround = true;
                }
                jumped = false;
                velocity.x = 0.f;
                velocity.y = 0.f;
                velocity.z = 0.f;
            } else {
                wallCollision = true;
                if (i == 0) {
                    velocity.x = 0.f;
                } else {
                    velocity.z = 0.f;
                }
            }
        }

        mc::Vector3f movDiff = finalPos - position;

        if (onGround && flying) {
            flying = false;
        }

        if (wallCollision && autoJumpEnabled) {

            mc::Vector3f nrmDiff = newPos - position;
            nrmDiff.Normalize();

            BlockID block = m_world->GetBlockID(mc::Vector3i(FastFloor(finalPos.x + nrmDiff.x),
                                                             FastFloor(finalPos.y + nrmDiff.y) + 2,
                                                             FastFloor(finalPos.z + nrmDiff.z)));
            BlockID landingBlock = m_world->GetBlockID(mc::Vector3i(FastFloor(finalPos.x + nrmDiff.x),
                                                                    FastFloor(finalPos.y + nrmDiff.y) + 1,
                                                                    FastFloor(finalPos.z + nrmDiff.z)));

            if ((block == BlockID::Air || block == BlockID::Lava || block == BlockID::Water) &&
                landingBlock != BlockID::Air && landingBlock != BlockID::Lava && landingBlock != BlockID::Water) {
                Jump(accl);
            }
        }

        if (crouching && crouchAdd > -0.3f) {
            crouchAdd -= SimStep * 2.f;
        }

        if (!crouching && crouchAdd < 0.0f) {
            crouchAdd += SimStep * 2.f;
        }

        if (crouching && !onGround && wasGrounded && finalPos.y < position.y && movDiff.x != 0.f && movDiff.z != 0.f) {
            finalPos = position;
            onGround = true;
            velocity.y = 0.f;
        }

        position = finalPos;

        velocity.x = velocity.x * 0.95f;
        // velocity.y = velocity.y;
        velocity.z = velocity.z * 0.95f;

        if (ABS(velocity.x) < 0.1f) {
            velocity.x = 0.f;
        }

        if (ABS(velocity.z) < 0.1f) {
            velocity.z = 0.f;
        }

        simStepAccum -= SimStep;
    }
}

void Player::PlaceBlock() {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        const int* offset = DirectionToOffset[viewRayCast.direction];
        mc::Vector3i placePos(viewRayCast.hitPos.x + offset[0], viewRayCast.hitPos.y + offset[1],
                              viewRayCast.hitPos.z + offset[2]);

        // Check if block placement would intersect with player
        AABB blockBox = AABB(mc::Vector3f(placePos.x, placePos.y, placePos.z),
                             mc::Vector3f(placePos.x + 1, placePos.y + 1, placePos.z + 1));

        if (bb.Intersect(blockBox)) {
            return;
        }

        // Place the block
        m_world->SetBlockAndMeta(placePos, MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).block,
                                 MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).meta);
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PlaceReplaceTimeout;
    }
}

void Player::BreakBlock() {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        m_world->SetBlockID(viewRayCast.hitPos, BlockID::Air);
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PlaceReplaceTimeout;
    }
}

void Player::HurtEntity() {
    // TODO:
}

// TODO: When triggered, this cuts the o3DS performance in half...
void Player::Interact(DebugUI* dbg) {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        BlockID id = m_world->GetBlockID(viewRayCast.hitPos);
        Metadata meta = m_world->GetMetadata(viewRayCast.hitPos);
        std::string logMsg = "Target at: " + to_string(viewRayCast.hitPos) +
            "  ID=" + std::to_string(static_cast<u8>(id)) + "  Meta=" + std::to_string(meta);

        dbg->Log(logMsg.c_str());
    }
}