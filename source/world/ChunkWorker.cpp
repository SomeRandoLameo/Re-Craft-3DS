#include "ChunkWorker.h"
#include "../misc/Crash.h"
#include <stdio.h>

ChunkWorker::ChunkWorker() : thread(nullptr), working(false), shouldStop(false) {
	WorkQueue_Init(&queue);

	if (R_FAILED(APT_SetAppCpuTimeLimit(30))) {
		Crash("Couldn't set AppCpuTimeLimit");
	}

	for (int i = 0; i < WorkerItemTypes_Count; i++) {
		vec_init(&handler[i]);
	}

	s32 prio;
	bool isNew3ds = false;
	APT_CheckNew3DS(&isNew3ds);
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

	thread = threadCreate(&ChunkWorker::MainloopWrapper, (void*)this,
						  CHUNKWORKER_THREAD_STACKSIZE, prio - 1,
						  isNew3ds ? 2 : 1, false);

	if (!thread) {
		Crash("Couldn't create worker thread");
	}
}

ChunkWorker::~ChunkWorker() {
	shouldStop = true;
	LightEvent_Signal(&queue.itemAddedEvent);
	threadJoin(thread, UINT64_MAX);
	threadFree(thread);

	WorkQueue_Deinit(&queue);

	for (int i = 0; i < WorkerItemTypes_Count; i++) {
		vec_deinit(&handler[i]);
	}
}

void ChunkWorker::Finish() {
	LightEvent_Signal(&queue.itemAddedEvent);
	while (working || queue.queue.length > 0) {
		svcSleepThread(1000000);
	}
}

void ChunkWorker::AddHandler(WorkerItemType type, WorkerFuncObj obj) {
	vec_push(&handler[type], obj);
}

void ChunkWorker::SetHandlerActive(WorkerItemType type, void* context, bool active) {
	for (size_t i = 0; i < handler[type].length; i++) {
		if (handler[type].data[i].context == context) {
			handler[type].data[i].active = active;
			return;
		}
	}
}

void ChunkWorker::MainloopWrapper(void* context) {
	ChunkWorker* worker = static_cast<ChunkWorker*>(context);
	worker->Mainloop();
}

void ChunkWorker::Mainloop() {
	vec_t(WorkerItem) privateQueue;
	vec_init(&privateQueue);

	while (!shouldStop || queue.queue.length > 0) {
		working = false;

		LightEvent_Wait(&queue.itemAddedEvent);
		LightEvent_Clear(&queue.itemAddedEvent);

		working = true;

		LightLock_Lock(&queue.listInUse);
		vec_pusharr(&privateQueue, queue.queue.data, queue.queue.length);
		vec_clear(&queue.queue);
		LightLock_Unlock(&queue.listInUse);

		while (privateQueue.length > 0) {
			WorkerItem item = vec_pop(&privateQueue);

			if (item.uuid == item.chunk->uuid) {
				for (int i = 0; i < handler[item.type].length; i++) {
					if (handler[item.type].data[i].active) {
						handler[item.type].data[i].func(&queue, item,
														handler[item.type].data[i].context);
					}
					svcSleepThread(7000);
				}

				switch (item.type) {
					case WorkerItemType_BaseGen:
						item.chunk->genProgress = ChunkGen_Terrain;
						break;
					case WorkerItemType_Decorate:
						item.chunk->genProgress = ChunkGen_Finished;
						break;
					default:
						break;
				}

				--item.chunk->tasksRunning;
				if (item.type == WorkerItemType_PolyGen) {
					--item.chunk->graphicalTasksRunning;
				}

				svcSleepThread(1000);
			}
		}
	}

	vec_deinit(&privateQueue);
}