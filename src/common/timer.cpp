
#include "timer.hpp"
#include "timer_pool.hpp"
#include "zeq_struct.hpp"

zeq_timer_t::zeq_timer_t(zeq_t* Z, uint32_t periodMilliseconds, zeq_timer_callback_t callback, void* data)
: ZeqObject(Z),
  m_poolIndex(INVALID_INDEX),
  m_periodMilliseconds(periodMilliseconds),
  m_callback(callback),
  m_userData(data)
{
    Z->timerPool().startTimer(this);
}

zeq_timer_t::~zeq_timer_t()
{
    stop();
}

void zeq_timer_t::stop()
{
    uint32_t index = m_poolIndex;
    
    if (index == INVALID_INDEX)
        return;
    
    m_poolIndex = INVALID_INDEX;
    Z()->timerPool().markTimerAsDead(index);
}

void zeq_timer_t::restart()
{
    if (m_poolIndex == INVALID_INDEX)
    {
        Z()->timerPool().startTimer(this);
        return;
    }
    
    Z()->timerPool().restartTimer(this);
}

int zeq_timer_t::get_int(void* object, int field)
{
    zeq_timer_t* timer = (zeq_timer_t*)object;
    
    switch (field)
    {
    case ZEQ_TIMER_PERIOD_UINT:
        return (int)timer->getPeriodMilliseconds();
    default:
        return 0;
    }
}

uint32_t zeq_timer_t::get_uint(void* object, int field)
{
    zeq_timer_t* timer = (zeq_timer_t*)object;
    
    switch (field)
    {
    case ZEQ_TIMER_PERIOD_UINT:
        return timer->getPeriodMilliseconds();
    default:
        return 0;
    }
}

void* zeq_timer_t::get_ptr(void* object, int field)
{
    zeq_timer_t* timer = (zeq_timer_t*)object;
    
    switch (field)
    {
    case ZEQ_TIMER_USERDATA_PTR:
        return timer->getUserData();
    default:
        return nullptr;
    }
}

/*
** API Functions
*/

zeq_timer_t* zeq_timer_start(zeq_t* Z, uint32_t periodMilliseconds, zeq_timer_callback_t callback, void* userdata)
{
    try
    {
        return new zeq_timer_t(Z, periodMilliseconds, callback, userdata);
    }
    catch (...)
    {
        return nullptr;
    }
}

void zeq_timer_stop(zeq_timer_t* timer)
{
    timer->stop();
}

void zeq_timer_restart(zeq_timer_t* timer)
{
    try
    {
        timer->restart();
    }
    catch (...)
    {
        
    }
}

void zeq_timer_destroy(zeq_timer_t* timer)
{
    if (!timer)
        return;
    
    delete timer;
}
