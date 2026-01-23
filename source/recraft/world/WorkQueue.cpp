#include "world/WorkQueue.hpp"

void WorkQueue_Init(WorkQueue* queue) {
    queue->queue.clear();
    LightLock_Init(&queue->listInUse);
    LightEvent_Init(&queue->itemAddedEvent, RESET_STICKY);
}

void WorkQueue_Deinit(WorkQueue* queue) {
    queue->queue.clear();
}

void WorkQueue_AddItem(WorkQueue* queue, WorkerItem item) {
    item.uuid = item.column->uuid;
    ++item.column->tasksRunning;
    if (item.type == WorkerItemType::PolyGen) ++item.column->graphicalTasksRunning;
    LightLock_Lock(&queue->listInUse);
    queue->queue.push_back(item);
    LightLock_Unlock(&queue->listInUse);

    LightEvent_Signal(&queue->itemAddedEvent);
}
