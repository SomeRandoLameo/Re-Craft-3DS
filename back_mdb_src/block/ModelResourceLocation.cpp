#include "client/renderer/block/ModelResourceLocation.hpp"

#include <algorithm>
#include <functional>

static std::string toLowerMRL(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

ModelResourceLocation::ParsedPath ModelResourceLocation::parsePathString(const std::string& pathIn) {
    ParsedPath result;

    auto hashPos = pathIn.find('#');

    if (hashPos != std::string::npos) {
        std::string variantRaw = pathIn.substr(hashPos + 1);
        result.variant = toLowerMRL(variantRaw);

        const std::string locationPart = (hashPos > 1) ? pathIn.substr(0, hashPos) : pathIn;
        auto [domain, path] = ResourceLocation::splitObjectName(locationPart);
        result.domain = std::move(domain);
        result.path   = std::move(path);
    } else {
        result.variant = "normal";
        auto [domain, path] = ResourceLocation::splitObjectName(pathIn);
        result.domain = std::move(domain);
        result.path   = std::move(path);
    }

    if (result.variant.empty())
        result.variant = "normal";

    return result;
}

ModelResourceLocation::ModelResourceLocation(const ParsedPath& parsed) :
    ResourceLocation(parsed.domain, parsed.path), m_variant(parsed.variant) {}

ModelResourceLocation::ModelResourceLocation(const std::string& pathIn) :
    ModelResourceLocation(parsePathString(pathIn)) {}

ModelResourceLocation::ModelResourceLocation(const ResourceLocation& location, const std::string& variantIn) :
    ModelResourceLocation(location.toString(), variantIn) {}

ModelResourceLocation::ModelResourceLocation(const std::string& location, const std::string& variantIn) :
    ModelResourceLocation(parsePathString(location + '#' + (variantIn.empty() ? "normal" : variantIn))) {}

const std::string& ModelResourceLocation::getVariant() const { return m_variant; }

std::string ModelResourceLocation::toString() const {
    const std::string base = ResourceLocation::toString();
    std::string result;
    result.reserve(base.size() + 1 + m_variant.size());
    result  = base;
    result += '#';
    result += m_variant;
    return result; // NRVO
}

bool ModelResourceLocation::operator==(const ModelResourceLocation& other) const {
    return ResourceLocation::operator==(other) && m_variant == other.m_variant;
}

bool ModelResourceLocation::operator!=(const ModelResourceLocation& other) const { return !(*this == other); }

std::size_t ModelResourceLocation::Hash::operator()(const ModelResourceLocation& mrl) const {
    std::size_t base = ResourceLocation::Hash{}(mrl);
    std::size_t vh = std::hash<std::string>{}(mrl.m_variant);
    return base * 31 + vh;
}

void ModelResourceLocation::to_json(nlohmann::json& j, const ModelResourceLocation& mrl) { j = mrl.toString(); }

void ModelResourceLocation::from_json(const nlohmann::json& j, ModelResourceLocation& mrl) {
    mrl = ModelResourceLocation(j.get<std::string>());
}
