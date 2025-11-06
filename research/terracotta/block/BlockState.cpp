#include "BlockState.h"

#include <mclib/common/Json.h>
#include <algorithm>

namespace terra {
namespace block {

static const std::string kNullProperty = "";

const std::string& BlockState::GetProperty(const std::string& property) const {
    auto iter = m_Properties.find(property);

    if (iter == m_Properties.end()) {
        return kNullProperty;
    }

    return iter->second;
}

std::unique_ptr<BlockState> BlockState::FromJSON(const mc::json& json) {
    u32 id = json.value("id", 0);
    std::unique_ptr<BlockState> state = std::make_unique<BlockState>(id);

    mc::json properties_node = json.value("properties", mc::json());

    std::vector<std::string> property_keys;

    if (properties_node.is_object()) {
        for (auto& kv : properties_node.items()) {
            std::string property = kv.key();
            std::string value = kv.value().get<std::string>();

            state->m_Properties[property] = value;
            property_keys.push_back(property);
        }
    }

    std::sort(property_keys.begin(), property_keys.end());

    for (std::size_t i = 0; i < property_keys.size(); ++i) {
        auto& property_key = property_keys[i];

        if (i != 0) {
            state->m_Variant += ",";
        }

        state->m_Variant += property_key + "=" + state->m_Properties[property_key];
    }

    return state;
}

} // ns block
} // ns terra
