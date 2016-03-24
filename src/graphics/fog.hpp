
#ifndef _ZEQ_FOG_HPP_
#define _ZEQ_FOG_HPP_

#include "define.hpp"
#include "opengl.hpp"

class Fog
{
private:
    float   m_red;
    float   m_green;
    float   m_blue;
    float   m_density;
    bool    m_useExp2;

public:
    Fog();

    void set(float r, float g, float b, float density, bool useExp2);

    inline void apply()
    {
        glEnable(GL_FOG);
        glHint(GL_FOG_HINT, GL_NICEST);
        
        float clr[4] = {m_red, m_green, m_blue, 1.0f};
        glFogfv(GL_FOG_COLOR, clr);
        
        glFogi(GL_FOG_MODE, m_useExp2 ? GL_EXP2 : GL_EXP);
        glFogf(GL_FOG_DENSITY, m_density);
    }
    
    inline static void disable()
    {
        glDisable(GL_FOG);
    }
    
    inline static void enable()
    {
        glEnable(GL_FOG);
    }
};

#endif//_ZEQ_FOG_HPP_
