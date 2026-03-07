#pragma once

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include "BlockPart.hpp"
#include "util/ResourceLocation.hpp"

class ModelBlock {
public:
    std::string name;

    ModelBlock(std::optional<ResourceLocation> parentLocationIn, std::vector<BlockPart> elementsIn,
               std::map<std::string, std::string> texturesIn, bool ambientOcclusionIn, bool gui3dIn //,
               // ItemCameraTransforms cameraTransformsIn, std::vector<ItemOverride> overridesIn
    );
    ModelBlock() : m_gui3d(true), m_ambientOcclusion(true) /*, m_cameraTransforms(ItemCameraTransforms::DEFAULT)*/ {}

    static ModelBlock deserialize(const nlohmann::json& j);

    const std::vector<BlockPart>& getElements() const;
    bool isAmbientOcclusion() const;
    bool isGui3d() const;
    bool isResolved() const;

    void getParentFromMap(const std::unordered_map<std::string, std::unique_ptr<ModelBlock>>& map);

    std::vector<ResourceLocation> getOverrideLocations() const;

    bool isTexturePresent(const std::string& textureName) const;
    std::string resolveTextureName(const std::string& textureName) const;

    const std::optional<ResourceLocation>& getParentLocation() const;

    const ModelBlock* getRootModel() const;

    static void checkModelHierarchy(const std::unordered_map<std::string, std::unique_ptr<ModelBlock>>& models);

    class LoopException : public std::runtime_error {
    public:
        LoopException() : std::runtime_error("Model hierarchy loop detected") {}
    };

    const std::map<std::string, std::string>& getTextures() const { return m_textures; }

private:
    std::vector<BlockPart> m_elements;
    bool m_gui3d;
    bool m_ambientOcclusion;
    // ItemCameraTransforms m_cameraTransforms;
    // std::vector<ItemOverride> m_overrides;
    std::map<std::string, std::string> m_textures;
    ModelBlock* m_parent = nullptr;
    std::optional<ResourceLocation> m_parentLocation;

    bool hasParent() const;
    bool startsWithHash(const std::string& s) const;

    struct Bookkeep {
        const ModelBlock* model;
        const ModelBlock* modelExt;
        explicit Bookkeep(const ModelBlock* m) : model(m), modelExt(nullptr) {}
    };

    std::string resolveTextureName(const std::string& textureName, Bookkeep& bookkeep) const;

    static std::vector<BlockPart> parseElements(const nlohmann::json& j);
    static std::string parseParent(const nlohmann::json& j);
    static std::map<std::string, std::string> parseTextures(const nlohmann::json& j);
    static bool parseAmbientOcclusion(const nlohmann::json& j);

    static void from_json(const nlohmann::json& j, ModelBlock& b);
};
