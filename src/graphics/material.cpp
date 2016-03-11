
#include "material.hpp"

zeq_material_t::zeq_material_t(ConvMaterial& mat)
: m_diffuseId(0),
  m_blendType((uint8_t)mat.getBlend()),
  m_texPtr(nullptr)
{
    m_tint.r = 255;
    m_tint.g = 255;
    m_tint.b = 255;
    m_tint.a = 255;
    
    AnimTexture* animTex = mat.getAnimTexture();
    
    if (animTex)
    {
        m_isAnimated    = true;
        m_animTexture   = animTex;
        
        animTex->addReferencingMaterial(this);
    }
    else
    {
        m_isAnimated    = false;
        Texture* tex    = mat.getTextureSingle();
        
        if (tex)
            m_texture = tex;
    }
    
    if (m_texPtr)
        m_texPtr->grab();
}

zeq_material_t::zeq_material_t(const zeq_material_t& copy)
: ConvObject(copy), // having 100 copies of one material's name isn't too great... (octree)
  m_diffuseId(copy.m_diffuseId),
  m_blendType(copy.m_blendType),
  m_isAnimated(copy.m_isAnimated),
  m_texPtr(copy.m_texPtr)
{
    m_tint.r = copy.m_tint.r;
    m_tint.g = copy.m_tint.g;
    m_tint.b = copy.m_tint.b;
    m_tint.a = copy.m_tint.a;
    
    if (m_texPtr)
        m_texPtr->grab();
}

zeq_material_t::~zeq_material_t()
{
    if (m_texPtr)
        m_texPtr->drop();
}

void zeq_material_t::activateBlend(zeq_blend_t blend)
{
    switch (blend)
    {
    case ZEQ_BLEND_PARTICLE:
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        // Fallthrough
    case ZEQ_BLEND_MASKED:
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GEQUAL, 0.5f);
        break;
    case ZEQ_BLEND_ADDITIVE:
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_SRC_COLOR);
        break;
    case ZEQ_BLEND_ALPHA:
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    default:
        break;
    }
}

void zeq_material_t::deactivateBlend(zeq_blend_t blend)
{
    switch (blend)
    {
    case ZEQ_BLEND_PARTICLE:
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        // Fallthrough
    case ZEQ_BLEND_MASKED:
        glDisable(GL_ALPHA_TEST);
        break;
    case ZEQ_BLEND_ADDITIVE:
    case ZEQ_BLEND_ALPHA:
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        break;
    default:
        break;
    }
}

void zeq_material_t::registerWithOpenGL()
{
    if (m_isAnimated)
    {
        m_animTexture->registerWithOpenGL();
        setDiffuseId(m_animTexture->getTexture().getDiffuseId());
    }
    else if (m_texture)
    {
        m_texture->registerWithOpenGL();
        setDiffuseId(m_texture->getDiffuseId());
    }
}
