
#ifndef _ZEQ_WLD_MATERIAL_HPP_
#define _ZEQ_WLD_MATERIAL_HPP_

#include "define.hpp"
#include "conv_material.hpp"

class WldMaterial : public ConvMaterial
{
private:
    zeq_blend_t getBlendValue(uint32_t visibility);

public:
    WldMaterial(const char* name, uint32_t visibility);
    virtual ~WldMaterial() { }
};

#endif//_ZEQ_WLD_MATERIAL_HPP_
