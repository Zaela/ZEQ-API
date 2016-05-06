
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

void ConvSkeleton::init(uint32_t count)
{
    m_boneCount = count;
    m_boneArray = new Bone[count];
}

void ConvSkeleton::setBone(uint32_t index, const Vec3& pos, const Quaternion& rot, const Vec3& scale)
{
    Bone& bone = m_boneArray[index];
    
    bone.pos    = pos;
    bone.rot    = rot;
    bone.scale  = scale;
}

void ConvSkeleton::setAttachPointType(uint32_t index, AttachPoint::Type type)
{
    m_boneArray[index].attachPointType = type;
}

void ConvSkeleton::addBoneNameToIndex(const std::string& name, uint32_t index)
{
    m_indicesByBoneName[name] = index;
    m_boneArray[index].name = name.c_str() + 3;
}

void ConvSkeleton::addChild(uint32_t parent, uint32_t child)
{
    m_boneArray[parent].children.push_back(child);
}

void ConvSkeleton::buildIndexMap()
{
    uint32_t index = 0;
    
    std::function<void(uint32_t)> recurse;
    recurse = [&, this](uint32_t i)
    {
        Bone& bone      = m_boneArray[i];
        m_indexMap[i]   = index++;
        
        for (uint32_t child : bone.children)
        {
            recurse(child);
        }
    };
    
    recurse(0);
}

bool ConvSkeleton::getIndexByName(const char* name, uint32_t& out)
{
    if (!m_indicesByBoneName.count(name))
        return false;
    
    out = m_indicesByBoneName[name];
    return true;
}
