#pragma once

#include "Variant.hpp"
#include "internal/json.hpp"

#include <vector>

class VariantList {
public:
    VariantList() = default;
    explicit VariantList(std::vector<Variant> variants);

    const std::vector<Variant>& getVariantList() const;

    bool operator==(const VariantList& other) const;
    bool operator!=(const VariantList& other) const;
    std::size_t hashCode() const;

private:
    std::vector<Variant> m_variantList;
};

void to_json(nlohmann::json& j, const VariantList& vl);
void from_json(const nlohmann::json& j, VariantList& vl);
