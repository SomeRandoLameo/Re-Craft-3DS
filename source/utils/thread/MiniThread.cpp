#include "MiniThread.hpp"

MiniThread::MiniThread() : running(true), threadHandle(0) {}

MiniThread::~MiniThread() {
    stop();
    if (threadHandle) {
        threadJoin(threadHandle, U64_MAX);
        threadFree(threadHandle);
        threadHandle = 0;
    }
}

void MiniThread::start() {
    if (threadHandle != 0) {
        return; 
    }
    
    s32 prio;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

    threadHandle = threadCreate(threadEntry, this, STACK_SIZE, prio + 1, -2, false);
    
    if (threadHandle == 0) {
        threadHandle = threadCreate(threadEntry, this, STACK_SIZE, prio, -1, false);
    }
}

void MiniThread::join() {
    if (threadHandle) {
        threadJoin(threadHandle, U64_MAX);
    }
}

void MiniThread::stop() {
    running = false;
}

void MiniThread::threadEntry(void* arg) {
    MiniThread* t = static_cast<MiniThread*>(arg);
    if (t && t->running) {
        try {
            t->run();
        } catch (...) {
            // Handle exceptions if necessary (will add later)
        }
    }
}