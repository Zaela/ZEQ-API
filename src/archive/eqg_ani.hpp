
#ifndef _ZEQ_EQG_ANI_HPP_
#define _ZEQ_EQG_ANI_HPP_

#include "define.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"
#include "eqg_common.hpp"
#include <string>

class EqgModel;

class EqgAni : public EqgCommon
{
private:
#pragma pack(1)
    struct Header
    {
        uint32_t signature; // "EQGA"
        uint32_t version;
        uint32_t stringBlockLen;
        uint32_t count;
    };
    
    struct HeaderV2 : public Header
    {
        uint32_t recurseBoneOrder; // We don't care about this field, need to adjust bone indices regardless
    };
    
    struct FrameHeader
    {
        uint32_t frameCount;
        uint32_t boneNameIndex;
    };
    
    struct Frame
    {
        uint32_t    milliseconds;
        Vec3        pos;
        Quaternion  rot;
        Vec3        scale;
    };
#pragma pack()

private:
    const char* m_name;
    
public:
    EqgAni(const char* name, byte* data, uint32_t len);
    ~EqgAni();

    void init();
    void extract(EqgModel* model);
};

#endif//_ZEQ_EQG_ANI_HPP_
