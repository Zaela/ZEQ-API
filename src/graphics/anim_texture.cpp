
#include "anim_texture.hpp"
#include "material.hpp"

AnimTexture::AnimTexture(zeq_t* Z, uint32_t animDelay, std::vector<Texture>& textures)
: m_timer(nullptr),
  m_currentIndex(0),
  m_count(textures.size())
{
    if (0 != animDelay)
        m_timer = zeq_timer_start(Z, animDelay, timerCallback, this);
    
    for (uint32_t i = 0; i < m_count; i++)
    {
        new (&m_textures[i]) Texture;
        m_textures[i] = std::move(textures[i]);
    }
}

AnimTexture* AnimTexture::create(zeq_t* Z, uint32_t animDelay, std::vector<Texture>& textures)
{
    size_t len = sizeof(AnimTexture) - sizeof(Texture) + (textures.size() * sizeof(Texture));
    
    AnimTexture* animTex = (AnimTexture*)(new byte[len]);
    new (animTex) AnimTexture(Z, animDelay, textures);
    
    return animTex;
}

AnimTexture::~AnimTexture()
{
    for (uint32_t i = 0; i < m_count; i++)
        m_textures[i].~Texture();
    
    if (m_timer)
        delete m_timer;
}

void AnimTexture::operator delete(void* ptr)
{
    byte* b = (byte*)ptr;
    ::operator delete[](b);
}

void AnimTexture::timerCallback(zeq_timer_t* timer)
{
    AnimTexture* animTex = (AnimTexture*)timer->getUserData();
    
    uint16_t index = animTex->m_currentIndex + 1;
    
    if (index == animTex->m_count)
        index = 0;
    
    animTex->m_currentIndex = index;
    
    uint32_t diffuseId = animTex->getTexture().getDiffuseId();
    
    if (!diffuseId)
        return;
    
    // Do updates
    for (zeq_material_t* mat : animTex->m_referencingMaterials)
    {
        mat->setDiffuseId(diffuseId);
    }
}

Texture& AnimTexture::getTexture()
{
    return m_textures[m_currentIndex];
}

void AnimTexture::addReferencingMaterial(zeq_material_t* mat)
{
    m_referencingMaterials.push_back(mat);
}

void AnimTexture::registerWithOpenGL()
{
    for (uint32_t i = 0; i < m_count; i++)
    {
        m_textures[i].registerWithOpenGL();
    }
}
