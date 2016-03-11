
#ifndef _ZEQ_VEC2_HPP_
#define _ZEQ_VEC2_HPP_

#include "define.hpp"

struct Vec2
{
    float x, y;
    
    Vec2() : x(0.0f), y(0.0f) { }
    Vec2(float _x, float _y) : x(_x), y(_y) { }
    
    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};

#endif//_ZEQ_VEC2_HPP_
