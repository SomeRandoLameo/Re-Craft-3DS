#include "blocks/Block.hpp"
#include "ReCraftCore.hpp"
#include "blocks/GrassBlock.h"
#include "blocks/LeavesBlock.h"
#include "util/EnumFacing.hpp"

Block* Block::setHardness(float hardness) {
    m_blockHardness = hardness;

    if (m_blockHardness < hardness * 5.0F) {
        m_blockResistance = hardness * 5.0F;
    }

    return this;
}

Block* Block::setBlockUnbreakable() {
    setHardness(-1.0F);
    return this;
}

Block* Block::setResistance(float resistance) {
    m_blockResistance = resistance * 3.0F;
    return this;
}

Block* Block::setUnlocalizedName(const std::string& name) {
    m_unlocalizedName = name;
    return this;
}

Block* Block::setSoundType(SoundType sound) {
    m_soundType = sound;
    return this;
}

Block* Block::setOpaque(bool opaque) {
    m_opaque = opaque;
    return this;
}

Block* Block::setSolid(bool solid) {
    m_solid = solid;
    return this;
}

Block* Block::setCreativeTab(CreativeTabs tab) {
    m_displayOnCreativeTab = tab;
    return this;
}

Block* Block::setLightLevel(float value) {
    m_lightValue = (int)(15.0F * value);
    return this;
}

Block* Block::setLightOpacity(int opacity) {
    m_lightOpacity = opacity;
    return this;
}

Block* Block::disableStats() {
    m_enableStats = false;
    return this;
}

void Block::getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const {
    out_rgb[0] = 255;
    out_rgb[1] = 255;
    out_rgb[2] = 255;
}

BlockID Block::GetID() const { return m_id; }

float Block::getHardness() const { return m_blockHardness; }

bool Block::isOpaque(Metadata metadata) const { return m_opaque; }

bool Block::isSolid() const { return m_solid; }

SoundType Block::getSoundType() const { return m_soundType; }

uint8_t Block::getLightEmission() const { return m_lightEmission; }


TextureMap BlockRegistry::m_textureMap;

BlockRegistry::BlockRegistry() {}

