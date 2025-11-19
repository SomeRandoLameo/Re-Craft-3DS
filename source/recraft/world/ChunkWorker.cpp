#include "world/ChunkWorker.h"
#include "misc/Crash.h"
#include <stdio.h>

ChunkWorker::ChunkWorker() : thread(nullptr), working(false), shouldStop(false) {
	WorkQueue_Init(&queue);

	if (R_FAILED(APT_SetAppCpuTimeLimit(30))) {
		Crash("Couldn't set AppCpuTimeLimit");
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

}

void ChunkWorker::Finish() {
	LightEvent_Signal(&queue.itemAddedEvent);
	while (working || queue.queue.size() > 0) {
		svcSleepThread(1000000);
	}
}

void ChunkWorker::AddHandler(WorkerItemType type, WorkerFuncObj obj) {
	handler[type].push_back(obj);
}

void ChunkWorker::SetHandlerActive(WorkerItemType type, void* context, bool active) {
	for (auto& h : handler[type]) {
		if (h.context == context) {
			h.active = active;
			return;
		}
	}
}

void ChunkWorker::MainloopWrapper(void* context) {
	ChunkWorker* worker = static_cast<ChunkWorker*>(context);
	worker->Mainloop();
}

void ChunkWorker::Mainloop() {
	std::vector<WorkerItem> privateQueue;
	privateQueue.clear();

	while (!shouldStop || queue.queue.size() > 0) {
		working = false;

		LightEvent_Wait(&queue.itemAddedEvent);
		LightEvent_Clear(&queue.itemAddedEvent);

		working = true;

		// Move queue items into private queue
		LightLock_Lock(&queue.listInUse);

		privateQueue.insert(
				privateQueue.end(),
				queue.queue.begin(),
				queue.queue.end()
		);
		queue.queue.clear();

		LightLock_Unlock(&queue.listInUse);

		// Process private queue
		while (!privateQueue.empty()) {
			WorkerItem item = privateQueue.back();
			privateQueue.pop_back();

			if (item.uuid == item.chunk->uuid) {

				for (auto& h : handler[item.type]) {
					if (h.active) {
						h.func(&queue, item, h.context);
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
}
