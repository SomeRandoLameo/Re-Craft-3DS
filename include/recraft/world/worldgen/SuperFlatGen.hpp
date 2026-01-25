#pragma once

#include "world/WorkQueue.hpp"

class World;
class SuperFlatGen {
public:
    void        Init(World* world);
    static void Generate(WorkQueue* queue, WorkerItem item, void* context);

private:
    World* m_world;
};
