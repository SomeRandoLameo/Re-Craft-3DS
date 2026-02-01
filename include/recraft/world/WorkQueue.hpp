#pragma once

#include "CT_Chunk.hpp"

#include <vector>
#include <3ds.h>

enum WorkerItemType {
	Load,
	Save,
	BaseGen,
	Decorate,
	PolyGen,
	Count
};

struct WorkerItem {
	WorkerItemType type;
	ChunkColumnPtr column;
	u32 uuid;
};

struct WorkQueue {
    std::vector<WorkerItem> queue;

	LightEvent itemAddedEvent;
	LightLock listInUse;
};

void WorkQueue_Init(WorkQueue* queue);

void WorkQueue_Deinit(WorkQueue* queue);

void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item);
