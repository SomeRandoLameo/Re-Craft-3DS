#include "blocks/CT_Block.hpp"

#include "rendering/TextureMap.hpp"
#include "rendering/VertexFmt.hpp"

static Texture_Map textureMap;

// PATH PREFIX
#define PPRX "romfs:/assets/textures/blocks/"

const char* blockTextures[] = {
        PPRX "stone.png",
        PPRX "dirt.png",
        PPRX "cobblestone.png",
        PPRX "grass_side.png",
        PPRX "grass_top.png",
        PPRX "stonebrick.png",
        PPRX "sand.png",
        PPRX "log_oak.png",
        PPRX "log_oak_top.png",
        PPRX "leaves_oak.png",
        PPRX "glass.png",
        PPRX "brick.png",
        PPRX "planks_oak.png",
        PPRX "wool.png",
        PPRX "bedrock.png",
        PPRX "gravel.png",
        PPRX "coarse_dirt.png",
        PPRX "door_top.png",
        PPRX "door_bottom.png",
        PPRX "snow_grass_side.png",
        PPRX "snow.png",
        PPRX "obsidian.png",
        PPRX "sandstone_side.png",
        PPRX "sandstone_top.png",
        PPRX "sandstone_bottom.png",
        PPRX "netherrack.png",
        PPRX "smooth_stone.png",
        PPRX "lava.png",
        PPRX "water.png",
        PPRX "grass_path_side.png",
        PPRX "grass_path_top.png",
        PPRX "crafting_table_side.png",
        PPRX "crafting_table_top.png",
        PPRX "iron_ore.png",
        PPRX "iron_block.png",
        PPRX "diamond_ore.png",
        PPRX "diamond_block.png",
        PPRX "gold_ore.png",
        PPRX "gold_block.png",
        PPRX "coal_ore.png",
        PPRX "coal_block.png",
        PPRX "emerald_ore.png",
        PPRX "emerald_block.png",
        PPRX "furnace_side.png",
        PPRX "furnace_front.png",
        PPRX "furnace_top.png"
};

static struct {
    int16_t u, v;
} icons[sizeof(blockTextures) / sizeof(blockTextures[0])];

void* Block_GetTextureMap() { return &textureMap.texture; } // BlockRegistry::GetTextureMap??

