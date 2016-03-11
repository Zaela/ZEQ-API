
#ifndef _ZEQ_VERTEX_HPP_
#define _ZEQ_VERTEX_HPP_

#include "define.hpp"
#include "vec3.hpp"

struct Vertex
{
    Vec3    pos;
    Vec3    normal;
    float   u, v;
    union
    {
        bool        moved;      // Used by EQG skeletons
        uint16_t    boneIndex;  // Used by WLD skeletons
    };
};

struct Triangle
{
    Vertex points[3];
};

#endif//_ZEQ_VERTEX_HPP_

