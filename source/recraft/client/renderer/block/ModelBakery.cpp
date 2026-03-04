#pragma once

#include "client/renderer/block/ModelBakery.hpp"

#include <fstream>
#include <sstream>


ModelBakery::ModelBakery(const std::string& assetRoot) : m_assetRoot(assetRoot) {}

void ModelBakery::bakeAll() {
    for (uint16_t i = 0; i < static_cast<uint16_t>(BlockID::Count); ++i) {
        BlockID id = static_cast<BlockID>(i);
        const BlockPtr block = BlockRegistry::GetBlock(id);
        if (!block)
            continue;

        const char* name = block->getName();
        if (!name || name[0] == '\0')
            continue;

        ModelBlockDefinition definition;
        if (!loadBlockstateJson(name, definition))
            continue;

        for (const auto& [variantKey, variantList] : definition.getVariants()) {
            if (variantList.getVariantList().empty())
                continue;

            const Variant& primary = variantList.getVariantList().front();

            ModelBlock model;
            if (!loadModelJson(primary.getModelLocation(), model))
                continue;

            ModelResourceLocation mrl(ResourceLocation("minecraft", name), variantKey);
            BakedBlockVariant baked = bakeVariant(mrl, variantList, model, primary);
            m_bakedRegistry.emplace(mrl.toString(), std::move(baked));
        }
    }

    // Resolve parent chains across all loaded models in one pass
    std::map<ResourceLocation, ModelBlock> modelMap;
    for (auto& [key, model] : m_modelCache)
        modelMap[ResourceLocation(key)] = model;

    std::map<ResourceLocation, ModelBlock*> ptrMap;
    for (auto& [loc, model] : modelMap)
        ptrMap[loc] = &model;

    for (auto& [loc, model] : modelMap)
        model.getParentFromMap(ptrMap);

    // Write resolved models back into cache
    for (auto& [loc, model] : modelMap)
        m_modelCache[loc.toString()] = model;
}

const BakedBlockVariant* ModelBakery::getVariant(BlockID id, uint8_t metadata) const {
    uint32_t key = (static_cast<uint32_t>(id) << 4) | (metadata & 0xF);
    auto it = m_metadataIndex.find(key);
    return it != m_metadataIndex.end() ? it->second : nullptr;
}

const BakedBlockVariant* ModelBakery::getVariant(const ModelResourceLocation& loc) const {
    auto it = m_bakedRegistry.find(loc.toString());
    return it != m_bakedRegistry.end() ? &it->second : nullptr;
}

const std::unordered_map<std::string, BakedBlockVariant>& ModelBakery::getAllVariants() const {
    return m_bakedRegistry;
}

bool ModelBakery::loadBlockstateJson(const std::string& blockName, ModelBlockDefinition& outDefinition) {
    const std::string path = blockstatePath(blockName);
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    try {
        nlohmann::json j;
        file >> j;
        outDefinition = ModelBlockDefinition::parseFromJson(j);
        m_blockstateCache[blockName] = outDefinition;
        return true;
    } catch (...) {
        return false;
    }
}

bool ModelBakery::loadModelJson(const ResourceLocation& location, ModelBlock& outModel) {
    const std::string key = location.toString();

    auto cached = m_modelCache.find(key);
    if (cached != m_modelCache.end()) {
        outModel = cached->second;
        return true;
    }

    const std::string path = modelPath(location);
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    try {
        nlohmann::json j;
        file >> j;
        outModel = ModelBlock::deserialize(j);
        outModel.name = key;
        m_modelCache[key] = outModel;
        return true;
    } catch (...) {
        return false;
    }
}

BakedBlockVariant ModelBakery::bakeVariant(const ModelResourceLocation& loc, const VariantList& variantList,
                                           const ModelBlock& model, const Variant& chosenVariant) {
    return BakedBlockVariant{loc, variantList, model, chosenVariant.getRotation(), chosenVariant.isUvLock()};
}

std::string ModelBakery::blockstatePath(const std::string& blockName) const {
    return m_assetRoot + "/blockstates/" + blockName + ".json";
}

std::string ModelBakery::modelPath(const ResourceLocation& location) const {
    return m_assetRoot + "/models/" + location.getResourcePath() + ".json";
}
