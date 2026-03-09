#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "ModelBlock.hpp"
#include "util/ResourceLocation.hpp"


class ModelPool {
public:
    static constexpr int32_t INVALID_INDEX = -1;

    ModelPool() { m_models.reserve(512); }

    int32_t internOrGet(const std::string& key, ModelBlock model);

    int32_t find(const std::string& key) const;

    bool contains(const std::string& key) const;

    void freeze();

    bool isFrozen() const { return m_frozen; }

    const ModelBlock& get(int32_t index) const;
    ModelBlock& get(int32_t index);

    const ModelBlock* findModel(const std::string& key) const;

    std::size_t size() const { return m_models.size(); }


    const std::vector<ModelBlock>& all() const { return m_models; }

private:
    std::vector<ModelBlock> m_models;
    std::unordered_map<std::string, int32_t> m_index;

    bool m_frozen = false;

    void checkCycles() const;
};
