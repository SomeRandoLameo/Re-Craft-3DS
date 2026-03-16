#include "block/Block.hpp"
#include "ReCraftCore.hpp"
#include "block/BlockGrass.h"
#include "block/BlockLeaves.h"
#include "block/BlockStone.h"
#include "util/EnumFacing.hpp"

Block& Block::setHardness(float hardness) {
    m_blockHardness = hardness;

    if (m_blockHardness < hardness * 5.0F) {
        m_blockResistance = hardness * 5.0F;
    }

    return *this;
}

Block& Block::setBlockUnbreakable() {
    setHardness(-1.0F);
    return *this;
}

Block& Block::setResistance(float resistance) {
    m_blockResistance = resistance * 3.0F;
    return *this;
}

Block& Block::setUnlocalizedName(const std::string& name) {
    m_unlocalizedName = name;
    return *this;
}

Block& Block::setSoundType(SoundType sound) {
    m_soundType = sound;
    return *this;
}

Block& Block::setOpaque(bool opaque) {
    m_opaque = opaque;
    return *this;
}

Block& Block::setSolid(bool solid) {
    m_solid = solid;
    return *this;
}

Block& Block::setCreativeTab(CreativeTabs tab) {
    m_displayOnCreativeTab = tab;
    return *this;
}

Block& Block::setLightLevel(float value) {
    m_lightValue = (int)(15.0F * value);
    return *this;
}

Block& Block::setLightOpacity(int opacity) {
    m_lightOpacity = opacity;
    return *this;
}

