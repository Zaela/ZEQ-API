
#include "fog.hpp"

Fog::Fog()
: m_red(0),
  m_green(0),
  m_blue(0),
  m_density(0.0f),
  m_useExp2(false)
{
    
}

void Fog::set(float r, float g, float b, float density, bool useExp2)
{
    m_red   = r;
    m_green = g;
    m_blue  = b;
    
    m_density = density;
    m_useExp2 = useExp2;
}
