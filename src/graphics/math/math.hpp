
#ifndef _ZEQ_MATH_HPP_
#define _ZEQ_MATH_HPP_

#include "define.hpp"

class Math
{
public:
    static float toRadians(float degrees)
    {
        return degrees * 3.14159f / 180.0f;
    }
    
    static float toDegrees(float radians)
    {
        return radians * 180.0f / 3.14159f;
    }
    
    static float sqrtReciprocal(float value)
    {
        return 1.0f / sqrt(value);
    }
    
    static float reciprocal(float value)
    {
        return 1.0f / value;
    }
};

#endif//_ZEQ_MATH_HPP_
