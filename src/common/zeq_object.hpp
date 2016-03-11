
#ifndef _ZEQ_OBJECT_HPP_
#define _ZEQ_OBJECT_HPP_

#include "define.hpp"

class ZeqObject
{
private:
    zeq_t* m_zeq;

public:
    ZeqObject(zeq_t* Z) : m_zeq(Z) { }
    
    zeq_t* Z() const { return m_zeq; }
};

#endif//_ZEQ_OBJECT_HPP_
