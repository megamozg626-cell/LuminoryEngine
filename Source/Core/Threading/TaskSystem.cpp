#include "TaskSystem.hpp"
#include "../Logging/Logger.hpp"
#include <thread>

namespace Luminary {

TaskSystem::TaskSystem(uint32_t threadCount)
    : m_ThreadCount(threadCount > 0 ? threadCount : std::thread::hardware_concurrency()) {
}

TaskSystem::~TaskSystem() {
    Shutdown();
}

void TaskSystem::Initialize() {
    if (m_Running) return;

    m_Running = true;
    m_Threads.reserve(m_ThreadCount);

    for (uint32_t i = 0; i < m_ThreadCount; ++i) {
        m_Threads.emplace_back(&TaskSystem::WorkerThread, this);
    }

    LOG_INFO("TaskSystem initialized with {} threads", m_ThreadCount);
}

void TaskSystem::Shutdown() {
    if (!m_Running) return;

    m_Running = false;
    m_QueueCV.notify_all();

    for (auto& thread : m_Threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    m_Threads.clear();
    LOG_INFO("TaskSystem shut down");
}

void TaskSystem::SubmitTask(TaskFunction task) {
    {
        std::lock_guard<std::mutex> lock(m_QueueMutex);
        m_TaskQueue.push(task);
    }
    m_QueueCV.notify_one();
}

size_t TaskSystem::GetPendingTasks() const {
    std::lock_guard<std::mutex> lock(m_QueueMutex);
    return m_TaskQueue.size();
}

void TaskSystem::WorkerThread() {
    while (m_Running) {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_QueueCV.wait(lock, [this] { return !m_TaskQueue.empty() || !m_Running; });

        if (!m_Running && m_TaskQueue.empty()) {
            break;
        }

        if (!m_TaskQueue.empty()) {
            TaskFunction task = std::move(m_TaskQueue.front());
            m_TaskQueue.pop();
            lock.unlock();

            try {
                task();
            } catch (const std::exception& e) {
                LOG_ERROR("Task execution failed: {}", e.what());
            }
        }
    }
}

} // namespace Luminary
