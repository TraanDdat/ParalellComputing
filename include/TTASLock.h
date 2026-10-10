/*----------TTAS Lock---------- */
#ifndef TTASLOCK_H
#define TTASLOCK_H

#include <atomic>
#include "MUTEXLock.h"

class TTASLock : public MUTEXLock
{
        private:
                std::atomic<bool> locked{false};
        public:
                TTASLock() = default;
                ~TTASLock() override = default;
                void lock() override;
                void unlock() override;
};

#endif