#include "world/worldgen/EmptyGen.hpp"

void EmptyGen::Init(World* world) {
    m_world = world;
}

void EmptyGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {}