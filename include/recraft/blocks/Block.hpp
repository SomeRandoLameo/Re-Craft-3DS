#pragma once

#include <stdint.h>

#include "client/renderer/TextureMap.hpp"
#include "mclib/block/Block.h"
#include "mclib/inventory/Slot.h"
#include "world/Direction.hpp"

// These IDs are pre-flattening update based. Some blocks are "Mapped" meaning 1 (stone) : 3 (variant - Diorite).
// Variants are metadata based.
enum class BlockID : u16 {
    Air,
    Stone,
    Grass,
    Dirt,
    Cobblestone,
    Planks,
    Sapling,
    Bedrock,
    Flowing_Water,
    Water,
    Flowing_Lava,
    Lava,
    Sand,
    Gravel,
    Gold_Ore,
    Iron_Ore,
    Coal_Ore,
    Log,
    Leaves,
    Sponge,
    Glass,
    Lapis_Ore,
    Lapis_Block,
    Dispenser,
    Sandstone,
    Noteblock,
    Bed,
    Golden_Rail,
    Detector_Rail,
    Sticky_Piston,
    Web,
    Tallgrass,
    Deadbush,
    Piston,
    Piston_Head,
    Wool,
    Piston_Extension,
    Yellow_Flower,
    Red_Flower,
    Brown_Mushroom,
    Red_Mushroom,
    Gold_Block,
    Iron_Block,
    Double_Stone_Slab,
    Stone_Slab,
    Brick_Block,
    Tnt,
    Bookshelf,
    Mossy_Cobblestone,
    Obsidian,
    Torch,
    Fire,
    Mob_Spawner,
    Oak_Stairs,
    Chest,
    Redstone_Wire,
    Diamond_Ore,
    Diamond_Block,
    Crafting_Table,
    Wheat,
    Farmland,
    Furnace,
    Lit_Furnace,
    Standing_Sign,
    Wooden_Door,
    Ladder,
    Rail,
    Stone_Stairs,
    Wall_Sign,
    Lever,
    Stone_Pressure_Plate,
    Iron_Door,
    Wooden_Pressure_Plate,
    Redstone_Ore,
    Lit_Redstone_Ore,
    Unlit_Redstone_Torch,
    Redstone_Torch,
    Stone_Button,
    Snow_Layer,
    Ice,
    Snow,
    Cactus,
    Clay,
    Reeds,
    Jukebox,
    Fence,
    Pumpkin,
    Netherrack,
    Soul_Sand,
    Glowstone,
    Portal,
    Lit_Pumpkin,
    Cake,
    Unpowered_Repeater,
    Powered_Repeater,
    Stained_Glass,
    Trapdoor,
    Monster_Egg,
    Stonebrick,
    Brown_Mushroom_Block,
    Red_Mushroom_Block,
    Iron_Bars,
    Glass_Pane,
    Melon_Block,
    Pumpkin_Stem,
    Melon_Stem,
    Vine,
    Fence_Gate,
    Brick_Stairs,
    Stone_Brick_Stairs,
    Mycelium,
    Waterlily,
    Nether_Brick,
    Nether_Brick_Fence,
    Nether_Brick_Stairs,
    Nether_Wart,
    Enchanting_Table,
    Brewing_Stand,
    Cauldron,
    End_Portal,
    End_Portal_Frame,
    End_Stone,
    Dragon_Egg,
    Redstone_Lamp,
    Lit_Redstone_Lamp,
    Double_Wooden_Slab,
    Wooden_Slab,
    Cocoa,
    Sandstone_Stairs,
    Emerald_Ore,
    Ender_Chest,
    Tripwire_Hook,
    Tripwire,
    Emerald_Block,
    Spruce_Stairs,
    Birch_Stairs,
    Jungle_Stairs,
    Command_Block,
    Beacon,
    Cobblestone_Wall,
    Flower_Pot,
    Carrots,
    Potatoes,
    Wooden_Button,
    Skull,
    Anvil,
    Trapped_Chest,
    Light_Weighted_Pressure_Plate,
    Heavy_Weighted_Pressure_Plate,
    Unpowered_Comparator,
    Powered_Comparator,
    Daylight_Detector,
    Redstone_Block,
    Quartz_Ore,
    Hopper,
    Quartz_Block,
    Quartz_Stairs,
    Activator_Rail,
    Dropper,
    Stained_Hardened_Clay,
    Stained_Glass_Pane,
    Leaves2,
    Log2,
    Acacia_Stairs,
    Dark_Oak_Stairs,
    Slime,
    Barrier,
    Iron_Trapdoor,
    Prismarine,
    Sea_Lantern,
    Hay_Block,
    Carpet,
    Hardened_Clay,
    Coal_Block,
    Packed_Ice,
    Double_Plant,
    Standing_Banner,
    Wall_Banner,
    Daylight_Detector_Inverted,
    Red_Sandstone,
    Red_Sandstone_Stairs,
    Double_Stone_Slab2,
    Stone_Slab2,
    Spruce_Fence_Gate,
    Birch_Fence_Gate,
    Jungle_Fence_Gate,
    Dark_Oak_Fence_Gate,
    Acacia_Fence_Gate,
    Spruce_Fence,
    Birch_Fence,
    Jungle_Fence,
    Dark_Oak_Fence,
    Acacia_Fence,
    Spruce_Door,
    Birch_Door,
    Jungle_Door,
    Acacia_Door,
    Dark_Oak_Door,
    End_Rod,
    Chorus_Plant,
    Chorus_Flower,
    Purpur_Block,
    Purpur_Pillar,
    Purpur_Stairs,
    Purpur_Double_Slab,
    Purpur_Slab,
    End_Bricks,
    Beetroots,
    Grass_Path,
    End_Gateway,
    Repeating_Command_Block,
    Chain_Command_Block,
    Frosted_Ice,
    Magma,
    Nether_Wart_Block,
    Red_Nether_Brick,
    Bone_Block,
    Structure_Void,
    Observer,
    White_Shulker_Box,
    Orange_Shulker_Box,
    Magenta_Shulker_Box,
    Light_Blue_Shulker_Box,
    Yellow_Shulker_Box,
    Lime_Shulker_Box,
    Pink_Shulker_Box,
    Gray_Shulker_Box,
    Silver_Shulker_Box,
    Cyan_Shulker_Box,
    Purple_Shulker_Box,
    Blue_Shulker_Box,
    Brown_Shulker_Box,
    Green_Shulker_Box,
    Red_Shulker_Box,
    Black_Shulker_Box,
    White_Glazed_Terracotta,
    Orange_Glazed_Terracotta,
    Magenta_Glazed_Terracotta,
    Light_Blue_Glazed_Terracotta,
    Yellow_Glazed_Terracotta,
    Lime_Glazed_Terracotta,
    Pink_Glazed_Terracotta,
    Gray_Glazed_Terracotta,
    Silver_Glazed_Terracotta,
    Cyan_Glazed_Terracotta,
    Purple_Glazed_Terracotta,
    Blue_Glazed_Terracotta,
    Brown_Glazed_Terracotta,
    Green_Glazed_Terracotta,
    Red_Glazed_Terracotta,
    Black_Glazed_Terracotta,
    Concrete,
    Concrete_Powder,
    // empty
    // empty
    Structure_Block = 255,
    Count,
};

