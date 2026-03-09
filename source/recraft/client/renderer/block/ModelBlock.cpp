#include "client/renderer/block/ModelBlock.hpp"
#include "client/renderer/block/ModelPool.hpp"
#include "misc/Crash.hpp"

#include <stdexcept>

ModelBlock::ModelBlock(std::optional<ResourceLocation> parentLocationIn, std::vector<BlockPart> elementsIn,
                       std::map<std::string, std::string> texturesIn, bool ambientOcclusionIn, bool gui3dIn) :
    m_parentIndex(-1), m_selfIndex(-1), m_elements(std::move(elementsIn)), m_gui3d(gui3dIn),
    m_ambientOcclusion(ambientOcclusionIn), m_textures(std::move(texturesIn)),
    m_parentLocation(std::move(parentLocationIn)) {}

ModelBlock ModelBlock::deserialize(const nlohmann::json& j) {
    ModelBlock b;
    ModelBlock::from_json(j, b);
    return b;
}

bool ModelBlock::isGui3d() const { return m_gui3d; }

const std::optional<ResourceLocation>& ModelBlock::getParentLocation() const { return m_parentLocation; }

bool ModelBlock::isResolved() const {
    return !m_parentLocation.has_value() || m_parentIndex != -1;
}

std::vector<ResourceLocation> ModelBlock::getOverrideLocations() const {
    return {}; // TODO
}

bool ModelBlock::startsWithHash(const std::string& s) const { return !s.empty() && s[0] == '#'; }

const std::vector<BlockPart>& ModelBlock::getElements(const ModelPool& pool) const {
    int32_t current = m_selfIndex;
    while (current != ModelPool::INVALID_INDEX) {
        const ModelBlock& m = pool.get(current);
        if (!m.m_elements.empty())
            return m.m_elements;
        current = m.m_parentIndex;
    }
    return m_elements;
}

bool ModelBlock::isAmbientOcclusion(const ModelPool& pool) const {
    int32_t current = m_selfIndex;
    int32_t last = current;
    while (current != ModelPool::INVALID_INDEX) {
        last = current;
        current = pool.get(current).m_parentIndex;
    }
    return pool.get(last).m_ambientOcclusion;
}

const ModelBlock* ModelBlock::getRootModel(const ModelPool& pool) const {
    int32_t current = m_selfIndex;
    int32_t last = current;
    while (current != ModelPool::INVALID_INDEX) {
        last = current;
        current = pool.get(current).m_parentIndex;
    }
    return &pool.get(last);
}

std::string ModelBlock::resolveTextureIter(const std::string& key, const ModelPool& pool) const {
    std::string current = key;

    constexpr int MAX_HOPS = 64;

    for (int hop = 0; hop < MAX_HOPS; ++hop) {
        bool found = false;
        int32_t node = m_selfIndex;

        while (node != ModelPool::INVALID_INDEX) {
            const ModelBlock& m = pool.get(node);
            auto it = m.m_textures.find(current);
            if (it != m.m_textures.end()) {
                const std::string& val = it->second;
                if (!startsWithHash(val))
                    return val;

                current = val.substr(1);
                found = true;
                break;
            }
            node = m.m_parentIndex;
        }

        if (!found)
            break;
    }

    return "missingno";
}

std::string ModelBlock::resolveTextureName(const std::string& textureName, const ModelPool& pool) const {
    const std::string key = startsWithHash(textureName) ? textureName.substr(1) : textureName;

    if (!startsWithHash(textureName))
        return textureName;

    return resolveTextureIter(key, pool);
}

bool ModelBlock::isTexturePresent(const std::string& textureName, const ModelPool& pool) const {
    return resolveTextureName(textureName, pool) != "missingno";
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
