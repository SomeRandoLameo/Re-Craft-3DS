#pragma once

#include <3ds.h>
#include <vector>
#include "WorkQueue.h"

#define CHUNKWORKER_THREAD_STACKSIZE (16 * 1024)

typedef void (*WorkerFunc)(WorkQueue* queue, WorkerItem item, void* context);

struct WorkerFuncObj{
	WorkerFunc func;
	void* context;
	bool active;
};

class ChunkWorker {
public:
	ChunkWorker();
	~ChunkWorker();

	// Delete copy constructor and assignment operator
	//ChunkWorker(const ChunkWorker&) = delete;
	//ChunkWorker& operator=(const ChunkWorker&) = delete;

	void Finish();
	void AddHandler(WorkerItemType type, WorkerFuncObj obj);
	void SetHandlerActive(WorkerItemType type, void* context, bool active);

	inline bool IsWorking() const { return m_working; }
	inline WorkQueue& GetQueue() { return m_queue; }

private:
	static void MainloopWrapper(void* context);
	void Mainloop();

	Thread m_thread = nullptr;
	WorkQueue m_queue;
    std::vector<WorkerFuncObj> m_handler[WorkerItemType::Count];
	volatile bool m_working = false;
	volatile bool m_shouldStop = false;
};