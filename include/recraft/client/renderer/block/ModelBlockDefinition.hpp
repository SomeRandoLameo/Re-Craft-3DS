#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include "VariantList.hpp"
#include "internal/json.hpp"

class ModelBlockDefinition {
public:
    ModelBlockDefinition() = default;

    static ModelBlockDefinition parseFromJson(const nlohmann::json& j) {
        ModelBlockDefinition def;

        if (!j.contains("variants"))
            throw std::runtime_error("No 'variants' found in blockstate JSON");

        for (auto& [key, value] : j["variants"].items()) {
            VariantList vl = value.get<VariantList>();
            def.m_variants.emplace(key, std::move(vl));
        }

        return def;
    }

    void addVariant(const std::string& key, VariantList variantList) { m_variants[key] = std::move(variantList); }

    bool hasVariant(const std::string& key) const { return m_variants.count(key) > 0; }

    const VariantList& getVariant(const std::string& key) const { return m_variants.at(key); }

    const std::unordered_map<std::string, VariantList>& getVariants() const { return m_variants; }

private:
    std::unordered_map<std::string, VariantList> m_variants;
};
