#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <mutex>
#include <stack>

namespace Luminary {

class MemoryPool {
public:
    explicit MemoryPool(size_t blockSize, size_t blockCount = 100);
    ~MemoryPool();

    void* Allocate();
    void Deallocate(void* ptr);

    size_t GetTotalMemory() const { return m_TotalSize; }
    size_t GetAllocatedMemory() const { return m_AllocatedSize; }
    size_t GetFreeBlocks() const { return m_FreeBlocks.size(); }

    void Reset();

private:
    size_t m_BlockSize;
    size_t m_TotalSize;
    size_t m_AllocatedSize = 0;
    std::vector<std::unique_ptr<uint8_t[]>> m_Memory;
    std::stack<void*> m_FreeBlocks;
    mutable std::mutex m_Mutex;
};

// Object pool template for specific types
template<typename T>
class ObjectPool {
public:
    ObjectPool(size_t capacity = 100) : m_Capacity(capacity), m_Pool(sizeof(T), capacity) {}

    template<typename... Args>
    T* Create(Args&&... args) {
        void* memory = m_Pool.Allocate();
        if (!memory) return nullptr;
        return new(memory) T(std::forward<Args>(args)...);
    }

    void Destroy(T* object) {
        if (!object) return;
        object->~T();
        m_Pool.Deallocate(object);
    }

    size_t GetCapacity() const { return m_Capacity; }

private:
    size_t m_Capacity;
    MemoryPool m_Pool;
};

} // namespace Luminary
