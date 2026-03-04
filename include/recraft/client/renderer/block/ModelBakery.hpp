#pragma once

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


#include "ModelBlock.hpp"
#include "ModelBlockDefinition.hpp"
#include "ModelResourceLocation.hpp"
#include "VariantList.hpp"
#include "blocks/Block.hpp"

// Represents a fully resolved, ready-to-use block model variant.
// One of these exists per unique IBlockState.
struct BakedBlockVariant {
    ModelResourceLocation location;
    VariantList variantList;
    ModelBlock model;
    std::optional<ModelRotation> rotation;
    bool uvLock = false;
};

class ModelBakery {
public:
    explicit ModelBakery(const std::string& assetRoot);

    void bakeAll();

    const BakedBlockVariant* getVariant(BlockID id, uint8_t metadata = 0) const;

    // Look up by the full ModelResourceLocation string, e.g. "minecraft:stone#variant=granite"
    const BakedBlockVariant* getVariant(const ModelResourceLocation& loc) const;

    const std::unordered_map<std::string, BakedBlockVariant>& getAllVariants() const;

private:
    bool loadBlockstateJson(const std::string& blockName, ModelBlockDefinition& outDefinition);

    bool loadModelJson(const ResourceLocation& location, ModelBlock& outModel);

    void resolveParents(ModelBlock& model, int depth = 0);

    BakedBlockVariant bakeVariant(const ModelResourceLocation& loc, const VariantList& variantList,
                                  const ModelBlock& model, const Variant& chosenVariant);

    std::string blockstatePath(const std::string& blockName) const;
    std::string modelPath(const ResourceLocation& location) const;


    std::string m_assetRoot;

    std::unordered_map<std::string, ModelBlock> m_modelCache;

    // Blockstate definition cache — keyed by block name
    std::unordered_map<std::string, ModelBlockDefinition> m_blockstateCache;

    std::unordered_map<std::string, BakedBlockVariant> m_bakedRegistry;

    std::unordered_map<uint32_t, const BakedBlockVariant*> m_metadataIndex;
};
