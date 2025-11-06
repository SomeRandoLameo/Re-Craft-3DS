#include "AssetCache.h"
#include "zip/ZipArchive.h"

#include <mclib/common/Json.h>
#include <mclib/block/Block.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "stb_image.h"

namespace terra {
namespace assets {

void AssetCache::SetMaxBlockId(std::size_t id) {
    m_BlockStates.resize(id + 1);
    m_VariantCache.resize(id + 1);

    std::fill(m_VariantCache.begin(), m_VariantCache.end(), nullptr);
}

terra::block::BlockState* AssetCache::GetBlockState(u32 block_id) const {
    if (block_id >= m_BlockStates.size()) return nullptr;

    return m_BlockStates[block_id].get();
}

block::BlockModel* AssetCache::GetBlockModel(const std::string& path) {
    auto iter = m_BlockModels.find(path);

    if (iter == m_BlockModels.end()) return nullptr;

    return iter->second.get();
}

std::vector<block::BlockModel*> AssetCache::GetBlockModels(const std::string& find) {
    std::vector<block::BlockModel*> result;

    for (auto& kv : m_BlockModels) {
        if (kv.first.find(find) != std::string::npos) {
            result.push_back(kv.second.get());
        }
    }

    return result;
}

void AssetCache::AddBlockState(std::unique_ptr<terra::block::BlockState> state) {
    m_BlockStates[state->GetId()] = std::move(state);
}

TextureHandle AssetCache::AddTexture(const std::string& path, const std::string& data) {
    return m_TextureArray.Append(path, data);
}

void AssetCache::AddBlockModel(const std::string& path, std::unique_ptr<block::BlockModel> model) {
    m_BlockModels[path] = std::move(model);
}

block::BlockVariant* AssetCache::GetVariantFromProperties(const std::string& block_name, const std::string& properties) {
    auto block_iter = m_BlockVariants.find(block_name);

    if (block_iter == m_BlockVariants.end()) return nullptr;

    for (auto&& variant : block_iter->second) {
        if (properties.find(variant->GetProperties()) != std::string::npos) {
            return variant.get();
        }
    }

    return nullptr;
}

void AssetCache::AddVariantModel(std::unique_ptr<block::BlockVariant> variant) {
    m_BlockVariants[variant->GetBlock()->GetName()].push_back(std::move(variant));
}

block::BlockVariant* AssetCache::GetVariant(mc::block::BlockPtr block) {
    block::BlockVariant* variant = m_VariantCache[block->GetType()];

    if (variant == nullptr) {
        variant = GetVariantFromProperties(block->GetName(), GetBlockState(block->GetType())->GetVariant());

        m_VariantCache[block->GetType()] = variant;
    }

    return variant;
}

} // ns assets
} // ns terra