BlockRegistry::BlockRegistry() {

    Texture_MapInit(&textureMap, blockTextures, sizeof(blockTextures) / sizeof(blockTextures[0]));

    for (size_t i = 0; i < sizeof(blockTextures) / sizeof(blockTextures[0]); i++) {
        auto icon = Texture_MapGetIcon(&textureMap, blockTextures[i]);
        icons[i].u = icon.u;
        icons[i].v = icon.v;
    }

    registerBlock(
        (new Block(BlockID::Air, "air"))
            ->setAllSidesTexture(0, 0)
            ->setOpaque(false)
            ->setSolid(false)
            ->setDestroyTime(0.0f)
    );

    registerBlock(
        (new Block(BlockID::Stone, "stone"))
            ->setAllSidesTexture(icons[0].u, icons[0].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(1.5f)
    );

    registerBlock(
        (new Block(BlockID::Dirt, "dirt"))
            ->setAllSidesTexture(icons[1].u, icons[1].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Cobblestone, "cobblestone"))
            ->setAllSidesTexture(icons[2].u, icons[2].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new GrassBlock(BlockID::Grass, "grass"))
            ->setTopBottomSidesTexture(
                icons[4].u, icons[4].v,  // top (grass_top)
                icons[1].u, icons[1].v,  // bottom (dirt)
                icons[3].u, icons[3].v   // sides (grass_side)
            )
            //->setSoundType(SoundType::GRASS)
            ->setDestroyTime(0.6f)
    );

    registerBlock(
        (new Block(BlockID::Stonebrick, "stonebrick"))
            ->setAllSidesTexture(icons[5].u, icons[5].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Sand, "sand"))
            ->setAllSidesTexture(icons[6].u, icons[6].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Log, "log"))
            ->setTopBottomSidesTexture(
                icons[8].u, icons[8].v,  // top (oaklog_top)
                icons[8].u, icons[8].v,  // bottom (oaklog_top)
                icons[7].u, icons[7].v   // sides (oaklog_side)
                )
            //->setSoundType(SoundType::WOOD)
            ->setDestroyTime(2.0f)
    );

    registerBlock(
        (new LeavesBlock(BlockID::Leaves, "leaves"))
            ->setAllSidesTexture(icons[9].u, icons[9].v)
            //->setSoundType(SoundType::GRASS)
            ->setOpaque(false)
            ->setDestroyTime(0.2f)
    );

    registerBlock(
        (new Block(BlockID::Glass, "glass"))
            ->setAllSidesTexture(icons[10].u, icons[10].v)
            //->setSoundType(SoundType::GLASS)
            ->setOpaque(false)
            ->setDestroyTime(0.3f)
    );

    registerBlock(
        (new Block(BlockID::Brick, "brick"))
            ->setAllSidesTexture(icons[11].u, icons[11].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(2.0f)
    );

    registerBlock(
        (new Block(BlockID::Planks, "planks"))
            ->setAllSidesTexture(icons[12].u, icons[12].v)
            //->setSoundType(SoundType::WOOD)
            ->setDestroyTime(2.0f)
    );

    registerBlock(
        (new WoolBlock(BlockID::Wool, "wool"))
            ->setAllSidesTexture(icons[13].u, icons[13].v)
            //->setSoundType(SoundType::WOOD)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Bedrock, "bedrock"))
            ->setAllSidesTexture(icons[14].u, icons[14].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(-1.0f)  // Unbreakable
    );

    registerBlock(
        (new Block(BlockID::Gravel, "gravel"))
            ->setAllSidesTexture(icons[15].u, icons[15].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Coarse, "coarse_dirt"))
            ->setAllSidesTexture(icons[16].u, icons[16].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    // TODO: Doors or multiblock structures
    //icon.door_top            = Texture_MapGetIcon(&textureMap, blockTextures[17]);
    //icon.door_bottom         = Texture_MapGetIcon(&textureMap, blockTextures[18]);


    registerBlock(
        (new Block(BlockID::Snow_Grass, "snow_grass"))
            ->setTopBottomSidesTexture(
                icons[20].u, icons[20].v,  // top
                icons[1].u, icons[1].v,  // bottom
                icons[19].u, icons[19].v   // sides
                )
            //->setSoundType(SoundType::GRASS)
            ->setDestroyTime(0.6f)
    );

    registerBlock(
        (new Block(BlockID::Snow, "snow"))
            ->setAllSidesTexture(icons[20].u, icons[20].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Obsidian, "obsidian"))
            ->setAllSidesTexture(icons[20].u, icons[20].v)
            //->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.5f)
    );

    registerBlock(
        (new Block(BlockID::Sandstone, "sandstone"))
            ->setTopBottomSidesTexture(
                icons[23].u, icons[23].v,  // top
                icons[24].u, icons[24].v,  // bottom
                icons[19].u, icons[19].v   // sides
                )
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Netherrack, "netherrack"))
            ->setAllSidesTexture(icons[25].u, icons[25].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Smooth_Stone, "smooth_stone"))
            ->setAllSidesTexture(icons[26].u, icons[26].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Lava, "lava"))
            ->setAllSidesTexture(icons[27].u, icons[27].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Water, "water"))
            ->setAllSidesTexture(icons[28].u, icons[28].v)
            ->setSoundType(SoundType::STONE)
            ->setOpaque(false)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Grass_Path, "grass_path"))
            ->setTopBottomSidesTexture(
                icons[30].u, icons[30].v,
                icons[1].u, icons[1].v,
                icons[29].u, icons[29].v
                )
            //->setSoundType(SoundType::GRASS)
            ->setDestroyTime(0.6f)
    );

    registerBlock(
        (new Block(BlockID::Crafting_Table, "crafting_table"))
            ->setTopBottomSidesTexture(
                icons[32].u, icons[32].v,
                icons[12].u, icons[12].v,
                icons[31].u, icons[31].v
                )
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Iron_Ore, "iron_ore"))
            ->setAllSidesTexture(icons[33].u, icons[33].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Iron_Block, "iron_block"))
            ->setAllSidesTexture(icons[34].u, icons[34].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Iron_Block, "iron_block"))
            ->setAllSidesTexture(icons[34].u, icons[34].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Diamond_Ore, "diamond_ore"))
            ->setAllSidesTexture(icons[35].u, icons[35].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Diamond_Block, "diamond_block"))
            ->setAllSidesTexture(icons[36].u, icons[36].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Gold_Ore, "gold_ore"))
            ->setAllSidesTexture(icons[37].u, icons[37].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Gold_Block, "gold_block"))
            ->setAllSidesTexture(icons[38].u, icons[38].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Coal_Ore, "coal_ore"))
            ->setAllSidesTexture(icons[39].u, icons[39].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Coal_Block, "coal_block"))
            ->setAllSidesTexture(icons[40].u, icons[40].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Coal_Ore, "emerald_ore"))
            ->setAllSidesTexture(icons[41].u, icons[41].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    registerBlock(
        (new Block(BlockID::Coal_Block, "emerald_block"))
            ->setAllSidesTexture(icons[42].u, icons[42].v)
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );

    //TODO: Propper furnace texture mapping
    registerBlock(
        (new Block(BlockID::Furnace, "furnace"))
            ->setTopBottomSidesTexture(
                icons[45].u, icons[45].v,
                icons[44].u, icons[44].v,
                icons[43].u, icons[43].v
                )
            ->setSoundType(SoundType::STONE)
            ->setDestroyTime(0.8f)
    );


    m_blocks.resize(static_cast<size_t>(BlockID::Count));
}

BlockRegistry::~BlockRegistry() {
    C3D_TexDelete(&textureMap.texture);
}