#pragma once

#include "../CT_World.h"

class SmeaGen {
public:
    void Init(World* world);
    static void Generate(WorkQueue* queue, WorkerItem item, void* context);
private:
    World* world;
};


