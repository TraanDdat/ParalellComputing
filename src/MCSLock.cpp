#include "MCSLock.h"
void MCSLock::lock()
{
    MCSNode* node = &getThreadNode(this);
    node->next.store(nullptr, std::memory_order_relaxed);
    node->waiting.store(true, std::memory_order_relaxed);
    // Append this thread to the queue.
    MCSNode* predecessor = tail.exchange(node, std::memory_order_acq_rel);
    if (predecessor != nullptr)
    {
        // Connect the predecessor to this node.
        predecessor->next.store(node, std::memory_order_release);
        // Wait on this thread's own flag.
        while (node->waiting.load(std::memory_order_acquire))
        {
            // Spin
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