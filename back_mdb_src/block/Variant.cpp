#include "client/renderer/block/Variant.hpp"

#include <sstream>
#include <stdexcept>

Variant::Variant(ResourceLocation modelLocation, const ModelRotation& rotation, bool uvLock, int weight) :
    m_modelLocation(std::move(modelLocation)), m_rotation(rotation), m_uvLock(uvLock), m_weight(weight) {}

const ResourceLocation& Variant::getModelLocation() const { return m_modelLocation; }
const ModelRotation&    Variant::getRotation()       const { return m_rotation; }
bool                    Variant::isUvLock()           const { return m_uvLock; }
int                     Variant::getWeight()          const { return m_weight; }

bool Variant::operator==(const Variant& other) const {
    return m_modelLocation == other.m_modelLocation && m_rotation == other.m_rotation &&
           m_uvLock == other.m_uvLock && m_weight == other.m_weight;
}

bool Variant::operator!=(const Variant& other) const { return !(*this == other); }

std::size_t Variant::hashCode() const {
    std::size_t h = ResourceLocation::Hash{}(m_modelLocation);
    h = 31 * h + m_rotation.hashCode();
    h = 31 * h + std::hash<bool>{}(m_uvLock);
    h = 31 * h + static_cast<std::size_t>(m_weight);
    return h;
}

std::string Variant::toString() const {
    std::ostringstream oss;
    oss << "Variant{modelLocation=" << m_modelLocation.toString()
        << ", rotation="            << m_rotation.toString()
        << ", uvLock="              << (m_uvLock ? "true" : "false")
        << ", weight="              << m_weight << '}';
    return oss.str();
}

static ResourceLocation resolveBlockLocation(const std::string& model) {
    ResourceLocation base(model);
    std::string blockPath;
    blockPath.reserve(6 + base.getResourcePath().size()); // "block/" = 6 chars
    blockPath  = "block/";
    blockPath += base.getResourcePath();
    return ResourceLocation(base.getResourceDomain(), std::move(blockPath));
}

static ModelRotation parseModelRotation(const nlohmann::json& j) {
    int x = j.value("x", 0);
    int y = j.value("y", 0);

    auto rotation = ModelRotation::getModelRotation(x, y);
    if (!rotation)
        throw nlohmann::json::parse_error::create(
            101, 0, "Invalid BlockModelRotation x: " + std::to_string(x) + ", y: " + std::to_string(y), nullptr);

    return *rotation;
}

static int parseWeight(const nlohmann::json& j) {
    int weight = j.value("weight", 1);
    if (weight < 1)
        throw nlohmann::json::parse_error::create(
            101, 0, "Invalid weight " + std::to_string(weight) + " found, expected integer >= 1", nullptr);
    return weight;
}

static Variant parseVariant(const nlohmann::json& j) {
    return Variant(
        resolveBlockLocation(j.at("model").get<std::string>()),
        parseModelRotation(j),
        j.value("uvlock", false),
        parseWeight(j));
}

Variant Variant::fromJson(const nlohmann::json& j) { return parseVariant(j); }

void from_json(const nlohmann::json& j, Variant& v) { v = parseVariant(j); }

void to_json(nlohmann::json& j, const Variant& v) {
    j = nlohmann::json{{"model",  v.getModelLocation().toString()},
                       {"x",      v.getRotation().getX()},
                       {"y",      v.getRotation().getY()},
                       {"uvlock", v.isUvLock()},
                       {"weight", v.getWeight()}};
}
