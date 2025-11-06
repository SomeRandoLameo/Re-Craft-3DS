#ifndef TERRACOTTA_PRIORITY_QUEUE_H_
#define TERRACOTTA_PRIORITY_QUEUE_H_

#include <algorithm>
#include <vector>

namespace terra {

template <typename T, typename Comparator, typename Container = std::vector<T>>
class PriorityQueue {
private:
    Container m_Container;
    Comparator m_Comp;

public:
    PriorityQueue() = default;
    PriorityQueue(const Comparator& cmp) : m_Comp(cmp) { }

    void Push(T item) {
        m_Container.push_back(item);
        std::push_heap(m_Container.begin(), m_Container.end(), m_Comp);
    }

    T Pop() {
        T item = m_Container.front();
        std::pop_heap(m_Container.begin(), m_Container.end(), m_Comp);
        m_Container.pop_back();
        return item;
    }

    void Update() {
        std::make_heap(m_Container.begin(), m_Container.end(), m_Comp);
    }

    bool Empty() const { return m_Container.empty(); }
    Container& GetData() { return m_Container; }
};

} // ns terra

#endif
