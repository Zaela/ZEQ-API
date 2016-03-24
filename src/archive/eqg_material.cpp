
#include "eqg_material.hpp"

EqgMaterial::EqgMaterial(const char* name)
: ConvMaterial(ZEQ_BLEND_SOLID)
{
    setName(name);
}
