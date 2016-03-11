
#include "conv_material.hpp"

ConvMaterial::ConvMaterial(zeq_blend_t blend)
: m_blendType(blend),
  m_animDelay(0),
  m_animTexture(nullptr),
  m_texture(nullptr)
{
    
}

ConvMaterial::ConvMaterial(init n)
: m_blendType(ZEQ_BLEND_INVISIBLE),
  m_animDelay(0),
  m_animTexture(nullptr),
  m_texture(nullptr)
{
    (void)n;
}

ConvMaterial::~ConvMaterial()
{
    if (m_animTexture)
        m_animTexture->drop();
    
    if (m_texture)
        m_texture->drop();
}

void ConvMaterial::addTexture(const std::string& name, byte* data, uint32_t len)
{
    m_textures.emplace_back(name, data, len, (m_blendType == ZEQ_BLEND_MASKED));
}

void ConvMaterial::prepareTextures(zeq_t* Z)
{
    if (m_textures.size() <= 1)
        return;
    
    m_animTexture = AnimTexture::create(Z, m_animDelay, m_textures);
    m_animTexture->grab();
}

Texture* ConvMaterial::getTextureSingle()
{
    if (!m_texture)
    {
        m_texture = new Texture(std::move(m_textures[0]));
        m_texture->grab();
    }
    
    return m_texture;
}

ConvMaterial* ConvMaterial::Null()
{
    static ConvMaterial* NullMaterial = new ConvMaterial(ConvMaterial::NULL_INIT);
    return NullMaterial;
}
