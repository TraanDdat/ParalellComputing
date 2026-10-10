/*----------TAS Lock----------*/
#ifndef TASLOCK_H
#define TASLOCK_H

#include <atomic>
#include "MUTEXLock.h"

class TASLock : public MUTEXLock
{
        private:
                std::atomic_flag flag;
        public:
                TASLock():flag(ATOMIC_FLAG_INIT){}
                ~TASLock() override = default;
                void lock() override;
                void unlock() override;
};

#endif