// TODO: Add more once sound engine is ready & move into sound engine stuff
enum class SoundType : u8 {
    WOOD,
    GROUND,
    PLANT,
    STONE,
    METAL,
    GLASS,
    CLOTH,
    SAND,
    SNOW,
    LADDER,
    ANVIL,
    SLIME,
    SOUND_TYPE_COUNT
};
// WIP
struct BlockTextures {
    int16_t top_u = 0;
    int16_t top_v = 0;
    int16_t bottom_u = 0;
    int16_t bottom_v = 0;
    int16_t north_u = 0;
    int16_t north_v = 0;
    int16_t south_u = 0;
    int16_t south_v = 0;
    int16_t east_u = 0;
    int16_t east_v = 0;
    int16_t west_u = 0;
    int16_t west_v = 0;

    // For simple blocks with same texture on all sides
    void setAllSides(int16_t u, int16_t v) {
        top_u = bottom_u = north_u = south_u = east_u = west_u = u;
        top_v = bottom_v = north_v = south_v = east_v = west_v = v;
    }

    // For blocks with different top/bottom/sides
    void setTopBottomSides(int16_t top_u_, int16_t top_v_, int16_t bottom_u_, int16_t bottom_v_, int16_t side_u_,
                           int16_t side_v_) {
        top_u = top_u_;
        top_v = top_v_;
        bottom_u = bottom_u_;
        bottom_v = bottom_v_;
        north_u = south_u = east_u = west_u = side_u_;
        north_v = south_v = east_v = west_v = side_v_;
    }
};

