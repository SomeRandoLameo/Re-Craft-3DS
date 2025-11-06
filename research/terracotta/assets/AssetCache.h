#ifndef TERRACOTTA_ASSETS_ASSETCACHE_H_
#define TERRACOTTA_ASSETS_ASSETCACHE_H_

#include "TextureArray.h"

#include <unordered_map>
#include <string>
#include <memory>
#include "../block/BlockState.h"
#include "../block/BlockModel.h"
#include "../block/BlockVariant.h"

#include <mclib/block/Block.h>

namespace terra {
namespace assets {

class ZipArchive;

class AssetCache {
public:
    AssetCache() { }

    ~AssetCache() { }
    block::BlockState* GetBlockState(u32 block_id) const;
    void AddBlockState(std::unique_ptr<terra::block::BlockState> state);
    void SetMaxBlockId(std::size_t id);

    TextureArray& GetTextures() { return m_TextureArray; }
    TextureHandle AddTexture(const std::string& path, const std::string& data);

    void AddVariantModel(std::unique_ptr<block::BlockVariant> variant);
    block::BlockVariant* GetVariant(mc::block::BlockPtr block);

    std::vector<block::BlockModel*> GetBlockModels(const std::string& find);
    block::BlockModel* GetBlockModel(const std::string& path);
    void AddBlockModel(const std::string& path, std::unique_ptr<block::BlockModel> model);

private:
    block::BlockVariant* GetVariantFromProperties(const std::string& block_name, const std::string& properties);

    // Maps block id to the BlockState
    std::vector<std::unique_ptr<terra::block::BlockState>> m_BlockStates;
    std::vector<terra::block::BlockVariant*> m_VariantCache;

    // Maps model path to a BlockModel
    std::unordered_map<std::string, std::unique_ptr<terra::block::BlockModel>> m_BlockModels;
    // Block name -> (list of Variants)
    std::unordered_map<std::string, std::vector<std::unique_ptr<block::BlockVariant>>> m_BlockVariants;

    TextureArray m_TextureArray;
};

} // assets
} // terra

extern std::unique_ptr<terra::assets::AssetCache> g_AssetCache;

#endif
