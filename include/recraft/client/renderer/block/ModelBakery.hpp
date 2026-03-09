#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ModelBlock.hpp"
#include "ModelBlockDefinition.hpp"
#include "ModelPool.hpp"
#include "ModelResourceLocation.hpp"
#include "VariantList.hpp"
#include "block/Block.hpp"

struct BakedBlockVariant {
    ModelResourceLocation location;
    VariantList variantList;
    int32_t modelIndex = ModelPool::INVALID_INDEX;
    std::optional<ModelRotation> rotation;
    bool uvLock = false;
};

class ModelBakery {
public:
    explicit ModelBakery(const std::string& assetRoot);

    void bakeAll();


    const BakedBlockVariant* getVariant(BlockID id, uint8_t metadata = 0) const;
    const BakedBlockVariant* getVariant(const ModelResourceLocation& loc) const;
    const std::unordered_map<std::string, BakedBlockVariant>& getAllVariants() const;

    const ModelPool& getPool() const { return m_pool; }

private:

    const ModelBlockDefinition* loadBlockstateJson(const std::string& blockName);

    int32_t loadModelJson(const ResourceLocation& location);

    BakedBlockVariant bakeVariant(const ModelResourceLocation& loc, const VariantList& variantList, int32_t modelIndex,
                                  const Variant& chosenVariant);

    std::string blockstatePath(const std::string& blockName) const;
    std::string modelPath(const ResourceLocation& location) const;

    std::string m_assetRoot;

    ModelPool m_pool;

    std::unordered_map<std::string, ModelBlockDefinition> m_blockstateCache;

    std::unordered_map<std::string, BakedBlockVariant> m_bakedRegistry;

    std::unordered_map<uint32_t, const BakedBlockVariant*> m_metadataIndex;
};
