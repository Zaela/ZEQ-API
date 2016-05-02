
#ifndef _ZEQ_SKELETON_HPP_
#define _ZEQ_SKELETON_HPP_

#include "define.hpp"
#include "conv_skeleton.hpp"
#include "attach_point.hpp"
#include "mat4.hpp"
#include <functional>

class Skeleton
{
public:
    struct Bone
    {
        bool                hasAnimFrames;
        
        Vec3                pos;
        Quaternion          rot;
        Vec3                scale;
        
        Mat4                globalAnimMatrix;
        Mat4                globalInverseMatrix;
        
        Mat4*               parentGlobalAnimMatrix;
        
        uint32_t            animHint;
        AttachPoint::Type   attachPointType;
    };
    
private:
    uint32_t    m_boneCount;
    Bone*       m_boneArray;
    
public:
    Skeleton();
    Skeleton(const Skeleton& copy);
    ~Skeleton();

    void init(ConvSkeleton& skele);

    bool hasBones() const { return m_boneArray != nullptr; }
};

#endif//_ZEQ_SKELETON_HPP_
