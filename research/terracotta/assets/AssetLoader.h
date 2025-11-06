#pragma once

#ifndef TERRACOTTA_ASSETS_ASSETLOADER_H_
#define TERRACOTTA_ASSETS_ASSETLOADER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "TextureArray.h"
#include "../block/BlockFace.h"
#include "../block/BlockElement.h"

namespace terra {

namespace block {

class BlockModel;

} // ns block

namespace assets {

// Stores the intermediate faces of a block. The textures are resolved later and stored into a final RenderableFace.
struct IntermediateFace {
    block::BlockFace face;
    block::BlockFace cull_face;
    std::string texture;
    int tint_index;
    glm::vec2 uv_from;
    glm::vec2 uv_to;
};

// Stores the intermediate element for a BlockModel. The faces are resolved later and stored in a final BlockElement.
struct IntermediateElement {
    glm::vec3 from;
    glm::vec3 to;
    bool shade;
    block::ElementRotation rotation;

    std::vector<IntermediateFace> faces;
};

class ZipArchive;
class AssetCache;

class AssetLoader {
public:
    AssetLoader(AssetCache& cache);

    bool LoadArchive(const std::string& archive_path, const std::string& blocks_path);

private:
    using TextureMap = std::unordered_map<std::string, std::string>;

    bool LoadBlockStates(const std::string& filename);
    std::size_t LoadBlockModels(terra::assets::ZipArchive& archive);
    void LoadBlockVariants(terra::assets::ZipArchive& archive);
    bool LoadTexture(terra::assets::ZipArchive& archive, const std::string& path, TextureHandle* handle);
    std::unique_ptr<block::BlockModel> LoadBlockModel(terra::assets::ZipArchive& archive, const std::string& path, TextureMap& texture_map, std::vector<IntermediateElement>& intermediates);

    AssetCache& m_Cache;
};

} // ns assets
} // ns terra

#endif

