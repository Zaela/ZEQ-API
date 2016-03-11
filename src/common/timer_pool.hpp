
#ifndef _ZEQ_TIMER_POOL_HPP_
#define _ZEQ_TIMER_POOL_HPP_

#include "define.hpp"
#include "timer.hpp"
#include "clock.hpp"
#include "bit.hpp"
#include "mutex.hpp"
#include <vector>

class TimerPool
{
private:
    AtomicMutex             m_mutex;
    uint32_t                m_capacity;
    uint32_t                m_count;
    uint64_t*               m_triggerTimes;
    zeq_timer_t**           m_timerObjects;
    std::vector<uint32_t>   m_triggered;

    static const uint32_t DEFAULT_CAPACITY = 32;

private:
    friend class zeq_timer_t;
    void swapAndPop(uint32_t index);
    void triggerTimer(uint32_t index);
    void realloc();

    void startTimer(zeq_timer_t* timer);
    void restartTimer(zeq_timer_t* timer);
    void markTimerAsDead(uint32_t index);

public:
    TimerPool();
    ~TimerPool();

    void check();
};

#endif//_ZEQ_TIMER_POOL_HPP_