typedef u8 Metadata;

class Block {
public:
    Block() :
        m_blockHardness(1.0f), m_opaque(true), m_solid(true), m_soundType(SoundType::STONE), m_hasMetadata(false),
        m_lightEmission(0) {}

    virtual ~Block() = default;

    /**
     * Sets how many hits it takes to break a block.
     */
    Block* setHardness(float hardness) {
        m_blockHardness = hardness;

        if (m_blockHardness < hardness * 5.0F) {
            m_blockResistance = hardness * 5.0F;
        }

        return this;
    }

    Block* setBlockUnbreakable() {
        setHardness(-1.0F);
        return this;
    }

    Block* setResistance(float resistance) {
        m_blockResistance = resistance * 3.0F;
        return this;
    }

    Block* setUnlocalizedName(const std::string& name) {
        m_unlocalizedName = name;
        return this;
    }

    Block* setSoundType(SoundType sound) {
        m_soundType = sound;
        return this;
    }

    Block* setOpaque(bool opaque) {
        m_opaque = opaque;
        return this;
    }

    Block* setSolid(bool solid) {
        m_solid = solid;
        return this;
    }

    Block* setLightEmission(uint8_t level) {
        m_lightEmission = level;
        return this;
    }

    Block* setHasMetadata(bool hasMetadata) {
        m_hasMetadata = hasMetadata;
        return this;
    }

    Block* setTextures(const BlockTextures& textures) {
        m_textures = textures;
        return this;
    }

    Block* setAllSidesTexture(int16_t u, int16_t v) {
        m_textures.setAllSides(u, v);
        return this;
    }

    Block* setAllSidesTexture(const TextureMap::Icon& icn) {
        m_textures.setAllSides(icn.u, icn.v);
        return this;
    }

    Block* setTopBottomSidesTexture(const TextureMap::Icon& top, const TextureMap::Icon& bottom,
                                    const TextureMap::Icon& side) {
        m_textures.setTopBottomSides(top.u, top.v, bottom.u, bottom.v, side.u, side.v);
        return this;
    }

    Block* setTopBottomSidesTexture(int16_t top_u, int16_t top_v, int16_t bottom_u, int16_t bottom_v, int16_t side_u,
                                    int16_t side_v) {
        m_textures.setTopBottomSides(top_u, top_v, bottom_u, bottom_v, side_u, side_v);
        return this;
    }

    Block* setTopBottomSouthSidesTexture(const TextureMap::Icon& top, const TextureMap::Icon& bottom,
                                         const TextureMap::Icon& south, const TextureMap::Icon& side) {
        m_textures.bottom_u = bottom.u;
        m_textures.bottom_v = bottom.v;
        m_textures.top_u = top.u;
        m_textures.top_v = top.v;
        m_textures.east_u = side.u;
        m_textures.east_v = side.v;
        m_textures.south_u = south.u;
        m_textures.south_v = south.v;
        m_textures.north_u = side.u;
        m_textures.north_v = side.v;
        m_textures.west_u = side.u;
        m_textures.west_v = side.v;
        return this;
    }

