#pragma once


#include <stdint.h>

#include "../world/Direction.hpp"

#include "mclib/block/Block.h"
#include "mclib/inventory/Slot.h"



enum class BlockID : u16 {
    Air,
    Stone,
    Dirt,
    Grass,
    Cobblestone,
    Sand,
    Log,
    Gravel,
    Leaves,
    Glass,
   	Stonebrick,
    Brick,
    Planks,
    Wool,
    Bedrock,
    Coarse,
    Door_Top,
   	Door_Bottom,
   	Snow_Grass,
   	Snow,
   	Obsidian,
   	Netherrack,
   	Sandstone,
   	Smooth_Stone,
   	Crafting_Table,
   	Grass_Path,
   	Water,
   	Lava,
	Iron_Ore,
	Coal_Ore,
	Diamond_Ore,
	Gold_Ore,
	Emerald_Ore,
	Gold_Block,
	Diamond_Block,
	Coal_Block,
	Iron_Block,
	Emerald_Block,
	Furnace,
	Count
};

// TODO: Add more once sound engine is ready & move into sound engine stuff
enum class SoundType : u8 {
    STONE,
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
    void setTopBottomSides(int16_t top_u_, int16_t top_v_,
                           int16_t bottom_u_, int16_t bottom_v_,
                           int16_t side_u_, int16_t side_v_) {
        top_u = top_u_; top_v = top_v_;
        bottom_u = bottom_u_; bottom_v = bottom_v_;
        north_u = south_u = east_u = west_u = side_u_;
        north_v = south_v = east_v = west_v = side_v_;
    }
};

typedef u8 Metadata;

class Block {
public:
    Block(BlockID id, const char* name)
        : m_id(id)
          , m_name(name)
          , m_destroyTime(1.0f)
          , m_opaque(true)
          , m_solid(true)
          , m_soundType(SoundType::STONE)
          , m_hasMetadata(false)
          , m_lightEmission(0)
    {
    }

    virtual ~Block() = default;

    // Fluent API for declarative configuration
    Block* setDestroyTime(float time) {
        m_destroyTime = time;
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

    Block* setTopBottomSidesTexture(int16_t top_u, int16_t top_v,
                                    int16_t bottom_u, int16_t bottom_v,
                                    int16_t side_u, int16_t side_v) {
        m_textures.setTopBottomSides(top_u, top_v, bottom_u, bottom_v, side_u, side_v);
        return this;
    }

    // Virtual methods for block behavior
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

    // Getters
    BlockID getID() const { return m_id; }
    const char* getName() const { return m_name; }
    float getDestroyTime() const { return m_destroyTime; }
    bool isOpaque(Metadata metadata = 0) const { return m_opaque; }
    bool isSolid() const { return m_solid; }
    SoundType getSoundType() const { return m_soundType; }
    uint8_t getLightEmission() const { return m_lightEmission; }

protected:
    BlockID m_id;
    const char* m_name;
    BlockTextures m_textures;
    float m_destroyTime;
    bool m_opaque;
    bool m_solid;
    SoundType m_soundType;
    bool m_hasMetadata;
    uint8_t m_lightEmission;
};

//TODO: Separate these into their own files
class GrassBlock : public Block {
public:
    GrassBlock(BlockID id, const char* name) : Block(id, name) {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        if (direction == Direction::Top) {
            out_rgb[0] = 140;
            out_rgb[1] = 214;
            out_rgb[2] = 123;
        } else {
            Block::getColor(metadata, direction, out_rgb);
        }
    }
};

class LeavesBlock : public Block {
public:
    LeavesBlock(BlockID id, const char* name) : Block(id, name) {}

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        out_rgb[0] = 140;
        out_rgb[1] = 214;
        out_rgb[2] = 123;
    }
};

class WoolBlock : public Block {
public:
    WoolBlock(BlockID id, const char* name) : Block(id, name) {
        setHasMetadata(true);
    }

    void getColor(Metadata metadata, Direction direction, Metadata out_rgb[]) const override {
        const uint32_t dyes[] = {
            0xFFFFFF, 0xD87F33, 0xB24CD8, 0x6699D8,
            0xE5E533, 0x7FCC19, 0xF27FA5, 0x4C4C4C,
            0x999999, 0x4C7F99, 0x7F3FB2, 0x334CB2,
            0x664C33, 0x667F33, 0x993333, 0x191919
        };

        if (metadata < 16) {
            uint32_t color = dyes[metadata];
            out_rgb[0] = (color >> 16) & 0xFF;
            out_rgb[1] = (color >> 8) & 0xFF;
            out_rgb[2] = color & 0xFF;
        } else {
            Block::getColor(metadata, direction, out_rgb);
        }
    }
};

class BlockRegistry {
public:
    BlockRegistry();
    ~BlockRegistry();

    static BlockRegistry& getInstance() {
        static BlockRegistry instance;
        return instance;
    }

    // Register a block (takes ownership)
    Block* registerBlock(Block* block) {
        uint16_t id = static_cast<uint16_t>(block->getID());
        while (id >= m_blocks.size()) {
            m_blocks.push_back(nullptr);  // or m_blocks.emplace_back(nullptr);
        }
        m_blocks[id] = std::unique_ptr<Block>(block);
        return block;
    }

    // Get block by ID
    const Block* getBlock(BlockID id) const {
        uint16_t index = static_cast<uint16_t>(id);
        if (index < m_blocks.size() && m_blocks[index]) {
            return m_blocks[index].get();
        }
        return m_blocks[0].get(); // Return Air as default
    }

    const Block* getBlock(uint16_t paletteId) const {
        return getBlock(static_cast<BlockID>(paletteId));
    }

    // Palette system (simple 1:1 mapping for now)
    static BlockID GetBlock(uint16_t paletteId) {
        return static_cast<BlockID>(paletteId);
    }

    static uint16_t GetId(BlockID block) {
        return static_cast<uint16_t>(block);
    }

private:
    std::vector<std::unique_ptr<Block>> m_blocks;
};


void* Block_GetTextureMap();