Block& Block::disableStats() {
    m_enableStats = false;
    return *this;
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

    RegisterBlock(BlockID::Air,
                  Block()
                      .setOpaque(false) //TODO: Remove
                      .setSolid(false) //TODO: Remove
                      .setUnlocalizedName("air"));

    RegisterBlock(BlockID::Stone, BlockStone().setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stone"));
    RegisterBlock(BlockID::Grass, BlockGrass().setHardness(0.6F).setSoundType(SoundType::PLANT).setUnlocalizedName("grass"));
    RegisterBlock(BlockID::Dirt, Block().setHardness(0.5F).setSoundType(SoundType::GROUND).setUnlocalizedName("dirt"));
    RegisterBlock(BlockID::Cobblestone, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stonebrick").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Planks, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("wood"));
    RegisterBlock(BlockID::Sapling, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("sapling"));
    RegisterBlock(BlockID::Bedrock, Block().setBlockUnbreakable().setResistance(6000000.0F).setSoundType(SoundType::STONE).setUnlocalizedName("bedrock").disableStats().setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Flowing_Water, Block().setHardness(100.0F).setLightOpacity(3).setUnlocalizedName("water").disableStats());
    RegisterBlock(BlockID::Water, Block().setHardness(100.0F).setLightOpacity(3).setUnlocalizedName("water").disableStats());
    RegisterBlock(BlockID::Flowing_Lava, Block().setHardness(100.0F).setLightLevel(1.0F).setUnlocalizedName("lava").disableStats());
    RegisterBlock(BlockID::Lava, Block().setHardness(100.0F).setLightLevel(1.0F).setUnlocalizedName("lava").disableStats());
    RegisterBlock(BlockID::Sand, Block().setHardness(0.5F).setSoundType(SoundType::SAND).setUnlocalizedName("sand"));
    RegisterBlock(BlockID::Gravel, Block().setHardness(0.6F).setSoundType(SoundType::GROUND).setUnlocalizedName("gravel"));
    RegisterBlock(BlockID::Gold_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreGold"));
    RegisterBlock(BlockID::Iron_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreIron"));
    RegisterBlock(BlockID::Coal_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreCoal"));
    RegisterBlock(BlockID::Log, Block().setUnlocalizedName("log"));
    RegisterBlock(BlockID::Leaves, BlockLeaves().setUnlocalizedName("leaves"));
    RegisterBlock(BlockID::Sponge, Block().setHardness(0.6F).setSoundType(SoundType::PLANT).setUnlocalizedName("sponge"));
    RegisterBlock(BlockID::Glass, Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("glass"));
    RegisterBlock(BlockID::Lapis_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreLapis"));
    RegisterBlock(BlockID::Lapis_Block, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("blockLapis").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Dispenser, Block().setHardness(3.5F).setSoundType(SoundType::STONE).setUnlocalizedName("dispenser"));
    RegisterBlock(BlockID::Sandstone, Block().setSoundType(SoundType::STONE).setHardness(0.8F).setUnlocalizedName("sandStone"));
    RegisterBlock(BlockID::Noteblock, Block().setSoundType(SoundType::WOOD).setHardness(0.8F).setUnlocalizedName("musicBlock"));
    RegisterBlock(BlockID::Bed, Block().setSoundType(SoundType::WOOD).setHardness(0.2F).setUnlocalizedName("bed").disableStats());
    RegisterBlock(BlockID::Golden_Rail, Block().setHardness(0.7F).setSoundType(SoundType::METAL).setUnlocalizedName("goldenRail"));
    RegisterBlock(BlockID::Detector_Rail, Block().setHardness(0.7F).setSoundType(SoundType::METAL).setUnlocalizedName("detectorRail"));
    RegisterBlock(BlockID::Sticky_Piston, Block().setUnlocalizedName("pistonStickyBase"));
    RegisterBlock(BlockID::Web, Block().setLightOpacity(1).setHardness(4.0F).setUnlocalizedName("web"));
    RegisterBlock(BlockID::Tallgrass, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("tallgrass"));
    RegisterBlock(BlockID::Deadbush, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("deadbush"));
    RegisterBlock(BlockID::Piston, Block().setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Piston_Head, Block().setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Wool, Block().setHardness(0.8F).setSoundType(SoundType::CLOTH).setUnlocalizedName("cloth"));
    RegisterBlock(BlockID::Piston_Extension, Block());
    RegisterBlock(BlockID::Yellow_Flower, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("flower1"));
    RegisterBlock(BlockID::Red_Flower, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("flower2"));
    RegisterBlock(BlockID::Brown_Mushroom, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setLightLevel(0.125F).setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Gold_Block, Block().setHardness(3.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("blockGold").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Iron_Block, Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("blockIron").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Double_Stone_Slab, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Stone_Slab, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Brick_Block, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("brick").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Tnt, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("tnt"));
    RegisterBlock(BlockID::Bookshelf, Block().setHardness(1.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("bookshelf"));
    RegisterBlock(BlockID::Mossy_Cobblestone, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stoneMoss").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Obsidian, Block().setHardness(50.0F).setResistance(2000.0F).setSoundType(SoundType::STONE).setUnlocalizedName("obsidian"));
    RegisterBlock(BlockID::Torch, Block().setHardness(0.0F).setLightLevel(0.9375F).setSoundType(SoundType::WOOD).setUnlocalizedName("torch"));
    RegisterBlock(BlockID::Fire, Block().setHardness(0.0F).setLightLevel(1.0F).setSoundType(SoundType::CLOTH).setUnlocalizedName("fire").disableStats());
    RegisterBlock(BlockID::Mob_Spawner, Block().setHardness(5.0F).setSoundType(SoundType::METAL).setUnlocalizedName("mobSpawner").disableStats());
    RegisterBlock(BlockID::Oak_Stairs, Block().setUnlocalizedName("stairsWood"));
    RegisterBlock(BlockID::Chest, Block().setHardness(2.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("chest"));
    RegisterBlock(BlockID::Redstone_Wire, Block().setHardness(0.0F).setSoundType(SoundType::STONE).setUnlocalizedName("redstoneDust").disableStats());
    RegisterBlock(BlockID::Diamond_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreDiamond"));
    RegisterBlock(BlockID::Diamond_Block, Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("blockDiamond").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Crafting_Table, Block().setHardness(2.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("workbench"));
    RegisterBlock(BlockID::Wheat, Block().setUnlocalizedName("crops"));
    RegisterBlock(BlockID::Farmland, Block().setHardness(0.6F).setSoundType(SoundType::GROUND).setUnlocalizedName("farmland"));
    RegisterBlock(BlockID::Furnace, Block().setHardness(3.5F).setSoundType(SoundType::STONE).setUnlocalizedName("furnace").setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::Lit_Furnace,  Block().setHardness(3.5F).setSoundType(SoundType::STONE).setLightLevel(0.875F).setUnlocalizedName("furnace"));
    RegisterBlock(BlockID::Standing_Sign, Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("sign").disableStats());
    RegisterBlock(BlockID::Wooden_Door, Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorOak").disableStats());
    RegisterBlock(BlockID::Ladder, Block().setHardness(0.4F).setSoundType(SoundType::LADDER).setUnlocalizedName("ladder"));
    RegisterBlock(BlockID::Rail, Block().setHardness(0.7F).setSoundType(SoundType::METAL).setUnlocalizedName("rail"));
    RegisterBlock(BlockID::Stone_Stairs, Block().setUnlocalizedName("stairsStone"));
    RegisterBlock(BlockID::Wall_Sign, Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("sign").disableStats());
    RegisterBlock(BlockID::Lever, Block().setHardness(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("lever"));
    RegisterBlock(BlockID::Stone_Pressure_Plate, Block().setHardness(0.5F).setSoundType(SoundType::STONE).setUnlocalizedName("pressurePlateStone"));
    RegisterBlock(BlockID::Iron_Door, Block().setHardness(5.0F).setSoundType(SoundType::METAL).setUnlocalizedName("doorIron").disableStats());
    RegisterBlock(BlockID::Wooden_Pressure_Plate, Block().setHardness(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("pressurePlateWood"));
    RegisterBlock(BlockID::Redstone_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreRedstone").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Lit_Redstone_Ore, Block().setLightLevel(0.625F).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreRedstone"));
    RegisterBlock(BlockID::Unlit_Redstone_Torch, Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("notGate"));
    RegisterBlock(BlockID::Redstone_Torch, Block().setHardness(0.0F).setLightLevel(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("notGate").setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Stone_Button, Block().setHardness(0.5F).setSoundType(SoundType::STONE).setUnlocalizedName("button"));
    RegisterBlock(BlockID::Snow_Layer, Block().setHardness(0.1F).setSoundType(SoundType::SNOW).setUnlocalizedName("snow").setLightOpacity(0));
    RegisterBlock(BlockID::Ice, Block().setHardness(0.5F).setLightOpacity(3).setSoundType(SoundType::GLASS).setUnlocalizedName("ice"));
    RegisterBlock(BlockID::Snow, Block().setHardness(0.2F).setSoundType(SoundType::SNOW).setUnlocalizedName("snow"));
    RegisterBlock(BlockID::Cactus, Block().setHardness(0.4F).setSoundType(SoundType::CLOTH).setUnlocalizedName("cactus"));
    RegisterBlock(BlockID::Clay, Block().setHardness(0.6F).setSoundType(SoundType::GROUND).setUnlocalizedName("clay"));
    RegisterBlock(BlockID::Reeds, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("reeds").disableStats());
    RegisterBlock(BlockID::Jukebox, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("jukebox"));
    RegisterBlock(BlockID::Fence, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("fence"));
    RegisterBlock(BlockID::Pumpkin, Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("pumpkin"));
    RegisterBlock(BlockID::Netherrack, Block().setHardness(0.4F).setSoundType(SoundType::STONE).setUnlocalizedName("hellrock"));
    RegisterBlock(BlockID::Soul_Sand, Block().setHardness(0.5F).setSoundType(SoundType::SAND).setUnlocalizedName("hellsand"));
    RegisterBlock(BlockID::Glowstone, Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setLightLevel(1.0F).setUnlocalizedName("lightgem"));
    RegisterBlock(BlockID::Portal, Block().setHardness(-1.0F).setSoundType(SoundType::GLASS).setLightLevel(0.75F).setUnlocalizedName("portal"));
    RegisterBlock(BlockID::Lit_Pumpkin, Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setLightLevel(1.0F).setUnlocalizedName("litpumpkin"));
    RegisterBlock(BlockID::Cake, Block().setHardness(0.5F).setSoundType(SoundType::CLOTH).setUnlocalizedName("cake").disableStats());
    RegisterBlock(BlockID::Unpowered_Repeater, Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("diode").disableStats());
    RegisterBlock(BlockID::Powered_Repeater, Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("diode").disableStats());
    RegisterBlock(BlockID::Stained_Glass,  Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("stainedGlass"));
    RegisterBlock(BlockID::Trapdoor, Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("trapdoor").disableStats());
    RegisterBlock(BlockID::Monster_Egg, Block().setHardness(0.75F).setUnlocalizedName("monsterStoneEgg"));
    RegisterBlock(BlockID::Stonebrick, Block().setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stonebricksmooth"));
    RegisterBlock(BlockID::Brown_Mushroom_Block, Block().setHardness(0.2F).setSoundType(SoundType::WOOD).setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom_Block, Block().setHardness(0.2F).setSoundType(SoundType::WOOD).setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Iron_Bars, Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("fenceIron"));
    RegisterBlock(BlockID::Glass_Pane, Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("thinGlass"));
    RegisterBlock(BlockID::Melon_Block, Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("melon"));
    RegisterBlock(BlockID::Pumpkin_Stem, Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Melon_Stem,Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Vine, Block().setHardness(0.2F).setSoundType(SoundType::PLANT).setUnlocalizedName("vine"));
    RegisterBlock(BlockID::Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("fenceGate"));
    RegisterBlock(BlockID::Brick_Stairs, Block().setUnlocalizedName("stairsBrick"));
    RegisterBlock(BlockID::Stone_Brick_Stairs,  Block().setUnlocalizedName("stairsStoneBrickSmooth"));
    RegisterBlock(BlockID::Mycelium, Block().setHardness(0.6F).setSoundType(SoundType::PLANT).setUnlocalizedName("mycel"));
    RegisterBlock(BlockID::Waterlily, Block().setHardness(0.0F).setSoundType(SoundType::PLANT).setUnlocalizedName("waterlily"));
    RegisterBlock(BlockID::Nether_Brick,  Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("netherBrick").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Nether_Brick_Fence, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("netherFence"));
    RegisterBlock(BlockID::Nether_Brick_Stairs, Block().setUnlocalizedName("stairsNetherBrick"));
    RegisterBlock(BlockID::Nether_Wart, Block().setUnlocalizedName("netherStalk"));
    RegisterBlock(BlockID::Enchanting_Table, Block().setHardness(5.0F).setResistance(2000.0F).setUnlocalizedName("enchantmentTable"));
    RegisterBlock(BlockID::Brewing_Stand, Block().setHardness(0.5F).setLightLevel(0.125F).setUnlocalizedName("brewingStand"));
    RegisterBlock(BlockID::Cauldron, Block().setHardness(2.0F).setUnlocalizedName("cauldron"));
    RegisterBlock(BlockID::End_Portal,  Block().setHardness(-1.0F).setResistance(6000000.0F));
    RegisterBlock(BlockID::End_Portal_Frame, Block().setSoundType(SoundType::GLASS).setLightLevel(0.125F).setHardness(-1.0F).setUnlocalizedName("endPortalFrame").setResistance(6000000.0F).setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::End_Stone, Block().setHardness(3.0F).setResistance(15.0F).setSoundType(SoundType::STONE).setUnlocalizedName("whiteStone").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Dragon_Egg, Block().setHardness(3.0F).setResistance(15.0F).setSoundType(SoundType::STONE).setLightLevel(0.125F).setUnlocalizedName("dragonEgg"));
    RegisterBlock(BlockID::Redstone_Lamp,  Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("redstoneLight").setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Lit_Redstone_Lamp, Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("redstoneLight"));
    RegisterBlock(BlockID::Double_Wooden_Slab,  Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Wooden_Slab,  Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Cocoa, Block().setHardness(0.2F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("cocoa"));
    RegisterBlock(BlockID::Sandstone_Stairs,  Block().setUnlocalizedName("stairsSandStone"));
    RegisterBlock(BlockID::Emerald_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("oreEmerald"));
    RegisterBlock(BlockID::Ender_Chest, Block().setHardness(22.5F).setResistance(1000.0F).setSoundType(SoundType::STONE).setUnlocalizedName("enderChest").setLightLevel(0.5F));
    RegisterBlock(BlockID::Tripwire_Hook,  Block().setUnlocalizedName("tripWireSource"));
    RegisterBlock(BlockID::Tripwire, Block().setUnlocalizedName("tripWire"));
    RegisterBlock(BlockID::Emerald_Block, Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("blockEmerald").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Spruce_Stairs, Block().setUnlocalizedName("stairsWoodSpruce"));
    RegisterBlock(BlockID::Birch_Stairs, Block().setUnlocalizedName("stairsWoodBirch"));
    RegisterBlock(BlockID::Jungle_Stairs, Block().setUnlocalizedName("stairsWoodJungle"));
    RegisterBlock(BlockID::Command_Block,  Block().setBlockUnbreakable().setResistance(6000000.0F).setUnlocalizedName("commandBlock"));
    RegisterBlock(BlockID::Beacon,  Block().setUnlocalizedName("beacon").setLightLevel(1.0F));
    RegisterBlock(BlockID::Cobblestone_Wall,  Block().setUnlocalizedName("cobbleWall"));
    RegisterBlock(BlockID::Flower_Pot, Block().setHardness(0.0F).setSoundType(SoundType::STONE).setUnlocalizedName("flowerPot"));
    RegisterBlock(BlockID::Carrots, Block().setUnlocalizedName("carrots"));
    RegisterBlock(BlockID::Potatoes, Block().setUnlocalizedName("potatoes"));
    RegisterBlock(BlockID::Wooden_Button, Block().setHardness(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("button"));
    RegisterBlock(BlockID::Skull, Block().setHardness(1.0F).setSoundType(SoundType::STONE).setUnlocalizedName("skull"));
    RegisterBlock(BlockID::Anvil,  Block().setHardness(5.0F).setSoundType(SoundType::ANVIL).setResistance(2000.0F).setUnlocalizedName("anvil"));
    RegisterBlock(BlockID::Trapped_Chest, Block().setHardness(2.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("chestTrap"));
    RegisterBlock(BlockID::Light_Weighted_Pressure_Plate, Block().setHardness(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("weightedPlate_light"));
    RegisterBlock(BlockID::Heavy_Weighted_Pressure_Plate,  Block().setHardness(0.5F).setSoundType(SoundType::WOOD).setUnlocalizedName("weightedPlate_heavy"));
    RegisterBlock(BlockID::Unpowered_Comparator,  Block().setHardness(0.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("comparator").disableStats());
    RegisterBlock(BlockID::Powered_Comparator,  Block().setHardness(0.0F).setLightLevel(0.625F).setSoundType(SoundType::WOOD).setUnlocalizedName("comparator").disableStats());
    RegisterBlock(BlockID::Daylight_Detector,  Block());
    RegisterBlock(BlockID::Redstone_Block, Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::METAL).setUnlocalizedName("blockRedstone").setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Quartz_Ore, Block().setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType::STONE).setUnlocalizedName("netherquartz"));
    RegisterBlock(BlockID::Hopper, Block().setHardness(3.0F).setResistance(8.0F).setSoundType(SoundType::METAL).setUnlocalizedName("hopper"));
    RegisterBlock(BlockID::Quartz_Block,  Block().setSoundType(SoundType::STONE).setHardness(0.8F).setUnlocalizedName("quartzBlock"));
    RegisterBlock(BlockID::Quartz_Stairs,  Block().setUnlocalizedName("stairsQuartz"));
    RegisterBlock(BlockID::Activator_Rail,  Block().setHardness(0.7F).setSoundType(SoundType::METAL).setUnlocalizedName("activatorRail"));
    RegisterBlock(BlockID::Dropper,Block().setHardness(3.5F).setSoundType(SoundType::STONE).setUnlocalizedName("dropper"));
    RegisterBlock(BlockID::Stained_Hardened_Clay,  Block().setHardness(1.25F).setResistance(7.0F).setSoundType(SoundType::STONE).setUnlocalizedName("clayHardenedStained"));
    RegisterBlock(BlockID::Stained_Glass_Pane,  Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setUnlocalizedName("thinStainedGlass"));
    RegisterBlock(BlockID::Leaves2, Block().setUnlocalizedName("leaves"));
    RegisterBlock(BlockID::Log2, Block().setUnlocalizedName("log"));
    RegisterBlock(BlockID::Acacia_Stairs, Block().setUnlocalizedName("stairsWoodAcacia"));
    RegisterBlock(BlockID::Dark_Oak_Stairs, Block().setUnlocalizedName("stairsWoodDarkOak"));
    RegisterBlock(BlockID::Slime, Block().setUnlocalizedName("slime").setSoundType(SoundType::SLIME));
    RegisterBlock(BlockID::Barrier, Block().setUnlocalizedName("barrier"));
    RegisterBlock(BlockID::Iron_Trapdoor, Block().setHardness(5.0F).setSoundType(SoundType::METAL).setUnlocalizedName("ironTrapdoor").disableStats());
    RegisterBlock(BlockID::Prismarine, Block().setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("prismarine"));
    RegisterBlock(BlockID::Sea_Lantern, Block().setHardness(0.3F).setSoundType(SoundType::GLASS).setLightLevel(1.0F).setUnlocalizedName("seaLantern"));
    RegisterBlock(BlockID::Hay_Block, Block().setHardness(0.5F).setSoundType(SoundType::PLANT).setUnlocalizedName("hayBlock").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Carpet, Block().setHardness(0.1F).setSoundType(SoundType::CLOTH).setUnlocalizedName("woolCarpet").setLightOpacity(0));
    RegisterBlock(BlockID::Hardened_Clay, Block().setHardness(1.25F).setResistance(7.0F).setSoundType(SoundType::STONE).setUnlocalizedName("clayHardened"));
    RegisterBlock(BlockID::Coal_Block,Block().setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("blockCoal").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Packed_Ice,Block().setHardness(0.5F).setSoundType(SoundType::GLASS).setUnlocalizedName("icePacked"));
    RegisterBlock(BlockID::Double_Plant, Block());
    RegisterBlock(BlockID::Standing_Banner,  Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("banner").disableStats());
    RegisterBlock(BlockID::Wall_Banner,Block().setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("banner").disableStats());
    RegisterBlock(BlockID::Daylight_Detector_Inverted,Block());
    RegisterBlock(BlockID::Red_Sandstone,  Block().setSoundType(SoundType::STONE).setHardness(0.8F).setUnlocalizedName("redSandStone"));
    RegisterBlock(BlockID::Red_Sandstone_Stairs,  Block().setUnlocalizedName("stairsRedSandStone"));
    RegisterBlock(BlockID::Double_Stone_Slab2,  Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Stone_Slab2,  Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Spruce_Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("spruceFenceGate"));
    RegisterBlock(BlockID::Birch_Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("birchFenceGate"));
    RegisterBlock(BlockID::Jungle_Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("jungleFenceGate"));
    RegisterBlock(BlockID::Dark_Oak_Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("darkOakFenceGate"));
    RegisterBlock(BlockID::Acacia_Fence_Gate, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("acaciaFenceGate"));
    RegisterBlock(BlockID::Spruce_Fence,  Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("spruceFence"));
    RegisterBlock(BlockID::Birch_Fence, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("birchFence"));
    RegisterBlock(BlockID::Jungle_Fence, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("jungleFence"));
    RegisterBlock(BlockID::Dark_Oak_Fence, Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("darkOakFence"));
    RegisterBlock(BlockID::Acacia_Fence,  Block().setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("acaciaFence"));
    RegisterBlock(BlockID::Spruce_Door, Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorSpruce").disableStats());
    RegisterBlock(BlockID::Birch_Door, Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorBirch").disableStats());
    RegisterBlock(BlockID::Jungle_Door,  Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorJungle").disableStats());
    RegisterBlock(BlockID::Acacia_Door,  Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorAcacia").disableStats());
    RegisterBlock(BlockID::Dark_Oak_Door, Block().setHardness(3.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("doorDarkOak").disableStats());
    RegisterBlock(BlockID::End_Rod, Block().setHardness(0.0F).setLightLevel(0.9375F).setSoundType(SoundType::WOOD).setUnlocalizedName("endRod"));
    RegisterBlock(BlockID::Chorus_Plant,  Block().setHardness(0.4F).setSoundType(SoundType::WOOD).setUnlocalizedName("chorusPlant"));
    RegisterBlock(BlockID::Chorus_Flower,  Block().setHardness(0.4F).setSoundType(SoundType::WOOD).setUnlocalizedName("chorusFlower"));
    RegisterBlock(BlockID::Purpur_Block, Block().setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setCreativeTab(CreativeTabs::BUILDING_BLOCKS).setUnlocalizedName("purpurBlock"));
    RegisterBlock(BlockID::Purpur_Pillar, Block().setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setCreativeTab(CreativeTabs::BUILDING_BLOCKS).setUnlocalizedName("purpurPillar"));
    RegisterBlock(BlockID::Purpur_Stairs, Block().setUnlocalizedName("stairsPurpur"));
    RegisterBlock(BlockID::Purpur_Double_Slab,  Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::Purpur_Slab,  Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::End_Bricks,  Block().setSoundType(SoundType::STONE).setHardness(0.8F).setCreativeTab(CreativeTabs::BUILDING_BLOCKS).setUnlocalizedName("endBricks"));
    RegisterBlock(BlockID::Beetroots,  Block().setUnlocalizedName("beetroots"));
    RegisterBlock(BlockID::Grass_Path, Block().setHardness(0.65F).setSoundType(SoundType::PLANT).setUnlocalizedName("grassPath").disableStats());
    RegisterBlock(BlockID::End_Gateway, Block().setHardness(-1.0F).setResistance(6000000.0F));
    RegisterBlock(BlockID::Repeating_Command_Block, Block().setBlockUnbreakable().setResistance(6000000.0F).setUnlocalizedName("repeatingCommandBlock"));
    RegisterBlock(BlockID::Chain_Command_Block, Block().setBlockUnbreakable().setResistance(6000000.0F).setUnlocalizedName("chainCommandBlock"));
    RegisterBlock(BlockID::Frosted_Ice, Block().setHardness(0.5F).setLightOpacity(3).setSoundType(SoundType::GLASS).setUnlocalizedName("frostedIce"));
    RegisterBlock(BlockID::Magma, Block().setHardness(0.5F).setSoundType(SoundType::STONE).setUnlocalizedName("magma"));
    RegisterBlock(BlockID::Nether_Wart_Block, Block().setCreativeTab(CreativeTabs::BUILDING_BLOCKS).setHardness(1.0F).setSoundType(SoundType::WOOD).setUnlocalizedName("netherWartBlock"));
    RegisterBlock(BlockID::Red_Nether_Brick, Block().setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setUnlocalizedName("redNetherBrick").setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Bone_Block, Block().setUnlocalizedName("boneBlock"));
    RegisterBlock(BlockID::Structure_Void, Block().setUnlocalizedName("structureVoid"));
    RegisterBlock(BlockID::Observer, Block().setHardness(3.0F).setUnlocalizedName("observer"));
    RegisterBlock(BlockID::White_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxWhite"));
    RegisterBlock(BlockID::Orange_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxOrange"));
    RegisterBlock(BlockID::Magenta_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxMagenta"));
    RegisterBlock(BlockID::Light_Blue_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxLightBlue"));
    RegisterBlock(BlockID::Yellow_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxYellow"));
    RegisterBlock(BlockID::Lime_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxLime"));
    RegisterBlock(BlockID::Pink_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxPink"));
    RegisterBlock(BlockID::Gray_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxGray"));
    RegisterBlock(BlockID::Silver_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxSilver"));
    RegisterBlock(BlockID::Cyan_Shulker_Box,  Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxCyan"));
    RegisterBlock(BlockID::Purple_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxPurple"));
    RegisterBlock(BlockID::Blue_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxBlue"));
    RegisterBlock(BlockID::Brown_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxBrown"));
    RegisterBlock(BlockID::Green_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxGreen"));
    RegisterBlock(BlockID::Red_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxRed"));
    RegisterBlock(BlockID::Black_Shulker_Box, Block().setHardness(2.0F).setSoundType(SoundType::STONE).setUnlocalizedName("shulkerBoxBlack"));
    RegisterBlock(BlockID::White_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Orange_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Magenta_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Light_Blue_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Yellow_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Lime_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Pink_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Gray_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Silver_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Cyan_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Purple_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Blue_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Brown_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Green_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Red_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Black_Glazed_Terracotta, Block());
    RegisterBlock(BlockID::Concrete, Block().setHardness(1.8F).setSoundType(SoundType::STONE).setUnlocalizedName("concrete"));
    RegisterBlock(BlockID::Concrete_Powder, Block().setHardness(0.5F).setSoundType(SoundType::SAND).setUnlocalizedName("concretePowder"));
    RegisterBlock(BlockID::Structure_Block, Block().setBlockUnbreakable().setResistance(6000000.0F).setUnlocalizedName("structureBlock"));
}
// clang-format on
BlockRegistry::~BlockRegistry() {}


// TODO: Block-Texture mapping
void BlockRegistry::GetTextureUV(BlockID blockId, uint8_t metadata,
                                 Direction direction, int16_t* out_uv) {
    out_uv[0] = 0;
    out_uv[1] = 0;

    const Block* block = GetBlock(blockId);
    if (!block) return;

    TextureSet ts = block->getTextures(metadata);
    const char* name = ts.get(direction);

    if (!name) {
        for (auto & face : ts.faces) {
            if (face) { name = face; break; }
        }
    }
    if (!name) return;

    const TextureMap::Icon& icon = GetTextureMapEx()->Get(name);
    out_uv[0] = icon.u;
    out_uv[1] = icon.v;
}