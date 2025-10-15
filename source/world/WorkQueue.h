#pragma once

#include <vec/vec.h>

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

static inline void WorkQueue_Init(WorkQueue* queue) {
	vec_init(&queue->queue);
	LightLock_Init(&queue->listInUse);
	LightEvent_Init(&queue->itemAddedEvent, RESET_STICKY);
}
static inline void WorkQueue_Deinit(WorkQueue* queue) { vec_deinit(&queue->queue); }

static inline void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item) {
	item.uuid = item.chunk->uuid;
	++item.chunk->tasksRunning;
	if (item.type == WorkerItemType_PolyGen) ++item.chunk->graphicalTasksRunning;
	LightLock_Lock(&queue->listInUse);
	vec_push(&queue->queue, item);
	LightLock_Unlock(&queue->listInUse);

	LightEvent_Signal(&queue->itemAddedEvent);
}
