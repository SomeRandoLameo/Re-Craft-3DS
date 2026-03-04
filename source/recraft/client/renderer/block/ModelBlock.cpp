#include "client/renderer/block/ModelBlock.hpp"

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
    return m_elements.empty() && hasParent() ? m_parent->getElements() : m_elements;
}

bool ModelBlock::isAmbientOcclusion() const {
    return hasParent() ? m_parent->isAmbientOcclusion() : m_ambientOcclusion;
}

bool ModelBlock::isResolved() const {
    return !m_parentLocation.has_value() || (m_parent != nullptr && m_parent->isResolved());
}

void ModelBlock::getParentFromMap(const std::map<ResourceLocation, ModelBlock*>& map) {
    if (m_parentLocation.has_value()) {
        auto it = map.find(*m_parentLocation);
        if (it != map.end())
            m_parent = it->second;
    }
}

std::vector<ResourceLocation> ModelBlock::getOverrideLocations() const {
    std::vector<ResourceLocation> locs;
    // for (const auto& o : m_overrides)
    //     locs.push_back(o.getLocation());
    return locs;
}

// const std::vector<ItemOverride>& ModelBlock::getOverrides() const { return m_overrides; }

// ItemOverrideList ModelBlock::createOverrides() const {
// return m_overrides.empty() ? ItemOverrideList::NONE : ItemOverrideList(m_overrides);
// }

std::optional<ResourceLocation> ModelBlock::getParentLocation() const { return m_parentLocation; }

const ModelBlock* ModelBlock::getRootModel() const { return hasParent() ? m_parent->getRootModel() : this; }

// ---- Texture resolution -----------------------------------------------------

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

    if (this == bk.modelExt) {
        // LOGGER warn would go here
        return "missingno";
    }

    auto it = m_textures.find(textureName.substr(1));
    std::string s = (it != m_textures.end()) ? it->second : "";

    if (s.empty() && hasParent())
        s = m_parent->resolveTextureName(textureName, bk);

    bk.modelExt = this;

    if (!s.empty() && startsWithHash(s))
        s = bk.model->resolveTextureName(s, bk);

    return (!s.empty() && !startsWithHash(s)) ? s : "missingno";
}

// ItemTransformVec3f ModelBlock::getTransform(ItemCameraTransforms::TransformType type) const {
//     if (m_parent && !m_cameraTransforms.hasCustomTransform(type))
//         return m_parent->getTransform(type);
//     return m_cameraTransforms.getTransform(type);
// }

// ItemCameraTransforms ModelBlock::getAllTransforms() const {
//     return ItemCameraTransforms(getTransform(ItemCameraTransforms::TransformType::THIRD_PERSON_LEFT_HAND),
//                                 getTransform(ItemCameraTransforms::TransformType::THIRD_PERSON_RIGHT_HAND),
//                                 getTransform(ItemCameraTransforms::TransformType::FIRST_PERSON_LEFT_HAND),
//                                 getTransform(ItemCameraTransforms::TransformType::FIRST_PERSON_RIGHT_HAND),
//                                 getTransform(ItemCameraTransforms::TransformType::HEAD),
//                                 getTransform(ItemCameraTransforms::TransformType::GUI),
//                                 getTransform(ItemCameraTransforms::TransformType::GROUND),
//                                 getTransform(ItemCameraTransforms::TransformType::FIXED));
// }

void ModelBlock::checkModelHierarchy(const std::map<ResourceLocation, ModelBlock>& models) {
    for (const auto& [loc, block] : models) {
        const ModelBlock* a = block.m_parent;
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
    if (j.contains("elements"))
        for (const auto& el : j.at("elements")) {
            BlockPart part;
            BlockPart::from_json(el, part);
            elements.push_back(std::move(part));
        }
    return elements;
}

std::string ModelBlock::parseParent(const nlohmann::json& j) { return j.value("parent", ""); }

std::map<std::string, std::string> ModelBlock::parseTextures(const nlohmann::json& j) {
    std::map<std::string, std::string> map;
    if (j.contains("textures"))
        for (const auto& [k, v] : j.at("textures").items())
            map[k] = v.get<std::string>();
    return map;
}

bool ModelBlock::parseAmbientOcclusion(const nlohmann::json& j) { return j.value("ambientocclusion", true); }

// std::vector<ItemOverride> ModelBlock::parseOverrides(const nlohmann::json& j) {
//     std::vector<ItemOverride> overrides;
//     if (j.contains("overrides"))
//         for (const auto& el : j.at("overrides")) {
//             ItemOverride o;
//             ItemOverride::from_json(el, o);
//             overrides.push_back(std::move(o));
//         }
//     return overrides;
// }


void ModelBlock::from_json(const nlohmann::json& j, ModelBlock& b) {
    std::string parent = parseParent(j);

    // ItemCameraTransforms transforms = ItemCameraTransforms::DEFAULT;
    // if (j.contains("display"))
    //     ItemCameraTransforms::from_json(j.at("display"), transforms);

    b = ModelBlock(parent.empty() ? std::nullopt : std::make_optional<ResourceLocation>(parent),
                   ModelBlock::parseElements(j), ModelBlock::parseTextures(j), ModelBlock::parseAmbientOcclusion(j),
                   true /*, transforms, ModelBlock::parseOverrides(j)*/);
}
