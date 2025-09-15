#pragma once
#include <3ds.h>
#include <functional>
#include <atomic>

class MiniThread {
public:
    MiniThread();
    virtual ~MiniThread();

    void start();

    void join();

    void stop();

    virtual void run() = 0;

protected:
    std::atomic<bool> running;

private:
    static void threadEntry(void* arg);
    Thread threadHandle;
    static const size_t STACK_SIZE = 4 * 1024;
};
