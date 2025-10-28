#pragma once

extern "C" {
    #include <vec/vec.h>
}
#include "CT_Chunk.h"

#include "../misc/Xorshift.h"

#include <stdbool.h>

#include <3ds.h>

typedef enum {
	WorkerItemType_Load,
	WorkerItemType_Save,
	WorkerItemType_BaseGen,
	WorkerItemType_Decorate,
	WorkerItemType_PolyGen,
	WorkerItemTypes_Count
} WorkerItemType;

typedef struct {
	WorkerItemType type;
	Chunk* chunk;
	uint32_t uuid;
} WorkerItem;

typedef struct {
	vec_t(WorkerItem) queue;

	LightEvent itemAddedEvent;
	LightLock listInUse;
} WorkQueue;

void WorkQueue_Init(WorkQueue* queue);

void WorkQueue_Deinit(WorkQueue* queue);

void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item);