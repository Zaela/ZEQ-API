
#include "conv_skeleton.hpp"

ConvSkeleton::ConvSkeleton()
: m_boneCount(0),
  m_boneArray(nullptr)
{
    
}

ConvSkeleton::~ConvSkeleton()
{
    if (m_boneArray)
        delete[] m_boneArray;
}
