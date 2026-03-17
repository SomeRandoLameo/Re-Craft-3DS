#include "block/Block.hpp"
#include "ReCraftCore.hpp"
#include "block/BlockAir.hpp"
#include "block/BlockBed.hpp"
#include "block/BlockDeadBush.hpp"
#include "block/BlockDirt.hpp"
#include "block/BlockDispenser.hpp"
#include "block/BlockGlass.hpp"
#include "block/BlockGrass.hpp"
#include "block/BlockGravel.hpp"
#include "block/BlockLeaves.hpp"
#include "block/BlockNote.hpp"
#include "block/BlockOldLeaves.hpp"
#include "block/BlockOldLog.hpp"
#include "block/BlockOre.hpp"
#include "block/BlockPistonBase.hpp"
#include "block/BlockPlanks.hpp"
#include "block/BlockRailDetector.hpp"
#include "block/BlockRailPowered.hpp"
#include "block/BlockSand.hpp"
#include "block/BlockSandStone.hpp"
#include "block/BlockSapling.hpp"
#include "block/BlockSponge.hpp"
#include "block/BlockStone.hpp"
#include "block/BlockTallGrass.hpp"
#include "block/BlockWeb.hpp"
#include "util/EnumFacing.hpp"

BlockPtr Block::setHardness(float hardness) {
    m_blockHardness = hardness;

    if (m_blockHardness < hardness * 5.0F) {
        m_blockResistance = hardness * 5.0F;
    }

    return this;
}

BlockPtr Block::setBlockUnbreakable() {
    setHardness(-1.0F);
    return this;
}

BlockPtr Block::setResistance(float resistance) {
    m_blockResistance = resistance * 3.0F;
    return this;
}

BlockPtr Block::setUnlocalizedName(const std::string& name) {
    m_unlocalizedName = name;
    return this;
}

BlockPtr Block::setSoundType(SoundType sound) {
    m_soundType = sound;
    return this;
}

BlockPtr Block::setOpaque(bool opaque) {
    m_opaque = opaque;
    return this;
}

BlockPtr Block::setSolid(bool solid) {
    m_solid = solid;
    return this;
}

BlockPtr Block::setCreativeTab(CreativeTabs tab) {
    m_displayOnCreativeTab = tab;
    return this;
}

BlockPtr Block::setLightLevel(float value) {
    m_lightValue = (int)(15.0F * value);
    return this;
}

BlockPtr Block::setLightOpacity(int opacity) {
    m_lightOpacity = opacity;
    return this;
}

