
#ifndef _ZEQ_MATERIAL_HPP_
#define _ZEQ_MATERIAL_HPP_

#include "define.hpp"
#include "anim_texture.hpp"
#include "texture.hpp"
#include "conv_object.hpp"
#include "conv_material.hpp"
#include "opengl.hpp"

class zeq_material_t : public ConvObject
{
private:
    uint32_t        m_diffuseId;
    zeq_color_t     m_tint;
    uint8_t         m_blendType;
    bool            m_isAnimated;

    union
    {
        RefCounter*     m_texPtr;
        AnimTexture*    m_animTexture;
        Texture*        m_texture;
    };
    
public:
    zeq_material_t(ConvMaterial& mat);
    zeq_material_t(const zeq_material_t& copy);
    virtual ~zeq_material_t();

    zeq_blend_t getBlend() const { return (zeq_blend_t)m_blendType; }
    
    uint32_t getDiffuseId() const { return m_diffuseId; }
    void setDiffuseId(uint32_t id) { m_diffuseId = id; }
    
    inline bool isInvisible() const { return (zeq_blend_t)m_blendType == ZEQ_BLEND_INVISIBLE || m_diffuseId == 0; }
    
    inline void setActiveTexture(uint32_t& prevDiffuseId)
    {
        uint32_t diffuseId = m_diffuseId;
        if (prevDiffuseId != diffuseId)
        {
            glBindTexture(GL_TEXTURE_2D, diffuseId);
            prevDiffuseId = diffuseId;
        }
    }
    
    inline void setActiveBlend(zeq_blend_t& prevBlend)
    {
        zeq_blend_t blend = (zeq_blend_t)m_blendType;
        if (prevBlend != blend)
        {
            deactivateBlend(prevBlend);
            activateBlend(blend);
            prevBlend = blend;
        }
    }
    
    static void activateBlend(zeq_blend_t blend);
    static void deactivateBlend(zeq_blend_t blend);
    
    void registerWithOpenGL();
};

#endif//_ZEQ_MATERIAL_HPP_
