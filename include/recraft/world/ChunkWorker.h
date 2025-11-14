#pragma once

#include <3ds.h>

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

	inline bool IsWorking() const { return working; }
	inline WorkQueue& GetQueue() { return queue; }

private:
	static void MainloopWrapper(void* context);
	void Mainloop();

	Thread thread;
	WorkQueue queue;
	vec_t(WorkerFuncObj) handler[WorkerItemTypes_Count];
	volatile bool working;
	volatile bool shouldStop;
};