#include "client/renderer/block/VariantList.hpp"

#include <stdexcept>

VariantList::VariantList(std::vector<Variant> variants) : m_variantList(std::move(variants)) {}

const std::vector<Variant>& VariantList::getVariantList() const { return m_variantList; }

bool VariantList::operator==(const VariantList& other) const { return m_variantList == other.m_variantList; }

bool VariantList::operator!=(const VariantList& other) const { return !(*this == other); }

std::size_t VariantList::hashCode() const {
    std::size_t h = 1;
    for (const auto& v : m_variantList)
        h = 31 * h + v.hashCode();
    return h;
}

void from_json(const nlohmann::json& j, VariantList& vl) {
    std::vector<Variant> variants;

    auto parseOne = [](const nlohmann::json& elem) {
        Variant v = Variant::fromJson(elem);
        return v;
    };

    if (j.is_array()) {
        if (j.empty())
            throw nlohmann::json::parse_error::create(101, 0, "Empty variant array", nullptr);
        for (const auto& elem : j)
            variants.push_back(parseOne(elem));
    } else {
        variants.push_back(parseOne(j));
    }

    vl = VariantList(std::move(variants));
}

void to_json(nlohmann::json& j, const VariantList& vl) {
    j = nlohmann::json::array();
    for (const auto& v : vl.getVariantList())
        j.push_back(v);
}
