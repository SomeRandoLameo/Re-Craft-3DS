#include "client/renderer/block/ModelBakery.hpp"
#include "misc/Crash.hpp"

#include <fstream>
#include <sstream>

ModelBakery::ModelBakery(const std::string& assetRoot) : m_assetRoot(assetRoot) {}

void ModelBakery::bakeAll() {
    for (uint16_t i = 0; i < static_cast<uint16_t>(BlockID::Count); ++i) {
        BlockID id = static_cast<BlockID>(i);

        if (!BlockRegistry::GetInstance().GetBlock(id))
            continue;

        const std::string& name = BlockRegistry::GetTextualID(id);
        if (name.empty())
            continue;

        const ModelBlockDefinition* definition = loadBlockstateJson(name);
        if (!definition)
            continue;

        for (const auto& [variantKey, variantList] : definition->getVariants()) {
            if (variantList.getVariantList().empty())
                continue;

            const Variant& primary = variantList.getVariantList().front();
            loadModelJson(primary.getModelLocation());
        }
    }

    for (auto& [key, modelPtr] : m_modelCache)
        modelPtr->getParentFromMap(m_modelCache);

    for (uint16_t i = 0; i < static_cast<uint16_t>(BlockID::Count); ++i) {
        BlockID id = static_cast<BlockID>(i);

        if (!BlockRegistry::GetBlock(id))
            continue;

        const std::string& name = BlockRegistry::GetTextualID(id);
        if (name.empty())
            continue;

        auto defIt = m_blockstateCache.find(name);
        if (defIt == m_blockstateCache.end())
            continue;

        uint8_t metadataIdx = 0;
        for (const auto& [variantKey, variantList] : defIt->second.getVariants()) {
            if (variantList.getVariantList().empty())
                continue;

            const Variant& primary = variantList.getVariantList().front();

            auto modelIt = m_modelCache.find(primary.getModelLocation().toString());
            if (modelIt == m_modelCache.end())
                continue;

            ModelResourceLocation mrl(ResourceLocation("minecraft", name), variantKey);
            BakedBlockVariant baked = bakeVariant(mrl, variantList, *modelIt->second, primary);

            auto [it, inserted] = m_bakedRegistry.emplace(mrl.toString(), std::move(baked));
            if (inserted) {
                uint32_t key = (static_cast<uint32_t>(id) << 4) | (metadataIdx & 0xF);
                m_metadataIndex[key] = &it->second;
            }
            metadataIdx++;
        }
    }
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

const ModelBlockDefinition* ModelBakery::loadBlockstateJson(const std::string& blockName) {
    auto existing = m_blockstateCache.find(blockName);
    if (existing != m_blockstateCache.end())
        return &existing->second;

    std::ifstream file(blockstatePath(blockName));
    if (!file.is_open())
        return nullptr;

    try {
        nlohmann::json j;
        file >> j;
        auto [it, _] = m_blockstateCache.emplace(blockName, ModelBlockDefinition::parseFromJson(j));
        return &it->second;
    } catch (...) {
        return nullptr;
    }
}

ModelBlock* ModelBakery::loadModelJson(const ResourceLocation& location) {
    const std::string key = location.toString();

    auto it = m_modelCache.find(key);
    if (it != m_modelCache.end())
        return it->second.get();

    std::ifstream file(modelPath(location));
    if (!file.is_open())
        return nullptr;

    try {
        nlohmann::json j;
        file >> j;

        auto model = std::make_unique<ModelBlock>(ModelBlock::deserialize(j));
        model->name = key;

        std::optional<ResourceLocation> parentLoc = model->getParentLocation();

        ModelBlock* raw = model.get();
        m_modelCache.emplace(key, std::move(model));

        if (parentLoc.has_value())
            loadModelJson(*parentLoc);

        return raw;
    } catch (...) {
        return nullptr;
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
