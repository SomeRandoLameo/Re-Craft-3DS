#include "world/ChunkWorker.h"
#include "misc/Crash.h"
#include <stdio.h>

ChunkWorker::ChunkWorker() {
	WorkQueue_Init(&m_queue);

	if (R_FAILED(APT_SetAppCpuTimeLimit(30))) {
		Crash("Couldn't set AppCpuTimeLimit");
	}

	s32 prio;
	bool isNew3ds = false;
	APT_CheckNew3DS(&isNew3ds);
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

	m_thread = threadCreate(&ChunkWorker::MainloopWrapper, (void*)this,
						  CHUNKWORKER_THREAD_STACKSIZE, prio - 1,
						  isNew3ds ? 2 : 1, false);

	if (!m_thread) {
		Crash("Couldn't create worker thread");
	}
}

ChunkWorker::~ChunkWorker() {
	m_shouldStop = true;
	LightEvent_Signal(&m_queue.itemAddedEvent);
	threadJoin(m_thread, UINT64_MAX);
	threadFree(m_thread);

	WorkQueue_Deinit(&m_queue);

}

void ChunkWorker::Finish() {
	LightEvent_Signal(&m_queue.itemAddedEvent);
	while (m_working || m_queue.queue.size() > 0) {
		svcSleepThread(1000000);
	}
}

void ChunkWorker::AddHandler(WorkerItemType type, WorkerFuncObj obj) {
	m_handler[type].push_back(obj);
}

void ChunkWorker::SetHandlerActive(WorkerItemType type, void* context, bool active) {
	for (auto& h : m_handler[type]) {
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

	while (!m_shouldStop || m_queue.queue.size() > 0) {
		m_working = false;

		LightEvent_Wait(&m_queue.itemAddedEvent);
		LightEvent_Clear(&m_queue.itemAddedEvent);

		m_working = true;

		// Move queue items into private queue
		LightLock_Lock(&m_queue.listInUse);

		privateQueue.insert(
				privateQueue.end(),
				m_queue.queue.begin(),
				m_queue.queue.end()
		);
		m_queue.queue.clear();

		LightLock_Unlock(&m_queue.listInUse);

		// Process private queue
		while (!privateQueue.empty()) {
			WorkerItem item = privateQueue.back();
			privateQueue.pop_back();

			if (item.uuid == item.column->uuid) {

				for (auto& h : m_handler[item.type]) {
					if (h.active) {
						h.func(&m_queue, item, h.context);
					}
					svcSleepThread(7000);
				}

				switch (item.type) {
					case WorkerItemType_BaseGen:
						item.column->genProgress = ChunkGen_Terrain;
						break;
					case WorkerItemType_Decorate:
						item.column->genProgress = ChunkGen_Finished;
						break;
					default:
						break;
				}

				--item.column->tasksRunning;
				if (item.type == WorkerItemType_PolyGen) {
					--item.column->graphicalTasksRunning;
				}

				svcSleepThread(1000);
			}
		}
	}
}
