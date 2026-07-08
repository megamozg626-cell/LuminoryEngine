#pragma once

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <future>

namespace Luminary {

using TaskFunction = std::function<void()>;

class TaskSystem {
public:
    explicit TaskSystem(uint32_t threadCount = 0);
    ~TaskSystem();

    void Initialize();
    void Shutdown();

    void SubmitTask(TaskFunction task);
    
    template<typename Func>
    auto SubmitTaskWithResult(Func&& func) -> std::future<decltype(func())> {
        auto task = std::make_shared<std::packaged_task<decltype(func())()>>(
            std::forward<Func>(func)
        );
        auto future = task->get_future();
        
        SubmitTask([task]() { (*task)(); });
        return future;
    }

    uint32_t GetThreadCount() const { return m_ThreadCount; }
    size_t GetPendingTasks() const;

private:
    uint32_t m_ThreadCount;
    std::vector<std::thread> m_Threads;
    std::queue<TaskFunction> m_TaskQueue;
    std::mutex m_QueueMutex;
    std::condition_variable m_QueueCV;
    std::atomic<bool> m_Running = false;

    void WorkerThread();
};

} // namespace Luminary
