
#ifndef _ZEQ_TIMER_HPP_
#define _ZEQ_TIMER_HPP_

#include "define.hpp"
#include "zeq_object.hpp"
#include <new>

class TimerPool;

struct zeq_timer_t : public ZeqObject
{
private:
    friend class TimerPool;

    uint32_t                m_poolIndex;
    uint32_t                m_periodMilliseconds;
    zeq_timer_callback_t    m_callback;
    union
    {
        void*   m_userData;
        int     m_luaCallback;
    };
    
    static const uint32_t INVALID_INDEX = 0xFFFFFFFF;

public:
    zeq_timer_t(zeq_t* Z, uint32_t periodMilliseconds, zeq_timer_callback_t callback, void* data = nullptr);
    ~zeq_timer_t();

    uint32_t    getPeriodMilliseconds() const { return m_periodMilliseconds; }
    void*       getUserData() const { return m_userData; }

    void stop();
    void restart();
    
    static int get_int(void* object, int field);
    static uint32_t get_uint(void* object, int field);
    static void* get_ptr(void* object, int field);
};

#endif//_ZEQ_TIMER_HPP_
