
#ifndef _ZEQ_CONV_SKELETON_HPP_
#define _ZEQ_CONV_SKELETON_HPP_

#include "define.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"
#include "attach_point.hpp"
#include <string>
#include <unordered_map>

class ConvSkeleton
{
public:
    struct Bone
    {
        uint32_t            childCount;
        AttachPoint::Type   attachPointType;
        Vec3                pos;
        Quaternion          rot;
        Vec3                scale;
    };
    
private:
    std::unordered_map<uint32_t, uint32_t>      m_indexMap;
    std::unordered_map<std::string, uint32_t>   m_indicesByBoneName;

    uint32_t    m_boneCount;
    Bone*       m_boneArray;
    
public:
    ConvSkeleton();
    ~ConvSkeleton();
};

#endif//_ZEQ_CONV_SKELETON_HPP_
