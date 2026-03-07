#include "client/renderer/block/ModelBlock.hpp"
#include "misc/Crash.hpp"

#include <stdexcept>

// TODO: Add the rest if needed
ModelBlock::ModelBlock(std::optional<ResourceLocation> parentLocationIn, std::vector<BlockPart> elementsIn,
                       std::map<std::string, std::string> texturesIn, bool ambientOcclusionIn, bool gui3dIn //,
                       // ItemCameraTransforms cameraTransformsIn, std::vector<ItemOverride> overridesIn
                       ) :
    m_elements(std::move(elementsIn)), m_gui3d(gui3dIn), m_ambientOcclusion(ambientOcclusionIn),
    // m_cameraTransforms(std::move(cameraTransformsIn)), m_overrides(std::move(overridesIn)),
    m_textures(std::move(texturesIn)), m_parentLocation(std::move(parentLocationIn)) {}

ModelBlock ModelBlock::deserialize(const nlohmann::json& j) {
    ModelBlock b;
    ModelBlock::from_json(j, b);
    return b;
}

bool ModelBlock::hasParent() const { return m_parent != nullptr; }

bool ModelBlock::isGui3d() const { return m_gui3d; }

const std::vector<BlockPart>& ModelBlock::getElements() const {
    if (m_elements.empty() && m_parent != nullptr)
        return m_parent->getElements();
    return m_elements;
}

bool ModelBlock::isAmbientOcclusion() const {
    return hasParent() ? m_parent->isAmbientOcclusion() : m_ambientOcclusion;
}

bool ModelBlock::isResolved() const {
    return !m_parentLocation.has_value() || (m_parent != nullptr && m_parent->isResolved());
}

void ModelBlock::getParentFromMap(const std::unordered_map<std::string, std::unique_ptr<ModelBlock>>& map) {
    if (!m_parentLocation.has_value())
        return;

    auto it = map.find(m_parentLocation->toString());
    if (it != map.end()) {
        m_parent = it->second.get();
        return;
    }

    ResourceLocation withNamespace("minecraft", m_parentLocation->getResourcePath());
    it = map.find(withNamespace.toString());
    if (it != map.end()) {
        m_parent = it->second.get();
        return;
    }

    Crash("ModelBlock: could not resolve parent {} for model", m_parentLocation->toString());
}

std::vector<ResourceLocation> ModelBlock::getOverrideLocations() const {
    return {}; // TODO
}

const std::optional<ResourceLocation>& ModelBlock::getParentLocation() const { return m_parentLocation; }

const ModelBlock* ModelBlock::getRootModel() const { return hasParent() ? m_parent->getRootModel() : this; }

bool ModelBlock::startsWithHash(const std::string& s) const { return !s.empty() && s[0] == '#'; }

bool ModelBlock::isTexturePresent(const std::string& textureName) const {
    return resolveTextureName(textureName) != "missingno";
}

std::string ModelBlock::resolveTextureName(const std::string& textureName) const {
    std::string t = startsWithHash(textureName) ? textureName : '#' + textureName;
    Bookkeep bk(this);
    return resolveTextureName(t, bk);
}

std::string ModelBlock::resolveTextureName(const std::string& textureName, Bookkeep& bk) const {
    if (!startsWithHash(textureName))
        return textureName;

    if (this == bk.modelExt)
        return "missingno";

    auto it = m_textures.find(textureName.substr(1));
    std::string s = (it != m_textures.end()) ? it->second : "";

    if (s.empty() && hasParent())
        s = m_parent->resolveTextureName(textureName, bk);

    bk.modelExt = this;

    if (!s.empty() && startsWithHash(s))
        s = bk.model->resolveTextureName(s, bk);

    return (!s.empty() && !startsWithHash(s)) ? s : "missingno";
}

void ModelBlock::checkModelHierarchy(const std::unordered_map<std::string, std::unique_ptr<ModelBlock>>& models) {
    for (const auto& [key, blockPtr] : models) {
        const ModelBlock* a = blockPtr->m_parent;
        if (!a)
            continue;
        const ModelBlock* b = a->m_parent;

        while (a != b) {
            if (!a->m_parent || !b->m_parent || !b->m_parent->m_parent)
                break;
            a = a->m_parent;
            b = b->m_parent->m_parent;
        }

        if (a == b)
            throw LoopException();
    }
}

std::vector<BlockPart> ModelBlock::parseElements(const nlohmann::json& j) {
    std::vector<BlockPart> elements;
    if (!j.contains("elements"))
        return elements;

    const auto& arr = j.at("elements");
    elements.reserve(arr.size());

    for (const auto& el : arr) {
        BlockPart& part = elements.emplace_back();
        BlockPart::from_json(el, part);
    }
    return elements;
}

std::string ModelBlock::parseParent(const nlohmann::json& j) { return j.value("parent", ""); }

std::map<std::string, std::string> ModelBlock::parseTextures(const nlohmann::json& j) {
    std::map<std::string, std::string> map;
    if (j.contains("textures"))
        for (const auto& [k, v] : j.at("textures").items())
            map.emplace(k, v.get<std::string>());
    return map;
}

bool ModelBlock::parseAmbientOcclusion(const nlohmann::json& j) { return j.value("ambientocclusion", true); }

void ModelBlock::from_json(const nlohmann::json& j, ModelBlock& b) {
    std::string parent = parseParent(j);
    b = ModelBlock(parent.empty() ? std::nullopt : std::make_optional<ResourceLocation>(parent), parseElements(j),
                   parseTextures(j), parseAmbientOcclusion(j), true);
}
