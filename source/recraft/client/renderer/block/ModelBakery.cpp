#include "client/renderer/block/ModelBakery.hpp"
#include "misc/Crash.hpp"

#include <fstream>
#include <sstream>

ModelBakery::ModelBakery(const std::string& assetRoot) : m_assetRoot(assetRoot) {}

void ModelBakery::bakeAll() {
    // ---- Pass 1: load all models (and their parents) into the cache ----
    for (uint16_t i = 0; i < static_cast<uint16_t>(BlockID::Count); ++i) {
        BlockID id = static_cast<BlockID>(i);

        BlockPtr block = BlockRegistry::GetInstance().GetBlock(id);
        if (!block)
            continue;

        const std::string& name = BlockRegistry::GetTextualID(id);
        if (name.empty())
            continue;

        ModelBlockDefinition definition;
        if (!loadBlockstateJson(name, definition))
            continue;

        for (const auto& [variantKey, variantList] : definition.getVariants()) {
            if (variantList.getVariantList().empty())
                continue;

            const Variant& primary = variantList.getVariantList().front();
            loadModelJson(primary.getModelLocation()); // inserts into m_modelCache
        }
    }

    // ---- Pass 2: resolve parents — ALL insertions are done, pointers are stable ----
    for (auto& [key, modelPtr] : m_modelCache)
        modelPtr->getParentFromMap(m_modelCache);

    // ---- Pass 3: bake variants ----
    for (uint16_t i = 0; i < static_cast<uint16_t>(BlockID::Count); ++i) {
        BlockID id = static_cast<BlockID>(i);

        const BlockPtr block = BlockRegistry::GetBlock(id);
        if (!block)
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

ModelBlock* ModelBakery::loadModelJson(const ResourceLocation& location) {
    const std::string key = location.toString();

    // Return existing entry — never re-insert, keeps all pointers stable
    auto it = m_modelCache.find(key);
    if (it != m_modelCache.end())
        return it->second.get();

    const std::string path = modelPath(location);
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;

    try {
        nlohmann::json j;
        file >> j;

        auto model = std::make_unique<ModelBlock>(ModelBlock::deserialize(j));
        model->name = key;

        // Grab parent location before moving the model into the cache
        std::optional<ResourceLocation> parentLoc = model->getParentLocation();

        // Insert into cache BEFORE recursing — prevents infinite loops on cycles
        ModelBlock* raw = model.get();
        m_modelCache.emplace(key, std::move(model));

        // Recursively load parent (will early-out above if already cached)
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