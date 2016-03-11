
#ifndef _ZEQ_CONV_MATERIAL_HPP_
#define _ZEQ_CONV_MATERIAL_HPP_

#include "define.hpp"
#include "conv_object.hpp"
#include "texture.hpp"
#include "anim_texture.hpp"
#include <vector>

class ConvMaterial : public ConvObject
{
private:
    zeq_blend_t             m_blendType;
    uint32_t                m_animDelay;
    std::vector<Texture>    m_textures;

    AnimTexture*            m_animTexture;
    Texture*                m_texture;

public:
    enum init
    {
        NULL_INIT
    };
    
public:
    ConvMaterial(zeq_blend_t blend);
    ConvMaterial(init n);
    virtual ~ConvMaterial();

    zeq_blend_t getBlend() const { return m_blendType; }
    void setBlend(zeq_blend_t blend) { m_blendType = blend; }
    void setAnimDelay(uint32_t ms) { m_animDelay = ms; }
    
    void addTexture(const std::string& name, byte* data, uint32_t len);
    void prepareTextures(zeq_t* Z);
    
    AnimTexture*    getAnimTexture() const { return m_animTexture; }
    Texture*        getTextureSingle();

    static ConvMaterial* Null();
};

#endif//_ZEQ_CONV_MATERIAL_HPP_
