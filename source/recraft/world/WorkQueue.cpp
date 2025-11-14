#include "world/WorkQueue.h"

void WorkQueue_Init(WorkQueue* queue) {
    vec_init(&queue->queue);
    LightLock_Init(&queue->listInUse);
    LightEvent_Init(&queue->itemAddedEvent, RESET_STICKY);
}
void WorkQueue_Deinit(WorkQueue* queue) { vec_deinit(&queue->queue); }

void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item) {
    item.uuid = item.chunk->uuid;
    ++item.chunk->tasksRunning;
    if (item.type == WorkerItemType_PolyGen) ++item.chunk->graphicalTasksRunning;
    LightLock_Lock(&queue->listInUse);
    vec_push(&queue->queue, item);
    LightLock_Unlock(&queue->listInUse);

    LightEvent_Signal(&queue->itemAddedEvent);
}
