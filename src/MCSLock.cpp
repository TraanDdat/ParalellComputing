#include "MCSLock.h"
void MCSLock::lock()
{
    // auto backoff = std::chrono::microseconds(1);
    // constexpr auto MAX_BACKOFF = std::chrono::microseconds(8);
    MCSNode* node = &getThreadNode(this);
    node->next.store(nullptr, std::memory_order_relaxed);
    node->waiting.store(true, std::memory_order_relaxed);
    // Append this thread to the queue.
    MCSNode* predecessor = tail.exchange(node, std::memory_order_acq_rel);
    if (predecessor != nullptr)
    {
        predecessor->next.store(node, std::memory_order_release);
        while (node->waiting.load(std::memory_order_acquire))
        {
            // std::this_thread::sleep_for(backoff);
            // backoff = std::min(backoff * 2, MAX_BACKOFF); 
        }
    }
}
void MCSLock::unlock()
{
    MCSNode* node = &getThreadNode(this);
    MCSNode* successor = node->next.load(std::memory_order_acquire);
    if (successor == nullptr)
    {
        MCSNode* expected = node;
        // Try to make the queue empty.
        if (tail.compare_exchange_strong(
                expected,
                nullptr,
                std::memory_order_release,
                std::memory_order_relaxed))
        {
            return;
        }
        do
        {
            successor = node->next.load(std::memory_order_acquire);
        }
        while (successor == nullptr);
    }
    successor->waiting.store(false, std::memory_order_release);
}