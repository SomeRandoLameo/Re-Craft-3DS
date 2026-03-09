#include "client/renderer/block/ModelPool.hpp"
#include "misc/Crash.hpp"

#include <stdexcept>

int32_t ModelPool::internOrGet(const std::string& key, ModelBlock model) {
    if (m_frozen)
        Crash("ModelPool::internOrGet called after freeze() — key: {}", key);

    auto it = m_index.find(key);
    if (it != m_index.end())
        return it->second;

    int32_t index = static_cast<int32_t>(m_models.size());
    model.m_selfIndex = index;
    model.m_parentIndex = INVALID_INDEX;
    model.name = key;

    m_models.push_back(std::move(model));
    m_index.emplace(key, index);
    return index;
}

int32_t ModelPool::find(const std::string& key) const {
    auto it = m_index.find(key);
    return it != m_index.end() ? it->second : INVALID_INDEX;
}

bool ModelPool::contains(const std::string& key) const { return m_index.count(key) > 0; }

void ModelPool::freeze() {
    if (m_frozen)
        return;

    for (ModelBlock& model : m_models) {
        const auto& parentLoc = model.getParentLocation();
        if (!parentLoc.has_value())
            continue;

        const std::string key = parentLoc->toString();
        int32_t idx = find(key);

        if (idx == INVALID_INDEX) {
            // Fallback: inject minecraft namespace
            ResourceLocation withNS("minecraft", parentLoc->getResourcePath());
            idx = find(withNS.toString());
        }

        if (idx == INVALID_INDEX)
            Crash("ModelPool: could not resolve parent '{}' for model '{}'", key, model.name);

        model.resolveParentIndex(idx);
    }

    m_frozen = true;

    checkCycles();
}

void ModelPool::checkCycles() const {
    for (const ModelBlock& model : m_models) {
        if (model.m_parentIndex == INVALID_INDEX)
            continue;

        int32_t slow = model.m_parentIndex;
        int32_t fast = model.m_parentIndex;

        while (true) {
            if (fast == INVALID_INDEX)
                break;
            fast = m_models[fast].m_parentIndex;

            if (fast == INVALID_INDEX)
                break;
            fast = m_models[fast].m_parentIndex;

            slow = m_models[slow].m_parentIndex;

            if (slow == fast)
                throw ModelBlock::LoopException();
        }
    }
}

const ModelBlock& ModelPool::get(int32_t index) const { return m_models.at(static_cast<std::size_t>(index)); }

ModelBlock& ModelPool::get(int32_t index) { return m_models.at(static_cast<std::size_t>(index)); }

const ModelBlock* ModelPool::findModel(const std::string& key) const {
    int32_t idx = find(key);
    return idx != INVALID_INDEX ? &m_models[idx] : nullptr;
}
