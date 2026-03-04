#pragma once

#include <string>
#include "util/ResourceLocation.hpp"

class ModelResourceLocation : public ResourceLocation {
public:
    struct ParsedPath {
        std::string domain;
        std::string path;
        std::string variant;
    };

    /**
     * Constructs from a full path string, e.g. "minecraft:stone#normal" or "stone#inventory"
     */
    explicit ModelResourceLocation(const std::string& pathIn);

    /**
     * Constructs from an existing ResourceLocation and a variant string.
     */
    ModelResourceLocation(const ResourceLocation& location, const std::string& variantIn);

    /**
     * Constructs from a location string and a separate variant string.
     * e.g. location="minecraft:stone", variantIn="inventory"
     */
    ModelResourceLocation(const std::string& location, const std::string& variantIn);

    const std::string& getVariant() const;

    bool operator==(const ModelResourceLocation& other) const;
    bool operator!=(const ModelResourceLocation& other) const;

    std::string toString() const;

    struct Hash {
        std::size_t operator()(const ModelResourceLocation& mrl) const;
    };

    /**
     * Parses a path string (potentially containing '#') into a {domain, path, variant} triple.
     * e.g. "minecraft:stone#inventory" -> {"minecraft", "stone", "inventory"}
     */
    static ParsedPath parsePathString(const std::string& pathIn);

private:
    explicit ModelResourceLocation(const ParsedPath& parsed);

    std::string m_variant;

    void to_json(nlohmann::json& j, const ModelResourceLocation& mrl);
    void from_json(const nlohmann::json& j, ModelResourceLocation& mrl);
};