BlockPtr Block::disableStats() {
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

    RegisterBlock(BlockID::Air,(new BlockAir())->setUnlocalizedName("air"));
    RegisterBlock(BlockID::Stone, (new BlockStone())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stone"));
    RegisterBlock(BlockID::Grass, (new BlockGrass())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("grass"));
    RegisterBlock(BlockID::Dirt, (new BlockDirt())->setHardness(0.5F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("dirt"));

//TODO: Blocks initialize always with material, also cobblestone is reused
    RegisterBlock(BlockID::Cobblestone, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("cobblestone")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));

    RegisterBlock(BlockID::Planks, (new BlockPlanks())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("wood"));
    RegisterBlock(BlockID::Sapling, (new BlockSapling())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("sapling"));

//TODO: BlockEmptyDrops
    RegisterBlock(BlockID::Bedrock, (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("bedrock")->disableStats()->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));

//TODO:BlockDynamicLiquid
    RegisterBlock(BlockID::Flowing_Water, (new Block())->setHardness(100.0F)->setLightOpacity(3)->setUnlocalizedName("water")->disableStats());
//TODO:BlockStaticLiquid
    RegisterBlock(BlockID::Water, (new Block())->setHardness(100.0F)->setLightOpacity(3)->setUnlocalizedName("water")->disableStats());
//TODO:BlockDynamicLiquid
    RegisterBlock(BlockID::Flowing_Lava, (new Block())->setHardness(100.0F)->setLightLevel(1.0F)->setUnlocalizedName("lava")->disableStats());
//TODO:BlockStaticLiquid
    RegisterBlock(BlockID::Lava, (new Block())->setHardness(100.0F)->setLightLevel(1.0F)->setUnlocalizedName("lava")->disableStats());

    RegisterBlock(BlockID::Sand, (new BlockSand())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("sand"));
    RegisterBlock(BlockID::Gravel, (new BlockGravel())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("gravel"));
    RegisterBlock(BlockID::Gold_Ore, (new BlockOre())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreGold"));
    RegisterBlock(BlockID::Iron_Ore, (new BlockOre())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreIron"));
    RegisterBlock(BlockID::Coal_Ore, (new BlockOre())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreCoal"));

//TODO:BlockOldLog->BlockLog->BlockRotatedPillar->Block-rotation
    RegisterBlock(BlockID::Log, (new BlockOldLog())->setUnlocalizedName("log"));

//TODO:BlockOldLeaf->BlockLeaves
    RegisterBlock(BlockID::Leaves, (new BlockOldLeaves())->setUnlocalizedName("leaves"));

    RegisterBlock(BlockID::Sponge, (new BlockSponge())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("sponge"));
    RegisterBlock(BlockID::Glass, (new BlockGlass())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("glass"));
    RegisterBlock(BlockID::Lapis_Ore, (new BlockOre())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreLapis"));

//TODO: How do you get your texture?
    RegisterBlock(BlockID::Lapis_Block, (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("blockLapis")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));

    RegisterBlock(BlockID::Dispenser, (new BlockDispenser())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("dispenser"));
    RegisterBlock(BlockID::Sandstone, (new BlockSandStone())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("sandStone"));
    RegisterBlock(BlockID::Noteblock, (new BlockNote())->setSoundType(SoundType::WOOD)->setHardness(0.8F)->setUnlocalizedName("musicBlock"));

//TODO:BlockHorizontal
    RegisterBlock(BlockID::Bed, (new BlockBed())->setSoundType(SoundType::WOOD)->setHardness(0.2F)->setUnlocalizedName("bed")->disableStats());

    RegisterBlock(BlockID::Golden_Rail, (new BlockRailPowered())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("goldenRail"));
    RegisterBlock(BlockID::Detector_Rail, (new BlockRailDetector())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("detectorRail"));
    RegisterBlock(BlockID::Sticky_Piston, (new BlockPistonBase(true))->setUnlocalizedName("pistonStickyBase"));
    RegisterBlock(BlockID::Web, (new BlockWeb())->setLightOpacity(1)->setHardness(4.0F)->setUnlocalizedName("web"));
    RegisterBlock(BlockID::Tallgrass, (new BlockTallGrass())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("tallgrass"));
    RegisterBlock(BlockID::Deadbush, (new BlockDeadBush())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("deadbush"));




    RegisterBlock(BlockID::Piston, (new Block())->setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Piston_Head, (new Block())->setUnlocalizedName("pistonBase"));
    RegisterBlock(BlockID::Wool, (new Block())->setHardness(0.8F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cloth"));
    RegisterBlock(BlockID::Piston_Extension, (new Block()));
    RegisterBlock(BlockID::Yellow_Flower, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("flower1"));
    RegisterBlock(BlockID::Red_Flower, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("flower2"));
    RegisterBlock(BlockID::Brown_Mushroom, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setLightLevel(0.125F)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Gold_Block, (new Block())->setHardness(3.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockGold")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Iron_Block, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockIron")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Double_Stone_Slab, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Stone_Slab, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab"));
    RegisterBlock(BlockID::Brick_Block, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("brick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Tnt, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("tnt"));
    RegisterBlock(BlockID::Bookshelf, (new Block())->setHardness(1.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("bookshelf"));
    RegisterBlock(BlockID::Mossy_Cobblestone, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneMoss")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Obsidian, (new Block())->setHardness(50.0F)->setResistance(2000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("obsidian"));
    RegisterBlock(BlockID::Torch, (new Block())->setHardness(0.0F)->setLightLevel(0.9375F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("torch"));
    RegisterBlock(BlockID::Fire, (new Block())->setHardness(0.0F)->setLightLevel(1.0F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("fire")->disableStats());
    RegisterBlock(BlockID::Mob_Spawner, (new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("mobSpawner")->disableStats());
    RegisterBlock(BlockID::Oak_Stairs, (new Block())->setUnlocalizedName("stairsWood"));
    RegisterBlock(BlockID::Chest, (new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chest"));
    RegisterBlock(BlockID::Redstone_Wire, (new Block())->setHardness(0.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("redstoneDust")->disableStats());
    RegisterBlock(BlockID::Diamond_Ore, (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreDiamond"));
    RegisterBlock(BlockID::Diamond_Block, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockDiamond")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Crafting_Table, (new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("workbench"));
    RegisterBlock(BlockID::Wheat, (new Block())->setUnlocalizedName("crops"));
    RegisterBlock(BlockID::Farmland, (new Block())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("farmland"));
    RegisterBlock(BlockID::Furnace, (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("furnace")->setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::Lit_Furnace, (new  Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setLightLevel(0.875F)->setUnlocalizedName("furnace"));
    RegisterBlock(BlockID::Standing_Sign, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("sign")->disableStats());
    RegisterBlock(BlockID::Wooden_Door, (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorOak")->disableStats());
    RegisterBlock(BlockID::Ladder, (new Block())->setHardness(0.4F)->setSoundType(SoundType::LADDER)->setUnlocalizedName("ladder"));
    RegisterBlock(BlockID::Rail, (new Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("rail"));
    RegisterBlock(BlockID::Stone_Stairs, (new Block())->setUnlocalizedName("stairsStone"));
    RegisterBlock(BlockID::Wall_Sign, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("sign")->disableStats());
    RegisterBlock(BlockID::Lever, (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("lever"));
    RegisterBlock(BlockID::Stone_Pressure_Plate, (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("pressurePlateStone"));
    RegisterBlock(BlockID::Iron_Door, (new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("doorIron")->disableStats());
    RegisterBlock(BlockID::Wooden_Pressure_Plate, (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pressurePlateWood"));
    RegisterBlock(BlockID::Redstone_Ore, (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreRedstone")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Lit_Redstone_Ore, (new Block())->setLightLevel(0.625F)->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreRedstone"));
    RegisterBlock(BlockID::Unlit_Redstone_Torch, (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("notGate"));
    RegisterBlock(BlockID::Redstone_Torch, (new Block())->setHardness(0.0F)->setLightLevel(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("notGate")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Stone_Button, (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("button"));
    RegisterBlock(BlockID::Snow_Layer, (new Block())->setHardness(0.1F)->setSoundType(SoundType::SNOW)->setUnlocalizedName("snow")->setLightOpacity(0));
    RegisterBlock(BlockID::Ice, (new Block())->setHardness(0.5F)->setLightOpacity(3)->setSoundType(SoundType::GLASS)->setUnlocalizedName("ice"));
    RegisterBlock(BlockID::Snow, (new Block())->setHardness(0.2F)->setSoundType(SoundType::SNOW)->setUnlocalizedName("snow"));
    RegisterBlock(BlockID::Cactus, (new Block())->setHardness(0.4F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cactus"));
    RegisterBlock(BlockID::Clay, (new Block())->setHardness(0.6F)->setSoundType(SoundType::GROUND)->setUnlocalizedName("clay"));
    RegisterBlock(BlockID::Reeds, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("reeds")->disableStats());
    RegisterBlock(BlockID::Jukebox, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("jukebox"));
    RegisterBlock(BlockID::Fence, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("fence"));
    RegisterBlock(BlockID::Pumpkin, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkin"));
    RegisterBlock(BlockID::Netherrack, (new Block())->setHardness(0.4F)->setSoundType(SoundType::STONE)->setUnlocalizedName("hellrock"));
    RegisterBlock(BlockID::Soul_Sand, (new Block())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("hellsand"));
    RegisterBlock(BlockID::Glowstone, (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setLightLevel(1.0F)->setUnlocalizedName("lightgem"));
    RegisterBlock(BlockID::Portal, (new Block())->setHardness(-1.0F)->setSoundType(SoundType::GLASS)->setLightLevel(0.75F)->setUnlocalizedName("portal"));
    RegisterBlock(BlockID::Lit_Pumpkin, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setLightLevel(1.0F)->setUnlocalizedName("litpumpkin"));
    RegisterBlock(BlockID::Cake, (new Block())->setHardness(0.5F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("cake")->disableStats());
    RegisterBlock(BlockID::Unpowered_Repeater, (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("diode")->disableStats());
    RegisterBlock(BlockID::Powered_Repeater, (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("diode")->disableStats());
    RegisterBlock(BlockID::Stained_Glass, (new  Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("stainedGlass"));
    RegisterBlock(BlockID::Trapdoor, (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("trapdoor")->disableStats());
    RegisterBlock(BlockID::Monster_Egg, (new Block())->setHardness(0.75F)->setUnlocalizedName("monsterStoneEgg"));
    RegisterBlock(BlockID::Stonebrick, (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stonebricksmooth"));
    RegisterBlock(BlockID::Brown_Mushroom_Block, (new Block())->setHardness(0.2F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Red_Mushroom_Block, (new Block())->setHardness(0.2F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("mushroom"));
    RegisterBlock(BlockID::Iron_Bars, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("fenceIron"));
    RegisterBlock(BlockID::Glass_Pane, (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("thinGlass"));
    RegisterBlock(BlockID::Melon_Block, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("melon"));
    RegisterBlock(BlockID::Pumpkin_Stem, (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Melon_Stem, (new Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("pumpkinStem"));
    RegisterBlock(BlockID::Vine, (new Block())->setHardness(0.2F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("vine"));
    RegisterBlock(BlockID::Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("fenceGate"));
    RegisterBlock(BlockID::Brick_Stairs, (new Block())->setUnlocalizedName("stairsBrick"));
    RegisterBlock(BlockID::Stone_Brick_Stairs, (new  Block())->setUnlocalizedName("stairsStoneBrickSmooth"));
    RegisterBlock(BlockID::Mycelium, (new Block())->setHardness(0.6F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("mycel"));
    RegisterBlock(BlockID::Waterlily, (new Block())->setHardness(0.0F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("waterlily"));
    RegisterBlock(BlockID::Nether_Brick, (new  Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherBrick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Nether_Brick_Fence, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherFence"));
    RegisterBlock(BlockID::Nether_Brick_Stairs, (new Block())->setUnlocalizedName("stairsNetherBrick"));
    RegisterBlock(BlockID::Nether_Wart, (new Block())->setUnlocalizedName("netherStalk"));
    RegisterBlock(BlockID::Enchanting_Table, (new Block())->setHardness(5.0F)->setResistance(2000.0F)->setUnlocalizedName("enchantmentTable"));
    RegisterBlock(BlockID::Brewing_Stand, (new Block())->setHardness(0.5F)->setLightLevel(0.125F)->setUnlocalizedName("brewingStand"));
    RegisterBlock(BlockID::Cauldron, (new Block())->setHardness(2.0F)->setUnlocalizedName("cauldron"));
    RegisterBlock(BlockID::End_Portal, (new  Block())->setHardness(-1.0F)->setResistance(6000000.0F));
    RegisterBlock(BlockID::End_Portal_Frame, (new Block())->setSoundType(SoundType::GLASS)->setLightLevel(0.125F)->setHardness(-1.0F)->setUnlocalizedName("endPortalFrame")->setResistance(6000000.0F)->setCreativeTab(CreativeTabs::DECORATIONS));
    RegisterBlock(BlockID::End_Stone, (new Block())->setHardness(3.0F)->setResistance(15.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("whiteStone")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Dragon_Egg, (new Block())->setHardness(3.0F)->setResistance(15.0F)->setSoundType(SoundType::STONE)->setLightLevel(0.125F)->setUnlocalizedName("dragonEgg"));
    RegisterBlock(BlockID::Redstone_Lamp, (new  Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("redstoneLight")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Lit_Redstone_Lamp, (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("redstoneLight"));
    RegisterBlock(BlockID::Double_Wooden_Slab, (new  Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Wooden_Slab, (new  Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("woodSlab"));
    RegisterBlock(BlockID::Cocoa, (new Block())->setHardness(0.2F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("cocoa"));
    RegisterBlock(BlockID::Sandstone_Stairs, (new  Block())->setUnlocalizedName("stairsSandStone"));
    RegisterBlock(BlockID::Emerald_Ore, (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("oreEmerald"));
    RegisterBlock(BlockID::Ender_Chest, (new Block())->setHardness(22.5F)->setResistance(1000.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("enderChest")->setLightLevel(0.5F));
    RegisterBlock(BlockID::Tripwire_Hook, (new  Block())->setUnlocalizedName("tripWireSource"));
    RegisterBlock(BlockID::Tripwire, (new Block())->setUnlocalizedName("tripWire"));
    RegisterBlock(BlockID::Emerald_Block, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockEmerald")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Spruce_Stairs, (new Block())->setUnlocalizedName("stairsWoodSpruce"));
    RegisterBlock(BlockID::Birch_Stairs, (new Block())->setUnlocalizedName("stairsWoodBirch"));
    RegisterBlock(BlockID::Jungle_Stairs, (new Block())->setUnlocalizedName("stairsWoodJungle"));
    RegisterBlock(BlockID::Command_Block, (new  Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("commandBlock"));
    RegisterBlock(BlockID::Beacon, (new  Block())->setUnlocalizedName("beacon")->setLightLevel(1.0F));
    RegisterBlock(BlockID::Cobblestone_Wall, (new  Block())->setUnlocalizedName("cobbleWall"));
    RegisterBlock(BlockID::Flower_Pot, (new Block())->setHardness(0.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("flowerPot"));
    RegisterBlock(BlockID::Carrots, (new Block())->setUnlocalizedName("carrots"));
    RegisterBlock(BlockID::Potatoes, (new Block())->setUnlocalizedName("potatoes"));
    RegisterBlock(BlockID::Wooden_Button, (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("button"));
    RegisterBlock(BlockID::Skull, (new Block())->setHardness(1.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("skull"));
    RegisterBlock(BlockID::Anvil, (new  Block())->setHardness(5.0F)->setSoundType(SoundType::ANVIL)->setResistance(2000.0F)->setUnlocalizedName("anvil"));
    RegisterBlock(BlockID::Trapped_Chest, (new Block())->setHardness(2.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chestTrap"));
    RegisterBlock(BlockID::Light_Weighted_Pressure_Plate, (new Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("weightedPlate_light"));
    RegisterBlock(BlockID::Heavy_Weighted_Pressure_Plate, (new  Block())->setHardness(0.5F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("weightedPlate_heavy"));
    RegisterBlock(BlockID::Unpowered_Comparator, (new  Block())->setHardness(0.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("comparator")->disableStats());
    RegisterBlock(BlockID::Powered_Comparator, (new  Block())->setHardness(0.0F)->setLightLevel(0.625F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("comparator")->disableStats());
    RegisterBlock(BlockID::Daylight_Detector, (new  Block()));
    RegisterBlock(BlockID::Redstone_Block, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("blockRedstone")->setCreativeTab(CreativeTabs::REDSTONE));
    RegisterBlock(BlockID::Quartz_Ore, (new Block())->setHardness(3.0F)->setResistance(5.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("netherquartz"));
    RegisterBlock(BlockID::Hopper, (new Block())->setHardness(3.0F)->setResistance(8.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("hopper"));
    RegisterBlock(BlockID::Quartz_Block, (new  Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("quartzBlock"));
    RegisterBlock(BlockID::Quartz_Stairs, (new  Block())->setUnlocalizedName("stairsQuartz"));
    RegisterBlock(BlockID::Activator_Rail, (new  Block())->setHardness(0.7F)->setSoundType(SoundType::METAL)->setUnlocalizedName("activatorRail"));
    RegisterBlock(BlockID::Dropper, (new Block())->setHardness(3.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("dropper"));
    RegisterBlock(BlockID::Stained_Hardened_Clay, (new  Block())->setHardness(1.25F)->setResistance(7.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("clayHardenedStained"));
    RegisterBlock(BlockID::Stained_Glass_Pane, (new  Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("thinStainedGlass"));
    RegisterBlock(BlockID::Leaves2, (new Block())->setUnlocalizedName("leaves"));
    RegisterBlock(BlockID::Log2, (new Block())->setUnlocalizedName("log"));
    RegisterBlock(BlockID::Acacia_Stairs, (new Block())->setUnlocalizedName("stairsWoodAcacia"));
    RegisterBlock(BlockID::Dark_Oak_Stairs, (new Block())->setUnlocalizedName("stairsWoodDarkOak"));
    RegisterBlock(BlockID::Slime, (new Block())->setUnlocalizedName("slime")->setSoundType(SoundType::SLIME));
    RegisterBlock(BlockID::Barrier, (new Block())->setUnlocalizedName("barrier"));
    RegisterBlock(BlockID::Iron_Trapdoor, (new Block())->setHardness(5.0F)->setSoundType(SoundType::METAL)->setUnlocalizedName("ironTrapdoor")->disableStats());
    RegisterBlock(BlockID::Prismarine, (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("prismarine"));
    RegisterBlock(BlockID::Sea_Lantern, (new Block())->setHardness(0.3F)->setSoundType(SoundType::GLASS)->setLightLevel(1.0F)->setUnlocalizedName("seaLantern"));
    RegisterBlock(BlockID::Hay_Block, (new Block())->setHardness(0.5F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("hayBlock")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Carpet, (new Block())->setHardness(0.1F)->setSoundType(SoundType::CLOTH)->setUnlocalizedName("woolCarpet")->setLightOpacity(0));
    RegisterBlock(BlockID::Hardened_Clay, (new Block())->setHardness(1.25F)->setResistance(7.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("clayHardened"));
    RegisterBlock(BlockID::Coal_Block, (new Block())->setHardness(5.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("blockCoal")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Packed_Ice, (new Block())->setHardness(0.5F)->setSoundType(SoundType::GLASS)->setUnlocalizedName("icePacked"));
    RegisterBlock(BlockID::Double_Plant, (new Block()));
    RegisterBlock(BlockID::Standing_Banner, (new  Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("banner")->disableStats());
    RegisterBlock(BlockID::Wall_Banner, (new Block())->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("banner")->disableStats());
    RegisterBlock(BlockID::Daylight_Detector_Inverted, (new Block()));
    RegisterBlock(BlockID::Red_Sandstone, (new  Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setUnlocalizedName("redSandStone"));
    RegisterBlock(BlockID::Red_Sandstone_Stairs, (new  Block())->setUnlocalizedName("stairsRedSandStone"));
    RegisterBlock(BlockID::Double_Stone_Slab2, (new  Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Stone_Slab2, (new  Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("stoneSlab2"));
    RegisterBlock(BlockID::Spruce_Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("spruceFenceGate"));
    RegisterBlock(BlockID::Birch_Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("birchFenceGate"));
    RegisterBlock(BlockID::Jungle_Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("jungleFenceGate"));
    RegisterBlock(BlockID::Dark_Oak_Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("darkOakFenceGate"));
    RegisterBlock(BlockID::Acacia_Fence_Gate, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("acaciaFenceGate"));
    RegisterBlock(BlockID::Spruce_Fence, (new  Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("spruceFence"));
    RegisterBlock(BlockID::Birch_Fence, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("birchFence"));
    RegisterBlock(BlockID::Jungle_Fence, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("jungleFence"));
    RegisterBlock(BlockID::Dark_Oak_Fence, (new Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("darkOakFence"));
    RegisterBlock(BlockID::Acacia_Fence, (new  Block())->setHardness(2.0F)->setResistance(5.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("acaciaFence"));
    RegisterBlock(BlockID::Spruce_Door, (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorSpruce")->disableStats());
    RegisterBlock(BlockID::Birch_Door, (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorBirch")->disableStats());
    RegisterBlock(BlockID::Jungle_Door, (new  Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorJungle")->disableStats());
    RegisterBlock(BlockID::Acacia_Door, (new  Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorAcacia")->disableStats());
    RegisterBlock(BlockID::Dark_Oak_Door, (new Block())->setHardness(3.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("doorDarkOak")->disableStats());
    RegisterBlock(BlockID::End_Rod, (new Block())->setHardness(0.0F)->setLightLevel(0.9375F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("endRod"));
    RegisterBlock(BlockID::Chorus_Plant, (new  Block())->setHardness(0.4F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chorusPlant"));
    RegisterBlock(BlockID::Chorus_Flower, (new  Block())->setHardness(0.4F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("chorusFlower"));
    RegisterBlock(BlockID::Purpur_Block, (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("purpurBlock"));
    RegisterBlock(BlockID::Purpur_Pillar, (new Block())->setHardness(1.5F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("purpurPillar"));
    RegisterBlock(BlockID::Purpur_Stairs, (new Block())->setUnlocalizedName("stairsPurpur"));
    RegisterBlock(BlockID::Purpur_Double_Slab, (new  Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::Purpur_Slab, (new  Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("purpurSlab"));
    RegisterBlock(BlockID::End_Bricks, (new  Block())->setSoundType(SoundType::STONE)->setHardness(0.8F)->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setUnlocalizedName("endBricks"));
    RegisterBlock(BlockID::Beetroots, (new  Block())->setUnlocalizedName("beetroots"));
    RegisterBlock(BlockID::Grass_Path, (new Block())->setHardness(0.65F)->setSoundType(SoundType::PLANT)->setUnlocalizedName("grassPath")->disableStats());
    RegisterBlock(BlockID::End_Gateway, (new Block())->setHardness(-1.0F)->setResistance(6000000.0F));
    RegisterBlock(BlockID::Repeating_Command_Block, (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("repeatingCommandBlock"));
    RegisterBlock(BlockID::Chain_Command_Block, (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("chainCommandBlock"));
    RegisterBlock(BlockID::Frosted_Ice, (new Block())->setHardness(0.5F)->setLightOpacity(3)->setSoundType(SoundType::GLASS)->setUnlocalizedName("frostedIce"));
    RegisterBlock(BlockID::Magma, (new Block())->setHardness(0.5F)->setSoundType(SoundType::STONE)->setUnlocalizedName("magma"));
    RegisterBlock(BlockID::Nether_Wart_Block, (new Block())->setCreativeTab(CreativeTabs::BUILDING_BLOCKS)->setHardness(1.0F)->setSoundType(SoundType::WOOD)->setUnlocalizedName("netherWartBlock"));
    RegisterBlock(BlockID::Red_Nether_Brick, (new Block())->setHardness(2.0F)->setResistance(10.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("redNetherBrick")->setCreativeTab(CreativeTabs::BUILDING_BLOCKS));
    RegisterBlock(BlockID::Bone_Block, (new Block())->setUnlocalizedName("boneBlock"));
    RegisterBlock(BlockID::Structure_Void, (new Block())->setUnlocalizedName("structureVoid"));
    RegisterBlock(BlockID::Observer, (new Block())->setHardness(3.0F)->setUnlocalizedName("observer"));
    RegisterBlock(BlockID::White_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxWhite"));
    RegisterBlock(BlockID::Orange_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxOrange"));
    RegisterBlock(BlockID::Magenta_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxMagenta"));
    RegisterBlock(BlockID::Light_Blue_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxLightBlue"));
    RegisterBlock(BlockID::Yellow_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxYellow"));
    RegisterBlock(BlockID::Lime_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxLime"));
    RegisterBlock(BlockID::Pink_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxPink"));
    RegisterBlock(BlockID::Gray_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxGray"));
    RegisterBlock(BlockID::Silver_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxSilver"));
    RegisterBlock(BlockID::Cyan_Shulker_Box, (new  Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxCyan"));
    RegisterBlock(BlockID::Purple_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxPurple"));
    RegisterBlock(BlockID::Blue_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBlue"));
    RegisterBlock(BlockID::Brown_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBrown"));
    RegisterBlock(BlockID::Green_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxGreen"));
    RegisterBlock(BlockID::Red_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxRed"));
    RegisterBlock(BlockID::Black_Shulker_Box, (new Block())->setHardness(2.0F)->setSoundType(SoundType::STONE)->setUnlocalizedName("shulkerBoxBlack"));
    RegisterBlock(BlockID::White_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Orange_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Magenta_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Light_Blue_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Yellow_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Lime_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Pink_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Gray_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Silver_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Cyan_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Purple_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Blue_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Brown_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Green_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Red_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Black_Glazed_Terracotta, (new Block()));
    RegisterBlock(BlockID::Concrete, (new Block())->setHardness(1.8F)->setSoundType(SoundType::STONE)->setUnlocalizedName("concrete"));
    RegisterBlock(BlockID::Concrete_Powder, (new Block())->setHardness(0.5F)->setSoundType(SoundType::SAND)->setUnlocalizedName("concretePowder"));
    RegisterBlock(BlockID::Structure_Block, (new Block())->setBlockUnbreakable()->setResistance(6000000.0F)->setUnlocalizedName("structureBlock"));
}
// clang-format on
BlockRegistry::~BlockRegistry() {}

// TODO: Block-Texture mapping
void BlockRegistry::GetTextureUV(BlockID blockId, uint8_t metadata, Direction direction, int16_t* out_uv) {
    out_uv[0] = 0;
    out_uv[1] = 0;

    const Block* block = GetBlock(blockId);
    if (!block)
        return;

    TextureSet ts = block->getTextures(metadata);
    const char* name = ts.get(direction);

    if (!name) {
        for (auto& face : ts.faces) {
            if (face) {
                name = face;
                break;
            }
        }
    }
    if (!name)
        return;

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), "blocks/%s", name);
    const TextureMap::Icon& icon = m_textureMap.Get(buffer);

    out_uv[0] = icon.u;
    out_uv[1] = icon.v;
}
