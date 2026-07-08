#include "MemoryPool.hpp"
#include "../Logging/Logger.hpp"

namespace Luminary {

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount)
    : m_BlockSize(blockSize), m_TotalSize(blockSize * blockCount) {
    
    for (size_t i = 0; i < blockCount; ++i) {
        auto block = std::make_unique<uint8_t[]>(blockSize);
        m_FreeBlocks.push(block.get());
        m_Memory.push_back(std::move(block));
    }
    
    LOG_DEBUG("MemoryPool created with {} blocks of {} bytes", blockCount, blockSize);
}

MemoryPool::~MemoryPool() {
    Reset();
}

void* MemoryPool::Allocate() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    if (m_FreeBlocks.empty()) {
        LOG_WARNING("MemoryPool exhausted, allocating new block");
        auto block = std::make_unique<uint8_t[]>(m_BlockSize);
        void* ptr = block.get();
        m_Memory.push_back(std::move(block));
        m_TotalSize += m_BlockSize;
        m_AllocatedSize += m_BlockSize;
        return ptr;
    }
    
    void* ptr = m_FreeBlocks.top();
    m_FreeBlocks.pop();
    m_AllocatedSize += m_BlockSize;
    
    return ptr;
}

void MemoryPool::Deallocate(void* ptr) {
    if (!ptr) return;
    
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_FreeBlocks.push(ptr);
    m_AllocatedSize -= m_BlockSize;
}

void MemoryPool::Reset() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Memory.clear();
    while (!m_FreeBlocks.empty()) {
        m_FreeBlocks.pop();
    }
    m_AllocatedSize = 0;
}

} // namespace Luminary