// TODO: Individual block classes
// clang-format off
void BlockRegistry::Init() {
    m_textureMap.Init("romfs:/assets/minecraft/textures/blocks");

    RegisterBlock(BlockID::Air, "air",
                  (new Block())
                      ->setOpaque(false) //TODO: Remove
                      ->setSolid(false) //TODO: Remove
                      ->setUnlocalizedName("air"));

    RegisterBlock(BlockID::Stone, "stone", (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stone"));
    RegisterBlock(BlockID::Grass, "grass", (new GrassBlock())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("grass"));
    RegisterBlock(BlockID::Dirt, "dirt",   (new Block())->setHardness(0.5F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("dirt"));
    RegisterBlock(BlockID::Cobblestone, "cobblestone", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stonebrick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Planks, "planks", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("wood"));
    RegisterBlock(BlockID::Sapling, "sapling", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("sapling"));
    RegisterBlock(BlockID::Bedrock, "bedrock",(new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("bedrock")->disableStats()->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Flowing_Water, "flowing_water", (new Block())->setHardness(100.0F)->setLightOpacity(3)->setUnlocalizedName("water")->disableStats());
    RegisterBlock(BlockID::Water, "water", (new Block())->setHardness(100.0F)->setLightOpacity(3)->setUnlocalizedName("water")->disableStats());
    RegisterBlock(BlockID::Flowing_Lava, "flowing_lava", (new Block())->setHardness(100.0F)->setLightLevel(1.0F)->setUnlocalizedName("lava")->disableStats());
    RegisterBlock(BlockID::Lava, "lava", (new Block())->setHardness(100.0F)->setLightLevel(1.0F)->setUnlocalizedName("lava")->disableStats());
    RegisterBlock(BlockID::Sand, "sand", (new Block())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("sand"));
    RegisterBlock(BlockID::Gravel, "gravel", (new Block())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("gravel"));
    RegisterBlock(BlockID::Gold_Ore, "gold_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreGold"));
    RegisterBlock(BlockID::Iron_Ore, "iron_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreIron"));
    RegisterBlock(BlockID::Coal_Ore, "coal_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreCoal"));
    RegisterBlock(BlockID::Log, "log", (new Block())->setUnlocalizedName("log"));
    RegisterBlock(BlockID::Leaves, "leaves", (new LeavesBlock())->setUnlocalizedName("leaves"));
    RegisterBlock(BlockID::Sponge, "sponge", (new Block())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("sponge"));
    RegisterBlock(BlockID::Glass, "glass", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("glass"));
    RegisterBlock(BlockID::Lapis_Ore, "lapis_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreLapis"));
    RegisterBlock(BlockID::Lapis_Block, "lapis_block", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("blockLapis")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Dispenser, "dispenser", (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("dispenser"));
    RegisterBlock(BlockID::Sandstone, "sandstone", (new Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("sandStone"));
    RegisterBlock(BlockID::Noteblock, "noteblock", (new Block())->setSoundType(SoundType::WOOD)->setHardness(0.8F)->setUnlocalizedName("musicBlock"));
    RegisterBlock(BlockID::Bed, "bed", (new Block())->setSoundType(SoundType::WOOD)->setHardness(0.2F)->setUnlocalizedName("bed")->disableStats());
    RegisterBlock(BlockID::Golden_Rail, "rail_golden", (new Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("goldenRail"));
    RegisterBlock(BlockID::Detector_Rail, "rail_detector", (new Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("detectorRail"));
    RegisterBlock(BlockID::Sticky_Piston, "sticky_piston", (new Block())->setUnlocalizedName("pistonStickyBase"));
    RegisterBlock(BlockID::Web, "web", (new Block())->setLightOpacity(1)->setHardness(4.0F)->setUnlocalizedName("web"));
    RegisterBlock(BlockID::Tallgrass, "tallgrass", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("tallgrass"));
    RegisterBlock(BlockID::Deadbush, "deadbush", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("deadbush"));
    RegisterBlock(BlockID::Piston, "piston", (new Block())->setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Piston_Head, "piston_head", (new Block())->setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Wool, "wool",(new Block())->setHardness(0.8F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cloth"));
    RegisterBlock(BlockID::Piston_Extension, "piston_extension", (new Block()));
    RegisterBlock(BlockID::Yellow_Flower, "yellow_flower", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("flower1"));
    RegisterBlock(BlockID::Red_Flower, "red_flower", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("flower2"));
    RegisterBlock(BlockID::Brown_Mushroom, "brown_mushroom", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setLightLevel(0.125F)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom, "red_mushroom", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Gold_Block, "gold_block", (new Block())->setHardness(3.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockGold")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Iron_Block, "iron_block", (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockIron")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Double_Stone_Slab, "double_stone_slab", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Stone_Slab, "stone_slab", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Brick_Block, "brick_block", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("brick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Tnt, "tnt", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("tnt"));
    RegisterBlock(BlockID::Bookshelf, "bookshelf", (new Block())->setHardness(1.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("bookshelf"));
    RegisterBlock(BlockID::Mossy_Cobblestone, "mossy_cobblestone", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneMoss")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Obsidian, "obsidian", (new Block())->setHardness(50.0F)->setResistance(2000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("obsidian"));
    RegisterBlock(BlockID::Torch, "torch", (new Block())->setHardness(0.0F)->setLightLevel(0.9375F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("torch"));
    RegisterBlock(BlockID::Fire, "fire", (new Block())->setHardness(0.0F)->setLightLevel(1.0F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("fire")->disableStats());
    RegisterBlock(BlockID::Mob_Spawner, "mob_spawner",(new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("mobSpawner")->disableStats());
    RegisterBlock(BlockID::Oak_Stairs, "oak_stairs", (new Block())->setUnlocalizedName("stairsWood"));
    RegisterBlock(BlockID::Chest, "chest", (new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chest"));
    RegisterBlock(BlockID::Redstone_Wire, "redstone_wire", (new Block())->setHardness(0.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("redstoneDust")->disableStats());
    RegisterBlock(BlockID::Diamond_Ore, "diamond_ore",(new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreDiamond"));
    RegisterBlock(BlockID::Diamond_Block, "diamond_block",(new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockDiamond")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Crafting_Table, "crafting_table",(new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("workbench"));
    RegisterBlock(BlockID::Wheat, "wheat", (new Block())->setUnlocalizedName("crops"));
    RegisterBlock(BlockID::Farmland, "farmland", (new Block())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("farmland"));
    RegisterBlock(BlockID::Furnace, "furnace", (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("furnace")->setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::Lit_Furnace, "lit_furnace", (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setLightLevel(0.875F)->setUnlocalizedName("furnace"));
    RegisterBlock(BlockID::Standing_Sign, "standing_sign", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("sign")->disableStats());
    RegisterBlock(BlockID::Wooden_Door, "wooden_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorOak")->disableStats());
    RegisterBlock(BlockID::Ladder, "ladder", (new Block())->setHardness(0.4F)->setSoundType(SoundType::LADDER)->setUnlocalizedName("ladder"));
    RegisterBlock(BlockID::Rail, "rail", (new Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("rail"));
    RegisterBlock(BlockID::Stone_Stairs, "stone_stairs", (new Block())->setUnlocalizedName("stairsStone"));
    RegisterBlock(BlockID::Wall_Sign, "wall_sign", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("sign")->disableStats());
    RegisterBlock(BlockID::Lever, "lever", (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("lever"));
    RegisterBlock(BlockID::Stone_Pressure_Plate, "stone_pressure_plate", (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("pressurePlateStone"));
    RegisterBlock(BlockID::Iron_Door, "iron_door", (new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("doorIron")->disableStats());
    RegisterBlock(BlockID::Wooden_Pressure_Plate, "wooden_pressure_plate", (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pressurePlateWood"));
    RegisterBlock(BlockID::Redstone_Ore, "redstone_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreRedstone")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Lit_Redstone_Ore, "lit_redstone_ore", (new Block())->setLightLevel(0.625F)->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreRedstone"));
    RegisterBlock(BlockID::Unlit_Redstone_Torch, "unlit_redstone_torch", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("notGate"));
    RegisterBlock(BlockID::Redstone_Torch, "redstone_torch",(new Block())->setHardness(0.0F)->setLightLevel(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("notGate")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Stone_Button, "stone_button", (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("button"));
    RegisterBlock(BlockID::Snow_Layer, "snow_layer", (new Block())->setHardness(0.1F)->setSoundType(SoundType::SNOW)->setUnlocalizedName("snow")->setLightOpacity(0));
    RegisterBlock(BlockID::Ice, "ice", (new Block())->setHardness(0.5F)->setLightOpacity(3)->setSoundType(SoundType::GLASS)->setUnlocalizedName("ice"));
    RegisterBlock(BlockID::Snow, "snow",(new Block())->setHardness(0.2F)->setSoundType(SoundType::SNOW)->setUnlocalizedName("snow"));
    RegisterBlock(BlockID::Cactus, "cactus", (new Block())->setHardness(0.4F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cactus"));
    RegisterBlock(BlockID::Clay, "clay", (new Block())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("clay"));
    RegisterBlock(BlockID::Reeds, "reeds", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("reeds")->disableStats());
    RegisterBlock(BlockID::Jukebox, "jukebox", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("jukebox"));
    RegisterBlock(BlockID::Fence, "fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("fence"));
    RegisterBlock(BlockID::Pumpkin, "pumpkin", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkin"));
    RegisterBlock(BlockID::Netherrack, "netherrack",(new Block())->setHardness(0.4F)->setSoundType(SoundType::STONE)->setUnlocalizedName("hellrock"));
    RegisterBlock(BlockID::Soul_Sand, "soul_sand", (new Block())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("hellsand"));
    RegisterBlock(BlockID::Glowstone, "glowstone", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setLightLevel(1.0F)->setUnlocalizedName("lightgem"));
    RegisterBlock(BlockID::Portal, "portal", (new Block())->setHardness(-1.0F)->setSoundType(SoundType::GLASS)->setLightLevel(0.75F)->setUnlocalizedName("portal"));
    RegisterBlock(BlockID::Lit_Pumpkin, "lit_pumpkin", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setLightLevel(1.0F)->setUnlocalizedName("litpumpkin"));
    RegisterBlock(BlockID::Cake, "cake", (new Block())->setHardness(0.5F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cake")->disableStats());
    RegisterBlock(BlockID::Unpowered_Repeater, "unpowered_repeater", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("diode")->disableStats());
    RegisterBlock(BlockID::Powered_Repeater, "powered_repeater", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("diode")->disableStats());
    RegisterBlock(BlockID::Stained_Glass, "stained_glass", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("stainedGlass"));
    RegisterBlock(BlockID::Trapdoor, "trapdoor", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("trapdoor")->disableStats());
    RegisterBlock(BlockID::Monster_Egg, "monster_egg", (new Block())->setHardness(0.75F)->setUnlocalizedName("monsterStoneEgg"));
    RegisterBlock(BlockID::Stonebrick, "stonebrick", (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stonebricksmooth"));
    RegisterBlock(BlockID::Brown_Mushroom_Block, "brown_mushroom_block", (new Block())->setHardness(0.2F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom_Block, "red_mushroom_block", (new Block())->setHardness(0.2F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Iron_Bars, "iron_bars", (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("fenceIron"));
    RegisterBlock(BlockID::Glass_Pane, "glass_pane", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("thinGlass"));
    RegisterBlock(BlockID::Melon_Block, "melon_block", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("melon"));
    RegisterBlock(BlockID::Pumpkin_Stem, "pumpkin_stem", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Melon_Stem, "melon_stem", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Vine, "vine", (new Block())->setHardness(0.2F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("vine"));
    RegisterBlock(BlockID::Fence_Gate, "fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("fenceGate"));
    RegisterBlock(BlockID::Brick_Stairs, "brick_stairs", (new Block())->setUnlocalizedName("stairsBrick"));
    RegisterBlock(BlockID::Stone_Brick_Stairs, "stone_brick_stairs", (new Block())->setUnlocalizedName("stairsStoneBrickSmooth"));
    RegisterBlock(BlockID::Mycelium, "mycelium", (new Block())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("mycel"));
    RegisterBlock(BlockID::Waterlily, "waterlily", (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("waterlily"));
    RegisterBlock(BlockID::Nether_Brick, "nether_brick", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherBrick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Nether_Brick_Fence, "nether_brick_fence", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherFence"));
    RegisterBlock(BlockID::Nether_Brick_Stairs, "nether_brick_stairs", (new Block())->setUnlocalizedName("stairsNetherBrick"));
    RegisterBlock(BlockID::Nether_Wart, "nether_wart", (new Block())->setUnlocalizedName("netherStalk"));
    RegisterBlock(BlockID::Enchanting_Table, "enchanting_table", (new Block())->setHardness(5.0F)->setResistance(2000.0F)->setUnlocalizedName("enchantmentTable"));
    RegisterBlock(BlockID::Brewing_Stand, "brewing_stand", (new Block())->setHardness(0.5F)->setLightLevel(0.125F)->setUnlocalizedName("brewingStand"));
    RegisterBlock(BlockID::Cauldron, "cauldron", (new Block())->setHardness(2.0F)->setUnlocalizedName("cauldron"));
    RegisterBlock(BlockID::End_Portal, "end_portal", (new Block())->setHardness(-1.0F)->setResistance(6000000.0F));
    RegisterBlock(BlockID::End_Portal_Frame, "end_portal_frame", (new Block())->setSoundType(SoundType::GLASS)->setLightLevel(0.125F)->setHardness(-1.0F)->setUnlocalizedName("endPortalFrame")->setResistance(6000000.0F)->setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::End_Stone, "end_stone", (new Block())->setHardness(3.0F)->setResistance(15.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("whiteStone")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Dragon_Egg, "dragon_egg", (new Block())->setHardness(3.0F)->setResistance(15.0F)->setSoundType(SoundType::STONE)->setLightLevel(0.125F)->setUnlocalizedName("dragonEgg"));
    RegisterBlock(BlockID::Redstone_Lamp, "redstone_lamp", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("redstoneLight")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Lit_Redstone_Lamp, "lit_redstone_lamp", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("redstoneLight"));
    RegisterBlock(BlockID::Double_Wooden_Slab, "double_wooden_slab", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Wooden_Slab, "wooden_slab", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Cocoa, "cocoa", (new Block())->setHardness(0.2F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("cocoa"));
    RegisterBlock(BlockID::Sandstone_Stairs, "sandstone_stairs", (new Block())->setUnlocalizedName("stairsSandStone"));
    RegisterBlock(BlockID::Emerald_Ore, "emerald_ore",(new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreEmerald"));
    RegisterBlock(BlockID::Ender_Chest, "ender_chest", (new Block())->setHardness(22.5F)->setResistance(1000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("enderChest")->setLightLevel(0.5F));
    RegisterBlock(BlockID::Tripwire_Hook, "tripwire_hook", (new Block())->setUnlocalizedName("tripWireSource"));
    RegisterBlock(BlockID::Tripwire, "tripwire", (new Block())->setUnlocalizedName("tripWire"));
    RegisterBlock(BlockID::Emerald_Block, "emerald_block",(new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockEmerald")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Spruce_Stairs, "spruce_stairs", (new Block())->setUnlocalizedName("stairsWoodSpruce"));
    RegisterBlock(BlockID::Birch_Stairs, "birch_stairs", (new Block())->setUnlocalizedName("stairsWoodBirch"));
    RegisterBlock(BlockID::Jungle_Stairs, "jungle_stairs", (new Block())->setUnlocalizedName("stairsWoodJungle"));
    RegisterBlock(BlockID::Command_Block, "command_block", (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("commandBlock"));
    RegisterBlock(BlockID::Beacon, "beacon", (new Block())->setUnlocalizedName("beacon")->setLightLevel(1.0F));
    RegisterBlock(BlockID::Cobblestone_Wall, "cobblestone_wall", (new Block())->setUnlocalizedName("cobbleWall"));
    RegisterBlock(BlockID::Flower_Pot, "flower_pot", (new Block())->setHardness(0.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("flowerPot"));
    RegisterBlock(BlockID::Carrots, "carrots", (new Block())->setUnlocalizedName("carrots"));
    RegisterBlock(BlockID::Potatoes, "potatoes", (new Block())->setUnlocalizedName("potatoes"));
    RegisterBlock(BlockID::Wooden_Button, "wooden_button", (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("button"));
    RegisterBlock(BlockID::Skull, "skull", (new Block())->setHardness(1.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("skull"));
    RegisterBlock(BlockID::Anvil, "anvil", (new Block())->setHardness(5.0F)->setSoundType(SoundType::ANVIL)->setResistance(2000.0F)->setUnlocalizedName("anvil"));
    RegisterBlock(BlockID::Trapped_Chest, "trapped_chest", (new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chestTrap"));
    RegisterBlock(BlockID::Light_Weighted_Pressure_Plate, "light_weighted_pressure_plate", (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("weightedPlate_light"));
    RegisterBlock(BlockID::Heavy_Weighted_Pressure_Plate, "heavy_weighted_pressure_plate", (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("weightedPlate_heavy"));
    RegisterBlock(BlockID::Unpowered_Comparator, "unpowered_comparator", (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("comparator")->disableStats());
    RegisterBlock(BlockID::Powered_Comparator, "powered_comparator", (new Block())->setHardness(0.0F)->setLightLevel(0.625F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("comparator")->disableStats());
    RegisterBlock(BlockID::Daylight_Detector, "daylight_detector", (new Block()));
    RegisterBlock(BlockID::Redstone_Block, "redstone_block", (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockRedstone")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Quartz_Ore, "quartz_ore", (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherquartz"));
    RegisterBlock(BlockID::Hopper, "hopper", (new Block())->setHardness(3.0F)->setResistance(8.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("hopper"));
    RegisterBlock(BlockID::Quartz_Block, "quartz_block", (new Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("quartzBlock"));
    RegisterBlock(BlockID::Quartz_Stairs, "quartz_stairs", (new Block())->setUnlocalizedName("stairsQuartz"));
    RegisterBlock(BlockID::Activator_Rail, "activator_rail", (new Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("activatorRail"));
    RegisterBlock(BlockID::Dropper, "dropper", (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("dropper"));
    RegisterBlock(BlockID::Stained_Hardened_Clay, "stained_hardened_clay", (new Block())->setHardness(1.25F)->setResistance(7.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("clayHardenedStained"));
    RegisterBlock(BlockID::Stained_Glass_Pane, "stained_glass_pane", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("thinStainedGlass"));
    RegisterBlock(BlockID::Leaves2, "leaves2", (new Block())->setUnlocalizedName("leaves"));
    RegisterBlock(BlockID::Log2, "log2", (new Block())->setUnlocalizedName("log"));
    RegisterBlock(BlockID::Acacia_Stairs, "acacia_stairs", (new Block())->setUnlocalizedName("stairsWoodAcacia"));
    RegisterBlock(BlockID::Dark_Oak_Stairs, "dark_oak_stairs", (new Block())->setUnlocalizedName("stairsWoodDarkOak"));
    RegisterBlock(BlockID::Slime, "slime", (new Block())->setUnlocalizedName("slime")->setSoundType(SoundType::SLIME));
    RegisterBlock(BlockID::Barrier, "barrier", (new Block())->setUnlocalizedName("barrier"));
    RegisterBlock(BlockID::Iron_Trapdoor, "iron_trapdoor", (new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("ironTrapdoor")->disableStats());
    RegisterBlock(BlockID::Prismarine, "prismarine", (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("prismarine"));
    RegisterBlock(BlockID::Sea_Lantern, "sea_lantern", (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setLightLevel(1.0F)->setUnlocalizedName("seaLantern"));
    RegisterBlock(BlockID::Hay_Block, "hay_block", (new Block())->setHardness(0.5F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("hayBlock")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Carpet, "carpet", (new Block())->setHardness(0.1F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("woolCarpet")->setLightOpacity(0));
    RegisterBlock(BlockID::Hardened_Clay, "hardened_clay", (new Block())->setHardness(1.25F)->setResistance(7.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("clayHardened"));
    RegisterBlock(BlockID::Coal_Block, "coal_block",(new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("blockCoal")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Packed_Ice, "packed_ice", (new Block())->setHardness(0.5F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("icePacked"));
    RegisterBlock(BlockID::Double_Plant, "double_plant", (new Block()));
    RegisterBlock(BlockID::Standing_Banner, "standing_banner", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("banner")->disableStats());
    RegisterBlock(BlockID::Wall_Banner, "wall_banner", (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("banner")->disableStats());
    RegisterBlock(BlockID::Daylight_Detector_Inverted, "daylight_detector_inverted", (new Block()));
    RegisterBlock(BlockID::Red_Sandstone, "red_sandstone", (new Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("redSandStone"));
    RegisterBlock(BlockID::Red_Sandstone_Stairs, "red_sandstone_stairs", (new Block())->setUnlocalizedName("stairsRedSandStone"));
    RegisterBlock(BlockID::Double_Stone_Slab2, "double_stone_slab2", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Stone_Slab2, "stone_slab2", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Spruce_Fence_Gate, "spruce_fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("spruceFenceGate"));
    RegisterBlock(BlockID::Birch_Fence_Gate, "birch_fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("birchFenceGate"));
    RegisterBlock(BlockID::Jungle_Fence_Gate, "jungle_fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("jungleFenceGate"));
    RegisterBlock(BlockID::Dark_Oak_Fence_Gate, "dark_oak_fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("darkOakFenceGate"));
    RegisterBlock(BlockID::Acacia_Fence_Gate, "acacia_fence_gate", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("acaciaFenceGate"));
    RegisterBlock(BlockID::Spruce_Fence, "spruce_fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("spruceFence"));
    RegisterBlock(BlockID::Birch_Fence, "birch_fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("birchFence"));
    RegisterBlock(BlockID::Jungle_Fence, "jungle_fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("jungleFence"));
    RegisterBlock(BlockID::Dark_Oak_Fence, "dark_oak_fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("darkOakFence"));
    RegisterBlock(BlockID::Acacia_Fence, "acacia_fence", (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("acaciaFence"));
    RegisterBlock(BlockID::Spruce_Door, "spruce_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorSpruce")->disableStats());
    RegisterBlock(BlockID::Birch_Door, "birch_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorBirch")->disableStats());
    RegisterBlock(BlockID::Jungle_Door, "jungle_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorJungle")->disableStats());
    RegisterBlock(BlockID::Acacia_Door, "acacia_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorAcacia")->disableStats());
    RegisterBlock(BlockID::Dark_Oak_Door, "dark_oak_door", (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorDarkOak")->disableStats());
    RegisterBlock(BlockID::End_Rod, "end_rod", (new Block())->setHardness(0.0F)->setLightLevel(0.9375F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("endRod"));
    RegisterBlock(BlockID::Chorus_Plant, "chorus_plant", (new Block())->setHardness(0.4F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chorusPlant"));
    RegisterBlock(BlockID::Chorus_Flower, "chorus_flower", (new Block())->setHardness(0.4F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chorusFlower"));
    RegisterBlock(BlockID::Purpur_Block, "purpur_block", (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("purpurBlock"));
    RegisterBlock(BlockID::Purpur_Pillar, "purpur_pillar", (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("purpurPillar"));
    RegisterBlock(BlockID::Purpur_Stairs, "purpur_stairs", (new Block())->setUnlocalizedName("stairsPurpur"));
    RegisterBlock(BlockID::Purpur_Double_Slab, "purpur_double_slab", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::Purpur_Slab, "purpur_slab", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::End_Bricks, "end_bricks", (new Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("endBricks"));
    RegisterBlock(BlockID::Beetroots, "beetroots", (new Block())->setUnlocalizedName("beetroots"));
    RegisterBlock(BlockID::Grass_Path, "grass_path",(new Block())->setHardness(0.65F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("grassPath")->disableStats());
    RegisterBlock(BlockID::End_Gateway, "end_gateway", (new Block())->setHardness(-1.0F)->setResistance(6000000.0F));
    RegisterBlock(BlockID::Repeating_Command_Block, "repeating_command_block", (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("repeatingCommandBlock"));
    RegisterBlock(BlockID::Chain_Command_Block, "chain_command_block", (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("chainCommandBlock"));
    RegisterBlock(BlockID::Frosted_Ice, "frosted_ice", (new Block())->setHardness(0.5F)->setLightOpacity(3)->setSoundType(SoundType::GLASS)->setUnlocalizedName("frostedIce"));
    RegisterBlock(BlockID::Magma, "magma", (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("magma"));
    RegisterBlock(BlockID::Nether_Wart_Block, "nether_wart_block", (new Block())->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("netherWartBlock"));
    RegisterBlock(BlockID::Red_Nether_Brick, "red_nether_brick", (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("redNetherBrick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Bone_Block, "bone_block", (new Block())->setUnlocalizedName("boneBlock"));
    RegisterBlock(BlockID::Structure_Void, "structure_void", (new Block())->setUnlocalizedName("structureVoid"));
    RegisterBlock(BlockID::Observer, "observer", (new Block())->setHardness(3.0F)->setUnlocalizedName("observer"));
    RegisterBlock(BlockID::White_Shulker_Box, "white_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxWhite"));
    RegisterBlock(BlockID::Orange_Shulker_Box, "orange_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxOrange"));
    RegisterBlock(BlockID::Magenta_Shulker_Box, "magenta_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxMagenta"));
    RegisterBlock(BlockID::Light_Blue_Shulker_Box, "light_blue_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxLightBlue"));
    RegisterBlock(BlockID::Yellow_Shulker_Box, "yellow_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxYellow"));
    RegisterBlock(BlockID::Lime_Shulker_Box, "lime_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxLime"));
    RegisterBlock(BlockID::Pink_Shulker_Box, "pink_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxPink"));
    RegisterBlock(BlockID::Gray_Shulker_Box, "gray_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxGray"));
    RegisterBlock(BlockID::Silver_Shulker_Box, "silver_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxSilver"));
    RegisterBlock(BlockID::Cyan_Shulker_Box, "cyan_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxCyan"));
    RegisterBlock(BlockID::Purple_Shulker_Box, "purple_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxPurple"));
    RegisterBlock(BlockID::Blue_Shulker_Box, "blue_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBlue"));
    RegisterBlock(BlockID::Brown_Shulker_Box, "brown_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBrown"));
    RegisterBlock(BlockID::Green_Shulker_Box, "green_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxGreen"));
    RegisterBlock(BlockID::Red_Shulker_Box, "red_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxRed"));
    RegisterBlock(BlockID::Black_Shulker_Box, "black_shulker_box", (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBlack"));
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
    RegisterBlock(BlockID::Concrete, "concrete", (new Block())->setHardness(1.8F)->setSoundType(SoundType::STONE)->setUnlocalizedName("concrete"));
    RegisterBlock(BlockID::Concrete_Powder, "concrete_powder", (new Block())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("concretePowder"));
    RegisterBlock(BlockID::Structure_Block, "structure_block",(new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("structureBlock"));
}
// clang-format on
BlockRegistry::~BlockRegistry() {}

void BlockRegistry::GetTextureUV(BlockID block, uint8_t metadata, Direction direction, int16_t* out_uv) {
    out_uv[0] = 0;
    out_uv[1] = 0;

    static const EnumFacing::Value dirToFacing[] = {
        EnumFacing::WEST, // West
        EnumFacing::EAST, // East
        EnumFacing::DOWN, // Bottom
        EnumFacing::UP, // Top
        EnumFacing::NORTH, // North
        EnumFacing::SOUTH, // South
    };

    const BakedBlockVariant* variant = ReCraftCore::GetInstance()->GetModelBakery()->getVariant(block, metadata);
    EnumFacing::Value facing = dirToFacing[direction];

    if (!variant || !variant->model)
        return;

    for (const BlockPart& part : variant->model->getElements()) {
        auto faceIt = part.mapFaces.find(facing);

        if (faceIt != part.mapFaces.end()) {
            std::string texName = variant->model->resolveTextureName(faceIt->second.texture);

            size_t slashPos = texName.rfind('/');
            std::string filename =
                (slashPos != std::string::npos) ? texName.substr(slashPos + 1) + ".png" : texName + ".png";

            const TextureMap::Icon& icon = BlockRegistry::GetTextureMapEx()->Get(filename.c_str());
            out_uv[0] = icon.u;
            out_uv[1] = icon.v;
            return;
        }
    }
}
