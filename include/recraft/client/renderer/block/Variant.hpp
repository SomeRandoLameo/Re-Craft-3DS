#pragma once

#include "ModelRotation.hpp"
#include "util/ResourceLocation.hpp"


class Variant {
public:
    Variant(ResourceLocation modelLocation, const ModelRotation& rotation, bool uvLock, int weight);

    const ResourceLocation& getModelLocation() const;
    const ModelRotation& getRotation() const;
    bool isUvLock() const;
    int getWeight() const;

    static Variant fromJson(const nlohmann::json& j);

    bool operator==(const Variant& other) const;
    bool operator!=(const Variant& other) const;

    std::size_t hashCode() const;
    std::string toString() const;

private:
    ResourceLocation m_modelLocation;
    ModelRotation m_rotation;
    bool m_uvLock;
    int m_weight;
};

void to_json(nlohmann::json& j, const Variant& v);
void from_json(const nlohmann::json& j, Variant& v);
