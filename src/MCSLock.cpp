#include "MCSLock.h"

thread_local MCSLock::Node MCSLock::node;

void MCSLock::lock_impl(Node& n)
{
    n.next.store(nullptr, std::memory_order_relaxed);
    n.locked.store(true, std::memory_order_relaxed);
    Node* predecessor =
        tail.exchange(&n, std::memory_order_acq_rel);
    if (predecessor != nullptr)
    {
        predecessor->next.store(
            &n,
            std::memory_order_release
        );
        while (n.locked.load(std::memory_order_acquire)){/* Spin */}
    }
}

void MCSLock::unlock_impl(Node& n)
{
    Node* successor =
        n.next.load(std::memory_order_acquire);
    if (successor == nullptr)
    {
        Node* expected = &n;
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
            successor =
                n.next.load(std::memory_order_acquire);
        }
        while (successor == nullptr);
    }
    successor->locked.store(
        false,
        std::memory_order_release
    );
}

void MCSLock::lock()
{
    lock_impl(node);
}

void MCSLock::unlock()
{
    unlock_impl(node);
}