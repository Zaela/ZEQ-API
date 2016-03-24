
#ifndef _ZEQ_EQG_MATERIAL_HPP_
#define _ZEQ_EQG_MATERIAL_HPP_

#include "define.hpp"
#include "conv_material.hpp"

class EqgMaterial : public ConvMaterial
{
public:
    EqgMaterial(const char* name);
    virtual ~EqgMaterial() { }
};

#endif//_ZEQ_EQG_MATERIAL_HPP_