    virtual void getTexture(Direction direction, int16_t* out_uv) const {
        switch (direction) {
        case Direction::Top:
            out_uv[0] = m_textures.top_u;
            out_uv[1] = m_textures.top_v;
            break;
        case Direction::Bottom:
            out_uv[0] = m_textures.bottom_u;
            out_uv[1] = m_textures.bottom_v;
            break;
        case Direction::North:
            out_uv[0] = m_textures.north_u;
            out_uv[1] = m_textures.north_v;
            break;
        case Direction::South:
            out_uv[0] = m_textures.south_u;
            out_uv[1] = m_textures.south_v;
            break;
        case Direction::East:
            out_uv[0] = m_textures.east_u;
            out_uv[1] = m_textures.east_v;
            break;
        case Direction::West:
            out_uv[0] = m_textures.west_u;
            out_uv[1] = m_textures.west_v;
            break;
        }
    }

    virtual void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const {
        out_rgb[0] = 255;
        out_rgb[1] = 255;
        out_rgb[2] = 255;
    }

    BlockID GetID() const { return m_id; }
    const char* getName() const { return m_identifier; }
    float getHardness() const { return m_blockHardness; }
    bool isOpaque(Metadata metadata = 0) const { return m_opaque; }
    bool isSolid() const { return m_solid; }
    SoundType getSoundType() const { return m_soundType; }
    uint8_t getLightEmission() const { return m_lightEmission; }

protected:
    BlockID m_id;
    const char* m_identifier;
    BlockTextures m_textures;
    float m_blockHardness;
    bool m_opaque;
    bool m_solid;
    SoundType m_soundType;
    bool m_hasMetadata;
    uint8_t m_lightEmission;
    float m_blockResistance;
    std::string m_unlocalizedName;
};

typedef Block* BlockPtr;

class BlockRegistry {
public:
    BlockRegistry();
    ~BlockRegistry();

    static BlockRegistry& GetInstance() {
        static BlockRegistry instance;
        return instance;
    }

    BlockPtr RegisterBlock(BlockID id, const std::string& textualID, BlockPtr block) {
        // Resize vector if needed
        while ((uint16_t)id >= m_blocks.size()) {
            m_blocks.push_back(nullptr);
        }

        // If this key matches the default key, set as default
        if (textualID == m_defaultKey) {
            m_defaultValue = block;
        }

        m_blocks[(uint16_t)id] = std::unique_ptr<Block>(block);
        m_keyToId[textualID] = (uint16_t)id;

        return block;
    }

    static const BlockPtr GetBlock(BlockID id) {
        const auto& blocks = GetInstance().m_blocks;
        uint16_t index = static_cast<uint16_t>(id);
        if (index < blocks.size() && blocks[index]) {
            return blocks[index].get();
        }
        return blocks[0].get();
    }

    static const BlockPtr GetBlockById(BlockID paletteId) { return GetBlock(paletteId); }

    static const BlockPtr GetBlockFromName(const std::string& name) {
        const auto& registry = GetInstance();
        auto it = registry.m_keyToId.find(name);
        if (it != registry.m_keyToId.end()) {
            return GetBlockById(static_cast<BlockID>(it->second));
        }
        return registry.m_defaultValue;
    }

    static BlockID GetBlockID(uint16_t paletteId) { return static_cast<BlockID>(paletteId); }

    static BlockID GetBlockID(const std::string& key) {
        const auto& registry = GetInstance();
        auto it = registry.m_keyToId.find(key);
        if (it != registry.m_keyToId.end()) {
            return static_cast<BlockID>(it->second);
        }
        return static_cast<BlockID>(0);
    }

    static uint16_t GetId(BlockID block) { return static_cast<uint16_t>(block); }

    static void SetDefaultKey(const std::string& key) { GetInstance().m_defaultKey = key; }

    static void* GetTextureMap() { return m_textureMap.GetTexture(); }
    static TextureMap* GetTextureMapEx() { return &m_textureMap; }

private:
    std::vector<std::unique_ptr<Block>> m_blocks;
    std::unordered_map<std::string, uint16_t> m_keyToId;
    std::string m_defaultKey = "air";
    BlockPtr m_defaultValue = nullptr;

    static TextureMap m_textureMap;
};
