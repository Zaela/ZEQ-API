
#ifndef _ZEQ_ANIM_TEXTURE_HPP_
#define _ZEQ_ANIM_TEXTURE_HPP_

#include "define.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "ref_counter.hpp"
#include <vector>

class AnimTexture : public RefCounter
{
private:
    zeq_timer_t* m_timer;

    // Materials that reference this animated texture, need to update them when the current texture changes
    std::vector<zeq_material_t*> m_referencingMaterials;

    uint16_t    m_currentIndex;
    uint16_t    m_count;
    Texture     m_textures[1]; // Variable-length stub
    
private:
    static void timerCallback(zeq_timer_t* timer);
    AnimTexture(zeq_t* Z, uint32_t animDelay, std::vector<Texture>& textures);

public:
    AnimTexture()                   = delete;
    AnimTexture(const AnimTexture&) = delete;
    static AnimTexture* create(zeq_t* Z, uint32_t animDelay, std::vector<Texture>& textures);
    virtual ~AnimTexture();

    void operator delete(void* ptr);

    Texture& getTexture();

    void addReferencingMaterial(zeq_material_t* mat);

    void registerWithOpenGL();
};

#endif//_ZEQ_ANIM_TEXTURE_HPP_
