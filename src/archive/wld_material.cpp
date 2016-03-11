
#include "wld_material.hpp"

WldMaterial::WldMaterial(const char* name, uint32_t visibility)
: ConvMaterial(getBlendValue(visibility))
{
    setName(name);
}

zeq_blend_t WldMaterial::getBlendValue(uint32_t visibility)
{
    switch (visibility)
    {
    case 0x00000000:
        return ZEQ_BLEND_INVISIBLE;
    case 0x00000553:
    case 0x80000013:
        return ZEQ_BLEND_MASKED;
    case 0x80000005:
    case 0x8000000a:
        return ZEQ_BLEND_ADDITIVE;
    case 0x80000009:
    case 0x8000000b:
    case 0x80000017:
        return ZEQ_BLEND_PARTICLE;
    case 0x80000001:
    case 0x80000014:
    default:
        return ZEQ_BLEND_SOLID;
    }
}
