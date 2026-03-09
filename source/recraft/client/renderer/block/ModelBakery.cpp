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
            loadModelJson(primary.getModelLocation()); // recurses for parents
        }
    }

    m_pool.freeze();

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
            const std::string modelKey = primary.getModelLocation().toString();

            int32_t modelIndex = m_pool.find(modelKey);
            if (modelIndex == ModelPool::INVALID_INDEX)
                continue;

            ModelResourceLocation mrl(ResourceLocation("minecraft", name), variantKey);
            BakedBlockVariant baked = bakeVariant(mrl, variantList, modelIndex, primary);

            auto [it, inserted] = m_bakedRegistry.emplace(mrl.toString(), std::move(baked));
            if (inserted) {
                uint32_t key = (static_cast<uint32_t>(id) << 4) | (metadataIdx & 0xF);
                m_metadataIndex[key] = &it->second;
            }
            ++metadataIdx;
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

int32_t ModelBakery::loadModelJson(const ResourceLocation& location) {
    const std::string key = location.toString();

    // Already in pool — return its index
    int32_t existing = m_pool.find(key);
    if (existing != ModelPool::INVALID_INDEX)
        return existing;

    std::ifstream file(modelPath(location));
    if (!file.is_open())
        return ModelPool::INVALID_INDEX;

    try {
        nlohmann::json j;
        file >> j;

        ModelBlock model = ModelBlock::deserialize(j);

        std::optional<ResourceLocation> parentLoc = model.getParentLocation();

        int32_t index = m_pool.internOrGet(key, std::move(model));

        if (parentLoc.has_value())
            loadModelJson(*parentLoc);

        return index;
    } catch (...) {
        return ModelPool::INVALID_INDEX;
    }
}

BakedBlockVariant ModelBakery::bakeVariant(const ModelResourceLocation& loc, const VariantList& variantList,
                                           int32_t modelIndex, const Variant& chosenVariant) {
    return BakedBlockVariant{loc, variantList, modelIndex, chosenVariant.getRotation(), chosenVariant.isUvLock()};
}

std::string ModelBakery::blockstatePath(const std::string& blockName) const {
    return m_assetRoot + "/blockstates/" + blockName + ".json";
}

std::string ModelBakery::modelPath(const ResourceLocation& location) const {
    return m_assetRoot + "/models/" + location.getResourcePath() + ".json";
}
