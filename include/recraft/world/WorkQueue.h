#pragma once

#include "CT_Chunk.h"
#include "../misc/Xorshift.h"

#include <stdbool.h>
#include <vector>
#include <3ds.h>

typedef enum {
	Load,
	Save,
	BaseGen,
	Decorate,
	PolyGen,
	Count
} WorkerItemType;

typedef struct {
	WorkerItemType type;
	ChunkColumnPtr column;
	uint32_t uuid;
} WorkerItem;

typedef struct {
    std::vector<WorkerItem> queue;

	LightEvent itemAddedEvent;
	LightLock listInUse;
} WorkQueue;

void WorkQueue_Init(WorkQueue* queue);

void WorkQueue_Deinit(WorkQueue* queue);

void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item);