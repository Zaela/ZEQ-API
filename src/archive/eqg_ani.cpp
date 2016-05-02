
#include "eqg_ani.hpp"
#include "eqg_model.hpp"

EqgAni::EqgAni(const char* name, byte* data, uint32_t len)
: EqgCommon(data, len),
  m_name(name)
{
    
}

EqgAni::~EqgAni()
{
    byte* data = getData();
    
    if (data)
        delete[] data;
}

void EqgAni::init()
{
    
}

void EqgAni::extract(EqgModel* model)
{
    
}
