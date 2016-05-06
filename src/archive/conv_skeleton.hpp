
#include <string>
#include <vector>
#ifndef _ZEQ_CONV_SKELETON_HPP_
#define _ZEQ_CONV_SKELETON_HPP_

#include "define.hpp"
#include "vec3.hpp"
#include "attach_point.hpp"
#include "quaternion.hpp"
#include <unordered_map>
#include <string>

class ConvSkeleton
{
public:
    struct Bone
    {
        std::string             name;
        std::vector<uint32_t>   children;
        AttachPoint::Type       attachPointType;
        Vec3                    pos;
        Quaternion              rot;
        Vec3                    scale;
        
        Bone() : attachPointType(AttachPoint::Type::NONE) { }
    };
    
private:
    std::unordered_map<uint32_t, uint32_t>      m_indexMap;
    std::unordered_map<std::string, uint32_t>   m_indicesByBoneName;

    uint32_t    m_boneCount;
    Bone*       m_boneArray;
    
public:
    ConvSkeleton();
    ~ConvSkeleton();

    void init(uint32_t count);

    void setBone(uint32_t index, const Vec3& pos, const Quaternion& rot, const Vec3& scale = Vec3(1.0f, 1.0f, 1.0f));
    void setAttachPointType(uint32_t index, AttachPoint::Type type);
    void addBoneNameToIndex(const std::string& name, uint32_t index);
    void addChild(uint32_t parent, uint32_t child);

    void buildIndexMap();
    std::unordered_map<uint32_t, uint32_t>& getIndexMap() { return m_indexMap; }
    
    bool getIndexByName(const char* name, uint32_t& out);
    
    bool hasBones() const { return m_boneArray != nullptr; }
    uint32_t getBoneCount() const { return m_boneCount; }
    Bone* getBoneArray() { return m_boneArray; }
};

#endif//_ZEQ_CONV_SKELETON_HPP_
