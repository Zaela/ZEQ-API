
#ifndef _ZEQ_REF_COUNTER_HPP_
#define _ZEQ_REF_COUNTER_HPP_

#include "define.hpp"

class RefCounter
{
private:
    int m_refCount;

public:
    RefCounter() : m_refCount(0) { }
    virtual ~RefCounter() { }
    
    void grab() { m_refCount++; }
    bool drop() { if (--m_refCount <= 0) { delete this; return true; } return false; }
};

#endif//_ZEQ_REF_COUNTER_HPP_
