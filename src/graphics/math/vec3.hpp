
#ifndef _ZEQ_VEC3_HPP_
#define _ZEQ_VEC3_HPP_

#include "define.hpp"
#include "math.hpp"
#include "vec2.hpp"

struct Vec3
{
    float x, y, z;
    
    Vec3() : x(0.0f), y(0.0f), z(0.0f) { }
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
    
    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    
    Vec3 operator+(const Vec3& o)
    {
        Vec3 v;
        v.x = x + o.x;
        v.y = y + o.y;
        v.z = z + o.z;
        return v;
    }
    
    Vec3 operator-(const Vec3& o)
    {
        Vec3 v;
        v.x = x - o.x;
        v.y = y - o.y;
        v.z = z - o.z;
        return v;
    }
    
    Vec3& operator+=(const Vec3& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    
    Vec3& operator-=(const Vec3& o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    
    Vec3& operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
    
    Vec3 operator*(float v)
    {
        Vec3 o;
        o.x = x * v;
        o.y = y * v;
        o.z = z * v;
        return o;
    }
    
    Vec3 operator/(float v)
    {
        Vec3 o;
        o.x = x / v;
        o.y = y / v;
        o.z = z / v;
        return o;
    }
    
    float lengthSquared() const
    {
        return x*x + y*y + z*z;
    }
    
    float length() const
    {
        return sqrt(lengthSquared());
    }
    
    Vec3& normalize()
    {
        float len = lengthSquared();
        
        if (len == 0.0f)
            return *this;
        
        len = Math::sqrtReciprocal(len);
        
        *this *= len;
        return *this;
    }
    
    float dotProduct(const Vec3& o)
    {
        return x * o.x + y * o.y + z * o.z;
    }
    
    Vec3 crossProduct(const Vec3& o)
    {
        return Vec3(
            y * o.z - z * o.y,
            z * o.x - x * o.z,
            x * o.y - y * o.z
        );
    }
    
    Vec2 horizontalAngle()
    {
        Vec2 v;
        
        v.y = Math::toDegrees(atan2(x, z));
        
        if (v.y < 0.0f)
            v.y += 360.0f;
        else if (v.y >= 360.0f)
            v.y -= 360.0f;
        
        float z1 = sqrt(x * x + z * z);
        
        v.x = Math::toDegrees(atan2(z1, y)) - 90.0f;
        
        if (v.x < 0.0f)
            v.x += 360.0f;
        else if (v.x >= 360.0f)
            v.x -= 360.0f;
        
        return v;
    }
    
    float getDistanceSquared(const Vec3& o) const
    {
        float dx = x - o.x;
        float dy = y - o.y;
        float dz = z - o.z;
        return dx*dx + dy*dy + dz*dz;
    }
};

#endif//_ZEQ_VEC3_HPP_
