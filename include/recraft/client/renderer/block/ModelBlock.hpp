#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include "BlockPart.hpp"
#include "util/ResourceLocation.hpp"

class ModelPool;

class ModelBlock {
public:
    std::string name;

    ModelBlock(std::optional<ResourceLocation> parentLocationIn, std::vector<BlockPart> elementsIn,
               std::map<std::string, std::string> texturesIn, bool ambientOcclusionIn, bool gui3dIn);

    ModelBlock() : m_parentIndex(-1), m_gui3d(true), m_ambientOcclusion(true) {}

    static ModelBlock deserialize(const nlohmann::json& j);

    const std::vector<BlockPart>& getElements(const ModelPool& pool) const;
    bool isAmbientOcclusion(const ModelPool& pool) const;
    std::string resolveTextureName(const std::string& textureName, const ModelPool& pool) const;
    bool isTexturePresent(const std::string& textureName, const ModelPool& pool) const;

    bool isGui3d() const;
    bool isResolved() const;
    const std::optional<ResourceLocation>& getParentLocation() const;
    const std::map<std::string, std::string>& getTextures() const { return m_textures; }
    std::vector<ResourceLocation> getOverrideLocations() const;

    int32_t m_parentIndex = -1;
    int32_t m_selfIndex = -1;

    void resolveParentIndex(int32_t index) { m_parentIndex = index; }

    const ModelBlock* getRootModel(const ModelPool& pool) const;

    class LoopException : public std::runtime_error {
    public:
        LoopException() : std::runtime_error("Model hierarchy loop detected") {}
    };

private:
    std::vector<BlockPart> m_elements;
    bool m_gui3d;
    bool m_ambientOcclusion;
    std::map<std::string, std::string> m_textures;
    std::optional<ResourceLocation> m_parentLocation;

    bool startsWithHash(const std::string& s) const;

    std::string resolveTextureIter(const std::string& key, const ModelPool& pool) const;

    static std::vector<BlockPart> parseElements(const nlohmann::json& j);
    static std::string parseParent(const nlohmann::json& j);
    static std::map<std::string, std::string> parseTextures(const nlohmann::json& j);
    static bool parseAmbientOcclusion(const nlohmann::json& j);
    static void from_json(const nlohmann::json& j, ModelBlock& b);
};
